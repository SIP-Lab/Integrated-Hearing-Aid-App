//
//  Settings.h
//  SPP_Integrated_App
//
//  Created by Akshay Chitale on 6/25/17.
// Modified by Tahsin Ahmed Chowdhury for Integration
//  Copyright © 2017 UT Dallas. All rights reserved.
//

#import <stdlib.h>

#ifndef Settings_h
#define Settings_h

typedef struct Settings {
    // Core values
    int fs;
    int frameSize;
    int stepSize;
    
    // Audio status flags
    int micStatus;
    int playAudio;
    
    // To modify output audio type
    int noiseReductionOutputType;
    int compressionOutputType;
    int doSaveFile;
    int classLabel;
    int decisionBufferLength;
    float decisionRate;
    float amplification;
    
    // For audio level based gains switching
    float dbpower;
    float quiet;
    float calibration;
    int autoGains;
    float guiUpdateInterval; // How long to wait (in seconds) before updating dbpower
    float noiseEstimateTime; //Noise estimation time (in seconds)
    
    float processTime;
    float noiseEstimationFrame;
} Settings;

Settings* newSettings();
void destroySettings(Settings* settings);

#endif /* Settings_h */

