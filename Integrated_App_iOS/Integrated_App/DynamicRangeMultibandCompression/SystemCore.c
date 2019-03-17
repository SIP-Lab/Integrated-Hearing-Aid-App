/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: SystemCore.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 28-Sep-2017 12:50:36
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "DynamicRangeFiveBandCompression.h"
#include "SystemCore.h"
#include "compressor.h"
#include "FiveBandCrossoverFilter.h"
#include "repmat.h"
#include "crossoverFilter.h"
#include "sort1.h"
#include "DynamicRangeFiveBandCompression_rtwutil.h"

/* Function Declarations */
static void SystemCore_checkTunableProps(crossoverFilter *obj);
static void SystemCore_setupAndReset(crossoverFilter *obj);
static boolean_T c_SystemCore_detectInputSizeCha(crossoverFilter *obj);

/* Function Definitions */

/*
 * Arguments    : crossoverFilter *obj
 * Return Type  : void
 */
static void SystemCore_checkTunableProps(crossoverFilter *obj)
{
  crossoverFilter *b_obj;
  int b_index;
  double freq[4];
  int iidx[4];
  float FsByTwo;
  float freqVect[4];
  float F1;
  boolean_T flag;
  if (obj->TunablePropsChanged) {
    obj->TunablePropsChanged = false;
    b_obj = obj;
    for (b_index = 0; b_index < 4; b_index++) {
      freq[b_index] = b_obj->pFreq[b_index];
    }

    sort(freq, iidx);
    for (b_index = 0; b_index < 4; b_index++) {
      FsByTwo = b_obj->pSampleRateDialog;
      FsByTwo /= 2.0F;
      F1 = (float)freq[b_index];
      if ((float)freq[b_index] >= FsByTwo) {
        F1 = FsByTwo - 1.0F;
      }

      if ((float)freq[b_index] == 0.0F) {
        F1 = 1.0F;
      }

      freqVect[b_index] = F1;
    }

    for (b_index = 0; b_index < 4; b_index++) {
      b_obj->pCrossoverFrequencies[b_index] = freqVect[b_index];
    }

    for (b_index = 0; b_index < 4; b_index++) {
      freq[b_index] = b_obj->pSlopes[b_index];
    }

    for (b_index = 0; b_index < 4; b_index++) {
      b_obj->pOrders[b_index] = (float)freq[b_index] / 6.0F;
    }

    for (b_index = 0; b_index < 4; b_index++) {
      freqVect[b_index] = b_obj->pOrders[iidx[b_index] - 1];
    }

    for (b_index = 0; b_index < 4; b_index++) {
      b_obj->pOrders[b_index] = freqVect[b_index];
    }

    flag = (b_obj->pFilter.isInitialized == 1);
    if (flag) {
      b_obj->pFilter.TunablePropsChanged = true;
    }

    for (b_index = 0; b_index < 4; b_index++) {
      b_obj->pFilter.CrossoverFrequencies[b_index] =
        b_obj->pCrossoverFrequencies[b_index];
    }

    flag = (b_obj->pFilter.isInitialized == 1);
    if (flag) {
      b_obj->pFilter.TunablePropsChanged = true;
    }

    for (b_index = 0; b_index < 4; b_index++) {
      b_obj->pFilter.CrossoverOrders[b_index] = b_obj->pOrders[b_index];
    }

    flag = (b_obj->pFilter.isInitialized == 1);
    if (flag) {
      b_obj->pFilter.TunablePropsChanged = true;
    }

    FsByTwo = b_obj->pSampleRateDialog;
    b_obj->pFilter.SampleRate = FsByTwo;
  }
}

/*
 * Arguments    : crossoverFilter *obj
 * Return Type  : void
 */
