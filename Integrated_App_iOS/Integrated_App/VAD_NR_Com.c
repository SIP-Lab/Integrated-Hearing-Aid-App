//
//  VAD_NR_Com.c
//  Voice Activity Detection with NoiseReduction and compression
//
//  Created by Abhishek Sehgal on 8/14/17.
//  Copyright © 2017 SIPLab. All rights reserved.
//


#include <sys/time.h>
#include "VAD_NR_Com.h"

struct timeval t1, t2;
double elapsedTime;
float dataIn[20] = {5, -5, 5, 100,
    5, -10, 5, 100,
    5, -20, 2, 50,
    4, -25, 2, 50,
    4, -35, 2, 100};

VADNoiseReductionCompression* initVAD_NoiseReduction_Compression(Settings* settings, const char *pathHybridDir, FILE *file2) {

    VADNoiseReductionCompression* inParam = (VADNoiseReductionCompression*)malloc(sizeof(VADNoiseReductionCompression));
    
    inParam->stepSize           = settings->stepSize;
    inParam->decimatedStepSize  = settings->stepSize/DECIMATION_FACTOR;
    inParam->samplingFrequency  = settings->fs/DECIMATION_FACTOR;
    
    inParam->input          = (float*)calloc(settings->stepSize, sizeof(float));
    inParam->downsampled    = (float*)calloc(settings->stepSize, sizeof(float));
    inParam->decimated      = (float*)calloc(inParam->decimatedStepSize, sizeof(float));
    inParam->processed      = (float*)calloc(inParam->decimatedStepSize, sizeof(float));
    inParam->frame          = (float*)calloc(inParam->decimatedStepSize*2, sizeof(float));
    
    /*for compression*/
    inParam->compressed      = (float*)calloc(inParam->decimatedStepSize, sizeof(float));
    /**/
    
    inParam->interpolated   = (float*)calloc(settings->stepSize, sizeof(float));
    inParam->output         = (float*)calloc(settings->stepSize, sizeof(float));
    
    /*for VAD initialization*/
    inParam->fft            = newTransform(2*inParam->decimatedStepSize,
                                           (int)(inParam->samplingFrequency/inParam->decimatedStepSize));
    inParam->melSpectrogram = initMelSpectrogram(NFILT, FREQLOW, FREQHIGH,
                                                 2*inParam->decimatedStepSize,
                                                 inParam->samplingFrequency,
                                                 inParam->fft->points);
    
    inParam->memoryPointer  = initializeVADParams(settings->fs/DECIMATION_FACTOR,
                                         inParam->decimatedStepSize,
                                         2*inParam->decimatedStepSize,
                                         16,
                                         1.0);
     /**/
    
    inParam->downsampleFilter       = initFIR(settings->stepSize);
    inParam->interpolationFilter    = initFIR(settings->stepSize);
    

    
    // For audio level
    
    inParam->spl = newSPLBuffer(settings->dbUpdateInterval * settings->fs / settings->stepSize, SPLBUFFER_MODE_FULL);
    
    wiener_ADAPTIVE_NE_initialize();
    DynamicRangeFiveBandCompression_initialize();
    return inParam;
}


