

#ifndef SUBBANDFEATURES_H_
#define SUBBANDFEATURES_H_
#define _USE_MATH_DEFINES

#include <stdlib.h>
#include <math.h>
//#include "Constants.h"

#ifdef LOGCAT_DEBUG
	#include <android/log.h>
#endif

typedef struct SubbandFeatures {
	float  Normalize;
	int    nFFT;
	int    nBands;
	float* SubbandFftBufferA;
	float* SubbandFftBufferB;
	float* previousSubbandFft;
	float* currentSubbandFft;
	float*  previousSubbandFftNorm;
	float** framePeriodicityBuffer;
	float** frameEntropyBuffer;
	int    oldestBandFeature;
	int    bufferLength;
	float*  subbandFeatureList;
	float*  bandPeriodicity;
	float*  bandEntropy;
	float*  bandPeriodicitySum; //Sum of all the max correlations
	float*  bandEntropySum; //Sum of all the entropies

} SubbandFeatures;

SubbandFeatures* initSubbandFeatures(int nFFT,int stepsize, int decisionBufferLength);
void computeSubbandFeatures(SubbandFeatures* SubbandFeature, float* frameFftPower, int test_first_frame);
void destroySubbandFeatures(SubbandFeatures** SubbandFeature);


#endif /* SUBBANDFEATURES_H_ */
