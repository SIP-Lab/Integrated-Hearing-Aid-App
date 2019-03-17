/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: mod.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 28-Sep-2017 12:50:36
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "DynamicRangeFiveBandCompression.h"
#include "mod.h"

/* Function Definitions */

/*
 * Arguments    : float x
 * Return Type  : float
 */
float b_mod(float x)
{
  float r;
  if ((!rtIsInfF(x)) && (!rtIsNaNF(x))) {
    if (x == 0.0F) {
      r = 0.0F;
    } else {
      r = (float)fmod(x, 2.0);
      if (r == 0.0F) {
        r = 0.0F;
      } else {
        if (x < 0.0F) {
          r += 2.0F;
        }
      }
    }
  } else {
    r = ((real32_T)rtNaN);
  }

  return r;
}

/*
 * File trailer for mod.c
 *
 * [EOF]
 */
