/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: filter.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 28-Sep-2017 12:50:36
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "DynamicRangeFiveBandCompression.h"
#include "filter.h"

/* Function Definitions */

/*
 * Arguments    : float b[3]
 *                float a[3]
 *                const float x[200]
 *                const float zi[2]
 *                float y[200]
 *                float zf[2]
 * Return Type  : void
 */
void filter(float b[3], float a[3], const float x[200], const float zi[2], float
            y[200], float zf[2])
{
  float a1;
  int k;
  int naxpy;
  int j;
  a1 = a[0];
  if ((!rtIsInfF(a[0])) && (!rtIsNaNF(a[0])) && (!(a[0] == 0.0F)) && (a[0] !=
       1.0F)) {
    for (k = 0; k < 3; k++) {
      b[k] /= a1;
    }

    for (k = 0; k < 2; k++) {
      a[k + 1] /= a1;
    }

    a[0] = 1.0F;
  }

  for (k = 0; k < 2; k++) {
    zf[k] = 0.0F;
    y[k] = zi[k];
  }

  memset(&y[2], 0, 198U * sizeof(float));
  for (k = 0; k < 200; k++) {
    naxpy = 200 - k;
    if (!(naxpy < 3)) {
      naxpy = 3;
    }

    for (j = 0; j + 1 <= naxpy; j++) {
      y[k + j] += x[k] * b[j];
    }

    naxpy = 199 - k;
    if (!(naxpy < 2)) {
      naxpy = 2;
    }

    a1 = -y[k];
    for (j = 1; j <= naxpy; j++) {
      y[k + j] += a1 * a[j];
    }
  }

  for (k = 0; k < 2; k++) {
    for (j = 0; j + 1 <= k + 1; j++) {
      zf[j] += x[k + 198] * b[(j - k) + 2];
    }
  }

  for (k = 0; k < 2; k++) {
    for (j = 0; j + 1 <= k + 1; j++) {
      zf[j] += -y[k + 198] * a[(j - k) + 2];
    }
  }
}

/*
 * File trailer for filter.c
 *
 * [EOF]
 */
