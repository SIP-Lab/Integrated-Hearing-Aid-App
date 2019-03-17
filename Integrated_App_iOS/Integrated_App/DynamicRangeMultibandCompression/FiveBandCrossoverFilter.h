/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: FiveBandCrossoverFilter.h
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 28-Sep-2017 12:50:36
 */

#ifndef FIVEBANDCROSSOVERFILTER_H
#define FIVEBANDCROSSOVERFILTER_H

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
extern void c_FiveBandCrossoverFilter_reset(c_audio_internal_FiveBandCrosso *obj);
extern void c_FiveBandCrossoverFilter_stepI(c_audio_internal_FiveBandCrosso *obj,
  const float x[200], float z1[200], float z2[200], float z3[200], float z4[200],
  float z5[200]);
extern void c_FiveBandCrossoverFilter_tuneC(c_audio_internal_FiveBandCrosso *obj);

#endif

/*
 * File trailer for FiveBandCrossoverFilter.h
 *
 * [EOF]
 */
