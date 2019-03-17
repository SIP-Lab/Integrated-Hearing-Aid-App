/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: wiener_ADAPTIVE_NE_types.h
 *
 * MATLAB Coder version            : 3.4
 * C/C++ source code generated on  : 06-Oct-2017 12:42:28
 */

#ifndef wiener_ADAPTIVE_NE_TYPES_H
#define wiener_ADAPTIVE_NE_TYPES_H

/* Include Files */
#include "rtwtypes.h"

/* Type Definitions */
#ifndef struct_emxArray_real32_T
#define struct_emxArray_real32_T

struct emxArray_real32_T
{
  float *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

#endif                                 /*struct_emxArray_real32_T*/

#ifndef typedef_emxArray_real32_T
#define typedef_emxArray_real32_T

typedef struct emxArray_real32_T emxArray_real32_T;

#endif                                 /*typedef_emxArray_real32_T*/
#endif

/*
 * File trailer for wiener_ADAPTIVE_NE_types.h
 *
 * [EOF]
 */
