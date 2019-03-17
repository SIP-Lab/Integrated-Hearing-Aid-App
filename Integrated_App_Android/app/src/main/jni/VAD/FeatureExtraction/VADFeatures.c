//
//  VADFeatures.c
//  VAD
//
//  Created by Sehgal, Abhishek on 2/8/16.
//  Copyright © 2016 SIPLab. All rights reserved.
//

#include "VADFeatures.h"
#define NBANDS 4
//#include <android/log.h>


VADFeatures* initVADFeatures(int nFFT, int decisionBufferLength) {

    VADFeatures* newVADFeatures = (VADFeatures*)malloc(sizeof(VADFeatures));
    int i;

    newVADFeatures->Normalize   = 1.0/decisionBufferLength;
    newVADFeatures->nBands      = NBANDS;
    newVADFeatures->nFFT        = nFFT/2;
    newVADFeatures->nFeatures   = 1 + (2 * NBANDS);
    newVADFeatures->bufferLength = decisionBufferLength;
    newVADFeatures->firstFrame  = 1;
    newVADFeatures->oldestPosition = 0;

    newVADFeatures->previousSubbandFFT = (float*)calloc(nFFT, sizeof(float));
    newVADFeatures->currentSubbandFFT = (float*)calloc(nFFT, sizeof(float));

    newVADFeatures->PSD = (float*)calloc(nFFT, sizeof(float));
    newVADFeatures->PSDBufferSum = (float*)calloc(nFFT, sizeof(float));
    newVADFeatures->PSDBuffer = (float**)malloc(newVADFeatures->bufferLength * sizeof(float*));
    for ( i = 0; i < newVADFeatures->bufferLength; i++ ) {
        newVADFeatures->PSDBuffer[i] = (float*)calloc(newVADFeatures->nFFT, sizeof(float));
    }

    newVADFeatures->featureBufferSum = (float*)calloc(newVADFeatures->nFeatures,sizeof(float));

    newVADFeatures->featureBuffer = (float**)malloc(newVADFeatures->nFeatures * sizeof(float*));
    //The decision buffer that holds all the features and all the values
    for (i = 0; i < newVADFeatures->nFeatures; i++) {
        newVADFeatures->featureBuffer[i] = (float*)calloc(decisionBufferLength, sizeof(float));
        //__android_log_print(ANDROID_LOG_ERROR,"VAD Initialization","Done %d %f", i, newVADFeatures->featureBuffer[i][0]);
    }



    newVADFeatures->VADFeatureList = (float*)calloc(newVADFeatures->nFeatures, sizeof(float));
    newVADFeatures->minEnergyBuffer = (float*)calloc(newVADFeatures->bufferLength, sizeof(float));

    return newVADFeatures;

}