static void SystemCore_setupAndReset(crossoverFilter *obj)
{
  crossoverFilter *b_obj;
  int b_index;
  cell_wrap_3 varSizes[1];
  static const unsigned char uv0[8] = { 200U, 1U, 1U, 1U, 1U, 1U, 1U, 1U };

  double freq[4];
  int iidx[4];
  float F0;
  float FsByTwo;
  float F1;
  float c_obj[4];
  boolean_T flag;
  static const short varargin_4[4] = { 100, 300, 400, 500 };

  b_obj = obj;
  b_obj->isInitialized = 1;
  for (b_index = 0; b_index < 8; b_index++) {
    varSizes[0].f1[b_index] = uv0[b_index];
  }

  b_obj->inputVarSize[0] = varSizes[0];
  b_obj->pNumChannels = 1.0;
  for (b_index = 0; b_index < 4; b_index++) {
    freq[b_index] = b_obj->pFreq[b_index];
  }

  sort(freq, iidx);
  for (b_index = 0; b_index < 4; b_index++) {
    b_obj->pCrossoverFrequencies[b_index] = (float)freq[b_index];
  }

  for (b_index = 0; b_index < 4; b_index++) {
    F0 = b_obj->pCrossoverFrequencies[b_index];
    FsByTwo = b_obj->pSampleRateDialog;
    FsByTwo /= 2.0F;
    F1 = F0;
    if (F0 >= FsByTwo) {
      F1 = FsByTwo - 1.0F;
    }

    if (F0 == 0.0F) {
      F1 = 1.0F;
    }

    b_obj->pCrossoverFrequencies[b_index] = F1;
  }

  for (b_index = 0; b_index < 4; b_index++) {
    freq[b_index] = b_obj->pSlopes[b_index];
  }

  for (b_index = 0; b_index < 4; b_index++) {
    b_obj->pOrders[b_index] = (float)freq[b_index] / 6.0F;
  }

  for (b_index = 0; b_index < 4; b_index++) {
    c_obj[b_index] = b_obj->pOrders[iidx[b_index] - 1];
  }

  for (b_index = 0; b_index < 4; b_index++) {
    b_obj->pOrders[b_index] = c_obj[b_index];
  }

  FsByTwo = b_obj->pSampleRateDialog;
  b_obj->pFilter.isInitialized = 0;
  flag = (b_obj->pFilter.isInitialized == 1);
  if (flag) {
    b_obj->pFilter.TunablePropsChanged = true;
  }

  b_obj->pFilter.SampleRate = FsByTwo;
  flag = (b_obj->pFilter.isInitialized == 1);
  if (flag) {
    b_obj->pFilter.TunablePropsChanged = true;
  }

  for (b_index = 0; b_index < 4; b_index++) {
    b_obj->pFilter.CrossoverFrequencies[b_index] = varargin_4[b_index];
  }

  flag = (b_obj->pFilter.isInitialized == 1);
  if (flag) {
    b_obj->pFilter.TunablePropsChanged = true;
  }

  for (b_index = 0; b_index < 4; b_index++) {
    b_obj->pFilter.CrossoverOrders[b_index] = 2.0F;
  }

  SystemCore_setup(&b_obj->pFilter);
  flag = (b_obj->pFilter.isInitialized == 1);
  if (flag) {
    b_obj->pFilter.TunablePropsChanged = true;
  }

  FsByTwo = b_obj->pSampleRateDialog;
  b_obj->pFilter.SampleRate = FsByTwo;
  flag = (b_obj->pFilter.isInitialized == 1);
  if (flag) {
    b_obj->pFilter.TunablePropsChanged = true;
  }

  for (b_index = 0; b_index < 4; b_index++) {
    b_obj->pFilter.CrossoverFrequencies[b_index] = b_obj->
      pCrossoverFrequencies[b_index];
  }

  flag = (b_obj->pFilter.isInitialized == 1);
  if (flag) {
    b_obj->pFilter.TunablePropsChanged = true;
  }

  for (b_index = 0; b_index < 4; b_index++) {
    b_obj->pFilter.CrossoverOrders[b_index] = b_obj->pOrders[b_index];
  }

  if (b_obj->pFilter.isInitialized == 1) {
    c_FiveBandCrossoverFilter_reset(&b_obj->pFilter);
  }

  b_obj->TunablePropsChanged = false;
  b_obj = obj;
  flag = (b_obj->pFilter.isInitialized == 1);
  if (flag) {
    b_obj->pFilter.TunablePropsChanged = true;
  }

  FsByTwo = b_obj->pSampleRateDialog;
  b_obj->pFilter.SampleRate = FsByTwo;
  flag = (b_obj->pFilter.isInitialized == 1);
  if (flag) {
    b_obj->pFilter.TunablePropsChanged = true;
  }

  for (b_index = 0; b_index < 4; b_index++) {
    b_obj->pFilter.CrossoverFrequencies[b_index] = b_obj->
      pCrossoverFrequencies[b_index];
  }

  flag = (b_obj->pFilter.isInitialized == 1);
  if (flag) {
    b_obj->pFilter.TunablePropsChanged = true;
  }

  for (b_index = 0; b_index < 4; b_index++) {
    b_obj->pFilter.CrossoverOrders[b_index] = b_obj->pOrders[b_index];
  }

  if (b_obj->pFilter.isInitialized == 1) {
    c_FiveBandCrossoverFilter_reset(&b_obj->pFilter);
  }
}

/*
 * Arguments    : crossoverFilter *obj
 * Return Type  : boolean_T
 */
