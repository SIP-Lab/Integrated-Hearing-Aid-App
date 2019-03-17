//
//  SpeechProcessing.c
//  SPP RF+SB
//
//  Created by Sehgal, Abhishek on 5/4/15.
//  Copyright (c) 2015 UT Dallas. All rights reserved.
//

#include "SpeechProcessing.h"
#include <unistd.h>

static int count = 0;
//
static int* smoothingBuffer;
static int smoothingBufferLength;

int noiseCount = 0;
static int resolution8k(int samplingFrequency, int frameSize){
    int pow2Size = 0x01;
    while (pow2Size < frameSize)
    {
        pow2Size = pow2Size << 1;
    }
   float binResolution = (float) samplingFrequency/pow2Size;
   return (int)lround(16000.0/binResolution);
//    return pow2Size;
}


long* initializeVADParams(int frequency, int stepsize, int windowsize, int decisionBufferLength, float decisionRate)
{
    Variables* inParam = (Variables*) malloc(sizeof(Variables));
    inParam->fft = newTransform(windowsize, (int)(frequency/stepsize));
    inParam->overlap = windowsize - stepsize;
    inParam->frequency = frequency;
    inParam->stepSize = stepsize;
    inParam->windowSize = windowsize;
    inParam->inputBuffer = (float*)calloc(windowsize,sizeof(float));
    inParam->outputBuffer = (float*)malloc(stepsize*sizeof(float));
    inParam->originalInput = (short*)malloc(stepsize*sizeof(short));
    inParam->noiseEstimate = (float*)calloc(windowsize,sizeof(float));//noiseBUffer
    
    // initialize SubbandFeatures
    //inParam->sbf = initSubbandFeatures(inParam->fft->points, stepsize, decisionBufferLength);
    //inParam->vf  = initVADFeatures(inParam->fft->points, decisionBufferLength);
    inParam->sbf = initSubbandFeatures(resolution8k(frequency,windowsize), stepsize, decisionBufferLength);
    inParam->vf  = initVADFeatures(resolution8k(frequency,windowsize), decisionBufferLength);
    inParam->vrf = initVADRandomForest(decisionBufferLength);
    inParam->class1 = -1;
    inParam->firstFrame = 1;
    inParam->decisionBufferLength = decisionBufferLength;
    count = 0;
    
    inParam->nFeatures = 2 * inParam->sbf->nBands + inParam->vf->nFeatures - 2 - 6;
    inParam->allFeatureList = (float*)calloc(inParam->nFeatures, sizeof(float));
    
    inParam->warmup = 2 * decisionBufferLength;
    
    //
    smoothingBufferLength = (int)(decisionRate*frequency)/(stepsize*decisionBufferLength);
    smoothingBuffer = calloc(sizeof(int), smoothingBufferLength);
    //
    
    return (long*)inParam;
}


void compute(long* memoryPointer, float* input, float quiet, float calibration)
{
    Variables* inParam = (Variables*)memoryPointer;
    
    int i;
    
    for (i = 0; i < inParam->overlap; i++) {
        inParam->inputBuffer[i] = inParam->inputBuffer[inParam->stepSize + i];
    }
    
    for (i=0; i<inParam->stepSize; i++)
    {
        inParam->originalInput[i] = input[i];
       // inParam->inputBuffer[inParam->overlap + i] = input[i]/32768.0f;
        inParam->inputBuffer[inParam->overlap + i] = input[i];
    }
    
    
    ForwardFFT(inParam->fft, inParam->inputBuffer, calibration);

    if (inParam->fft->dbpower > quiet)
    {

        computeSubbandFeatures(inParam->sbf, inParam->fft->power, inParam->firstFrame);
        computeVADFeatures(inParam->vf, inParam->fft->power);
        //Combine the features for VAD decision
        for (i = 0; i < 2 * inParam->sbf->nBands - 2; i++) {
            inParam->allFeatureList[i] = inParam->sbf->subbandFeatureList[i];
            
        }
        for (i = 2 * inParam->sbf->nBands - 2; i < inParam->vf->nFeatures - 1; i++) {
            //Changes for 9 features
            inParam->allFeatureList[i] = inParam->vf->VADFeatureList[i - (2 * inParam->sbf->nBands - 2)];
        }
        //Changes for 9 features
        inParam->allFeatureList[inParam->nFeatures - 1] = inParam->vf->VADFeatureList[inParam->vf->nFeatures - 1];
        
        count++;
        if (count > inParam->warmup - 1) {

            evalVADTrees(inParam->vrf, inParam->allFeatureList);
            
            inParam->class1 = inParam->vrf->vadClassDecision;
            
            int i, noise = 0, speech = 0;
            
            for (i = smoothingBufferLength - 1; i > 0; i--) {
                smoothingBuffer[i] = smoothingBuffer[i-1];
            }
            smoothingBuffer[0] = inParam->vrf->vadClassDecision;
            
            for (i = 0; i < smoothingBufferLength; i++) {
                switch (smoothingBuffer[i]) {
                    case 1:
                        noise++;
                        break;
                    case 2:
                        speech++;
                        break;
                    default:
                        break;
                }
            }
            
            if(noise > speech){
                inParam->class1 = 1;
            }
            else if (speech > noise) {
                inParam->class1 = 2;
            }
            else {
                inParam->class1 = 1;
            }
            count = 0;
            inParam->warmup = inParam->decisionBufferLength;
        }
    }
    else {
        inParam->class1 = 0;
        count = 0;
    }
}

int getClass(long* memoryPointer)
{
    Variables* inParam = (Variables*)memoryPointer;
    
    return inParam->class1;
}

void copyArray(long* memoryPointer, float* array) {
    Variables* inParam = (Variables*)memoryPointer;
    
    int i;
    for (i = 0; i < inParam->nFeatures; i++)
    {
        array[i] = inParam->allFeatureList[i];
    }
    array[inParam->nFeatures]     = inParam->class1;
    array[inParam->nFeatures + 1] = inParam->vrf->vadClassDecision;
    array[inParam->nFeatures + 2] = inParam->fft->dbpower;
    
}

int getElements(long* memoryPointer) {
    Variables* inParam = (Variables*)memoryPointer;
    return inParam->nFeatures + 3;
}
float getdbPower(long* memoryPointer) {
    Variables* inParam = (Variables*)memoryPointer;
    return inParam->fft->dbpower;
}

