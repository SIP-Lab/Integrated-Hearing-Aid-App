/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: rdivide.c
 *
 * MATLAB Coder version            : 3.4
 * C/C++ source code generated on  : 06-Oct-2017 12:42:28
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "wiener_ADAPTIVE_NE.h"
#include "rdivide.h"

/* Function Definitions */

/*
 * Arguments    : const float x[512]
 *                const emxArray_real32_T *y
 *                float z[512]
 * Return Type  : void
 */
void rdivide(const float x[512], const emxArray_real32_T *y, float z[512])
{
  int i0;
  for (i0 = 0; i0 < 512; i0++) {
    z[i0] = x[i0] / y->data[i0];
  }
}

/*
 * File trailer for rdivide.c
 *
 * [EOF]
 */
