//
//  SpeechProcessing.c
// Signal Processing Pipeline:
//  Voice Activity Detection with NoiseReduction and compression
//
//  Created by Abhishek Sehgal on 8/14/17.
//  Modified by Chowdhury, Tahsin on 11/02/17 for integration
//  Copyright © 2017 SIPLab. All rights reserved.
//

#include <sys/time.h>
#include "SpeechProcessing.h"

#include <android/log.h>

//-----------Initiliazing parameteres for the overall pipeline including three modues:
//-----------VAD, Noise Reduction and Compression-------------//

VADNoiseReductionCompression* initVAD_NoiseReduction_Compression(Settings* settings) {
    
    VADNoiseReductionCompression* inParam = (VADNoiseReductionCompression*)malloc(sizeof(VADNoiseReductionCompression));
    
    inParam->stepSize            = settings->stepSize;
    inParam->decimatedStepSize   = settings->stepSize/DECIMATION_FACTOR;
    inParam->decimatedWindowSize = settings->frameSize/DECIMATION_FACTOR;
    inParam->overlap             = inParam->decimatedWindowSize - inParam->decimatedStepSize;
    inParam->decimatedFs         = settings->fs/DECIMATION_FACTOR;
    inParam->input               = (float*)calloc(settings->stepSize, sizeof(float));
    inParam->downsampled         = (float*)calloc(settings->stepSize, sizeof(float));
    inParam->inputBuffer         = (float*)calloc(inParam->decimatedWindowSize,sizeof(float));
    inParam->decimated           = (float*)calloc(inParam->decimatedStepSize, sizeof(float));
    inParam->noise_reduced       = (float*)calloc(inParam->decimatedStepSize, sizeof(float));
    /*for compression*/
    inParam->compressed          = (float*)calloc(inParam->decimatedStepSize, sizeof(float));
    /**/
    
    inParam->interpolated        = (float*)calloc(settings->stepSize, sizeof(float));
    inParam->output              = (float*)calloc(settings->stepSize, sizeof(float));
    
    inParam->downsampleFilter    = initFIR(settings->stepSize);
    inParam->interpolationFilter = initFIR(settings->stepSize);
    
    // For audio level
    inParam->fft                 = newTransform(inParam->decimatedWindowSize,
                                                (int)(inParam->decimatedFs/inParam->decimatedStepSize));
    
    inParam->spl                 = newSPLBuffer(settings->guiUpdateInterval * settings->fs / settings->stepSize,
                                                SPLBUFFER_MODE_FULL);
    
    /*for VAD initialization*///Remove/update according to new VAD
    inParam->melSpectrogram = initMelSpectrogram(NFILT, FREQLOW, FREQHIGH, inParam->decimatedWindowSize, inParam->decimatedFs, inParam->fft->points);
    
    //For Adaptive Noise Reduction Initialization
    wiener_ADAPTIVE_NE_initialize();

    //For Compression Initialization
    DynamicRangeFiveBandCompression_initialize();

    return inParam;
}

//Notes:
//1. Here, 50% overlapping is performed after downsampling the audio signal and stored in 'inParam->inputBuffer'.
//   This is used for FFT calculation (Transform section) to calculate power, which is the input to the VAD module.
//
//2. Noise Reduction code is converted from MATLAB to C using MATLAB Coder, and it uses its own fft with 50%
//   overlapping inside noise reduction module. So, 'inParam->decimated' is used as input to it. If users want to
//   change the module or replace with another Noise reduction that requires input with overlap, they can use
//   'inParam->inputBuffer'.
//
//3. Compression module is placed after noise reduction module so that it can use the processed output from noise
//   reduction for compression as its input. This can also be used with Noise Reduction is turned off.
//

