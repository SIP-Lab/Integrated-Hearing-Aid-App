/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: compressor.h
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 28-Sep-2017 12:50:36
 */

#ifndef COMPRESSOR_H
#define COMPRESSOR_H

/* Include Files */
#include <float.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "DynamicRangeFiveBandCompression_types.h"

/* Function Declarations */
extern compressor *compressor_compressor(compressor *obj, float varargin_2,
  float varargin_4, float varargin_6, float varargin_10, float varargin_12,
  float varargin_16);
extern void compressor_computeGain(const compressor *obj, const float xG[200],
  float G[200]);

#endif

/*
 * File trailer for compressor.h
 *
 * [EOF]
 */
