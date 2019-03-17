//
//  CompressionSettingController.h
//  Integrated_App
//
//  Created by Tahsin Ahmed Chowdhury on 11/3/17.
//  Copyright © 2017 UT Dallas. All rights reserved.
//

#ifndef CompressionSettingController_h
#define CompressionSettingController_h

#include <stdio.h>

//Compression Parameters passed throw dataIn
extern float dataIn[20];
//extern float crossoverSlop; //This parameter is kept fixed
//extern float makeupGain;   //This parameter ios kept fixed
typedef unsigned char boolean_T;
//These flags are to updated compression when parameters are changed
extern boolean_T dRC1_not_empty;
extern boolean_T dRC2_not_empty;
extern boolean_T dRC3_not_empty;
extern boolean_T dRC4_not_empty;
extern boolean_T dRC5_not_empty;

#endif /* CompressionSettingController_h */