void doNoiseReduction_Compression_withVAD(VADNoiseReductionCompression *_ptr, float *_in, float* _out, Settings* settings){
    
    VADNoiseReductionCompression *inParam = _ptr;
    
    int i,j;
    // Convert from Short to Float
    for (i = 0; i < inParam->stepSize; i++) {
        //inParam->input[i] = _in[i] * S2F;
        inParam->input[i] = _in[i];
    }

    // Downsample the audio
    processFIRFilter(inParam->downsampleFilter, inParam->input, inParam->downsampled);
    
    // Decimate the audio
    for (i = 0, j = 0; i < inParam->decimatedStepSize; i++, j+= DECIMATION_FACTOR) {
        inParam->decimated[i] = inParam->downsampled[j];
    }

    //Performing 50% overlapping of input data for power calculation (used for Audio Level and VAD)
    for (i = 0; i < inParam->overlap; i++) {
        inParam->inputBuffer[i] = inParam->inputBuffer[inParam->decimatedStepSize + i];
    }
    for (i=0; i<inParam->decimatedStepSize; i++)
    {
        inParam->inputBuffer[inParam->overlap + i] = inParam->decimated[i];
    }

    //Calculate and Save Audio Level to Settings
    ForwardFFT(inParam->fft, inParam->inputBuffer, settings->calibration);
    inParam->spl->buffer(inParam->spl, inParam->fft->dbpower, settings);

    //------------VAD: Feature Extraction and Random Forest Classifer--------------//
    updateImage(inParam->melSpectrogram, inParam->fft->power);
    //----------------------------VAD Ends------------------------------------------//


    /***---------------Adaptive Noise Reduction----------------------------**///Always calculating

    wiener_ADAPTIVE_NE(inParam->decimated, inParam->decimatedFs, settings->classLabel, settings->noiseEstimationFrame, inParam->noise_reduced);

    /***---------------Adaptive Noise Reduction ENDS: Remove This section if want to replace with another  Noise Reduction------**/

    //when noise reduction is on only,
    if (settings->noiseReductionOutputType && !settings->compressionOutputType) {

        //Interpolate the Audio
        for (i = 0, j = 0; i < inParam->decimatedStepSize; i++, j+=3) {
            inParam->interpolated[j] = inParam->noise_reduced[i];
        }
        // Low-Pass filter the Interpolated Audio
        processFIRFilter(inParam->interpolationFilter, inParam->interpolated, inParam->output);
        // Convert to Short from Float
        for (i = 0; i < inParam->stepSize; i++) {
            //_out[i] = (short)(settings->amplification*2*F2S * inParam->output[i]);
            _out[i] = (settings->amplification * 2 * inParam->output[i]);
        }
    }
    //when compression is on only
    else if(settings->compressionOutputType && !settings->noiseReductionOutputType){

        /***--------------Compression---------------------------------**/

        DynamicRangeFiveBandCompression(inParam->decimated, inParam->decimatedFs, dataIn, inParam->compressed);

        /***-------------Compression ENDS: Remove This section if want to replace with another Compression---------------------------**/

        //Interpolate the Audio
        for (i = 0, j = 0; i < inParam->decimatedStepSize; i++, j+=3) {
            inParam->interpolated[j] = inParam->compressed[i];
        }
        // Low-Pass filter the Interpolated Audio
        processFIRFilter(inParam->interpolationFilter, inParam->interpolated, inParam->output);
        // Convert to Short from Float
        for (i = 0; i < inParam->stepSize; i++) {
            //_out[i] = (short)(settings->amplification*2*F2S * inParam->output[i]);
            _out[i] = (settings->amplification * 2 * inParam->output[i]);
        }
    }
    // when both noise reduction and compression is on
    else if(settings->noiseReductionOutputType && settings->compressionOutputType){

        /***-------------Compression---------------------------------**/

        DynamicRangeFiveBandCompression(inParam->noise_reduced, inParam->decimatedFs, dataIn, inParam->compressed);

        /***-------------Compression ENDS: Remove This section if want to replace with another Compression---------------------------**/

        //Interpolate the Audio
        for (i = 0, j = 0; i < inParam->decimatedStepSize; i++, j+=3) {
            inParam->interpolated[j] = inParam->compressed[i];
        }
        // Low-Pass filter the Interpolated Audio
        processFIRFilter(inParam->interpolationFilter, inParam->interpolated, inParam->output);
        // Convert to Short from Float
        for (i = 0; i < inParam->stepSize; i++) {
            //_out[i] = (short)(settings->amplification*2*F2S * inParam->output[i]);
            _out[i] = (settings->amplification * 2 * inParam->output[i]);
        }
    }
    else {
        //memcpy(_out, _in, inParam->stepSize * sizeof(short));
        memcpy(_out, _in, inParam->stepSize * sizeof(float));
    }
}

void destroyVAD_NoiseReduction_Compression(VADNoiseReductionCompression** _ptr) {
    if (*_ptr != NULL) {
        
        wiener_ADAPTIVE_NE_terminate();
        DynamicRangeFiveBandCompression_terminate();
        
        if ((*_ptr)->input != NULL){
            free((*_ptr)->input);
            (*_ptr)->input = NULL;
        }
        if ((*_ptr)->downsampled != NULL){
            free((*_ptr)->downsampled);
            (*_ptr)->downsampled = NULL;
        }
        if ((*_ptr)->decimated != NULL){
            free((*_ptr)->decimated);
            (*_ptr)->decimated = NULL;
        }
        if ((*_ptr)->noise_reduced != NULL){
            free((*_ptr)->noise_reduced);
            (*_ptr)->noise_reduced = NULL;
        }
        
        if ((*_ptr)->inputBuffer != NULL){
            free((*_ptr)->inputBuffer);
            (*_ptr)->inputBuffer = NULL;
        }
        
        /*for compression*/
        if ((*_ptr)->compressed != NULL){
            free((*_ptr)->compressed);
            (*_ptr)->compressed = NULL;
        }
        /**/
        
        if ((*_ptr)->interpolated != NULL){
            free((*_ptr)->interpolated);
            (*_ptr)->interpolated = NULL;
        }
        if ((*_ptr)->output != NULL){
            free((*_ptr)->output);
            (*_ptr)->output = NULL;
        }
        
//        if ((*_ptr)->memoryPointer != NULL){
//            free((*_ptr)->memoryPointer);
//            (*_ptr)->memoryPointer = NULL;
//        }

        /**/
        
        destroyFIR(&(*_ptr)->downsampleFilter);
        destroyFIR(&(*_ptr)->interpolationFilter);
        destroyTransform(&(*_ptr)->fft);
        destroySPLBuffer(&(*_ptr)->spl);
        
        free(*_ptr);
        *_ptr = NULL;
    }
}

