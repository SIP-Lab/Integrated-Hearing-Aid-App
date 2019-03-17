/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: DynamicRangeFiveBandCompression.h
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 28-Sep-2017 12:50:36
 */

#ifndef DYNAMICRANGEFIVEBANDCOMPRESSION_H
#define DYNAMICRANGEFIVEBANDCOMPRESSION_H

/* Include Files */
#include <float.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "DynamicRangeFiveBandCompression_types.h"

extern boolean_T dRC1_not_empty;
extern boolean_T dRC2_not_empty;
extern boolean_T dRC3_not_empty;
extern boolean_T dRC4_not_empty;
extern boolean_T dRC5_not_empty;

/* Function Declarations */
extern void DynamicRangeFiveBandCompression(const float b_signal[200], float Fs,
  const float data[20], float slope, float mg, float y[200]);
extern void c_DynamicRangeFiveBandCompressi(void);

#endif

/*
 * File trailer for DynamicRangeFiveBandCompression.h
 *
 * [EOF]
 */
