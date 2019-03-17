/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: abs.c
 *
 * MATLAB Coder version            : 3.4
 * C/C++ source code generated on  : 06-Oct-2017 12:42:28
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "abs.h"
#include "rtwutil.h"
/* Function Definitions */

/*
 * Arguments    : const creal32_T x[400]
 *                float y[400]
 * Return Type  : void
 */
void b_abs(const creal32_T x[400], float y[400])
{
  int k;
  for (k = 0; k < 400; k++) {
    y[k] = rt_hypotf_snf(x[k].re, x[k].im);
  }
}

/*
 * Arguments    : const creal32_T x[512]
 *                float y[512]
 * Return Type  : void
 */
void c_abs(const creal32_T x[512], float y[512])
{
  int k;
  for (k = 0; k < 512; k++) {
    y[k] = rt_hypotf_snf(x[k].re, x[k].im);
  }
}

/* Function Definitions */

/*
 * Arguments    : const float x[200]
 *                float y[200]
 * Return Type  : void
 */
void b_abs_c(const float x[200], float y[200])
{
  int k;
  for (k = 0; k < 200; k++) {
    y[k] = (float)fabs(x[k]);
  }
}


/*
 * File trailer for abs.c
 *
 * [EOF]
 */
