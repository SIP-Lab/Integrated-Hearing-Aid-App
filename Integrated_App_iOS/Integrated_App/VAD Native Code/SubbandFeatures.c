

#include <math.h>
#include "SubbandFeatures.h"
#include <stdio.h>
#include <stdlib.h>

/* Compute Subband Features of Band Periodicity and Band Entropy
   Use the FFT already calculated from previous operations as input
   1) Chop the FFT into M subbands - can probably use careful indexing
   	   for 1:M subbands
   2) Calculate the normalized correlation between current frame and previous frame
   3) Calculate band periodicity (Sum correlation peaks across N frames, and divide by N)
   4) Create frame histogram using a defined constant number of bins
   5) P_histbin = (count in bin)/(number of bins)
   6) Calculate frame entropy by summing across all the histogram bins in each subband: -sum(P_histbin * log2(P_histbin))
   7) Calculate band entropy (Sum entropy across N frames, and divide by N)

Assumptions: (need to float check)
   N_frames = decisionBufferLength

*/
static inline float entropy(float* in, int len);

	//SubBand parameters
	const int NBANDS=4;

    const int BP_BAND_CALC[4]={1, 1, 1, 1}; // Only calculate Band Periodicity for the subbands with a 1
    const int BE_BAND_CALC[4]={1, 1, 1, 1}; // Only calculate Band Entropy for the subbands with a 1

	//const int BP_BAND_CALC[8]={1, 1, 1, 1, 1, 1, 1, 1}; // Only calculate Band Periodicity for the subbands with a 1
	//const int BE_BAND_CALC[8]={1, 1, 1, 1, 1, 1, 1, 1}; // Only calculate Band Entropy for the subbands with a 1

SubbandFeatures* initSubbandFeatures(int nFFT, int stepsize, int decisionBufferLength)
{
	int b, i;
	int bpCalcBands=0;
	int beCalcBands=0;
	int bufferLength = decisionBufferLength; //Is this a correct assumption

	SubbandFeatures* newSubbandFeatures = (SubbandFeatures*)malloc(sizeof(SubbandFeatures));
	newSubbandFeatures->Normalize = 1/(float)decisionBufferLength;
	newSubbandFeatures->nFFT = nFFT/2;
	newSubbandFeatures->nBands = NBANDS;
	newSubbandFeatures->SubbandFftBufferA = (float*)calloc(nFFT,sizeof(float));
	newSubbandFeatures->SubbandFftBufferB = (float*)calloc(nFFT,sizeof(float));
	newSubbandFeatures->previousSubbandFft = newSubbandFeatures->SubbandFftBufferB;
	newSubbandFeatures->currentSubbandFft = newSubbandFeatures->SubbandFftBufferA;


	for (b=0;b<NBANDS;b++)
	{
		if (BP_BAND_CALC[b]==1) {bpCalcBands++;}
		if (BE_BAND_CALC[b]==1) {beCalcBands++;}
	}
	newSubbandFeatures->previousSubbandFftNorm = (float*)calloc(NBANDS,sizeof(float));
	//newSubbandFeatures->previousSubbandFftNorm = (float*)malloc(bpCalcBands,sizeof(float));
	newSubbandFeatures->bufferLength = bufferLength;
	// Create buffers to store a list of values from multiple frames for the purpose of averaging.
	newSubbandFeatures->framePeriodicityBuffer = (float**)malloc(bpCalcBands*sizeof(float*));
	for (i=0;i<bpCalcBands;i++)
	{
		newSubbandFeatures->framePeriodicityBuffer[i]=calloc(bufferLength,sizeof(float*)); // float* or float? //Tahsin
	}
	newSubbandFeatures->frameEntropyBuffer = (float**)malloc(beCalcBands*sizeof(float*));
	for (i=0;i<beCalcBands;i++)
	{
		newSubbandFeatures->frameEntropyBuffer[i]=calloc(bufferLength,sizeof(float*)); // float* or float? //Tahsin
	}
	// Maintain a pointer to the oldest frame's band features, for use in the moving average
	newSubbandFeatures->oldestBandFeature = 0;

	// Allocate a list of the latest frame's subband features. This is the input
	// to the random forest classifier.
	newSubbandFeatures->subbandFeatureList = (float*)calloc((bpCalcBands+beCalcBands),sizeof(float));
	// To make the indexing more clear in the Subband Feature calculation code,
	// create separate pointers to the band periodicity and band entropy in the
	// subbandFeatureList already allocated.
	newSubbandFeatures->bandPeriodicity = newSubbandFeatures->subbandFeatureList;
	newSubbandFeatures->bandEntropy = newSubbandFeatures->subbandFeatureList + bpCalcBands;

	newSubbandFeatures->bandPeriodicitySum = (float*)calloc((bpCalcBands),sizeof(float));
	newSubbandFeatures->bandEntropySum =  (float*)calloc((beCalcBands),sizeof(float));
	return newSubbandFeatures;
}

