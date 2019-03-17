/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: FiveBandCrossoverFilter.c
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 28-Sep-2017 12:50:36
 */

/* Include Files */
#include "rt_nonfinite.h"
#include "DynamicRangeFiveBandCompression.h"
#include "FiveBandCrossoverFilter.h"
#include "filter.h"
#include "tuneCrossoverFilterCoefficients.h"

/* Function Definitions */

/*
 * Arguments    : c_audio_internal_FiveBandCrosso *obj
 * Return Type  : void
 */
void c_FiveBandCrossoverFilter_reset(c_audio_internal_FiveBandCrosso *obj)
{
  c_audio_internal_FiveBandCrosso *b_obj;
  int i7;
  b_obj = obj;
  for (i7 = 0; i7 < 8; i7++) {
    b_obj->pCR1_States_LP[i7] = 0.0F;
  }

  for (i7 = 0; i7 < 8; i7++) {
    b_obj->pCR1_States_HP[i7] = 0.0F;
  }

  for (i7 = 0; i7 < 8; i7++) {
    b_obj->pCR2_States_LP[i7] = 0.0F;
  }

  for (i7 = 0; i7 < 8; i7++) {
    b_obj->pCR2_States_HP[i7] = 0.0F;
  }

  for (i7 = 0; i7 < 8; i7++) {
    b_obj->pCR3_States_LP[i7] = 0.0F;
  }

  for (i7 = 0; i7 < 8; i7++) {
    b_obj->pCR3_States_HP[i7] = 0.0F;
  }

  for (i7 = 0; i7 < 8; i7++) {
    b_obj->pCR4_States_LP[i7] = 0.0F;
  }

  for (i7 = 0; i7 < 8; i7++) {
    b_obj->pCR4_States_HP[i7] = 0.0F;
  }

  for (i7 = 0; i7 < 8; i7++) {
    b_obj->pAP1_States_LP[i7] = 0.0F;
  }

  for (i7 = 0; i7 < 8; i7++) {
    b_obj->pAP1_States_HP[i7] = 0.0F;
  }

  for (i7 = 0; i7 < 8; i7++) {
    b_obj->pAP2_States_LP[i7] = 0.0F;
  }

  for (i7 = 0; i7 < 8; i7++) {
    b_obj->pAP2_States_HP[i7] = 0.0F;
  }

  for (i7 = 0; i7 < 8; i7++) {
    b_obj->pAP3_States_LP[i7] = 0.0F;
  }

  for (i7 = 0; i7 < 8; i7++) {
    b_obj->pAP3_States_HP[i7] = 0.0F;
  }

  for (i7 = 0; i7 < 8; i7++) {
    b_obj->pAP4_States_LP[i7] = 0.0F;
  }

  for (i7 = 0; i7 < 8; i7++) {
    b_obj->pAP4_States_HP[i7] = 0.0F;
  }

  c_FiveBandCrossoverFilter_tuneC(obj);
}

/*
 * Arguments    : c_audio_internal_FiveBandCrosso *obj
 *                const float x[200]
 *                float z1[200]
 *                float z2[200]
 *                float z3[200]
 *                float z4[200]
 *                float z5[200]
 * Return Type  : void
 */
