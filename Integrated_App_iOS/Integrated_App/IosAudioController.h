//
//  IosAudioController.h
//  Aruts
//
//  Created by Simon Epskamp on 10/11/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AudioToolbox/AudioToolbox.h>
#import <AVFoundation/AVFoundation.h>
#import <AudioUnit/AudioUnit.h>

#ifdef __cplusplus
extern "C"{
#endif
#import "Settings.h"
#import "VAD_NR_Com.h"
#ifdef __cplusplus
}
#endif


#ifndef max
#define max( a, b ) ( ((a) > (b)) ? (a) : (b) )
#endif

#ifndef min
#define min( a, b ) ( ((a) < (b)) ? (a) : (b) )
#endif

@interface IosAudioController : NSObject {
	AudioComponentInstance 	audioUnit;
	AudioBuffer tempBuffer; // this will hold the latest data from the microphone
    Settings* settings; // holds previously global settings
}

@property (readonly) AudioComponentInstance audioUnit;
@property (readonly) AudioBuffer tempBuffer;
@property (readwrite) Settings* settings;

- (void) start;
- (void) stop;
- (void) destructSettings;
- (void) processStream;

@end

// setup a global iosAudio variable, accessible everywhere (Only for use in AudioSettingsController.swift)
extern IosAudioController* iosAudio;
