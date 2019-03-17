/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: fft.h
 *
 * MATLAB Coder version            : 3.4
 * C/C++ source code generated on  : 06-Oct-2017 12:42:28
 */

#ifndef FFT_H
#define FFT_H

/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "wiener_ADAPTIVE_NE_types.h"

/* Function Declarations */
extern void b_fft(const float x[400], creal32_T y[512]);
extern void fft(const float x[400], creal32_T y[400]);

#endif

/*
 * File trailer for fft.h
 *
 * [EOF]
 */
