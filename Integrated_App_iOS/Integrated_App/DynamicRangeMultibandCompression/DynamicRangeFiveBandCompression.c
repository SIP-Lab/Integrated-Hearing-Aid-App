/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: DynamicRangeFiveBandCompression.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 28-Sep-2017 12:50:36
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "DynamicRangeFiveBandCompression.h"
#include "SystemCore.h"
#include "compressor.h"
#include "crossoverFilter.h"
//#include <stdio.h>

/* Variable Definitions */
static crossoverFilter crossover;
static boolean_T crossover_not_empty;
static float crossOverFreq[4];
static compressor dRC1;
 boolean_T dRC1_not_empty;
static compressor dRC2;
 boolean_T dRC2_not_empty;
static compressor dRC3;
 boolean_T dRC3_not_empty;
static compressor dRC4;
 boolean_T dRC4_not_empty;
static compressor dRC5;
 boolean_T dRC5_not_empty;

/* Function Definitions */

/*
 * Arguments    : const float b_signal[200]
 *                float Fs
 *                const float data[20]
 *                float slope
 *                float mg
 *                float y[200]
 * Return Type  : void
 */
void DynamicRangeFiveBandCompression(const float b_signal[200], float Fs, const
  float data[20], float slope, float mg, float y[200])
{
  float a5[200];
  float a4[200];
  float y3[200];
  float y4[200];
  float y5[200];
  float a2[200];
  float a3[200];
  int i0;

  /*  Resample (downloanSample */
  /* nFs = 16000; */
  /* pnSample = nSample; */
  /* signal = lowpassfilter(signal); */
  /* signal = downsample(signal, 3); */
  /* signal = decimate(signal, 3, 'fir'); */
  /* nSample = length(signal); */
  /* persistent hammingD; */
  /* if(isempty(hammingD)) */
  /*     hammingD = hamming(lengthBuffer); */
  /* end */
  /* buffer = buffer.*hammingD; */
  /* may need some changes for buffer/overlap for input signal */
  /*  */
  /*    */
  if (!crossover_not_empty) {
    crossoverFilter_crossoverFilter(&crossover, crossOverFreq, slope, Fs);
    crossover_not_empty = true;
  }

  SystemCore_parenReference(&crossover, b_signal, a5, a4, y3, y4, y5);
  if (!dRC1_not_empty) {
    compressor_compressor(&dRC1, Fs, data[0], data[1], data[2]/1000, data[3]/1000, mg);
     // printf("%f %f %f %f\n", data[0], data[1], data[2]/1000, data[3]/1000);
    dRC1_not_empty = true;
  }

  if (!dRC2_not_empty) {
    compressor_compressor(&dRC2, Fs, data[4], data[5], data[6]/1000, data[7]/1000, mg);
    dRC2_not_empty = true;
    //  printf("%f %f %f %f\n", data[4], data[5], data[6]/1000, data[7]/1000);
      
  }

  if (!dRC3_not_empty) {
    compressor_compressor(&dRC3, Fs, data[8], data[9], data[10]/1000, data[11]/1000, mg);
    dRC3_not_empty = true;
    //  printf("%f %f %f %f\n", data[8], data[9], data[10]/1000, data[11]/1000);
  }

  if (!dRC4_not_empty) {
    compressor_compressor(&dRC4, Fs, data[12], data[13], data[14]/1000, data[15]/1000, mg);
    dRC4_not_empty = true;
    //  printf("%f %f %f %f\n", data[12], data[13], data[14]/1000, data[15]/1000);
  }

  if (!dRC5_not_empty) {
    compressor_compressor(&dRC5, Fs, data[16], data[17], data[18]/1000, data[19]/1000, mg);
    dRC5_not_empty = true;
   //   printf("%f %f %f %f\n", data[16], data[17], data[18]/1000, data[19]/1000);
  }

  b_SystemCore_parenReference(&dRC1, a5, y);
  b_SystemCore_parenReference(&dRC2, a4, a2);
  b_SystemCore_parenReference(&dRC3, y3, a3);
  b_SystemCore_parenReference(&dRC4, y4, a4);
  b_SystemCore_parenReference(&dRC5, y5, a5);

  /*  New audio output */
  for (i0 = 0; i0 < 200; i0++) {
    y[i0] = (((y[i0] + a2[i0]) + a3[i0]) + a4[i0]) + a5[i0];
  }

  /*  upsample data */
  /* y = resample(y, Fs, nFs); */
  /* y = upsample(y, 3); */
  /* y = lowpassfilter(y); */
  /* y = y(1:pnSample); */
  /* y = interp(y, 3); */
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void c_DynamicRangeFiveBandCompressi(void)
{
  int i8;
  static const short iv0[4] = { 500, 1000, 2000, 4000 };

  for (i8 = 0; i8 < 4; i8++) {
    crossOverFreq[i8] = iv0[i8];
  }

  dRC5_not_empty = false;
  dRC4_not_empty = false;
  dRC3_not_empty = false;
  dRC2_not_empty = false;
  dRC1_not_empty = false;
  crossover_not_empty = false;

  /*   */
  /* [375,1125,2250,4500]% for midfreq [250,750,1500,3000,6000] */
  /* [375, 625, 875, 1250, 1750, 2500, 3500, 5000] % for midfreq [250, 500,750,1000,1500,2000,3000,4000,6000] */
}

/*
 * File trailer for DynamicRangeFiveBandCompression.c
 *
 * [EOF]
 */