void computeVADFeatures(VADFeatures* VADFeature, float* frameFftPower) {

    VADFeatures* vf = VADFeature;

    int i,b,k;
    float prod = 0;
    float minEng = 0, mean = 0, sum_deviation = 0, std = 0, diff = 0;
    float average = 0, minimum = 0;
    int bandwidth = 0;
    vf->spectralFlux = 0;

    //Store current frame FFT as the current FFT
    vf->currentSubbandFFT[0] = sqrt(frameFftPower[0]);
    vf->PSDBufferSum[0] -= vf->PSDBuffer[vf->oldestPosition][0];
    vf->PSDBufferSum[0] += frameFftPower[0] + 0.001;
    vf->PSDBuffer[vf->oldestPosition][0] = frameFftPower[0] + 0.001;
    vf->PSD[0] = 10 * log10(vf->Normalize * vf->PSDBufferSum[0]);

    for (i = 1; i < vf->nFFT - 1; i++) {
        vf->currentSubbandFFT[i] = sqrt(frameFftPower[i]);
        vf->PSDBufferSum[i] -= vf->PSDBuffer[vf->oldestPosition][i];
        vf->PSDBufferSum[i] += 2 * frameFftPower[i] + 0.001;
        vf->PSDBuffer[vf->oldestPosition][i] = 2 * frameFftPower[i] + 0.001;
        vf->PSD[i] = 10 * log10(vf->Normalize * vf->PSDBufferSum[i]);
    }


    //If it's the first frame, store the current FFT as the previous one
    if (vf->firstFrame == 1) {
        for (i = 0; i < vf->nFFT - 1; i++) {
            vf->previousSubbandFFT[i] = sqrt(frameFftPower[i]);
        }
    }

    for (i = 0; i < vf->nFFT-1; i++) {
        prod = log(vf->currentSubbandFFT[i] + 0.001) - log(vf->previousSubbandFFT[i] + 0.001);
        vf->spectralFlux += prod * prod;
    }

    vf->spectralFlux = vf->spectralFlux/vf->nFFT;

    //__android_log_print(ANDROID_LOG_ERROR,"Spectral Flux","Done %f %d", vf->featureBuffer[6][vf->oldestPosition], vf->nFeatures);


    //Store the spectral flux as the last feature in the feature list
    vf->featureBufferSum[vf->nFeatures - 1] -= vf->featureBuffer[vf->nFeatures - 1][vf->oldestPosition];
    vf->featureBufferSum[vf->nFeatures - 1] += vf->spectralFlux;
    vf->featureBuffer[vf->nFeatures - 1][vf->oldestPosition] = vf->spectralFlux;
    vf->VADFeatureList[vf->nFeatures - 1] = vf->featureBufferSum[vf->nFeatures - 1] * vf->Normalize;

    //printf("%f, %f, %f, %d \n", vf->spectralFlux, vf->featureBufferSum[vf->nFeatures - 1],vf->featureBufferSum[vf->nFeatures - 1] * vf->Normalize, count++);

    //__android_log_print(ANDROID_LOG_ERROR,"Spectral Flux","Done");


    //Traverse over the number of bands
    for (b = 0; b < vf->nBands; b++) {

        minEng = 0;
        mean = 0;
        sum_deviation = 0;
        std = 0;
        diff = 0;
        bandwidth = vf->nFFT/vf->nBands;

        //Find the energy in the first band
        if (b == 0) {

            for (k = 0; k < bandwidth; k++) {
                //Stores the energy of the 1st band
                minEng += frameFftPower[k];
                mean += vf->PSD[k];
            }
            //Calculate the mean of the frame
            mean = mean/(bandwidth);

            //Calculate the standard deviation of the frame
            for (k = 0; k < (bandwidth); k++) {
                sum_deviation += (vf->PSD[k] - mean) * (vf->PSD[k] - mean);
            }

            //store the standard deviation of the frame
            std = sqrt(sum_deviation/(bandwidth));

            //__android_log_print(ANDROID_LOG_ERROR,"First Energy","Started");

            //Store the Energy of the 1st Band as the first feature in the list
            vf->featureBufferSum[b] -= vf->featureBuffer[b][vf->oldestPosition];
            vf->featureBufferSum[b] += minEng;
            vf->featureBuffer[b][vf->oldestPosition] = minEng;
            vf->minEnergyBuffer[vf->oldestPosition] = minEng;

            minimum = vf->minEnergyBuffer[0];
            //printf("%f,",vf->minEnergyBuffer[0]);
            for(i = 1; i < vf->bufferLength; i++)	{
                if ( vf->minEnergyBuffer[i] < minimum )	{
                    minimum = vf->minEnergyBuffer[i];
                }
                //printf("%f,",vf->minEnergyBuffer[i]);
            }
            //printf("\n");

            average = vf->featureBufferSum[b] * vf->Normalize;



            vf->VADFeatureList[b] = (average - minimum)/(average + 1e-11);

            //__android_log_print(ANDROID_LOG_ERROR, "Minimum Energy", "%f, %f", minimum, vf->VADFeatureList[b]);

            //__android_log_print(ANDROID_LOG_ERROR,"First Energy","Done");

            //Store the slope of the PSD as the Nth feature in the list
            vf->featureBufferSum[b+NBANDS] -= vf->featureBuffer[b+NBANDS][vf->oldestPosition];
            vf->featureBufferSum[b+NBANDS] += std;
            vf->featureBuffer[b+NBANDS][vf->oldestPosition] = std;
            vf->VADFeatureList[b+NBANDS] = vf->featureBufferSum[b+NBANDS] * vf->Normalize;

            //__android_log_print(ANDROID_LOG_ERROR,"PSD Slope","Done");



        }

        else {

            //Traverse over the FFT samples in the band to calculate PSD features
            for (k = b * (bandwidth); k < ((b+1) * (bandwidth)); k++) {
                diff += vf->PSD[k] - vf->PSD[k - (bandwidth)];
                mean += vf->PSD[k];

            }

            //Calculate the mean of the frame
            mean = mean/(bandwidth);
            

            //Calculate the standard deviation of the frame
            for (k = b * (bandwidth); k < ((b+1) * (bandwidth)); k++) {

                sum_deviation += (vf->PSD[k]-mean)*(vf->PSD[k]-mean);
            }

            //store the standard deviation of the frame
            std = sqrt(sum_deviation/(bandwidth));

            //mean of the PSD Difference
            diff = -diff/(bandwidth);

            //Store the Difference PSD as the bth feature in the list
            vf->featureBufferSum[b] -= vf->featureBuffer[b][vf->oldestPosition];
            vf->featureBufferSum[b] += diff;
            vf->featureBuffer[b][vf->oldestPosition] = diff;
            vf->VADFeatureList[b] = vf->featureBufferSum[b] * vf->Normalize;

            //__android_log_print(ANDROID_LOG_ERROR,"PSD Difference","Done");



            //Store the slope of the PSD as the Nth feature in the list
            vf->featureBufferSum[b+NBANDS] -= vf->featureBuffer[b+NBANDS][vf->oldestPosition];
            vf->featureBufferSum[b+NBANDS] += std;
            vf->featureBuffer[b+NBANDS][vf->oldestPosition] = std;
            vf->VADFeatureList[b+NBANDS] = vf->featureBufferSum[b+NBANDS] * vf->Normalize;

            //__android_log_print(ANDROID_LOG_ERROR,"PSD Slope","Done");
        }

    }

    if(vf->oldestPosition <= vf->bufferLength - 2)
        vf->oldestPosition++;
    else
        vf->oldestPosition = 0;

    //Store the current band as the previous band
    for (i = 0; i < vf->nFFT - 1; i++) {

        vf->previousSubbandFFT[i] = vf->currentSubbandFFT[i];

    }
    vf->firstFrame  = 0;





    /*
     for (i = 0; i < vf->nFeatures; i++) {
     printf("%f, ", vf->VADFeatureList[i]);
     }
     printf("\n");
     */
}