void doNoiseReduction_Compression_withVAD(VADNoiseReductionCompression *_ptr,
                                          short *_in,
                                          short* _out,
                                          Settings* settings){

    
    VADNoiseReductionCompression *inParam = _ptr;
    
    int i,j;
    
    
    for (i = 0; i < inParam->stepSize; i++) {
        inParam->input[i] = _in[i] * S2F;
    }
    
    //Downsample the Audio
    processFIRFilter(inParam->downsampleFilter, inParam->input, inParam->downsampled);
    
    //Decimate the Audio
    for(i = 0, j = 0; i < inParam->decimatedStepSize; i++, j+= 3){
        inParam->decimated[i] = inParam->downsampled[j];
        
        inParam->frame[i] = inParam->frame[i+inParam->decimatedStepSize];
        inParam->frame[i+inParam->decimatedStepSize] = inParam->decimated[i];
    }
    
    ForwardFFT(inParam->fft, inParam->frame, settings->calibration);
    updateImage(inParam->melSpectrogram, inParam->fft->power);
    
    /*
    int i,j;
    
    // Convert from Short to Float
    for (i = 0; i < inParam->stepSize; i++) {
        inParam->input[i] = _in[i] * S2F;
    }
    
    // Downsample the audio
    processFIRFilter(inParam->downsampleFilter, inParam->input, inParam->downsampled);
    
    // Decimate the audio
    for (i = 0, j = 0; i < inParam->decimatedStepSize; i++, j+= 3) {
        inParam->decimated[i] = inParam->downsampled[j];
    }
    //VAD computation
    compute(inParam->memoryPointer, inParam->decimated, 52.0f, settings->calibration);

    // SAVE Audio level to Settings
    Variables* vars = (Variables*)inParam->memoryPointer;
    inParam->spl->buffer(inParam->spl, vars->fft->dbpower, settings);
    
    */
    
    //Adaptive Noise Reduction
    wiener_ADAPTIVE_NE(inParam->decimated, -1, 16000, settings->classLabel, settings->noiseEstimationFrame, inParam->processed);

    //when noise reduction is on only
    if (settings->noiseReductionOutputType && !settings->compressionOutputType) {
        
            //Adaptive Noise Reduction
            //wiener_ADAPTIVE_NE(inParam->decimated, -1, 16000, getClass(inParam->memoryPointer), settings->noiseEstimationFrame, inParam->processed);
            //Interpolate the Audio
            for (i = 0, j = 0; i < inParam->decimatedStepSize; i++, j+=3) {
                inParam->interpolated[j] = inParam->processed[i];
            }
            // Low-Pass filter the Interpolated Audio
            processFIRFilter(inParam->interpolationFilter, inParam->interpolated, inParam->output);
            // Convert to Short from Float
            for (i = 0; i < inParam->stepSize; i++) {
                _out[i] = (short)(settings->amplification*2*F2S * inParam->output[i]);
            }
    }
    //when compression is on only
    else if(settings->compressionOutputType && !settings->noiseReductionOutputType){
        // Compression
        DynamicRangeFiveBandCompression(inParam->decimated, 16000, dataIn, 48, 0, inParam->compressed);
        //Interpolate the Audio
        for (i = 0, j = 0; i < inParam->decimatedStepSize; i++, j+=3) {
            inParam->interpolated[j] = inParam->compressed[i];
        }
        // Low-Pass filter the Interpolated Audio
        processFIRFilter(inParam->interpolationFilter, inParam->interpolated, inParam->output);
        // Convert to Short from Float
        for (i = 0; i < inParam->stepSize; i++) {
            _out[i] = (short)(settings->amplification*2*F2S * inParam->output[i]);
        }
    }
    // when both noise reduction and compression is on
    else if(settings->noiseReductionOutputType && settings->compressionOutputType){
        //Adaptive Noise Reduction
        //wiener_ADAPTIVE_NE(inParam->decimated, -1, 16000, getClass(inParam->memoryPointer), settings->noiseEstimationFrame, inParam->processed);
        // Compression
        DynamicRangeFiveBandCompression(inParam->processed, 16000, dataIn, 48, 0, inParam->compressed);
        //Interpolate the Audio
        for (i = 0, j = 0; i < inParam->decimatedStepSize; i++, j+=3) {
            inParam->interpolated[j] = inParam->compressed[i];
        }
        // Low-Pass filter the Interpolated Audio
        processFIRFilter(inParam->interpolationFilter, inParam->interpolated, inParam->output);
        // Convert to Short from Float
        for (i = 0; i < inParam->stepSize; i++) {
            _out[i] = (short)(settings->amplification*2*F2S * inParam->output[i]);
        }
    }
    else {
        memcpy(_out, _in, inParam->stepSize * sizeof(short));
    }
}

void destroyVAD_NoiseReduction_Compression(VADNoiseReductionCompression** _ptr) {
    if (*_ptr != NULL) {
        
        wiener_ADAPTIVE_NE_initialize();
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
        if ((*_ptr)->processed != NULL){
            free((*_ptr)->processed);
            (*_ptr)->processed = NULL;
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
        
        /*for VAD Parameters*/
        if ((*_ptr)->memoryPointer != NULL){
            free((*_ptr)->memoryPointer);
            (*_ptr)->memoryPointer = NULL;
        }
        /**/
        
        if ((*_ptr)->spl != NULL){
            free((*_ptr)->spl);
            (*_ptr)->spl = NULL;
        }
        
        destroyFIR(&(*_ptr)->downsampleFilter);
        destroyFIR(&(*_ptr)->interpolationFilter);
        free(*_ptr);
        *_ptr = NULL;
    }
}

void getMelImage(VADNoiseReductionCompression* memoryPointer, float** melImage){
    VADNoiseReductionCompression* inParam = (VADNoiseReductionCompression*) memoryPointer;
    for (size_t i = 0; i < NFILT; i++) {
        for (size_t j = 0; j < NFILT; j++) {
            melImage[i][j] = inParam->melSpectrogram->melSpectrogramImage[i][j];
        }
    }
}
