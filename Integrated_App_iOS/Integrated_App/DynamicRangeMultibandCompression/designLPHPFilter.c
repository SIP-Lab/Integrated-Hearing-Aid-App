/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: designLPHPFilter.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 28-Sep-2017 12:50:36
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "DynamicRangeFiveBandCompression.h"
#include "designLPHPFilter.h"
#include "SystemCore.h"
#include "DynamicRangeFiveBandCompression_rtwutil.h"

/* Function Declarations */
static void b_blt(const float Ba_data[], const int Ba_size[2], const float
                  Aa_data[], const int Aa_size[2], float B_data[], int B_size[2],
                  float A_data[], int A_size[2]);
static void blt(const float Ba_data[], const int Ba_size[2], const float
                Aa_data[], const int Aa_size[2], float B_data[], int B_size[2],
                float A_data[], int A_size[2]);
static float rt_remf_snf(float u0, float u1);

/* Function Definitions */

/*
 * Arguments    : const float Ba_data[]
 *                const int Ba_size[2]
 *                const float Aa_data[]
 *                const int Aa_size[2]
 *                float B_data[]
 *                int B_size[2]
 *                float A_data[]
 *                int A_size[2]
 * Return Type  : void
 */
static void b_blt(const float Ba_data[], const int Ba_size[2], const float
                  Aa_data[], const int Aa_size[2], float B_data[], int B_size[2],
                  float A_data[], int A_size[2])
{
  int loop_ub;
  int ii;
  int Bhat_size_idx_0;
  int Ahat_size_idx_0;
  float Bhat_data[12];
  float Ahat_data[12];
  float B2_data[4];
  float A2_data[4];
  int idx;
  boolean_T x_data[4];
  int ii_size_idx_0;
  boolean_T exitg1;
  int ii_data[4];
  float i1idx_data[4];
  float D;
  int nx;
  B_size[0] = Ba_size[0];
  B_size[1] = 5;
  loop_ub = Ba_size[0] * 5;
  for (ii = 0; ii < loop_ub; ii++) {
    B_data[ii] = 0.0F;
  }

  A_size[0] = Ba_size[0];
  A_size[1] = 5;
  loop_ub = Ba_size[0] * 5;
  for (ii = 0; ii < loop_ub; ii++) {
    A_data[ii] = 0.0F;
  }

  Bhat_size_idx_0 = Ba_size[0];
  loop_ub = Ba_size[0] * 3;
  for (ii = 0; ii < loop_ub; ii++) {
    Bhat_data[ii] = 0.0F;
  }

  Ahat_size_idx_0 = Ba_size[0];
  loop_ub = Ba_size[0] * 3;
  for (ii = 0; ii < loop_ub; ii++) {
    Ahat_data[ii] = 0.0F;
  }

  loop_ub = Ba_size[0];
  for (ii = 0; ii < loop_ub; ii++) {
    B2_data[ii] = Ba_data[ii + (Ba_size[0] << 1)];
  }

  loop_ub = Aa_size[0];
  for (ii = 0; ii < loop_ub; ii++) {
    A2_data[ii] = Aa_data[ii + (Aa_size[0] << 1)];
  }

  loop_ub = Ba_size[0];
  for (ii = 0; ii < loop_ub; ii++) {
    x_data[ii] = (((Ba_data[ii + Ba_size[0]] != 0.0F) || (Aa_data[ii + Aa_size[0]]
      != 0.0F)) && ((B2_data[ii] == 0.0F) && (A2_data[ii] == 0.0F)));
  }

  idx = 0;
  ii_size_idx_0 = Ba_size[0];
  ii = 1;
  exitg1 = false;
  while ((!exitg1) && (ii <= loop_ub)) {
    if (x_data[ii - 1]) {
      idx++;
      ii_data[idx - 1] = ii;
      if (idx >= loop_ub) {
        exitg1 = true;
      } else {
        ii++;
      }
    } else {
      ii++;
    }
  }

  if (Ba_size[0] == 1) {
    if (idx == 0) {
      ii_size_idx_0 = 0;
    }
  } else if (1 > idx) {
    ii_size_idx_0 = 0;
  } else {
    ii_size_idx_0 = idx;
  }

  for (ii = 0; ii < ii_size_idx_0; ii++) {
    i1idx_data[ii] = (float)ii_data[ii];
  }

  if (!(ii_size_idx_0 == 0)) {
    D = Aa_data[(int)i1idx_data[0] - 1] + Aa_data[((int)i1idx_data[0] + Aa_size
      [0]) - 1];
    Bhat_data[(int)i1idx_data[0] - 1] = (Ba_data[(int)i1idx_data[0] - 1] +
      Ba_data[((int)i1idx_data[0] + Ba_size[0]) - 1]) / D;
    Bhat_data[((int)i1idx_data[0] + Ba_size[0]) - 1] = (Ba_data[(int)i1idx_data
      [0] - 1] - Ba_data[((int)i1idx_data[0] + Ba_size[0]) - 1]) / D;
    Ahat_data[(int)i1idx_data[0] - 1] = 1.0F;
    Ahat_data[((int)i1idx_data[0] + Ba_size[0]) - 1] = (Aa_data[(int)i1idx_data
      [0] - 1] - Aa_data[((int)i1idx_data[0] + Aa_size[0]) - 1]) / D;
  }

  loop_ub = Ba_size[0];
  for (ii = 0; ii < loop_ub; ii++) {
    x_data[ii] = ((B2_data[ii] != 0.0F) || (A2_data[ii] != 0.0F));
  }

  nx = Ba_size[0];
  idx = 0;
  ii_size_idx_0 = Ba_size[0];
  ii = 1;
  exitg1 = false;
  while ((!exitg1) && (ii <= nx)) {
    if (x_data[ii - 1]) {
      idx++;
      ii_data[idx - 1] = ii;
      if (idx >= nx) {
        exitg1 = true;
      } else {
        ii++;
      }
    } else {
      ii++;
    }
  }

  if (Ba_size[0] == 1) {
    if (idx == 0) {
      ii_size_idx_0 = 0;
    }
  } else if (1 > idx) {
    ii_size_idx_0 = 0;
  } else {
    ii_size_idx_0 = idx;
  }

  for (ii = 0; ii < ii_size_idx_0; ii++) {
    B2_data[ii] = (float)ii_data[ii];
  }

  for (nx = 0; nx < ii_size_idx_0; nx++) {
    D = (Aa_data[(int)B2_data[nx] - 1] + Aa_data[((int)B2_data[nx] + Aa_size[0])
         - 1]) + Aa_data[((int)B2_data[nx] + (Aa_size[0] << 1)) - 1];
    Bhat_data[(int)B2_data[nx] - 1] = ((Ba_data[(int)B2_data[nx] - 1] + Ba_data
      [((int)B2_data[nx] + Ba_size[0]) - 1]) + Ba_data[((int)B2_data[nx] +
      (Ba_size[0] << 1)) - 1]) / D;
    Bhat_data[((int)B2_data[nx] + Bhat_size_idx_0) - 1] = 2.0F * (Ba_data[(int)
      B2_data[nx] - 1] - Ba_data[((int)B2_data[nx] + (Ba_size[0] << 1)) - 1]) /
      D;
    Bhat_data[((int)B2_data[nx] + (Bhat_size_idx_0 << 1)) - 1] = ((Ba_data[(int)
      B2_data[nx] - 1] - Ba_data[((int)B2_data[nx] + Ba_size[0]) - 1]) +
      Ba_data[((int)B2_data[nx] + (Ba_size[0] << 1)) - 1]) / D;
    Ahat_data[(int)B2_data[nx] - 1] = 1.0F;
    Ahat_data[((int)B2_data[nx] + Ahat_size_idx_0) - 1] = 2.0F * (Aa_data[(int)
      B2_data[nx] - 1] - Aa_data[((int)B2_data[nx] + (Aa_size[0] << 1)) - 1]) /
      D;
    Ahat_data[((int)B2_data[nx] + (Ahat_size_idx_0 << 1)) - 1] = ((Aa_data[(int)
      B2_data[nx] - 1] - Aa_data[((int)B2_data[nx] + Aa_size[0]) - 1]) +
      Aa_data[((int)B2_data[nx] + (Aa_size[0] << 1)) - 1]) / D;
  }

  loop_ub = Ba_size[0];
  nx = Ba_size[0];
  for (ii = 0; ii < 3; ii++) {
    for (idx = 0; idx < loop_ub; idx++) {
      B_data[idx + B_size[0] * ii] = Bhat_data[idx + Bhat_size_idx_0 * ii];
    }

    for (idx = 0; idx < nx; idx++) {
      A_data[idx + A_size[0] * ii] = Ahat_data[idx + Ahat_size_idx_0 * ii];
    }
  }
}

