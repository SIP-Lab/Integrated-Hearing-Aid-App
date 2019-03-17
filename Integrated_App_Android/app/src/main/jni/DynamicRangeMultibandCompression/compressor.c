/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: compressor.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 28-Sep-2017 12:50:36
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "DynamicRangeFiveBandCompression.h"
#include "compressor.h"
#include "power.h"
#include "abs.h"

/* Function Declarations */
static void b_compressor_compressor(compressor **obj, float varargin_2, float
  varargin_4, float varargin_6, float varargin_10, float varargin_12, float
  varargin_16);

/* Function Definitions */

/*
 * Arguments    : compressor **obj
 *                float varargin_2
 *                float varargin_4
 *                float varargin_6
 *                float varargin_10
 *                float varargin_12
 *                float varargin_16
 * Return Type  : void
 */
static void b_compressor_compressor(compressor **obj, float varargin_2, float
  varargin_4, float varargin_6, float varargin_10, float varargin_12, float
  varargin_16)
{
  compressor *b_obj;
  boolean_T flag;
  b_obj = *obj;
  *obj = b_obj;
  b_obj = *obj;
  *obj = b_obj;
  (*obj)->pNumChannels = -1.0;
  b_obj = *obj;
  *obj = b_obj;
  b_obj = *obj;
  *obj = b_obj;
  b_obj = *obj;
  *obj = b_obj;
  b_obj = *obj;
  *obj = b_obj;
  b_obj = *obj;
  *obj = b_obj;
  b_obj = *obj;
  *obj = b_obj;
  (*obj)->isInitialized = 0;
  b_obj = *obj;
  *obj = b_obj;
  b_obj = *obj;
  *obj = b_obj;
  b_obj = *obj;
  *obj = b_obj;
  b_obj = *obj;
  *obj = b_obj;
  b_obj = *obj;
  *obj = b_obj;
  b_obj = *obj;
  *obj = b_obj;
  b_obj = *obj;
  *obj = b_obj;
  b_obj = *obj;
  flag = (b_obj->isInitialized == 1);
  if (flag) {
    b_obj->TunablePropsChanged = true;
  }

  b_obj->pSampleRateDialog = varargin_2;
  flag = (b_obj->isInitialized == 1);
  if (flag) {
    b_obj->TunablePropsChanged = true;
  }

  b_obj->Ratio = varargin_4;
  flag = (b_obj->isInitialized == 1);
  if (flag) {
    b_obj->TunablePropsChanged = true;
  }

  b_obj->Threshold = varargin_6;
  flag = (b_obj->isInitialized == 1);
  if (flag) {
    b_obj->TunablePropsChanged = true;
  }

  b_obj->KneeWidth = 5.0F;
  flag = (b_obj->isInitialized == 1);
  if (flag) {
    b_obj->TunablePropsChanged = true;
  }

  b_obj->AttackTime = varargin_10;
  flag = (b_obj->isInitialized == 1);
  if (flag) {
    b_obj->TunablePropsChanged = true;
  }

  b_obj->ReleaseTime = varargin_12;
  flag = (b_obj->isInitialized == 1);
  if (flag) {
    b_obj->TunablePropsChanged = true;
  }

  b_obj->MakeUpGain = varargin_16;
}

/*
 * Arguments    : compressor *obj
 *                float varargin_2
 *                float varargin_4
 *                float varargin_6
 *                float varargin_10
 *                float varargin_12
 *                float varargin_16
 * Return Type  : compressor *
 */
compressor *compressor_compressor(compressor *obj, float varargin_2, float
  varargin_4, float varargin_6, float varargin_10, float varargin_12, float
  varargin_16)
{
  compressor *b_obj;
  b_obj = obj;
  b_compressor_compressor(&b_obj, varargin_2, varargin_4, varargin_6,
    varargin_10, varargin_12, varargin_16);
  return b_obj;
}

/*
 * Arguments    : const compressor *obj
 *                const float xG[200]
 *                float G[200]
 * Return Type  : void
 */
void compressor_computeGain(const compressor *obj, const float xG[200], float G
  [200])
{
  float W;
  float R;
  float T;
  int trueCount;
  int i;
  int partialTrueCount;
  boolean_T ind2;
  boolean_T b_ind2[200];
  unsigned char tmp_data[200];
  float b_data[200];
  float b_xG[200];
  float y;
  unsigned char b_tmp_data[200];
  int xG_size[1];
  int b_size[1];
  W = obj->KneeWidth;
  R = obj->Ratio;
  T = obj->Threshold;
  trueCount = 0;
  for (i = 0; i < 200; i++) {
    G[i] = xG[i];
    ind2 = (2.0F * (xG[i] - T) > W);
    if (ind2) {
      trueCount++;
    }

    b_ind2[i] = ind2;
  }

  partialTrueCount = 0;
  for (i = 0; i < 200; i++) {
    if (b_ind2[i]) {
      tmp_data[partialTrueCount] = (unsigned char)(i + 1);
      partialTrueCount++;
    }
  }

  for (partialTrueCount = 0; partialTrueCount < trueCount; partialTrueCount++) {
    b_data[partialTrueCount] = (xG[tmp_data[partialTrueCount] - 1] - T) / R;
  }

  partialTrueCount = 0;
  for (i = 0; i < 200; i++) {
    if (b_ind2[i]) {
      G[i] = T + b_data[partialTrueCount];
      partialTrueCount++;
    }
  }

  if (W != 0.0F) {
    for (i = 0; i < 200; i++) {
      b_xG[i] = xG[i] - T;
    }

    b_abs_c(b_xG, b_data);
    trueCount = 0;
    for (i = 0; i < 200; i++) {
      ind2 = (2.0F * b_data[i] <= W);
      if (ind2) {
        trueCount++;
      }

      b_ind2[i] = ind2;
    }

    partialTrueCount = 0;
    for (i = 0; i < 200; i++) {
      if (b_ind2[i]) {
        b_tmp_data[partialTrueCount] = (unsigned char)(i + 1);
        partialTrueCount++;
      }
    }

    y = W / 2.0F;
    R = 1.0F / R - 1.0F;
    xG_size[0] = trueCount;
    for (partialTrueCount = 0; partialTrueCount < trueCount; partialTrueCount++)
    {
      b_xG[partialTrueCount] = (xG[b_tmp_data[partialTrueCount] - 1] - T) + y;
    }

    power_c(b_xG, xG_size, b_data, b_size);
    y = 2.0F * W;
    i = b_size[0];
    for (partialTrueCount = 0; partialTrueCount < i; partialTrueCount++) {
      b_data[partialTrueCount] = R * b_data[partialTrueCount] / y;
    }

    partialTrueCount = 0;
    for (i = 0; i < 200; i++) {
      if (b_ind2[i]) {
        G[i] = xG[i] + b_data[partialTrueCount];
        partialTrueCount++;
      }
    }
  }

  for (partialTrueCount = 0; partialTrueCount < 200; partialTrueCount++) {
    G[partialTrueCount] -= xG[partialTrueCount];
  }
}

/*
 * File trailer for compressor.c
 *
 * [EOF]
 */