void destroyVADFeatures(VADFeatures** vf) {

    int i;
    if (*vf != NULL) {

        for (i = 0; i < (*vf)->nFeatures; i++) {

            if ((*vf)->featureBuffer[i] != NULL) {
                free((*vf)->featureBuffer[i]);
                (*vf)->featureBuffer[i] = NULL;
            }
        }

        if ((*vf)->featureBuffer != NULL) {
            free((*vf)->featureBuffer);
            (*vf)->featureBuffer = NULL;
        }

        if ((*vf)->previousSubbandFFT != NULL) {
            free((*vf)->previousSubbandFFT);
            (*vf)->previousSubbandFFT = NULL;
        }

        if ((*vf)->currentSubbandFFT != NULL) {
            free((*vf)->currentSubbandFFT);
            (*vf)->currentSubbandFFT = NULL;
        }

        if ((*vf)->PSD != NULL) {
            free((*vf)->PSD);
            (*vf)->PSD = NULL;
        }

        if ((*vf)->featureBufferSum != NULL) {
            free((*vf)->featureBufferSum);
            (*vf)->featureBufferSum = NULL;
        }

        if ((*vf)->VADFeatureList != NULL) {
            free((*vf)->VADFeatureList);
            (*vf)->VADFeatureList = NULL;
        }

        free(*vf);
        *vf = NULL;
    }

}