static boolean_T c_SystemCore_detectInputSizeCha(crossoverFilter *obj)
{
  boolean_T anyInputSizeChanged;
  int k;
  boolean_T exitg1;
  static const unsigned char uv1[8] = { 200U, 1U, 1U, 1U, 1U, 1U, 1U, 1U };

  static const unsigned char inSize[8] = { 200U, 1U, 1U, 1U, 1U, 1U, 1U, 1U };

  anyInputSizeChanged = false;
  k = 0;
  exitg1 = false;
  while ((!exitg1) && (k < 8)) {
    if (obj->inputVarSize[0].f1[k] != uv1[k]) {
      anyInputSizeChanged = true;
      for (k = 0; k < 8; k++) {
        obj->inputVarSize[0].f1[k] = inSize[k];
      }

      exitg1 = true;
    } else {
      k++;
    }
  }

  return anyInputSizeChanged;
}

/*
 * Arguments    : crossoverFilter *obj
 *                const float varargin_1[200]
 *                float varargout_1[200]
 *                float varargout_2[200]
 *                float varargout_3[200]
 *                float varargout_4[200]
 *                float varargout_5[200]
 * Return Type  : void
 */
void SystemCore_parenReference(crossoverFilter *obj, const float varargin_1[200],
  float varargout_1[200], float varargout_2[200], float varargout_3[200], float
  varargout_4[200], float varargout_5[200])
{
  crossoverFilter *b_obj;
  b_obj = obj;
  if (b_obj->isInitialized != 1) {
    SystemCore_setupAndReset(b_obj);
  }

  SystemCore_checkTunableProps(b_obj);
  c_SystemCore_detectInputSizeCha(b_obj);
  crossoverFilter_stepImpl(b_obj, varargin_1, varargout_1, varargout_2,
    varargout_3, varargout_4, varargout_5);
}

/*
 * Arguments    : c_audio_internal_FiveBandCrosso *obj
 * Return Type  : void
 */
void SystemCore_setup(c_audio_internal_FiveBandCrosso *obj)
{
  c_audio_internal_FiveBandCrosso *b_obj;
  double dv0[12];
  int i1;
  obj->isInitialized = 1;
  b_obj = obj;
  repmat(dv0);
  for (i1 = 0; i1 < 12; i1++) {
    b_obj->pCR1_B_LP[i1] = (float)dv0[i1];
  }

  repmat(dv0);
  for (i1 = 0; i1 < 12; i1++) {
    b_obj->pCR1_A_LP[i1] = (float)dv0[i1];
  }

  repmat(dv0);
  for (i1 = 0; i1 < 12; i1++) {
    b_obj->pCR1_B_HP[i1] = (float)dv0[i1];
  }

  repmat(dv0);
  for (i1 = 0; i1 < 12; i1++) {
    b_obj->pCR1_A_HP[i1] = (float)dv0[i1];
  }

  repmat(dv0);
  for (i1 = 0; i1 < 12; i1++) {
    b_obj->pCR2_B_LP[i1] = (float)dv0[i1];
  }

  repmat(dv0);
  for (i1 = 0; i1 < 12; i1++) {
    b_obj->pCR2_A_LP[i1] = (float)dv0[i1];
  }

  repmat(dv0);
  for (i1 = 0; i1 < 12; i1++) {
    b_obj->pCR2_B_HP[i1] = (float)dv0[i1];
  }

  repmat(dv0);
  for (i1 = 0; i1 < 12; i1++) {
    b_obj->pCR2_A_HP[i1] = (float)dv0[i1];
  }

  repmat(dv0);
  for (i1 = 0; i1 < 12; i1++) {
    b_obj->pCR3_B_LP[i1] = (float)dv0[i1];
  }

  repmat(dv0);
  for (i1 = 0; i1 < 12; i1++) {
    b_obj->pCR3_A_LP[i1] = (float)dv0[i1];
  }

  repmat(dv0);
  for (i1 = 0; i1 < 12; i1++) {
    b_obj->pCR3_B_HP[i1] = (float)dv0[i1];
  }

  repmat(dv0);
  for (i1 = 0; i1 < 12; i1++) {
    b_obj->pCR3_A_HP[i1] = (float)dv0[i1];
  }

  repmat(dv0);
  for (i1 = 0; i1 < 12; i1++) {
    b_obj->pCR4_B_LP[i1] = (float)dv0[i1];
  }

  repmat(dv0);
  for (i1 = 0; i1 < 12; i1++) {
    b_obj->pCR4_A_LP[i1] = (float)dv0[i1];
  }

  repmat(dv0);
  for (i1 = 0; i1 < 12; i1++) {
    b_obj->pCR4_B_HP[i1] = (float)dv0[i1];
  }

  repmat(dv0);
  for (i1 = 0; i1 < 12; i1++) {
    b_obj->pCR4_A_HP[i1] = (float)dv0[i1];
  }

  for (i1 = 0; i1 < 4; i1++) {
    b_obj->pPhaseMult[i1] = 0.0F;
  }

  c_FiveBandCrossoverFilter_tuneC(b_obj);
  obj->TunablePropsChanged = false;
}

