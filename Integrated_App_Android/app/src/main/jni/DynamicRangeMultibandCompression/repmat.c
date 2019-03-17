/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: repmat.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 28-Sep-2017 12:50:36
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "DynamicRangeFiveBandCompression.h"
#include "repmat.h"

/* Function Definitions */

/*
 * Arguments    : double b[12]
 * Return Type  : void
 */
void repmat(double b[12])
{
  int jtilecol;
  int ibtile;
  int k;
  static const signed char a[3] = { 1, 0, 0 };

  for (jtilecol = 0; jtilecol < 4; jtilecol++) {
    ibtile = jtilecol * 3;
    for (k = 0; k < 3; k++) {
      b[ibtile + k] = a[k];
    }
  }
}

/*
 * File trailer for repmat.c
 *
 * [EOF]
 */
