/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: crossoverFilter.h
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 28-Sep-2017 12:50:36
 */

#ifndef CROSSOVERFILTER_H
#define CROSSOVERFILTER_H

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
extern crossoverFilter *crossoverFilter_crossoverFilter(crossoverFilter *obj,
  const float varargin_2[4], float varargin_3, float varargin_5);
extern void crossoverFilter_stepImpl(crossoverFilter *obj, const float x[200],
  float varargout_1[200], float varargout_2[200], float varargout_3[200], float
  varargout_4[200], float varargout_5[200]);

#endif

/*
 * File trailer for crossoverFilter.h
 *
 * [EOF]
 */