/*
 * Arguments    : const float Ba_data[]
 *                const int Ba_size[2]
 *                const float Aa_data[]
 *                const int Aa_size[2]
 *                float B_data[]
 *                int B_size[2]
 *                float A_data[]
 *                int A_size[2]
 * Return Type  : void
 */
static void blt(const float Ba_data[], const int Ba_size[2], const float
                Aa_data[], const int Aa_size[2], float B_data[], int B_size[2],
                float A_data[], int A_size[2])
{
  int loop_ub;
  int ii;
  int Bhat_size_idx_0;
  int Ahat_size_idx_0;
  float Bhat_data[12];
  float Ahat_data[12];
  float B2_data[4];
  float A2_data[4];
  int idx;
  boolean_T x_data[4];
  int ii_size_idx_0;
  boolean_T exitg1;
  int ii_data[4];
  float i1idx_data[4];
  float D;
  int nx;
  B_size[0] = Ba_size[0];
  B_size[1] = 5;
  loop_ub = Ba_size[0] * 5;
  for (ii = 0; ii < loop_ub; ii++) {
    B_data[ii] = 0.0F;
  }

  A_size[0] = Ba_size[0];
  A_size[1] = 5;
  loop_ub = Ba_size[0] * 5;
  for (ii = 0; ii < loop_ub; ii++) {
    A_data[ii] = 0.0F;
  }

  Bhat_size_idx_0 = Ba_size[0];
  loop_ub = Ba_size[0] * 3;
  for (ii = 0; ii < loop_ub; ii++) {
    Bhat_data[ii] = 0.0F;
  }

  Ahat_size_idx_0 = Ba_size[0];
  loop_ub = Ba_size[0] * 3;
  for (ii = 0; ii < loop_ub; ii++) {
    Ahat_data[ii] = 0.0F;
  }

  loop_ub = Ba_size[0];
  for (ii = 0; ii < loop_ub; ii++) {
    B2_data[ii] = Ba_data[ii + (Ba_size[0] << 1)];
  }

  loop_ub = Aa_size[0];
  for (ii = 0; ii < loop_ub; ii++) {
    A2_data[ii] = Aa_data[ii + (Aa_size[0] << 1)];
  }

  loop_ub = Ba_size[0];
  for (ii = 0; ii < loop_ub; ii++) {
    x_data[ii] = (((Ba_data[ii + Ba_size[0]] != 0.0F) || (Aa_data[ii + Aa_size[0]]
      != 0.0F)) && ((B2_data[ii] == 0.0F) && (A2_data[ii] == 0.0F)));
  }

  idx = 0;
  ii_size_idx_0 = Ba_size[0];
  ii = 1;
  exitg1 = false;
  while ((!exitg1) && (ii <= loop_ub)) {
    if (x_data[ii - 1]) {
      idx++;
      ii_data[idx - 1] = ii;
      if (idx >= loop_ub) {
        exitg1 = true;
      } else {
        ii++;
      }
    } else {
      ii++;
    }
  }

  if (Ba_size[0] == 1) {
    if (idx == 0) {
      ii_size_idx_0 = 0;
    }
  } else if (1 > idx) {
    ii_size_idx_0 = 0;
  } else {
    ii_size_idx_0 = idx;
  }

  for (ii = 0; ii < ii_size_idx_0; ii++) {
    i1idx_data[ii] = (float)ii_data[ii];
  }

  if (!(ii_size_idx_0 == 0)) {
    D = Aa_data[(int)i1idx_data[0] - 1] + Aa_data[((int)i1idx_data[0] + Aa_size
      [0]) - 1];
    Bhat_data[(int)i1idx_data[0] - 1] = (Ba_data[(int)i1idx_data[0] - 1] +
      Ba_data[((int)i1idx_data[0] + Ba_size[0]) - 1]) / D;
    Bhat_data[((int)i1idx_data[0] + Ba_size[0]) - 1] = (Ba_data[(int)i1idx_data
      [0] - 1] - Ba_data[((int)i1idx_data[0] + Ba_size[0]) - 1]) / D;
    Ahat_data[(int)i1idx_data[0] - 1] = 1.0F;
    Ahat_data[((int)i1idx_data[0] + Ba_size[0]) - 1] = (Aa_data[(int)i1idx_data
      [0] - 1] - Aa_data[((int)i1idx_data[0] + Aa_size[0]) - 1]) / D;
  }

  loop_ub = Ba_size[0];
  for (ii = 0; ii < loop_ub; ii++) {
    x_data[ii] = ((B2_data[ii] != 0.0F) || (A2_data[ii] != 0.0F));
  }

  nx = Ba_size[0];
  idx = 0;
  ii_size_idx_0 = Ba_size[0];
  ii = 1;
  exitg1 = false;
  while ((!exitg1) && (ii <= nx)) {
    if (x_data[ii - 1]) {
      idx++;
      ii_data[idx - 1] = ii;
      if (idx >= nx) {
        exitg1 = true;
      } else {
        ii++;
      }
    } else {
      ii++;
    }
  }

  if (Ba_size[0] == 1) {
    if (idx == 0) {
      ii_size_idx_0 = 0;
    }
  } else if (1 > idx) {
    ii_size_idx_0 = 0;
  } else {
    ii_size_idx_0 = idx;
  }

  for (ii = 0; ii < ii_size_idx_0; ii++) {
    B2_data[ii] = (float)ii_data[ii];
  }

  for (nx = 0; nx < ii_size_idx_0; nx++) {
    D = (Aa_data[(int)B2_data[nx] - 1] + Aa_data[((int)B2_data[nx] + Aa_size[0])
         - 1]) + Aa_data[((int)B2_data[nx] + (Aa_size[0] << 1)) - 1];
    Bhat_data[(int)B2_data[nx] - 1] = ((Ba_data[(int)B2_data[nx] - 1] + Ba_data
      [((int)B2_data[nx] + Ba_size[0]) - 1]) + Ba_data[((int)B2_data[nx] +
      (Ba_size[0] << 1)) - 1]) / D;
    Bhat_data[((int)B2_data[nx] + Bhat_size_idx_0) - 1] = 2.0F * (Ba_data[(int)
      B2_data[nx] - 1] - Ba_data[((int)B2_data[nx] + (Ba_size[0] << 1)) - 1]) /
      D;
    Bhat_data[((int)B2_data[nx] + (Bhat_size_idx_0 << 1)) - 1] = ((Ba_data[(int)
      B2_data[nx] - 1] - Ba_data[((int)B2_data[nx] + Ba_size[0]) - 1]) +
      Ba_data[((int)B2_data[nx] + (Ba_size[0] << 1)) - 1]) / D;
    Ahat_data[(int)B2_data[nx] - 1] = 1.0F;
    Ahat_data[((int)B2_data[nx] + Ahat_size_idx_0) - 1] = 2.0F * (Aa_data[(int)
      B2_data[nx] - 1] - Aa_data[((int)B2_data[nx] + (Aa_size[0] << 1)) - 1]) /
      D;
    Ahat_data[((int)B2_data[nx] + (Ahat_size_idx_0 << 1)) - 1] = ((Aa_data[(int)
      B2_data[nx] - 1] - Aa_data[((int)B2_data[nx] + Aa_size[0]) - 1]) +
      Aa_data[((int)B2_data[nx] + (Aa_size[0] << 1)) - 1]) / D;
  }

  loop_ub = Ba_size[0];
  nx = Ba_size[0];
  for (ii = 0; ii < 3; ii++) {
    for (idx = 0; idx < loop_ub; idx++) {
      B_data[idx + B_size[0] * ii] = Bhat_data[idx + Bhat_size_idx_0 * ii];
    }

    for (idx = 0; idx < nx; idx++) {
      A_data[idx + A_size[0] * ii] = Ahat_data[idx + Ahat_size_idx_0 * ii];
    }
  }

  nx = Ba_size[0];
  for (ii = 0; ii < nx; ii++) {
    i1idx_data[ii] = -B_data[ii + B_size[0]];
  }

  loop_ub = Ba_size[0];
  for (ii = 0; ii < loop_ub; ii++) {
    B_data[ii + B_size[0]] = i1idx_data[ii];
  }

  nx = Ba_size[0];
  for (ii = 0; ii < nx; ii++) {
    i1idx_data[ii] = -A_data[ii + A_size[0]];
  }

  loop_ub = Ba_size[0];
  for (ii = 0; ii < loop_ub; ii++) {
    A_data[ii + A_size[0]] = i1idx_data[ii];
  }
}

