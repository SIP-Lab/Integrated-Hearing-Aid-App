//
//  CompressionSettingController.c
//  Integrated_App
//
//  Created by Tahsin Ahmed Chowdhury on 11/3/17.
//  Copyright © 2017 UT Dallas. All rights reserved.
//

#include "CompressionSettingController.h"

//Giving a Default Set of values
// CR = Compression Ration
// CT = Compression Threshold in dB
// AT = Attack Time in ms
// RT = Release Time in ms
//-----------------CR--CT--AT--RT--------------------//
//..................|...|...|...|....................//
//------------------v...v...v...v--------------------//
float dataIn[20] = {5, -5, 5, 100,    //Band 1
                    5, -10, 5, 100,   //Band 2
                    5, -20, 2, 50,    //Band 3
                    4, -25, 2, 50,    //Band 4
                    4, -35, 2, 100};  //Band 5
//float crossoverSlop = 48.0f; //range is from 6 to 48// see matlab reference with user guide
//float makeupGain = 0.0f;

boolean_T dRC1_not_empty;
boolean_T dRC2_not_empty;
boolean_T dRC3_not_empty;
boolean_T dRC4_not_empty;
boolean_T dRC5_not_empty;
