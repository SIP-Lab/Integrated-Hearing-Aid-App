/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: tuneCrossoverFilterCoefficients.h
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 28-Sep-2017 12:50:36
 */

#ifndef TUNECROSSOVERFILTERCOEFFICIENTS_H
#define TUNECROSSOVERFILTERCOEFFICIENTS_H

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
extern void tuneCrossoverFilterCoefficients(float freq, float order, float Fs,
  float BL[12], float AL[12], float BH[12], float AH[12], float *phaseMult);

#endif

/*
 * File trailer for tuneCrossoverFilterCoefficients.h
 *
 * [EOF]
 */