/*
 * Arguments    : float u0
 *                float u1
 * Return Type  : float
 */
static float rt_remf_snf(float u0, float u1)
{
  float y;
  float b_u1;
  float q;
  if (!((!rtIsNaNF(u0)) && (!rtIsInfF(u0)) && ((!rtIsNaNF(u1)) && (!rtIsInfF(u1)))))
  {
    y = ((real32_T)rtNaN);
  } else {
    if (u1 < 0.0F) {
      b_u1 = (float)ceil(u1);
    } else {
      b_u1 = (float)floor(u1);
    }

    if ((u1 != 0.0F) && (u1 != b_u1)) {
      q = (float)fabs(u0 / u1);
      if ((float)fabs(q - (float)floor(q + 0.5F)) <= FLT_EPSILON * q) {
        y = 0.0F * u0;
      } else {
        y = (float)fmod(u0, u1);
      }
    } else {
      y = (float)fmod(u0, u1);
    }
  }

  return y;
}

/*
 * Arguments    : double N
 *                float BW
 *                float B_data[]
 *                int B_size[2]
 *                float A_data[]
 *                int A_size[2]
 * Return Type  : void
 */
void b_designLPHPFilter(double N, float BW, float B_data[], int B_size[2], float
  A_data[], int A_size[2])
{
  double No2;
  int loop_ub;
  int i6;
  int r;
  float L;
  float WB;
  float a;
  int offset;
  int Ba_size[2];
  int Aa_size[2];
  float Ba_data[12];
  float Aa_data[12];
  float b_B_data[20];
  int b_B_size[2];
  float b_A_data[20];
  int b_A_size[2];
  int y_size_idx_1;
  float y_data[3];
  float i_data[3];
  float si_data[3];
  float b_a;
  signed char tmp_data[3];
  float c;
  float y;
  No2 = N / 2.0;
  B_size[0] = 3;
  B_size[1] = (int)No2;
  loop_ub = 3 * (int)No2;
  for (i6 = 0; i6 < loop_ub; i6++) {
    B_data[i6] = 0.0F;
  }

  loop_ub = (int)No2;
  for (i6 = 0; i6 < loop_ub; i6++) {
    B_data[3 * i6] = 1.0F;
  }

  A_size[0] = 2;
  A_size[1] = (int)No2;
  loop_ub = (int)No2 << 1;
  for (i6 = 0; i6 < loop_ub; i6++) {
    A_data[i6] = 0.0F;
  }

  r = (int)rt_remf_snf((float)No2, 2.0F);
  L = ((float)No2 - (float)r) / 2.0F;
  WB = (float)tan(3.14159274F * BW / 2.0F);
  a = rt_powf_snf(0.99999994F, 1.0F / (float)No2);
  offset = !(r == 0);
  Ba_size[0] = (int)(L + (float)offset);
  Ba_size[1] = 3;
  loop_ub = (int)(L + (float)offset) * 3;
  for (i6 = 0; i6 < loop_ub; i6++) {
    Ba_data[i6] = 0.0F;
  }

  Aa_size[0] = (int)(L + (float)offset);
  Aa_size[1] = 3;
  loop_ub = (int)(L + (float)offset) * 3;
  for (i6 = 0; i6 < loop_ub; i6++) {
    Aa_data[i6] = 0.0F;
  }

  if (r != 0) {
    Ba_data[0] = 0.0F * WB;
    Ba_data[Ba_size[0]] = a;
    Ba_data[Ba_size[0] << 1] = 0.0F;
    Aa_data[0] = WB;
    Aa_data[Aa_size[0]] = a;
    Aa_data[Aa_size[0] << 1] = 0.0F;
  }

  if (L > 0.0F) {
    if (L < 1.0F) {
      y_size_idx_1 = 0;
    } else {
      y_size_idx_1 = (int)floor(L - 1.0) + 1;
      loop_ub = (int)floor(L - 1.0);
      for (i6 = 0; i6 <= loop_ub; i6++) {
        y_data[i6] = (float)(1.0 + (double)i6);
      }
    }

    for (i6 = 0; i6 < y_size_idx_1; i6++) {
      i_data[i6] = y_data[i6];
    }

    for (i6 = 0; i6 < y_size_idx_1; i6++) {
      si_data[i6] = 3.14159274F * ((2.0F * i_data[i6] - 1.0F) / (float)No2) /
        2.0F;
    }

    for (r = 0; r + 1 <= y_size_idx_1; r++) {
      si_data[r] = (float)sin(si_data[r]);
    }

    for (i6 = 0; i6 < y_size_idx_1; i6++) {
      tmp_data[i6] = (signed char)((signed char)((float)offset + i_data[i6]) - 1);
    }

    b_a = 0.0F * (WB * WB);
    c = a * a;
    loop_ub = (int)L;
    for (i6 = 0; i6 < loop_ub; i6++) {
      Ba_data[tmp_data[i6]] = b_a;
    }

    for (i6 = 0; i6 < y_size_idx_1; i6++) {
      Ba_data[tmp_data[i6] + Ba_size[0]] = 0.0F * si_data[i6] * WB;
    }

    loop_ub = (int)L;
    for (i6 = 0; i6 < loop_ub; i6++) {
      Ba_data[tmp_data[i6] + (Ba_size[0] << 1)] = c;
    }

    for (i6 = 0; i6 < y_size_idx_1; i6++) {
      tmp_data[i6] = (signed char)((signed char)((float)offset + i_data[i6]) - 1);
    }

    c = WB * WB;
    y = 2.0F * a;
    b_a = a * a;
    loop_ub = (int)L;
    for (i6 = 0; i6 < loop_ub; i6++) {
      Aa_data[tmp_data[i6]] = c;
    }

    for (i6 = 0; i6 < y_size_idx_1; i6++) {
      Aa_data[tmp_data[i6] + Aa_size[0]] = y * si_data[i6] * WB;
    }

    loop_ub = (int)L;
    for (i6 = 0; i6 < loop_ub; i6++) {
      Aa_data[tmp_data[i6] + (Aa_size[0] << 1)] = b_a;
    }
  }

  b_blt(Ba_data, Ba_size, Aa_data, Aa_size, b_B_data, b_B_size, b_A_data,
        b_A_size);
  loop_ub = b_B_size[0] - 1;
  for (i6 = 0; i6 <= loop_ub; i6++) {
    for (r = 0; r < 3; r++) {
      B_data[r + 3 * i6] = b_B_data[i6 + b_B_size[0] * r];
    }
  }

  loop_ub = b_A_size[0] - 1;
  for (i6 = 0; i6 <= loop_ub; i6++) {
    for (r = 0; r < 2; r++) {
      A_data[r + (i6 << 1)] = b_A_data[i6 + b_A_size[0] * (1 + r)];
    }
  }
}

