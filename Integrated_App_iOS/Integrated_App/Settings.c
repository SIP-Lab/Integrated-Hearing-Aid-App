//
//  Settings.c
//  SPP_Integrated_App
//
//  Created by Akshay Chitale on 6/25/17.
//  Copyright © 2017 UT Dallas. All rights reserved.
//

#import "Settings.h"

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
//    newSettings->fileName = "TestSignal";
//
//    // Points to arrays from AudioSettingsController.swift, so no calloc/free needed here
//    newSettings->lowDef = 55.0;
//    newSettings->mediumDef = 65.0;
//    newSettings->highDef = 75.0;
//    newSettings->lowGains = NULL;
//    newSettings->mediumGains = NULL;
//    newSettings->highGains = NULL;
//    newSettings->gains = NULL;
//    newSettings->numGains = 5;
    newSettings->amplification = 1.0;
    
    newSettings->dbpower = 0;
    newSettings->calibration = -93.9794;
    newSettings->autoGains = 1;
    newSettings->dbUpdateInterval = 0.5;
    newSettings->noiseEstimateTime = 0.4;
//    newSettings->currentGains = 1;
    newSettings->alpha = 0.5;
    newSettings->beta = 0.5;
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
