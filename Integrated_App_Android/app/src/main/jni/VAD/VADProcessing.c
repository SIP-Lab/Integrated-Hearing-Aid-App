//
//  VADProcessing.c
//  SPP RF+SB
//
//  Created by Sehgal, Abhishek on 5/4/15.
//  Modified by Tahsin Chowdhury on 11/02/17 for integration
//  Copyright (c) 2015 UT Dallas. All rights reserved.
//

#include "VADProcessing.h"
#include <unistd.h>

static int count = 0;
//
static int* smoothingBuffer;
static int smoothingBufferLength;

int noiseCount = 0;
inline int resolution8k(int samplingFrequency, int frameSize){
    int pow2Size = 0x01;
    while (pow2Size < frameSize)
    {
        pow2Size = pow2Size << 1;
    }
    float binResolution = (float) samplingFrequency/pow2Size;
    return (int)lround(16000.0/binResolution);
}


long* initializeVADParams(int frequency, int stepsize, int windowsize, int decisionBufferLength, float decisionRate)
{
    Variables* inParam = (Variables*) malloc(sizeof(Variables));
    // initialize SubbandFeatures
    //inParam->sbf = initSubbandFeatures(inParam->fft->points, stepsize, decisionBufferLength);
    //inParam->vf  = initVADFeatures(inParam->fft->points, decisionBufferLength);
    inParam->sbf = initSubbandFeatures(resolution8k(frequency,windowsize), stepsize, decisionBufferLength);
    inParam->vf  = initVADFeatures(resolution8k(frequency,windowsize), decisionBufferLength);
    inParam->vrf = initVADRandomForest(decisionBufferLength);
    inParam->classVAD = -1;
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


int compute(long* memoryPointer, Transform* trans, float quiet)
{
    Variables* inParam = (Variables*)memoryPointer;
    Transform* fft = trans;
    int i;
    
    if (fft->dbpower > quiet) {
        
/***....................Feature Extraction Starts...................***/
        
        computeSubbandFeatures(inParam->sbf, fft->power, inParam->firstFrame);
        computeVADFeatures(inParam->vf, fft->power);
        
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
        
/***....................Feature Extraction Ends.......................***/

        count++;
        if (count > inParam->warmup - 1) {
            
/***....................RF1:VAD Starts............................***/
            
            evalVADTrees(inParam->vrf, inParam->allFeatureList);
            VADDecisionSmoothing(inParam);
            
/***.....................RF1:VAD ENDs.............................***/
            count = 0;
            inParam->warmup = inParam->decisionBufferLength;
        }
    }
    else {
        inParam->classVAD = 0;
        count = 0;
    }
    return inParam->classVAD;
}

void VADDecisionSmoothing(Variables* vars) {
    Variables* inParam = vars;
    inParam->classVAD = inParam->vrf->vadClassDecision;
    
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
        inParam->classVAD = 1;
    }
    else if (speech > noise) {
        inParam->classVAD = 2;
    }
    else {
        inParam->classVAD = 1;
    }
}

int getClass(long* memoryPointer)
{
    Variables* inParam = (Variables*)memoryPointer;
    return inParam->classVAD;
}

void destroyVadParams(long** memoryPointer) {
    Variables** var = (Variables**)memoryPointer;
    if(*var != NULL) {
        if((*var)->allFeatureList !=NULL) {
            free((*var)->allFeatureList);
            (*var)->allFeatureList = NULL;
        }
        
        destroySubbandFeatures(&(*var)->sbf);
        destroyVADFeatures(&(*var)->vf);
        destroyVADRandomForest(&(*var)->vrf);
        free(*var);
        *var = NULL;
    }
    if(smoothingBuffer!=NULL) {
        free(smoothingBuffer);
        smoothingBuffer = NULL;
    }
}
