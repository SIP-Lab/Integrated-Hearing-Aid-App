/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: tuneCrossoverFilterCoefficients.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 28-Sep-2017 12:50:36
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "DynamicRangeFiveBandCompression.h"
#include "tuneCrossoverFilterCoefficients.h"
#include "repmat.h"
#include "designLPHPFilter.h"
#include "mod.h"

/* Function Definitions */

/*
 * Arguments    : float freq
 *                float order
 *                float Fs
 *                float BL[12]
 *                float AL[12]
 *                float BH[12]
 *                float AH[12]
 *                float *phaseMult
 * Return Type  : void
 */
void tuneCrossoverFilterCoefficients(float freq, float order, float Fs, float
  BL[12], float AL[12], float BH[12], float AH[12], float *phaseMult)
{
  float FCast;
  float FsbyTwo;
  boolean_T isEven;
  int ord;
  float B0L_data[21];
  int B0L_size[2];
  float A0L_data[14];
  int A0L_size[2];
  float B0H_data[21];
  int B0H_size[2];
  float A0H_data[14];
  int A0H_size[2];
  double dv1[12];
  int loop_ub;
  int i3;
  int i4;
  FCast = freq;
  FsbyTwo = Fs / 2.0F;
  if (freq >= FsbyTwo) {
    FCast = FsbyTwo - 1.0F;
  }

  FCast /= FsbyTwo;
  FsbyTwo = order;
  if ((order == 2.0F) || (order == 6.0F)) {
    *phaseMult = -1.0F;
  } else {
    *phaseMult = 1.0F;
  }

  isEven = false;
  if (b_mod(order) == 0.0F) {
    FsbyTwo = order / 2.0F;
    isEven = true;
  }

  switch ((int)FsbyTwo) {
   case 1:
    ord = 2;
    break;

   case 2:
    ord = 4;
    break;

   case 3:
    ord = 6;
    break;

   case 4:
    ord = 8;
    break;

   case 5:
    ord = 10;
    break;

   case 6:
    ord = 12;
    break;

   case 7:
    ord = 14;
    break;

   default:
    ord = 2;
    break;
  }

  designLPHPFilter(ord, 1.0F - FCast, B0L_data, B0L_size, A0L_data, A0L_size);
  b_designLPHPFilter(ord, FCast, B0H_data, B0H_size, A0H_data, A0H_size);
  repmat(dv1);
  for (ord = 0; ord < 12; ord++) {
    BL[ord] = (float)dv1[ord];
  }

  repmat(dv1);
  for (ord = 0; ord < 12; ord++) {
    AL[ord] = (float)dv1[ord];
  }

  repmat(dv1);
  for (ord = 0; ord < 12; ord++) {
    BH[ord] = (float)dv1[ord];
  }

  repmat(dv1);
  for (ord = 0; ord < 12; ord++) {
    AH[ord] = (float)dv1[ord];
  }

  if (isEven) {
    FCast = (float)ceil(FsbyTwo / 2.0F);
    if (1.0F > FCast) {
      loop_ub = -1;
    } else {
      loop_ub = (int)FCast - 1;
    }

    for (ord = 0; ord <= loop_ub; ord++) {
      for (i3 = 0; i3 < 3; i3++) {
        BL[i3 + 3 * ord] = B0L_data[i3 + B0L_size[0] * ord];
      }
    }

    FCast = (float)ceil(FsbyTwo / 2.0F);
    if (1.0F > FCast) {
      loop_ub = -1;
    } else {
      loop_ub = (int)FCast - 1;
    }

    if (3.0F > (3.0F + (float)ceil(FsbyTwo / 2.0F)) - 1.0F) {
      ord = 0;
    } else {
      ord = 2;
    }

    for (i3 = 0; i3 <= loop_ub; i3++) {
      for (i4 = 0; i4 < 3; i4++) {
        BL[i4 + 3 * (ord + i3)] = B0L_data[i4 + B0L_size[0] * i3];
      }
    }

    FCast = (float)ceil(FsbyTwo / 2.0F);
    if (1.0F > FCast) {
      loop_ub = -1;
    } else {
      loop_ub = (int)FCast - 1;
    }

    for (ord = 0; ord <= loop_ub; ord++) {
      for (i3 = 0; i3 < 2; i3++) {
        AL[(i3 + 3 * ord) + 1] = A0L_data[i3 + A0L_size[0] * ord];
      }
    }

    FCast = (float)ceil(FsbyTwo / 2.0F);
    if (1.0F > FCast) {
      loop_ub = -1;
    } else {
      loop_ub = (int)FCast - 1;
    }

    if (3.0F > (3.0F + (float)ceil(FsbyTwo / 2.0F)) - 1.0F) {
      ord = 0;
    } else {
      ord = 2;
    }

    for (i3 = 0; i3 <= loop_ub; i3++) {
      for (i4 = 0; i4 < 2; i4++) {
        AL[(i4 + 3 * (ord + i3)) + 1] = A0L_data[i4 + A0L_size[0] * i3];
      }
    }

    FCast = (float)ceil(FsbyTwo / 2.0F);
    if (1.0F > FCast) {
      loop_ub = -1;
    } else {
      loop_ub = (int)FCast - 1;
    }

    for (ord = 0; ord <= loop_ub; ord++) {
      for (i3 = 0; i3 < 3; i3++) {
        BH[i3 + 3 * ord] = B0H_data[i3 + B0H_size[0] * ord];
      }
    }

    FCast = (float)ceil(FsbyTwo / 2.0F);
    if (1.0F > FCast) {
      loop_ub = -1;
    } else {
      loop_ub = (int)FCast - 1;
    }

    if (3.0F > (3.0F + (float)ceil(FsbyTwo / 2.0F)) - 1.0F) {
      ord = 0;
    } else {
      ord = 2;
    }

    for (i3 = 0; i3 <= loop_ub; i3++) {
      for (i4 = 0; i4 < 3; i4++) {
        BH[i4 + 3 * (ord + i3)] = B0H_data[i4 + B0H_size[0] * i3];
      }
    }

    FCast = (float)ceil(FsbyTwo / 2.0F);
    if (1.0F > FCast) {
      loop_ub = -1;
    } else {
      loop_ub = (int)FCast - 1;
    }

    for (ord = 0; ord <= loop_ub; ord++) {
      for (i3 = 0; i3 < 2; i3++) {
        AH[(i3 + 3 * ord) + 1] = A0H_data[i3 + A0H_size[0] * ord];
      }
    }

    FCast = (float)ceil(FsbyTwo / 2.0F);
    if (1.0F > FCast) {
      loop_ub = -1;
    } else {
      loop_ub = (int)FCast - 1;
    }

    if (3.0F > (3.0F + (float)ceil(FsbyTwo / 2.0F)) - 1.0F) {
      ord = 0;
    } else {
      ord = 2;
    }

    for (i3 = 0; i3 <= loop_ub; i3++) {
      for (i4 = 0; i4 < 2; i4++) {
        AH[(i4 + 3 * (ord + i3)) + 1] = A0H_data[i4 + A0H_size[0] * i3];
      }
    }
  } else {
    FCast = (float)ceil(FsbyTwo / 2.0F);
    if (1.0F > FCast) {
      loop_ub = -1;
    } else {
      loop_ub = (int)FCast - 1;
    }

    for (ord = 0; ord <= loop_ub; ord++) {
      for (i3 = 0; i3 < 3; i3++) {
        BL[i3 + 3 * ord] = B0L_data[i3 + B0L_size[0] * ord];
      }
    }

    FCast = (float)ceil(FsbyTwo / 2.0F);
    if (1.0F > FCast) {
      loop_ub = -1;
    } else {
      loop_ub = (int)FCast - 1;
    }

    for (ord = 0; ord <= loop_ub; ord++) {
      for (i3 = 0; i3 < 2; i3++) {
        AL[(i3 + 3 * ord) + 1] = A0L_data[i3 + A0L_size[0] * ord];
      }
    }

    FCast = (float)ceil(FsbyTwo / 2.0F);
    if (1.0F > FCast) {
      loop_ub = -1;
    } else {
      loop_ub = (int)FCast - 1;
    }

    for (ord = 0; ord <= loop_ub; ord++) {
      for (i3 = 0; i3 < 3; i3++) {
        BH[i3 + 3 * ord] = B0H_data[i3 + B0H_size[0] * ord];
      }
    }

    FCast = (float)ceil(FsbyTwo / 2.0F);
    if (1.0F > FCast) {
      loop_ub = -1;
    } else {
      loop_ub = (int)FCast - 1;
    }

    for (ord = 0; ord <= loop_ub; ord++) {
      for (i3 = 0; i3 < 2; i3++) {
        AH[(i3 + 3 * ord) + 1] = A0H_data[i3 + A0H_size[0] * ord];
      }
    }
  }
}

/*
 * File trailer for tuneCrossoverFilterCoefficients.c
 *
 * [EOF]
 */
