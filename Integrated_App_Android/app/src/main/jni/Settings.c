//
//  Settings.c
//  SPP_Integrated_App
//
//  Created by Akshay Chitale on 6/25/17.
// Modified by Tahsin Ahmed Chowdhury for Integration
//  Copyright © 2017 UT Dallas. All rights reserved.
//

#include "Settings.h"

Settings* newSettings() {
    
    Settings* newSettings = (Settings*)malloc(sizeof(Settings));
    
    // Set defaults
    newSettings->fs =  48000;
    newSettings->frameSize = 1200; //64;
    newSettings->stepSize = 600; //600;
    newSettings->doSaveFile = 0;
    newSettings->micStatus = 1;
    newSettings->playAudio = 0;
    newSettings->classLabel = -1;
    
    newSettings->noiseReductionOutputType = 0;
    newSettings->compressionOutputType = 0;
    newSettings->amplification = 1.0f;
    newSettings->decisionBufferLength = 16;
    newSettings->decisionRate = 1.0f;
    
    newSettings->dbpower = 0;
    //newSettings->quiet = 52.0f;//For iOS
    newSettings->quiet = 60.0f; // For Android
    newSettings->calibration = -93.9794f;
    newSettings->autoGains = 1;
    newSettings->guiUpdateInterval = 0.15f;
    newSettings->noiseEstimateTime = 0.4f;
    newSettings->processTime = 0.0f;
    newSettings->noiseEstimationFrame = newSettings->noiseEstimateTime*(float)(newSettings->fs)/(float)(newSettings->stepSize);
    return newSettings;
}

void destroySettings(Settings* settings) {
    if(settings != NULL){
        free(settings);
        settings = NULL;
    }
}

