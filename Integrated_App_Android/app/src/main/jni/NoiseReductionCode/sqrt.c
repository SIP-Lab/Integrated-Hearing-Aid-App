/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: sqrt.c
 *
 * MATLAB Coder version            : 3.4
 * C/C++ source code generated on  : 06-Oct-2017 12:42:28
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "wiener_ADAPTIVE_NE.h"
#include "sqrt.h"

/* Function Definitions */

/*
 * Arguments    : float x[512]
 * Return Type  : void
 */
void b_sqrt(float x[512])
{
  int k;
  for (k = 0; k < 512; k++) {
    x[k] = (float)sqrt(x[k]);
  }
}

/*
 * File trailer for sqrt.c
 *
 * [EOF]
 */
