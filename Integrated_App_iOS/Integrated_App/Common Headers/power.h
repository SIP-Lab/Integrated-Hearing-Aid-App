/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: power.h
 *
 * MATLAB Coder version            : 3.4
 * C/C++ source code generated on  : 06-Oct-2017 12:42:28
 */

#ifndef POWER_H
#define POWER_H

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
extern void b_power(const float a[512], float y[512]);
extern void c_power(const float a_data[], const int a_size[2], float y_data[],
                    int y_size[2]);
extern void power(const float a[400], float y[400]);
extern void power_c(const float a_data[], const int a_size[1], float y_data[], int
                  y_size[1]);
#endif

/*
 * File trailer for power.h
 *
 * [EOF]
 */
