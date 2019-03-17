/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: crossoverFilter.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 28-Sep-2017 12:50:36
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "DynamicRangeFiveBandCompression.h"
#include "crossoverFilter.h"
#include "FiveBandCrossoverFilter.h"
#include "SystemCore.h"

/* Function Declarations */
static void b_crossoverFilter_crossoverFilt(crossoverFilter **obj, const float
  varargin_2[4], float varargin_3, float varargin_5);
static double rt_roundd_snf(double u);

/* Function Definitions */

/*
 * Arguments    : crossoverFilter **obj
 *                const float varargin_2[4]
 *                float varargin_3
 *                float varargin_5
 * Return Type  : void
 */
static void b_crossoverFilter_crossoverFilt(crossoverFilter **obj, const float
  varargin_2[4], float varargin_3, float varargin_5)
{
  int b_index;
  static const short iv1[4] = { 100, 800, 1500, 3000 };

  crossoverFilter *b_obj;
  boolean_T flag;
  double A;
  (*obj)->pNumChannels = -1.0;
  for (b_index = 0; b_index < 4; b_index++) {
    (*obj)->pFreq[b_index] = iv1[b_index];
  }

  for (b_index = 0; b_index < 4; b_index++) {
    (*obj)->pSlopes[b_index] = 12.0;
  }

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
  flag = (b_obj->isInitialized == 1);
  if (flag) {
    b_obj->TunablePropsChanged = true;
  }

  for (b_index = 0; b_index < 4; b_index++) {
    b_obj->pFreq[b_index] = varargin_2[b_index];
  }

  flag = (b_obj->isInitialized == 1);
  if (flag) {
    b_obj->TunablePropsChanged = true;
  }

  for (b_index = 0; b_index < 4; b_index++) {
    b_obj->pSlopes[b_index] = varargin_3;
  }

  for (b_index = 0; b_index < 4; b_index++) {
    A = b_obj->pSlopes[b_index];
    A = 6.0 * rt_roundd_snf(A / 6.0);
    if (!(A < 48.0)) {
      A = 48.0;
    }

    b_obj->pSlopes[b_index] = A;
    if (b_obj->pSlopes[b_index] == 0.0) {
      b_obj->pSlopes[b_index] = 6.0;
    }
  }

  flag = (b_obj->isInitialized == 1);
  if (flag) {
    b_obj->TunablePropsChanged = true;
  }

  b_obj->pSampleRateDialog = varargin_5;
}

/*
 * Arguments    : double u
 * Return Type  : double
 */
static double rt_roundd_snf(double u)
{
  double y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }

  return y;
}

/*
 * Arguments    : crossoverFilter *obj
 *                const float varargin_2[4]
 *                float varargin_3
 *                float varargin_5
 * Return Type  : crossoverFilter *
 */
crossoverFilter *crossoverFilter_crossoverFilter(crossoverFilter *obj, const
  float varargin_2[4], float varargin_3, float varargin_5)
{
  crossoverFilter *b_obj;
  b_obj = obj;
  b_crossoverFilter_crossoverFilt(&b_obj, varargin_2, varargin_3, varargin_5);
  return b_obj;
}

/*
 * Arguments    : crossoverFilter *obj
 *                const float x[200]
 *                float varargout_1[200]
 *                float varargout_2[200]
 *                float varargout_3[200]
 *                float varargout_4[200]
 *                float varargout_5[200]
 * Return Type  : void
 */
void crossoverFilter_stepImpl(crossoverFilter *obj, const float x[200], float
  varargout_1[200], float varargout_2[200], float varargout_3[200], float
  varargout_4[200], float varargout_5[200])
{
  c_audio_internal_FiveBandCrosso *b_obj;
  b_obj = &obj->pFilter;
  if (b_obj->isInitialized != 1) {
    SystemCore_setup(b_obj);
    c_FiveBandCrossoverFilter_reset(b_obj);
  }

  if (b_obj->TunablePropsChanged) {
    b_obj->TunablePropsChanged = false;
    c_FiveBandCrossoverFilter_tuneC(b_obj);
  }

  c_FiveBandCrossoverFilter_stepI(b_obj, x, varargout_1, varargout_2,
    varargout_3, varargout_4, varargout_5);
}

/*
 * File trailer for crossoverFilter.c
 *
 * [EOF]
 */