/*
 * Arguments    : double N
 *                float BW
 *                float B_data[]
 *                int B_size[2]
 *                float A_data[]
 *                int A_size[2]
 * Return Type  : void
 */
void designLPHPFilter(double N, float BW, float B_data[], int B_size[2], float
                      A_data[], int A_size[2])
{
  double No2;
  int loop_ub;
  int i5;
  int r;
  float L;
  float WB;
  float a;
  int offset;
  int Ba_size[2];
  int Aa_size[2];
  float Ba_data[12];
  float Aa_data[12];
  float b_B_data[20];
  int b_B_size[2];
  float b_A_data[20];
  int b_A_size[2];
  int y_size_idx_1;
  float y_data[3];
  float i_data[3];
  float si_data[3];
  float b_a;
  signed char tmp_data[3];
  float c;
  float y;
  No2 = N / 2.0;
  B_size[0] = 3;
  B_size[1] = (int)No2;
  loop_ub = 3 * (int)No2;
  for (i5 = 0; i5 < loop_ub; i5++) {
    B_data[i5] = 0.0F;
  }

  loop_ub = (int)No2;
  for (i5 = 0; i5 < loop_ub; i5++) {
    B_data[3 * i5] = 1.0F;
  }

  A_size[0] = 2;
  A_size[1] = (int)No2;
  loop_ub = (int)No2 << 1;
  for (i5 = 0; i5 < loop_ub; i5++) {
    A_data[i5] = 0.0F;
  }

  r = (int)rt_remf_snf((float)No2, 2.0F);
  L = ((float)No2 - (float)r) / 2.0F;
  WB = (float)tan(3.14159274F * BW / 2.0F);
  a = rt_powf_snf(0.99999994F, 1.0F / (float)No2);
  offset = !(r == 0);
  Ba_size[0] = (int)(L + (float)offset);
  Ba_size[1] = 3;
  loop_ub = (int)(L + (float)offset) * 3;
  for (i5 = 0; i5 < loop_ub; i5++) {
    Ba_data[i5] = 0.0F;
  }

  Aa_size[0] = (int)(L + (float)offset);
  Aa_size[1] = 3;
  loop_ub = (int)(L + (float)offset) * 3;
  for (i5 = 0; i5 < loop_ub; i5++) {
    Aa_data[i5] = 0.0F;
  }

  if (r != 0) {
    Ba_data[0] = 0.0F * WB;
    Ba_data[Ba_size[0]] = a;
    Ba_data[Ba_size[0] << 1] = 0.0F;
    Aa_data[0] = WB;
    Aa_data[Aa_size[0]] = a;
    Aa_data[Aa_size[0] << 1] = 0.0F;
  }

  if (L > 0.0F) {
    if (L < 1.0F) {
      y_size_idx_1 = 0;
    } else {
      y_size_idx_1 = (int)floor(L - 1.0) + 1;
      loop_ub = (int)floor(L - 1.0);
      for (i5 = 0; i5 <= loop_ub; i5++) {
        y_data[i5] = (float)(1.0 + (double)i5);
      }
    }

    for (i5 = 0; i5 < y_size_idx_1; i5++) {
      i_data[i5] = y_data[i5];
    }

    for (i5 = 0; i5 < y_size_idx_1; i5++) {
      si_data[i5] = 3.14159274F * ((2.0F * i_data[i5] - 1.0F) / (float)No2) /
        2.0F;
    }

    for (r = 0; r + 1 <= y_size_idx_1; r++) {
      si_data[r] = (float)sin(si_data[r]);
    }

    for (i5 = 0; i5 < y_size_idx_1; i5++) {
      tmp_data[i5] = (signed char)((signed char)((float)offset + i_data[i5]) - 1);
    }

    b_a = 0.0F * (WB * WB);
    c = a * a;
    loop_ub = (int)L;
    for (i5 = 0; i5 < loop_ub; i5++) {
      Ba_data[tmp_data[i5]] = b_a;
    }

    for (i5 = 0; i5 < y_size_idx_1; i5++) {
      Ba_data[tmp_data[i5] + Ba_size[0]] = 0.0F * si_data[i5] * WB;
    }

    loop_ub = (int)L;
    for (i5 = 0; i5 < loop_ub; i5++) {
      Ba_data[tmp_data[i5] + (Ba_size[0] << 1)] = c;
    }

    for (i5 = 0; i5 < y_size_idx_1; i5++) {
      tmp_data[i5] = (signed char)((signed char)((float)offset + i_data[i5]) - 1);
    }

    c = WB * WB;
    y = 2.0F * a;
    b_a = a * a;
    loop_ub = (int)L;
    for (i5 = 0; i5 < loop_ub; i5++) {
      Aa_data[tmp_data[i5]] = c;
    }

    for (i5 = 0; i5 < y_size_idx_1; i5++) {
      Aa_data[tmp_data[i5] + Aa_size[0]] = y * si_data[i5] * WB;
    }

    loop_ub = (int)L;
    for (i5 = 0; i5 < loop_ub; i5++) {
      Aa_data[tmp_data[i5] + (Aa_size[0] << 1)] = b_a;
    }
  }

  blt(Ba_data, Ba_size, Aa_data, Aa_size, b_B_data, b_B_size, b_A_data, b_A_size);
  loop_ub = b_B_size[0] - 1;
  for (i5 = 0; i5 <= loop_ub; i5++) {
    for (r = 0; r < 3; r++) {
      B_data[r + 3 * i5] = b_B_data[i5 + b_B_size[0] * r];
    }
  }

  loop_ub = b_A_size[0] - 1;
  for (i5 = 0; i5 <= loop_ub; i5++) {
    for (r = 0; r < 2; r++) {
      A_data[r + (i5 << 1)] = b_A_data[i5 + b_A_size[0] * (1 + r)];
    }
  }
}

/*
 * File trailer for designLPHPFilter.c
 *
 * [EOF]
 */