void c_FiveBandCrossoverFilter_stepI(c_audio_internal_FiveBandCrosso *obj, const
  float x[200], float z1[200], float z2[200], float z3[200], float z4[200],
  float z5[200])
{
  int i;
  float B1L[12];
  float A1L[12];
  float B1H[12];
  float A1H[12];
  float B2L[12];
  float A2L[12];
  float B2H[12];
  float A2H[12];
  float B3L[12];
  float A3L[12];
  float B3H[12];
  float A3H[12];
  float B4L[12];
  float A4L[12];
  float B4H[12];
  float outL[200];
  float A4H[12];
  int b_index;
  float S[8];
  float outH[200];
  float b_z5[200];
  float b_B1L[3];
  float b_B4H[3];
  float b_S[2];
  float b;
  float b_A4H[3];
  float out3L[200];
  for (i = 0; i < 12; i++) {
    B1L[i] = obj->pCR1_B_LP[i];
  }

  for (i = 0; i < 12; i++) {
    A1L[i] = obj->pCR1_A_LP[i];
  }

  for (i = 0; i < 12; i++) {
    B1H[i] = obj->pCR1_B_HP[i];
  }

  for (i = 0; i < 12; i++) {
    A1H[i] = obj->pCR1_A_HP[i];
  }

  for (i = 0; i < 12; i++) {
    B2L[i] = obj->pCR2_B_LP[i];
  }

  for (i = 0; i < 12; i++) {
    A2L[i] = obj->pCR2_A_LP[i];
  }

  for (i = 0; i < 12; i++) {
    B2H[i] = obj->pCR2_B_HP[i];
  }

  for (i = 0; i < 12; i++) {
    A2H[i] = obj->pCR2_A_HP[i];
  }

  for (i = 0; i < 12; i++) {
    B3L[i] = obj->pCR3_B_LP[i];
  }

  for (i = 0; i < 12; i++) {
    A3L[i] = obj->pCR3_A_LP[i];
  }

  for (i = 0; i < 12; i++) {
    B3H[i] = obj->pCR3_B_HP[i];
  }

  for (i = 0; i < 12; i++) {
    A3H[i] = obj->pCR3_A_HP[i];
  }

  for (i = 0; i < 12; i++) {
    B4L[i] = obj->pCR4_B_LP[i];
  }

  for (i = 0; i < 12; i++) {
    A4L[i] = obj->pCR4_A_LP[i];
  }

  for (i = 0; i < 12; i++) {
    B4H[i] = obj->pCR4_B_HP[i];
  }

  for (i = 0; i < 12; i++) {
    A4H[i] = obj->pCR4_A_HP[i];
  }

  memcpy(&outL[0], &x[0], 200U * sizeof(float));
  for (i = 0; i < 8; i++) {
    S[i] = obj->pCR1_States_LP[i];
  }

  for (b_index = 0; b_index < 4; b_index++) {
    for (i = 0; i < 3; i++) {
      b_B1L[i] = B1L[i + 3 * b_index];
      b_B4H[i] = A1L[i + 3 * b_index];
    }

    memcpy(&b_z5[0], &outL[0], 200U * sizeof(float));
    for (i = 0; i < 2; i++) {
      b_S[i] = S[i + (b_index << 1)];
    }

    filter(b_B1L, b_B4H, b_z5, b_S, outL, *(float (*)[2])&S[b_index << 1]);
  }

  for (i = 0; i < 8; i++) {
    obj->pCR1_States_LP[i] = S[i];
  }

  memcpy(&outH[0], &x[0], 200U * sizeof(float));
  for (i = 0; i < 8; i++) {
    S[i] = obj->pCR1_States_HP[i];
  }

  for (b_index = 0; b_index < 4; b_index++) {
    for (i = 0; i < 3; i++) {
      b_B4H[i] = B1H[i + 3 * b_index];
      b_A4H[i] = A1H[i + 3 * b_index];
    }

    memcpy(&b_z5[0], &outH[0], 200U * sizeof(float));
    for (i = 0; i < 2; i++) {
      b_S[i] = S[i + (b_index << 1)];
    }

    filter(b_B4H, b_A4H, b_z5, b_S, outH, *(float (*)[2])&S[b_index << 1]);
  }

  for (i = 0; i < 8; i++) {
    obj->pCR1_States_HP[i] = S[i];
  }

  b = obj->pPhaseMult[2];
  for (i = 0; i < 200; i++) {
    out3L[i] = outL[i];
    outH[i] *= b;
  }

  for (i = 0; i < 8; i++) {
    S[i] = obj->pAP4_States_LP[i];
  }

  for (b_index = 0; b_index < 4; b_index++) {
    for (i = 0; i < 3; i++) {
      b_B4H[i] = B4L[i + 3 * b_index];
      b_A4H[i] = A4L[i + 3 * b_index];
    }

    memcpy(&b_z5[0], &out3L[0], 200U * sizeof(float));
    for (i = 0; i < 2; i++) {
      b_S[i] = S[i + (b_index << 1)];
    }

    filter(b_B4H, b_A4H, b_z5, b_S, out3L, *(float (*)[2])&S[b_index << 1]);
  }

  for (i = 0; i < 8; i++) {
    obj->pAP4_States_LP[i] = S[i];
  }

  for (i = 0; i < 12; i++) {
    B1L[i] = B4H[i];
  }

  b = obj->pPhaseMult[3];
  for (i = 0; i < 3; i++) {
    B1L[i] = B4H[i] * b;
  }

  for (i = 0; i < 8; i++) {
    S[i] = obj->pAP4_States_HP[i];
  }

  for (b_index = 0; b_index < 4; b_index++) {
    for (i = 0; i < 3; i++) {
      b_B1L[i] = B1L[i + 3 * b_index];
      b_A4H[i] = A4H[i + 3 * b_index];
    }

    memcpy(&b_z5[0], &outL[0], 200U * sizeof(float));
    for (i = 0; i < 2; i++) {
      b_S[i] = S[i + (b_index << 1)];
    }

    filter(b_B1L, b_A4H, b_z5, b_S, outL, *(float (*)[2])&S[b_index << 1]);
  }

  for (i = 0; i < 8; i++) {
    obj->pAP4_States_HP[i] = S[i];
  }

  for (i = 0; i < 200; i++) {
    b = out3L[i] + outL[i];
    z2[i] = b;
    out3L[i] = b;
  }

  for (i = 0; i < 8; i++) {
    S[i] = obj->pCR2_States_LP[i];
  }

  for (b_index = 0; b_index < 4; b_index++) {
    for (i = 0; i < 3; i++) {
      b_B4H[i] = B2L[i + 3 * b_index];
      b_A4H[i] = A2L[i + 3 * b_index];
    }

    memcpy(&b_z5[0], &z2[0], 200U * sizeof(float));
    for (i = 0; i < 2; i++) {
      b_S[i] = S[i + (b_index << 1)];
    }

    filter(b_B4H, b_A4H, b_z5, b_S, z2, *(float (*)[2])&S[b_index << 1]);
  }

  for (i = 0; i < 8; i++) {
    obj->pCR2_States_LP[i] = S[i];
  }

  for (i = 0; i < 8; i++) {
    S[i] = obj->pCR2_States_HP[i];
  }

  for (b_index = 0; b_index < 4; b_index++) {
    for (i = 0; i < 3; i++) {
      b_B4H[i] = B2H[i + 3 * b_index];
      b_A4H[i] = A2H[i + 3 * b_index];
    }

    memcpy(&b_z5[0], &out3L[0], 200U * sizeof(float));
    for (i = 0; i < 2; i++) {
      b_S[i] = S[i + (b_index << 1)];
    }

    filter(b_B4H, b_A4H, b_z5, b_S, out3L, *(float (*)[2])&S[b_index << 1]);
  }

  for (i = 0; i < 8; i++) {
    obj->pCR2_States_HP[i] = S[i];
  }

  b = obj->pPhaseMult[1];
  for (i = 0; i < 200; i++) {
    z1[i] = z2[i];
    out3L[i] *= b;
  }

  for (i = 0; i < 8; i++) {
    S[i] = obj->pCR3_States_LP[i];
  }

  for (b_index = 0; b_index < 4; b_index++) {
    for (i = 0; i < 3; i++) {
      b_B4H[i] = B3L[i + 3 * b_index];
      b_A4H[i] = A3L[i + 3 * b_index];
    }

    memcpy(&b_z5[0], &z1[0], 200U * sizeof(float));
    for (i = 0; i < 2; i++) {
      b_S[i] = S[i + (b_index << 1)];
    }

    filter(b_B4H, b_A4H, b_z5, b_S, z1, *(float (*)[2])&S[b_index << 1]);
  }

  for (i = 0; i < 8; i++) {
    obj->pCR3_States_LP[i] = S[i];
  }

  for (i = 0; i < 8; i++) {
    S[i] = obj->pCR3_States_HP[i];
  }

  for (b_index = 0; b_index < 4; b_index++) {
    for (i = 0; i < 3; i++) {
      b_B4H[i] = B3H[i + 3 * b_index];
      b_A4H[i] = A3H[i + 3 * b_index];
    }

    memcpy(&b_z5[0], &z2[0], 200U * sizeof(float));
    for (i = 0; i < 2; i++) {
      b_S[i] = S[i + (b_index << 1)];
    }

    filter(b_B4H, b_A4H, b_z5, b_S, z2, *(float (*)[2])&S[b_index << 1]);
  }

  for (i = 0; i < 8; i++) {
    obj->pCR3_States_HP[i] = S[i];
  }

  memcpy(&z3[0], &out3L[0], 200U * sizeof(float));
  for (i = 0; i < 8; i++) {
    S[i] = obj->pAP3_States_LP[i];
  }

  for (b_index = 0; b_index < 4; b_index++) {
    for (i = 0; i < 3; i++) {
      b_B4H[i] = B3L[i + 3 * b_index];
      b_A4H[i] = A3L[i + 3 * b_index];
    }

    memcpy(&b_z5[0], &z3[0], 200U * sizeof(float));
    for (i = 0; i < 2; i++) {
      b_S[i] = S[i + (b_index << 1)];
    }

    filter(b_B4H, b_A4H, b_z5, b_S, z3, *(float (*)[2])&S[b_index << 1]);
  }

  for (i = 0; i < 8; i++) {
    obj->pAP3_States_LP[i] = S[i];
  }

  for (i = 0; i < 8; i++) {
    S[i] = obj->pAP3_States_HP[i];
  }

  b = obj->pPhaseMult[0];
  for (i = 0; i < 3; i++) {
    B3H[i] *= b;
  }

  for (b_index = 0; b_index < 4; b_index++) {
    for (i = 0; i < 3; i++) {
      b_B4H[i] = B3H[i + 3 * b_index];
      b_A4H[i] = A3H[i + 3 * b_index];
    }

    memcpy(&b_z5[0], &out3L[0], 200U * sizeof(float));
    for (i = 0; i < 2; i++) {
      b_S[i] = S[i + (b_index << 1)];
    }

    filter(b_B4H, b_A4H, b_z5, b_S, out3L, *(float (*)[2])&S[b_index << 1]);
  }

  for (i = 0; i < 8; i++) {
    obj->pAP3_States_HP[i] = S[i];
  }

  for (i = 0; i < 200; i++) {
    outL[i] = outH[i];
    z3[i] += out3L[i];
  }

  for (i = 0; i < 8; i++) {
    S[i] = obj->pAP2_States_LP[i];
  }

  for (b_index = 0; b_index < 4; b_index++) {
    for (i = 0; i < 3; i++) {
      b_B4H[i] = B3L[i + 3 * b_index];
      b_A4H[i] = A3L[i + 3 * b_index];
    }

    memcpy(&b_z5[0], &outL[0], 200U * sizeof(float));
    for (i = 0; i < 2; i++) {
      b_S[i] = S[i + (b_index << 1)];
    }

    filter(b_B4H, b_A4H, b_z5, b_S, outL, *(float (*)[2])&S[b_index << 1]);
  }

  for (i = 0; i < 8; i++) {
    obj->pAP2_States_LP[i] = S[i];
  }

  for (i = 0; i < 8; i++) {
    S[i] = obj->pAP2_States_HP[i];
  }

  for (b_index = 0; b_index < 4; b_index++) {
    for (i = 0; i < 3; i++) {
      b_B4H[i] = B3H[i + 3 * b_index];
      b_A4H[i] = A3H[i + 3 * b_index];
    }

    memcpy(&b_z5[0], &outH[0], 200U * sizeof(float));
    for (i = 0; i < 2; i++) {
      b_S[i] = S[i + (b_index << 1)];
    }

    filter(b_B4H, b_A4H, b_z5, b_S, outH, *(float (*)[2])&S[b_index << 1]);
  }

  for (i = 0; i < 8; i++) {
    obj->pAP2_States_HP[i] = S[i];
  }

  for (i = 0; i < 200; i++) {
    b = outL[i] + outH[i];
    z5[i] = b;
    outL[i] = b;
  }

  for (i = 0; i < 8; i++) {
    S[i] = obj->pAP1_States_LP[i];
  }

  for (b_index = 0; b_index < 4; b_index++) {
    for (i = 0; i < 3; i++) {
      b_B4H[i] = B2L[i + 3 * b_index];
      b_A4H[i] = A2L[i + 3 * b_index];
    }

    memcpy(&b_z5[0], &z5[0], 200U * sizeof(float));
    for (i = 0; i < 2; i++) {
      b_S[i] = S[i + (b_index << 1)];
    }

    filter(b_B4H, b_A4H, b_z5, b_S, z5, *(float (*)[2])&S[b_index << 1]);
  }

  for (i = 0; i < 8; i++) {
    obj->pAP1_States_LP[i] = S[i];
  }

  for (i = 0; i < 8; i++) {
    S[i] = obj->pAP1_States_HP[i];
  }

  b = obj->pPhaseMult[1];
  for (i = 0; i < 3; i++) {
    B2H[i] *= b;
  }

  for (b_index = 0; b_index < 4; b_index++) {
    for (i = 0; i < 3; i++) {
      b_B4H[i] = B2H[i + 3 * b_index];
      b_A4H[i] = A2H[i + 3 * b_index];
    }

    memcpy(&b_z5[0], &outL[0], 200U * sizeof(float));
    for (i = 0; i < 2; i++) {
      b_S[i] = S[i + (b_index << 1)];
    }

    filter(b_B4H, b_A4H, b_z5, b_S, outL, *(float (*)[2])&S[b_index << 1]);
  }

  for (i = 0; i < 8; i++) {
    obj->pAP1_States_HP[i] = S[i];
  }

  for (i = 0; i < 200; i++) {
    b = z5[i] + outL[i];
    z4[i] = b;
    z5[i] = b;
  }

  for (i = 0; i < 8; i++) {
    S[i] = obj->pCR4_States_LP[i];
  }

  for (b_index = 0; b_index < 4; b_index++) {
    for (i = 0; i < 3; i++) {
      b_B4H[i] = B4L[i + 3 * b_index];
      b_A4H[i] = A4L[i + 3 * b_index];
    }

    memcpy(&b_z5[0], &z4[0], 200U * sizeof(float));
    for (i = 0; i < 2; i++) {
      b_S[i] = S[i + (b_index << 1)];
    }

    filter(b_B4H, b_A4H, b_z5, b_S, z4, *(float (*)[2])&S[b_index << 1]);
  }

  for (i = 0; i < 8; i++) {
    obj->pCR4_States_LP[i] = S[i];
  }

  for (i = 0; i < 8; i++) {
    S[i] = obj->pCR4_States_HP[i];
  }

  for (b_index = 0; b_index < 4; b_index++) {
    for (i = 0; i < 3; i++) {
      b_B4H[i] = B4H[i + 3 * b_index];
      b_A4H[i] = A4H[i + 3 * b_index];
    }

    memcpy(&b_z5[0], &z5[0], 200U * sizeof(float));
    for (i = 0; i < 2; i++) {
      b_S[i] = S[i + (b_index << 1)];
    }

    filter(b_B4H, b_A4H, b_z5, b_S, z5, *(float (*)[2])&S[b_index << 1]);
  }

  for (i = 0; i < 8; i++) {
    obj->pCR4_States_HP[i] = S[i];
  }

  b = obj->pPhaseMult[0];
  for (i = 0; i < 200; i++) {
    z2[i] *= b;
  }

  b = obj->pPhaseMult[3];
  for (i = 0; i < 200; i++) {
    z5[i] *= b;
  }
}