void computeSubbandFeatures(SubbandFeatures* SubbandFeature, float* frameFftPower, int test_first_frame)
{

    SubbandFeatures* sbf = SubbandFeature;
	//complexValues prev,curr,temp; //pointers to previous frame FFT and current frame FFT
	int b; // subband index
	int m; // frame fft bin index for correlation function input
	int k; // correlation function output index
	int i;
	int bpCalcBand,beCalcBand;

    int index1, index2; //current and previous index for correlation calculation
    int indx = (sbf->nFFT/sbf->nBands)-1;
    float cbn_k,cbn_kneg; // correlation function outputs
    float cbnk_array[indx];
    float cbnkneg_array[indx];
	//float cbn_temp; // correlation function output
	float cbn_max;
	float entropybn;

	float norm;
	//float norm_temp;
	float* previousFft = sbf->previousSubbandFft;
	float* currentFft = sbf->currentSubbandFft;
	//float* temp_pointer;

	float* previousSubbandNorm = sbf->previousSubbandFftNorm;



	// Copy input frame FFT power into the buffer
	// This section should be combined with the following section
	for(i=0;i<sbf->nFFT-1;i++)
	{
		*currentFft = sqrt(frameFftPower[i]);
		currentFft++;
	}

	if(test_first_frame==1)
	{
		for(i=0;i<sbf->nFFT-1;i++)
		{
			*previousFft = sqrt(frameFftPower[i]);
			previousFft++;
//			previousSubbandNorm[i/(sbf->nFFT/sbf->nBands)] += frameFftPower[i]*frameFftPower[i];
			previousSubbandNorm[i/(sbf->nFFT/sbf->nBands)] += frameFftPower[i];
		}

		for(i=0;i<sbf->nBands;i++)
		{
			previousSubbandNorm[i] = sqrt(previousSubbandNorm[i]);
		}

		//reset the pointer to the beginning
		previousFft = sbf->previousSubbandFft;
	}

	//reset the pointer to the beginning
	currentFft = sbf->currentSubbandFft;


	bpCalcBand=0;
	beCalcBand=0;
	for (b=0;b<sbf->nBands;b++)
	{
		if (BP_BAND_CALC[b]==1) // only calculate band features we plan to use
		{
			cbn_max = 0.0f;
			norm = 0.0f;
			//Calculate the correlation function for the current and previous frame FFTs
			for (k=0;k < sbf->nFFT/sbf->nBands; k++) //
			{
				cbn_k = 0.0f;
				cbn_kneg = 0.0f;
				// Do the left half of the correlation function
				// using cbn_kneg            _______________
				//                          |  Current FFT  |
				//                          |_______________|
				//                          _______________
				// start                   |  Previous FFT |
				// k=1                     |_______________|
				//             _______________
				//            |  Previous FFT |
				//            |_______________| <-----------
				//
				index1 = b*sbf->nFFT/sbf->nBands;
				index2 = b*sbf->nFFT/sbf->nBands + k;
				for (m=k;m<sbf->nFFT/sbf->nBands;m++)
				{
					//puts("index1=%d: currFFT:%f prevFFT:%f",index1,currentFft[index1],previousFft[index1]);
					cbn_kneg += currentFft[index1]*previousFft[index2];
					cbn_k    += currentFft[index2]*previousFft[index1];
					if (k==0)
					norm += currentFft[index1]*currentFft[index1]; // sum all squared inputs to get norm denominator factor
					index1++;
					index2++;
				}
				// Do the right half of the correlation function
				// using cbn_k_______________
				//           |  Current FFT  |
				//           |_______________|
				//            _______________
				// start     |  Previous FFT |
				// k=0       |_______________| ----------->
				//                           _______________
				// end                      |  Previous FFT |
				// k=nFFT/nBands            |_______________|

				cbnk_array[k] = cbn_k;
				cbnkneg_array[k] = cbn_kneg;
				// use the maximum of cbn_k and cbn_kneg to look for peak
				if (cbn_kneg > cbn_k) cbn_k = cbn_kneg;

				// Normalize the correlation function
				// Already summed squares, take square root here
				if (k==0)
				norm = sqrt(norm);
				cbn_k = cbn_k*(1/(1E-11 + norm*previousSubbandNorm[b]));
				if (cbn_k>cbn_max)
					cbn_max = cbn_k; // keep track of max cbn to get peak
//				if (k==0)
//					norm_temp = norm; //Save the norm for the next frame
			}
            
			previousSubbandNorm[b] = norm;
			// Multiply the cbn by 1/N
			//cbn_max = 1;
			//cbn_max = sbf->Normalize * cbn_max;

			// Remove the oldest cbn from the sum
			sbf->bandPeriodicitySum[b] -= sbf->framePeriodicityBuffer[bpCalcBand][sbf->oldestBandFeature];
			// Add the newest cbn to the sum
			sbf->bandPeriodicitySum[b] += cbn_max;
			// Remove the oldest cbn from the average
			//sbf->bandPeriodicity[b] -= sbf->framePeriodicityBuffer[bpCalcBand][sbf->oldestBandFeature];
			// Add the newest cbn to the average
			//sbf->bandPeriodicity[b] += cbn_max;
			// Store the newest cbn to the periodicity buffer
			sbf->framePeriodicityBuffer[bpCalcBand][sbf->oldestBandFeature]=cbn_max;

			// Multiply the cbn sum by 1/N to get the average
			sbf->bandPeriodicity[b] = sbf->bandPeriodicitySum[b] * sbf->Normalize;

			bpCalcBand++;
		}
        
		if (BE_BAND_CALC[b]==1) // only calculate band features we plan to use
		{
			entropybn = entropy(&currentFft[b*sbf->nFFT/sbf->nBands],sbf->nFFT/sbf->nBands);
			// Multiply frame entropy by 1/N
			//entropybn = sbf->Normalize * entropybn;
			//entropybn=1;

			// Remove the oldest entropy from the sum
			sbf->bandEntropySum[b] -= sbf->frameEntropyBuffer[beCalcBand][sbf->oldestBandFeature];
			// Remove the oldest entropy from the average
			//sbf->bandEntropy[b] -= sbf->frameEntropyBuffer[beCalcBand][sbf->oldestBandFeature];
			// Add the newest entropy to the sum
			sbf->bandEntropySum[b] += entropybn;
			// Add the newest entropy to the average
			//sbf->bandEntropy[b] += entropybn;
			// Store the newest entropy to the buffer
			sbf->frameEntropyBuffer[beCalcBand][sbf->oldestBandFeature] = entropybn;
			// Multiply the entropy sum by 1/N to get the average
			sbf->bandEntropy[b] = sbf->bandEntropySum[b] * sbf->Normalize;
			beCalcBand++;
		}
	}

	// Update the pointer in the subband features
	if (sbf->oldestBandFeature < sbf->bufferLength-2)
		sbf->oldestBandFeature++;
	else
		sbf->oldestBandFeature=0;

	// Update the current and previous buffer pointers so the oldest buffer gets overwritten
//	temp_pointer = currentFft;
//	currentFft   = previousFft;
//	previousFft  = temp_pointer;
	sbf->currentSubbandFft = previousFft;
	sbf->previousSubbandFft = currentFft;
}

