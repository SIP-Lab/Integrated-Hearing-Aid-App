/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: DynamicRangeFiveBandCompression_types.h
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 28-Sep-2017 12:50:36
 */

#ifndef DYNAMICRANGEFIVEBANDCOMPRESSION_TYPES_H
#define DYNAMICRANGEFIVEBANDCOMPRESSION_TYPES_H

/* Include Files */
#include "rtwtypes.h"

/* Type Definitions */
#ifndef typedef_c_audio_internal_FiveBandCrosso
#define typedef_c_audio_internal_FiveBandCrosso

typedef struct {
  int isInitialized;
  boolean_T TunablePropsChanged;
  float CrossoverFrequencies[4];
  float CrossoverOrders[4];
  float SampleRate;
  float pCR1_B_LP[12];
  float pCR1_A_LP[12];
  float pCR1_B_HP[12];
  float pCR1_A_HP[12];
  float pCR2_B_LP[12];
  float pCR2_A_LP[12];
  float pCR2_B_HP[12];
  float pCR2_A_HP[12];
  float pCR3_B_LP[12];
  float pCR3_A_LP[12];
  float pCR3_B_HP[12];
  float pCR3_A_HP[12];
  float pCR4_B_LP[12];
  float pCR4_A_LP[12];
  float pCR4_B_HP[12];
  float pCR4_A_HP[12];
  float pPhaseMult[4];
  float pCR1_States_LP[8];
  float pCR1_States_HP[8];
  float pCR2_States_LP[8];
  float pCR2_States_HP[8];
  float pCR3_States_LP[8];
  float pCR3_States_HP[8];
  float pCR4_States_LP[8];
  float pCR4_States_HP[8];
  float pAP1_States_LP[8];
  float pAP1_States_HP[8];
  float pAP2_States_LP[8];
  float pAP2_States_HP[8];
  float pAP3_States_LP[8];
  float pAP3_States_HP[8];
  float pAP4_States_LP[8];
  float pAP4_States_HP[8];
} c_audio_internal_FiveBandCrosso;

#endif                                 /*typedef_c_audio_internal_FiveBandCrosso*/

#ifndef typedef_cell_wrap_3
#define typedef_cell_wrap_3

typedef struct {
  unsigned int f1[8];
} cell_wrap_3;

#endif                                 /*typedef_cell_wrap_3*/

#ifndef typedef_compressor
#define typedef_compressor

typedef struct {
  int isInitialized;
  boolean_T TunablePropsChanged;
  cell_wrap_3 inputVarSize[1];
  float pSampleRateDialog;
  float Threshold;
  float AttackTime;
  float ReleaseTime;
  double pNumChannels;
  float pAlphaA;
  float pAlphaR;
  float pLevelDetectionState;
  float MakeUpGain;
  float KneeWidth;
  float pMakeUpGain;
  float Ratio;
} compressor;

#endif                                 /*typedef_compressor*/

#ifndef typedef_crossoverFilter
#define typedef_crossoverFilter

typedef struct {
  int isInitialized;
  boolean_T TunablePropsChanged;
  cell_wrap_3 inputVarSize[1];
  float pSampleRateDialog;
  float pOrders[4];
  float pCrossoverFrequencies[4];
  double pNumChannels;
  double pFreq[4];
  double pSlopes[4];
  c_audio_internal_FiveBandCrosso pFilter;
} crossoverFilter;

#endif                                 /*typedef_crossoverFilter*/
#endif

/*
 * File trailer for DynamicRangeFiveBandCompression_types.h
 *
 * [EOF]
 */