/*
 * Arguments    : compressor *obj
 *                const float varargin_1[200]
 *                float varargout_1[200]
 * Return Type  : void
 */
void b_SystemCore_parenReference(compressor *obj, const float varargin_1[200],
  float varargout_1[200])
{
  compressor *b_obj;
  int k;
  float Fs;
  cell_wrap_3 varSizes[1];
  static const unsigned char inSize[8] = { 200U, 1U, 1U, 1U, 1U, 1U, 1U, 1U };

  boolean_T exitg1;
  float alphaR;
  static const unsigned char uv2[8] = { 200U, 1U, 1U, 1U, 1U, 1U, 1U, 1U };

  float fv0[200];
  float G[200];
  float y[201];
  b_obj = obj;
  if (b_obj->isInitialized != 1) {
    b_obj->isInitialized = 1;
    for (k = 0; k < 8; k++) {
      varSizes[0].f1[k] = inSize[k];
    }

    b_obj->inputVarSize[0] = varSizes[0];
    b_obj->pLevelDetectionState = 0.0F;
    Fs = b_obj->pSampleRateDialog;
    alphaR = b_obj->AttackTime * Fs;
    b_obj->pAlphaA = (float)exp(-2.19722462F / alphaR);
    alphaR = b_obj->ReleaseTime * Fs;
    b_obj->pAlphaR = (float)exp(-2.19722462F / alphaR);
    b_obj->pNumChannels = 1.0;
    b_obj->pMakeUpGain = b_obj->MakeUpGain;
    b_obj->TunablePropsChanged = false;
    b_obj->pLevelDetectionState = 0.0F;
    Fs = b_obj->pSampleRateDialog;
    alphaR = b_obj->AttackTime * Fs;
    b_obj->pAlphaA = (float)exp(-2.19722462F / alphaR);
    alphaR = b_obj->ReleaseTime * Fs;
    b_obj->pAlphaR = (float)exp(-2.19722462F / alphaR);
  }

  if (b_obj->TunablePropsChanged) {
    b_obj->TunablePropsChanged = false;
    Fs = b_obj->pSampleRateDialog;
    alphaR = b_obj->AttackTime * Fs;
    b_obj->pAlphaA = (float)exp(-2.19722462F / alphaR);
    alphaR = b_obj->ReleaseTime * Fs;
    b_obj->pAlphaR = (float)exp(-2.19722462F / alphaR);
    b_obj->pMakeUpGain = b_obj->MakeUpGain;
  }

  k = 0;
  exitg1 = false;
  while ((!exitg1) && (k < 8)) {
    if (b_obj->inputVarSize[0].f1[k] != uv2[k]) {
      for (k = 0; k < 8; k++) {
        b_obj->inputVarSize[0].f1[k] = inSize[k];
      }

      exitg1 = true;
    } else {
      k++;
    }
  }

  for (k = 0; k < 200; k++) {
    Fs = (float)fabs(varargin_1[k]);
    if (!(Fs > 1.1920929E-7F)) {
      Fs = 1.1920929E-7F;
    }

    fv0[k] = 20.0F * (float)log10(Fs);
  }

  compressor_computeGain(b_obj, fv0, G);
  memset(&y[0], 0, 201U * sizeof(float));
  y[0] = b_obj->pLevelDetectionState;
  Fs = b_obj->pAlphaA;
  alphaR = b_obj->pAlphaR;
  for (k = 0; k < 200; k++) {
    if (G[k] <= y[k]) {
      y[k + 1] = Fs * y[k] + (1.0F - Fs) * G[k];
    } else {
      y[k + 1] = alphaR * y[k] + (1.0F - alphaR) * G[k];
    }

    G[k] = y[k + 1];
  }

  b_obj->pLevelDetectionState = y[200];
  for (k = 0; k < 200; k++) {
    Fs = G[k] + b_obj->pMakeUpGain;
    Fs /= 20.0F;
    varargout_1[k] = varargin_1[k] * rt_powf_snf(10.0F, Fs);
  }
}

/*
 * File trailer for SystemCore.c
 *
 * [EOF]
 */
