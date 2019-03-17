/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: designLPHPFilter.h
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 28-Sep-2017 12:50:36
 */

#ifndef DESIGNLPHPFILTER_H
#define DESIGNLPHPFILTER_H

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
extern void b_designLPHPFilter(double N, float BW, float B_data[], int B_size[2],
  float A_data[], int A_size[2]);
extern void designLPHPFilter(double N, float BW, float B_data[], int B_size[2],
  float A_data[], int A_size[2]);

#endif

/*
 * File trailer for designLPHPFilter.h
 *
 * [EOF]
 */
