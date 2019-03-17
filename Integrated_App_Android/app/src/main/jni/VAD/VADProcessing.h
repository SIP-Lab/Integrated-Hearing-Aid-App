//
//  VADProcessing.h
//  SPP RF+SB
//
//  Created by Sehgal, Abhishek on 5/4/15.
//  Modified by Chowdhury, Tahsin on 11/02/17 for integration
//  Copyright (c) 2015 UT Dallas. All rights reserved.
//

#ifndef VAD_PROCESSING_H
#define VAD_PROCESSING_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "Transforms.h"
#include "SubbandFeatures.h"
#include "VADFeatures.h"
#include "VADRandomForest.h"

typedef struct Variables {
    SubbandFeatures* sbf;
    VADFeatures* vf;
    VADRandomForests* vrf;
    float* allFeatureList;
    int nFeatures;
    int classVAD;
    int firstFrame;
    int decisionBufferLength;
    int warmup;
    
} Variables;

long* initializeVADParams(int frequency, int stepsize, int windowSize, int decisionBufferLength, float decisionRate);
int compute(long* memoryPointer, Transform* trans, float quiet);
int getClass(long* memoryPointer);
void destroyVadParams(long** memoryPointer);
void VADDecisionSmoothing(Variables* vars);

#endif //VAD_PROCESSING_H

