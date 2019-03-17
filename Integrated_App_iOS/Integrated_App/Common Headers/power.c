/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: power.c
 *
 * MATLAB Coder version            : 3.4
 * C/C++ source code generated on  : 06-Oct-2017 12:42:28
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "wiener_ADAPTIVE_NE.h"
#include "power.h"
#include "DynamicRangeFiveBandCompression.h"
/* Function Definitions */

/*
 * Arguments    : const float a[512]
 *                float y[512]
 * Return Type  : void
 */
void b_power(const float a[512], float y[512])
{
  int k;
  for (k = 0; k < 512; k++) {
    y[k] = a[k] * a[k];
  }
}

/*
 * Arguments    : const float a_data[]
 *                const int a_size[2]
 *                float y_data[]
 *                int y_size[2]
 * Return Type  : void
 */
void c_power(const float a_data[], const int a_size[2], float y_data[], int
             y_size[2])
{
  int k;
  y_size[0] = 1;
  y_size[1] = (short)a_size[1];
  for (k = 0; k + 1 <= (short)a_size[1]; k++) {
    y_data[k] = a_data[k] * a_data[k];
  }
}

/*
 * Arguments    : const float a[400]
 *                float y[400]
 * Return Type  : void
 */
void power(const float a[400], float y[400])
{
  int k;
  for (k = 0; k < 400; k++) {
    y[k] = a[k] * a[k];
  }
}

/* Function Definitions */

/*
 * Arguments    : const float a_data[]
 *                const int a_size[1]
 *                float y_data[]
 *                int y_size[1]
 * Return Type  : void
 */
void power_c(const float a_data[], const int a_size[1], float y_data[], int
           y_size[1])
{
  int k;
  y_size[0] = (unsigned char)a_size[0];
  for (k = 0; k + 1 <= a_size[0]; k++) {
    y_data[k] = a_data[k] * a_data[k];
  }
}

/*
 * File trailer for power.c
 *
 * [EOF]
 */
