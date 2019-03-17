/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: DynamicRangeFiveBandCompression_terminate.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 28-Sep-2017 12:50:36
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "DynamicRangeFiveBandCompression.h"
#include "DynamicRangeFiveBandCompression_terminate.h"

/* Function Definitions */

/*
 * Arguments    : void
 * Return Type  : void
 */
void DynamicRangeFiveBandCompression_terminate(void)
{
  /* (no terminate code required) */
    dRC5_not_empty = false;
    dRC4_not_empty = false;
    dRC3_not_empty = false;
    dRC2_not_empty = false;
    dRC1_not_empty = false;
}

/*
 * File trailer for DynamicRangeFiveBandCompression_terminate.c
 *
 * [EOF]
 */
