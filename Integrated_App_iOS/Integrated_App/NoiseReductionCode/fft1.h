/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: fft1.h
 *
 * MATLAB Coder version            : 3.4
 * C/C++ source code generated on  : 06-Oct-2017 12:42:28
 */

#ifndef FFT1_H
#define FFT1_H

/* Include Files */
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "wiener_ADAPTIVE_NE_types.h"

/* Function Declarations */
extern void b_r2br_r2dit_trig(const creal32_T x[1024], const float costab[513],
  const float sintab[513], creal32_T y[1024]);
extern void r2br_r2dit_trig(const creal32_T x[799], const float costab[513],
  const float sintab[513], creal32_T y[1024]);
extern void r2br_r2dit_trig_impl(const creal32_T x[400], const float costab[513],
  const float sintab[513], creal32_T y[1024]);

#endif

/*
 * File trailer for fft1.h
 *
 * [EOF]
 */
