/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: sortIdx.h
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 28-Sep-2017 12:50:36
 */

#ifndef SORTIDX_H
#define SORTIDX_H

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
extern void merge(int idx[4], double x[4], int offset, int np, int nq, int
                  iwork[4], double xwork[4]);

#endif

/*
 * File trailer for sortIdx.h
 *
 * [EOF]
 */
