/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: abs.h
 *
 * MATLAB Coder version            : 3.4
 * C/C++ source code generated on  : 06-Oct-2017 12:42:28
 */

#ifndef ABS_H
#define ABS_H

/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "wiener_ADAPTIVE_NE_types.h"
#include "DynamicRangeFiveBandCompression_types.h"

/* Function Declarations */
extern void b_abs(const creal32_T x[400], float y[400]);
extern void c_abs(const creal32_T x[512], float y[512]);
extern void b_abs_c(const float x[200], float y[200]);
#endif

/*
 * File trailer for abs.h
 *
 * [EOF]
 */