/*
 * Arguments    : c_audio_internal_FiveBandCrosso *obj
 * Return Type  : void
 */
void c_FiveBandCrossoverFilter_tuneC(c_audio_internal_FiveBandCrosso *obj)
{
  float BL1[12];
  float AL1[12];
  float BH1[12];
  float AH1[12];
  float f0;
  int i2;
  tuneCrossoverFilterCoefficients(obj->CrossoverFrequencies[2],
    obj->CrossoverOrders[2], obj->SampleRate, BL1, AL1, BH1, AH1, &f0);
  obj->pPhaseMult[2] = f0;
  for (i2 = 0; i2 < 12; i2++) {
    obj->pCR1_B_LP[i2] = BL1[i2];
  }

  for (i2 = 0; i2 < 12; i2++) {
    obj->pCR1_A_LP[i2] = AL1[i2];
  }

  for (i2 = 0; i2 < 12; i2++) {
    obj->pCR1_B_HP[i2] = BH1[i2];
  }

  for (i2 = 0; i2 < 12; i2++) {
    obj->pCR1_A_HP[i2] = AH1[i2];
  }

  tuneCrossoverFilterCoefficients(obj->CrossoverFrequencies[1],
    obj->CrossoverOrders[1], obj->SampleRate, BL1, AL1, BH1, AH1, &f0);
  obj->pPhaseMult[1] = f0;
  for (i2 = 0; i2 < 12; i2++) {
    obj->pCR2_B_LP[i2] = BL1[i2];
  }

  for (i2 = 0; i2 < 12; i2++) {
    obj->pCR2_A_LP[i2] = AL1[i2];
  }

  for (i2 = 0; i2 < 12; i2++) {
    obj->pCR2_B_HP[i2] = BH1[i2];
  }

  for (i2 = 0; i2 < 12; i2++) {
    obj->pCR2_A_HP[i2] = AH1[i2];
  }

  tuneCrossoverFilterCoefficients(obj->CrossoverFrequencies[0],
    obj->CrossoverOrders[0], obj->SampleRate, BL1, AL1, BH1, AH1, &f0);
  obj->pPhaseMult[0] = f0;
  for (i2 = 0; i2 < 12; i2++) {
    obj->pCR3_B_LP[i2] = BL1[i2];
  }

  for (i2 = 0; i2 < 12; i2++) {
    obj->pCR3_A_LP[i2] = AL1[i2];
  }

  for (i2 = 0; i2 < 12; i2++) {
    obj->pCR3_B_HP[i2] = BH1[i2];
  }

  for (i2 = 0; i2 < 12; i2++) {
    obj->pCR3_A_HP[i2] = AH1[i2];
  }

  tuneCrossoverFilterCoefficients(obj->CrossoverFrequencies[3],
    obj->CrossoverOrders[3], obj->SampleRate, BL1, AL1, BH1, AH1, &f0);
  obj->pPhaseMult[3] = f0;
  for (i2 = 0; i2 < 12; i2++) {
    obj->pCR4_B_LP[i2] = BL1[i2];
  }

  for (i2 = 0; i2 < 12; i2++) {
    obj->pCR4_A_LP[i2] = AL1[i2];
  }

  for (i2 = 0; i2 < 12; i2++) {
    obj->pCR4_B_HP[i2] = BH1[i2];
  }

  for (i2 = 0; i2 < 12; i2++) {
    obj->pCR4_A_HP[i2] = AH1[i2];
  }
}

/*
 * File trailer for FiveBandCrossoverFilter.c
 *
 * [EOF]
 */
