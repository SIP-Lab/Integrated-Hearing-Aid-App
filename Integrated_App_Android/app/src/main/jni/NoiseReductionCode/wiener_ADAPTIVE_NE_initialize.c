/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: wiener_ADAPTIVE_NE_initialize.c
 *
 * MATLAB Coder version            : 3.4
 * C/C++ source code generated on  : 06-Oct-2017 12:42:28
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "wiener_ADAPTIVE_NE.h"
#include "wiener_ADAPTIVE_NE_initialize.h"

/* Function Definitions */

/*
 * Arguments    : void
 * Return Type  : void
 */
void wiener_ADAPTIVE_NE_initialize(void)
{
  rt_InitInfAndNaN(8U);
  wiener_ADAPTIVE_NE_init();
}

/*
 * File trailer for wiener_ADAPTIVE_NE_initialize.c
 *
 * [EOF]
 */
