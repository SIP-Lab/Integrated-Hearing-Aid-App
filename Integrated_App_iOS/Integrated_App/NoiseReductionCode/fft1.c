/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: fft1.c
 *
 * MATLAB Coder version            : 3.4
 * C/C++ source code generated on  : 06-Oct-2017 12:42:28
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "wiener_ADAPTIVE_NE.h"
#include "fft1.h"

/* Function Definitions */

/*
 * Arguments    : const creal32_T x[1024]
 *                const float costab[513]
 *                const float sintab[513]
 *                creal32_T y[1024]
 * Return Type  : void
 */
void b_r2br_r2dit_trig(const creal32_T x[1024], const float costab[513], const
  float sintab[513], creal32_T y[1024])
{
  int ix;
  int ju;
  int iy;
  int i;
  boolean_T tst;
  float temp_re;
  float temp_im;
  int iheight;
  int istart;
  int b_j;
  float twid_re;
  float twid_im;
  int ihi;
  ix = 0;
  ju = 0;
  iy = 0;
  for (i = 0; i < 1023; i++) {
    y[iy] = x[ix];
    iy = 1024;
    tst = true;
    while (tst) {
      iy >>= 1;
      ju ^= iy;
      tst = ((ju & iy) == 0);
    }

    iy = ju;
    ix++;
  }

  y[iy] = x[ix];
  for (i = 0; i <= 1023; i += 2) {
    temp_re = y[i + 1].re;
    temp_im = y[i + 1].im;
    y[i + 1].re = y[i].re - y[i + 1].re;
    y[i + 1].im = y[i].im - y[i + 1].im;
    y[i].re += temp_re;
    y[i].im += temp_im;
  }

  iy = 2;
  ix = 4;
  ju = 256;
  iheight = 1021;
  while (ju > 0) {
    for (i = 0; i < iheight; i += ix) {
      temp_re = y[i + iy].re;
      temp_im = y[i + iy].im;
      y[i + iy].re = y[i].re - temp_re;
      y[i + iy].im = y[i].im - temp_im;
      y[i].re += temp_re;
      y[i].im += temp_im;
    }

    istart = 1;
    for (b_j = ju; b_j < 512; b_j += ju) {
      twid_re = costab[b_j];
      twid_im = sintab[b_j];
      i = istart;
      ihi = istart + iheight;
      while (i < ihi) {
        temp_re = twid_re * y[i + iy].re - twid_im * y[i + iy].im;
        temp_im = twid_re * y[i + iy].im + twid_im * y[i + iy].re;
        y[i + iy].re = y[i].re - temp_re;
        y[i + iy].im = y[i].im - temp_im;
        y[i].re += temp_re;
        y[i].im += temp_im;
        i += ix;
      }

      istart++;
    }

    ju /= 2;
    iy = ix;
    ix += ix;
    iheight -= iy;
  }

  for (iy = 0; iy < 1024; iy++) {
    y[iy].re *= 0.0009765625F;
    y[iy].im *= 0.0009765625F;
  }
}

/*
 * Arguments    : const creal32_T x[799]
 *                const float costab[513]
 *                const float sintab[513]
 *                creal32_T y[1024]
 * Return Type  : void
 */