void destroySubbandFeatures(SubbandFeatures** sbf)
{
	int bpCalcBands=0;
	int beCalcBands=0;
	int b;
	if(*sbf != NULL){
		for (b=0;b<NBANDS;b++)
		{
			if (BP_BAND_CALC[b]==1) {
				if((*sbf)->framePeriodicityBuffer[bpCalcBands] != NULL){
					free((*sbf)->framePeriodicityBuffer[bpCalcBands]);
					(*sbf)->framePeriodicityBuffer[bpCalcBands]=NULL;
				}
				bpCalcBands++;
			}
			if (BE_BAND_CALC[b]==1) {
				if((*sbf)->frameEntropyBuffer[beCalcBands] != NULL){
					free((*sbf)->frameEntropyBuffer[beCalcBands]);
					(*sbf)->frameEntropyBuffer[beCalcBands]=NULL;
				}
				beCalcBands++;
			}
		}
		if((*sbf)->framePeriodicityBuffer != NULL){
			free((*sbf)->framePeriodicityBuffer);
			(*sbf)->framePeriodicityBuffer = NULL;
		}
		if((*sbf)->frameEntropyBuffer != NULL){
			free((*sbf)->frameEntropyBuffer);
			(*sbf)->frameEntropyBuffer = NULL;
		}
		if((*sbf)->SubbandFftBufferA != NULL){
			free((*sbf)->SubbandFftBufferA);
			(*sbf)->SubbandFftBufferA = NULL;
		}
		if((*sbf)->SubbandFftBufferB != NULL){
			free((*sbf)->SubbandFftBufferB);
			(*sbf)->SubbandFftBufferB = NULL;
		}
//		if((*sbf)->previousSubbandFft != NULL){
//			free((*sbf)->previousSubbandFft);
//			(*sbf)->previousSubbandFft = NULL;
//		}
//		if((*sbf)->currentSubbandFft != NULL){
//			free((*sbf)->currentSubbandFft);
//			(*sbf)->currentSubbandFft = NULL;
//		}
		if((*sbf)->subbandFeatureList != NULL){
			free((*sbf)->subbandFeatureList);
			(*sbf)->subbandFeatureList = NULL;
		}
//		if((*sbf)->bandPeriodicity != NULL){
//			free((*sbf)->bandPeriodicity);
//			(*sbf)->bandPeriodicity = NULL;
//		}
//		if((*sbf)->bandEntropy != NULL){
//			free((*sbf)->bandEntropy);
//			(*sbf)->bandEntropy = NULL;
//		}
		if((*sbf)->bandPeriodicitySum != NULL){
			free((*sbf)->bandPeriodicitySum);
			(*sbf)->bandPeriodicitySum = NULL;
		}
		if((*sbf)->bandEntropySum != NULL){
			free((*sbf)->bandEntropySum);
			(*sbf)->bandEntropySum = NULL;
		}
		if((*sbf)->previousSubbandFftNorm != NULL){
			free((*sbf)->previousSubbandFftNorm);
			(*sbf)->previousSubbandFftNorm = NULL;
		}

		free(*sbf);
		*sbf=NULL;

	}
}

