//
//  SpeechProcessing.h
//  SPP RF+SB
//
//  Created by Sehgal, Abhishek on 5/4/15.
//  Copyright (c) 2015 UT Dallas. All rights reserved.
//

#ifndef __Speech_Processing_Pipeline__SpeechProcessing__
#define __Speech_Processing_Pipeline__SpeechProcessing__

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "Transforms.h"
#include "SubbandFeatures.h"
#include "VADFeatures.h"
#include "VADRandomForest.h"

typedef struct Variables {
    Transform* fft;
    SubbandFeatures* sbf;
    VADFeatures* vf;
    VADRandomForests* vrf;
    float* inputBuffer;
    float* outputBuffer;
    float* noiseEstimate;//noiseEstimate;
    short* originalInput;
    float* allFeatureList;
    int nFeatures;
    int frequency;
    int stepSize;
    int windowSize;
    int overlap;
    int class1;
    int firstFrame;
    int decisionBufferLength;
    int warmup;
    
} Variables;

long* initializeVADParams(int frequency, int stepsize, int windowSize, int decisionBufferLength, float decisionRate);
void compute(long* memoryPointer, float* input, float quiet, float calibration);
int getClass(long* memoryPointer);
void copyArray(long* memoryPointer, float* array);
int getElements(long* memoryPointer);
float getdbPower(long* memoryPointer);

#endif /* defined(__Speech_Processing_Pipeline__SpeechProcessing__) */