void r2br_r2dit_trig(const creal32_T x[799], const float costab[513], const
                     float sintab[513], creal32_T y[1024])
{
  int i;
  int ix;
  int ju;
  int iy;
  boolean_T tst;
  float temp_re;
  float temp_im;
  int iheight;
  int istart;
  int b_j;
  float twid_re;
  float twid_im;
  int ihi;
  for (i = 0; i < 1024; i++) {
    y[i].re = 0.0F;
    y[i].im = 0.0F;
  }

  ix = 0;
  ju = 0;
  iy = 0;
  for (i = 0; i < 798; i++) {
    y[iy] = x[ix];
    iy = 1024;
    tst = true;
    while (tst) {
      iy >>= 1;
      ju ^= iy;
      tst = ((ju & iy) == 0);
    }

    iy = ju;
    ix++;
  }

  y[iy] = x[ix];
  for (i = 0; i <= 1023; i += 2) {
    temp_re = y[i + 1].re;
    temp_im = y[i + 1].im;
    y[i + 1].re = y[i].re - y[i + 1].re;
    y[i + 1].im = y[i].im - y[i + 1].im;
    y[i].re += temp_re;
    y[i].im += temp_im;
  }

  iy = 2;
  ix = 4;
  ju = 256;
  iheight = 1021;
  while (ju > 0) {
    for (i = 0; i < iheight; i += ix) {
      temp_re = y[i + iy].re;
      temp_im = y[i + iy].im;
      y[i + iy].re = y[i].re - temp_re;
      y[i + iy].im = y[i].im - temp_im;
      y[i].re += temp_re;
      y[i].im += temp_im;
    }

    istart = 1;
    for (b_j = ju; b_j < 512; b_j += ju) {
      twid_re = costab[b_j];
      twid_im = sintab[b_j];
      i = istart;
      ihi = istart + iheight;
      while (i < ihi) {
        temp_re = twid_re * y[i + iy].re - twid_im * y[i + iy].im;
        temp_im = twid_re * y[i + iy].im + twid_im * y[i + iy].re;
        y[i + iy].re = y[i].re - temp_re;
        y[i + iy].im = y[i].im - temp_im;
        y[i].re += temp_re;
        y[i].im += temp_im;
        i += ix;
      }

      istart++;
    }

    ju /= 2;
    iy = ix;
    ix += ix;
    iheight -= iy;
  }
}

/*
 * Arguments    : const creal32_T x[400]
 *                const float costab[513]
 *                const float sintab[513]
 *                creal32_T y[1024]
 * Return Type  : void
 */
void r2br_r2dit_trig_impl(const creal32_T x[400], const float costab[513], const
  float sintab[513], creal32_T y[1024])
{
  int i;
  int ix;
  int ju;
  int iy;
  boolean_T tst;
  float temp_re;
  float temp_im;
  int iheight;
  int istart;
  int b_j;
  float twid_re;
  float twid_im;
  int ihi;
  for (i = 0; i < 1024; i++) {
    y[i].re = 0.0F;
    y[i].im = 0.0F;
  }

  ix = 0;
  ju = 0;
  iy = 0;
  for (i = 0; i < 399; i++) {
    y[iy] = x[ix];
    iy = 1024;
    tst = true;
    while (tst) {
      iy >>= 1;
      ju ^= iy;
      tst = ((ju & iy) == 0);
    }

    iy = ju;
    ix++;
  }

  y[iy] = x[ix];
  for (i = 0; i <= 1023; i += 2) {
    temp_re = y[i + 1].re;
    temp_im = y[i + 1].im;
    y[i + 1].re = y[i].re - y[i + 1].re;
    y[i + 1].im = y[i].im - y[i + 1].im;
    y[i].re += temp_re;
    y[i].im += temp_im;
  }

  iy = 2;
  ix = 4;
  ju = 256;
  iheight = 1021;
  while (ju > 0) {
    for (i = 0; i < iheight; i += ix) {
      temp_re = y[i + iy].re;
      temp_im = y[i + iy].im;
      y[i + iy].re = y[i].re - temp_re;
      y[i + iy].im = y[i].im - temp_im;
      y[i].re += temp_re;
      y[i].im += temp_im;
    }

    istart = 1;
    for (b_j = ju; b_j < 512; b_j += ju) {
      twid_re = costab[b_j];
      twid_im = sintab[b_j];
      i = istart;
      ihi = istart + iheight;
      while (i < ihi) {
        temp_re = twid_re * y[i + iy].re - twid_im * y[i + iy].im;
        temp_im = twid_re * y[i + iy].im + twid_im * y[i + iy].re;
        y[i + iy].re = y[i].re - temp_re;
        y[i + iy].im = y[i].im - temp_im;
        y[i].re += temp_re;
        y[i].im += temp_im;
        i += ix;
      }

      istart++;
    }

    ju /= 2;
    iy = ix;
    ix += ix;
    iheight -= iy;
  }
}

/*
 * File trailer for fft1.c
 *
 * [EOF]
 */