static inline float entropy(float* in, int len)
{
	int i;
	int bin;
	float H=0.0f;
	float interval;
	int   p[len];
	float probability;
	float max = in[0];
	float min = in[0];
	//float recip_length = 1.0f/(float)len;
	// Find the range of the data
	// and initialize the histogram to 0
	for (i=0;i<len;i++)
	{
		if (max < in[i]) max = in[i];
		if (min > in[i]) min = in[i];
		p[i]=0;
	}
	// Make a histogram of the data
	interval = (max - min)/(float)(len);
    if (interval == 0) {
        interval = 1E-11;
    }
	for (i=0;i<len;i++)
	{
		bin =(int)((in[i]-min)/interval);
		if (bin==len) bin=bin-1; // catch the max value in the maximum bin
		p[bin]++;
	}

	for (i=0;i<len;i++)
	{
		float temp;
		//-sum(P_histbin * log2(P_histbin))
		//H-=(float)p[i]*recip_length*logf(p[i])/0.63915; // using ln(p[i])/ln(2); ln(2) = 0.63915
		if (p[i]>0) // skip any bins that are 0
		{
			probability = (float)p[i]/(float)len;             // probability of this bin
			//temp = log2f( probability);
			temp = logf(probability)/logf(2);
			H-=probability * temp ;
		}

	}
	return H;
}
