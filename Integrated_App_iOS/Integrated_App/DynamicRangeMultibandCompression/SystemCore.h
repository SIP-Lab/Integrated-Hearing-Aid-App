/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: SystemCore.h
 *
 * MATLAB Coder version            : 3.3
 * C/C++ source code generated on  : 28-Sep-2017 12:50:36
 */

#ifndef SYSTEMCORE_H
#define SYSTEMCORE_H

/* Include Files */
#include <float.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "DynamicRangeFiveBandCompression_types.h"

/* Function Declarations */
extern void SystemCore_parenReference(crossoverFilter *obj, const float
  varargin_1[200], float varargout_1[200], float varargout_2[200], float
  varargout_3[200], float varargout_4[200], float varargout_5[200]);
extern void SystemCore_setup(c_audio_internal_FiveBandCrosso *obj);
extern void b_SystemCore_parenReference(compressor *obj, const float varargin_1
  [200], float varargout_1[200]);

#endif

/*
 * File trailer for SystemCore.h
 *
 * [EOF]
 */
