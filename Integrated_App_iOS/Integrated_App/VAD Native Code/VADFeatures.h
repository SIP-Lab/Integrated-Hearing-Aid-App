//
//  VADFeatures.h
//  VAD
//
//  Created by Sehgal, Abhishek on 2/8/16.
//  Copyright © 2016 SIPLab. All rights reserved.
//

#ifndef VADFeatures_h
#define VADFeatures_h

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
    Feature Arrangement for the VAD:

    NBANDS = 4

    1       Mean of 1st Band
    2,3,4   Difference PSD
    5,6,7,8 Slope of PSD
    9       Spectral Flux

 */


typedef struct VADFeatures {

    float Normalize;
    int nFFT;
    int nFeatures;
    int nBands;
    int bufferLength;
    int firstFrame;
    int oldestPosition;

    float* previousSubbandFFT;
    float* currentSubbandFFT;

    float* PSD;
    float** PSDBuffer;
    float* PSDBufferSum;

    float** featureBuffer;
    float* featureBufferSum;
    float* minEnergyBuffer;


    float spectralFlux;

    float* VADFeatureList;

} VADFeatures;

VADFeatures* initVADFeatures(int nFFT, int decisionBufferLength);
void computeVADFeatures(VADFeatures* VADFeature, float* frameFftPower);
void destroyVADFeatures(VADFeatures** VADFeature);


#endif /* VADFeatures_h */
