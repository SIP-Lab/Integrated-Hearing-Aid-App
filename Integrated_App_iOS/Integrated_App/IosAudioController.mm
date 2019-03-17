//
//  IosAudioController.m
//  Aruts
//
//  Created by Simon Epskamp on 10/11/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "IosAudioController.h"
#import <AudioToolbox/AudioToolbox.h>
#import <time.h>
#import "TPCircularBuffer.h"
#import "MovingAverageBuffer.h"
#import "TensorFlowInference.h"

#define kOutputBus 0
#define kInputBus 1
#define FRAMESIZE 64

#define SAVEBUTTON 1
#define LOADBUTTON 0
#define SAVEFEATBUTTON 0

NSURL *fileURL;
FILE* f1, *f2;

const char *pathHybridDir;
const char *pathFeatureSaveDir;
FILE     *file;
FILE     *file2;
NSString *str;
NSString *UserStr;

IosAudioController* iosAudio;
int count = 0;
AudioStreamBasicDescription audioFormat;
NSData *classifierParams;
VADNoiseReductionCompression *vadNoiseRedCom;
ExtAudioFileRef fileRef;
TPCircularBuffer* inBuffer;
TPCircularBuffer* outBuffer;
int pCount=0;
NSTimer* timer, *vadTimer;
NSDate* startTime;
MovingAverageBuffer* timeBuffer;
MovingAverageBuffer* classBuffer;

TensorFlowInference* tensorflowInference;

void checkStatus(OSStatus error){
    if (error) {
        char errorString[20];
        // See if it appears to be a 4-char-code
        *(UInt32 *)(errorString + 1) = CFSwapInt32HostToBig(error);
        if (isprint(errorString[1]) && isprint(errorString[2]) &&
            isprint(errorString[3]) && isprint(errorString[4])) {
            errorString[0] = errorString[5] = '\'';
            errorString[6] = '\0';
        } else
            // No, format it as an integer
            sprintf(errorString, "%d", (int)error);
        
        fprintf(stderr, "Error: (%s)\n", errorString);
    }
}

//Error Check Function
static void CheckError (OSStatus error, const char *operation) {
    if (error == noErr) return;
    
    char errorString[20];
    // See if it appears to be a 4-char-code
    *(UInt32 *)(errorString + 1) = CFSwapInt32HostToBig(error);
    if (isprint(errorString[1]) && isprint(errorString[2]) &&
        isprint(errorString[3]) && isprint(errorString[4])) {
        errorString[0] = errorString[5] = '\'';
        errorString[6] = '\0';
    } else
        // No, format it as an integer
        sprintf(errorString, "%d", (int)error);
    
    fprintf(stderr, "Error: %s (%s)\n", operation, errorString);
    
    exit(1);
}

// deprecated
//void initAudio(NSString* fileName) {
//
//    // You can change the file to use by changing the source below with
//    // New audio resource location is AudioFiles
//    NSString* source = [[NSBundle mainBundle] pathForResource:fileName ofType:@"caf" inDirectory:@"AudioFiles"];
//    const char * cString = [source cStringUsingEncoding:NSASCIIStringEncoding];
//    CFStringRef str = CFStringCreateWithCString(NULL, cString, kCFStringEncodingMacRoman);
//    CFURLRef inputFileURL = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, str, kCFURLPOSIXPathStyle, false);
//
//    AudioFileID fileID;
//    OSStatus err = AudioFileOpenURL(inputFileURL, kAudioFileReadPermission, 0, &fileID);
//    CheckError(err, "AudioFileOpenURL");
//
//
//    err = ExtAudioFileOpenURL(inputFileURL, &fileRef);
//    CheckError(err, "ExtAudioFileOpenURL");
//
//    err = ExtAudioFileSetProperty(fileRef, kExtAudioFileProperty_ClientDataFormat, sizeof(AudioStreamBasicDescription), &audioFormat);
//    CheckError(err, "ExtAudioFileSetProperty");
//}


/**
 This callback is called when new audio data from the microphone is
 available.
 */
static OSStatus recordingCallback(void *inRefCon,
                                  AudioUnitRenderActionFlags *ioActionFlags,
                                  const AudioTimeStamp *inTimeStamp,
                                  UInt32 inBusNumber,
                                  UInt32 inNumberFrames,
                                  AudioBufferList *ioData) {
    
    Settings* settings = (Settings*)(inRefCon); // For accessing settings
    
    // Because of the way our audio format (setup below) is chosen:
    // we only need 1 buffer, since it is mono
    // Samples are 16 bits = 2 bytes.
    // 1 frame includes only 1 sample
    
    AudioBuffer buffer;
    
    buffer.mNumberChannels = 1;
    buffer.mDataByteSize = inNumberFrames * 2;
    buffer.mData = malloc( inNumberFrames * 2 );
    
    // Put buffer in a AudioBufferList
    AudioBufferList bufferList;
    bufferList.mNumberBuffers = 1;
    bufferList.mBuffers[0] = buffer;
    
    // Then:
    // Obtain recorded samples
    
    if (settings->micStatus) {
        OSStatus status;
        
        status = AudioUnitRender([iosAudio audioUnit],
                                 ioActionFlags,
                                 inTimeStamp,
                                 inBusNumber,
                                 inNumberFrames,
                                 &bufferList);
        checkStatus(status);
        
        // Now, we have the samples we just read sitting in buffers in bufferList
        // Process the new data
        TPCircularBufferProduceBytes(inBuffer, (void*)bufferList.mBuffers[0].mData, bufferList.mBuffers[0].mDataByteSize);
        
        if(inBuffer->fillCount >= settings->frameSize*sizeof(short)){
            startTime = [NSDate date];
            [iosAudio processStream];
            [timeBuffer addDatum:[NSNumber numberWithFloat:[[NSDate date] timeIntervalSinceDate:startTime]]];
        }
    }
    else {
        UInt32 frameCount = settings->frameSize;
        OSStatus err = ExtAudioFileRead(fileRef, &frameCount, &bufferList);
        CheckError(err,"File Read");
        if(frameCount > 0) {
            
            AudioBuffer audioBuffer = bufferList.mBuffers[0];
            
            TPCircularBufferProduceBytes(inBuffer, audioBuffer.mData, audioBuffer.mDataByteSize);
            
            if (inBuffer->fillCount >= settings->frameSize*sizeof(short)) {
                startTime = [NSDate date];
                [iosAudio processStream];
                [timeBuffer addDatum:[NSNumber numberWithFloat:[[NSDate date] timeIntervalSinceDate:startTime]]];
            }
        }
        
        else{
            
            [iosAudio stop];
            //enableButtons();
        }
    }
    
    
    // release the malloc'ed data in the buffer we created earlier
    free(bufferList.mBuffers[0].mData);
    
    return noErr;
}

/**
 This callback is called when the audioUnit needs new data to play through the
 speakers. If you don't have any, just don't write anything in the buffers
 */
static OSStatus playbackCallback(void *inRefCon,
                                 AudioUnitRenderActionFlags *ioActionFlags,
                                 const AudioTimeStamp *inTimeStamp,
                                 UInt32 inBusNumber,
                                 UInt32 inNumberFrames,
                                 AudioBufferList *ioData) {
    
    Settings* settings = (Settings*)(inRefCon); // For accessing settings

    // Notes: ioData contains buffers (may be more than one!)
    // Fill them up as much as you can. Remember to set the size value in each buffer to match how
    // much data is in the buffer.
    if (!settings->playAudio) {
        void* emptyBuffer = calloc(inNumberFrames,sizeof(short));
        memcpy(ioData->mBuffers[0].mData, emptyBuffer, ioData->mBuffers[0].mDataByteSize);
        free(emptyBuffer);
        return noErr;
    }
    
    
    for (int i=0; i < ioData->mNumberBuffers; i++) { // in practice we will only ever have 1 buffer, since audio format is mono
        AudioBuffer buffer = ioData->mBuffers[i];
        
        if (outBuffer->fillCount > ioData->mBuffers[0].mDataByteSize) {
            
            int32_t availableBytes;
            short *tail = (short*)TPCircularBufferTail(outBuffer, &availableBytes);
            
            memcpy(buffer.mData, tail, buffer.mDataByteSize);
            TPCircularBufferConsume(outBuffer, buffer.mDataByteSize);
            
        }
    }
    
    return noErr;
}

@implementation IosAudioController{
//    tensorflow::GraphDef graph;
//    tensorflow::Session *session;
}

@synthesize audioUnit, tempBuffer, settings;

/**
 Initialize the audioUnit and allocate our own temporary buffer.
 The temporary buffer will hold the latest data coming in from the microphone,
 and will be copied to the output when this is requested.
 */
- (id) init {
    self = [super init];
    
    self.settings = newSettings();
    
    [self initAudioUnit:true];
    //[self prepareAudioFileToRecord];
    return self;
}

/**
 Destroy settings struct
 */
- (void) destructSettings {
    destroySettings(self.settings);
}

/**
 Initializes audio, if BOOL constructor is true, does it for the initializer
 */
- (void) initAudioUnit: (BOOL)constructor {
    OSStatus status;
    //self.audioRecorderPointer = &audioRecorder;
    if(!constructor) {
        CheckError(AudioUnitUninitialize(audioUnit), "AudioUnit Uninitialization");
    }
    
    [[AVAudioSession sharedInstance] setCategory:AVAudioSessionCategoryPlayAndRecord
                                           error:NULL];
    [[AVAudioSession sharedInstance] setMode:AVAudioSessionModeMeasurement
                                       error:NULL];
    [[AVAudioSession sharedInstance] setPreferredSampleRate:self.settings->fs
                                                      error:NULL];
    [[AVAudioSession sharedInstance] setPreferredIOBufferDuration:FRAMESIZE/(float)self.settings->fs
                                                            error:NULL];
    
    // Describe audio component
    AudioComponentDescription desc;
    desc.componentType = kAudioUnitType_Output;
    desc.componentSubType = kAudioUnitSubType_RemoteIO;
    desc.componentFlags = 0;
    desc.componentFlagsMask = 0;
    desc.componentManufacturer = kAudioUnitManufacturer_Apple;
    
    // Get component
    AudioComponent inputComponent = AudioComponentFindNext(NULL, &desc);
    
    // Get audio units
    status = AudioComponentInstanceNew(inputComponent, &audioUnit);
    checkStatus(status);
    
    // Enable IO for recording
    UInt32 flag = 1;
    status = AudioUnitSetProperty(audioUnit,
                                  kAudioOutputUnitProperty_EnableIO,
                                  kAudioUnitScope_Input,
                                  kInputBus,
                                  &flag,
                                  sizeof(flag));
    checkStatus(status);
    
    // Enable IO for playback
    //    if(!constructor) {
    //        flag = [self settings]->playAudio;
    //    }
    flag = 1;
    status = AudioUnitSetProperty(audioUnit,
                                  kAudioOutputUnitProperty_EnableIO,
                                  kAudioUnitScope_Output,
                                  kOutputBus,
                                  &flag,
                                  sizeof(flag));
    checkStatus(status);
    
    // Describe format
    //AudioStreamBasicDescription audioFormat;
    audioFormat.mSampleRate            = [self settings]->fs;
    if(constructor) {
        audioFormat.mFormatID            = kAudioFormatLinearPCM;
        audioFormat.mFormatFlags        =  kAudioFormatFlagIsPacked | kAudioFormatFlagIsSignedInteger;
        audioFormat.mFramesPerPacket    = 1;
        audioFormat.mChannelsPerFrame    = 1;
        audioFormat.mBitsPerChannel        = 16;
        audioFormat.mBytesPerPacket        = 2;
        audioFormat.mBytesPerFrame        = 2;
    }
    
    // Apply format
    status = AudioUnitSetProperty(audioUnit,
                                  kAudioUnitProperty_StreamFormat,
                                  kAudioUnitScope_Output,
                                  kInputBus,
                                  &audioFormat,
                                  sizeof(audioFormat));
    checkStatus(status);
    status = AudioUnitSetProperty(audioUnit,
                                  kAudioUnitProperty_StreamFormat,
                                  kAudioUnitScope_Input,
                                  kOutputBus,
                                  &audioFormat,
                                  sizeof(audioFormat));
    checkStatus(status);
    
    
    // Set input callback
    AURenderCallbackStruct callbackStruct;
    callbackStruct.inputProc = recordingCallback;
    callbackStruct.inputProcRefCon = (void *)([self settings]); // Only need settings, not all of self
    status = AudioUnitSetProperty(audioUnit,
                                  kAudioOutputUnitProperty_SetInputCallback,
                                  kAudioUnitScope_Global,
                                  kInputBus,
                                  &callbackStruct,
                                  sizeof(callbackStruct));
    checkStatus(status);
    
    // Set output callback
    callbackStruct.inputProc = playbackCallback;
    callbackStruct.inputProcRefCon = (void *)([self settings]); // Only need settings, not all of self
    status = AudioUnitSetProperty(audioUnit,
                                  kAudioUnitProperty_SetRenderCallback,
                                  kAudioUnitScope_Global,
                                  kOutputBus,
                                  &callbackStruct,
                                  sizeof(callbackStruct));
    checkStatus(status);
    
    // Disable buffer allocation for the recorder (optional - do this if we want to pass in our own)
    flag = 0;
    status = AudioUnitSetProperty(audioUnit,
                                  kAudioUnitProperty_ShouldAllocateBuffer,
                                  kAudioUnitScope_Output,
                                  kInputBus,
                                  &flag,
                                  sizeof(flag));
    
    
    // Allocate our own buffers (1 channel, 16 bits per sample, thus 16 bits per frame, thus 2 bytes per frame).
    // Practice learns the buffers used contain 512 frames, if this changes it will be fixed in processAudio.
    tempBuffer.mNumberChannels = 1;
    tempBuffer.mDataByteSize = [self settings]->frameSize * sizeof(short);
    tempBuffer.mData = malloc( [self settings]->frameSize * sizeof(short) );
    
    NSLog(@"Sampling rate: %.0f", [AVAudioSession sharedInstance].sampleRate);
    NSLog(@"Input Buffer: %.0f", [AVAudioSession sharedInstance].IOBufferDuration*[AVAudioSession sharedInstance].sampleRate);
    
    // Initialise
    timeBuffer = [[MovingAverageBuffer alloc] initWithPeriod:round(settings->fs/settings->stepSize)];
    classBuffer = [[MovingAverageBuffer alloc] initWithPeriod:11];
    status = AudioUnitInitialize(audioUnit);
    checkStatus(status);
}

/**
 Start the audioUnit. This means data will be provided from
 the microphone, and requested for feeding to the speakers, by
 use of the provided callbacks.
 */


- (void) start {
    
    openFile();
    
    vadNoiseRedCom = initVAD_NoiseReduction_Compression([self settings], pathHybridDir, file2);
    inBuffer = (TPCircularBuffer*)malloc(sizeof(TPCircularBuffer));
    outBuffer = (TPCircularBuffer*)malloc(sizeof(TPCircularBuffer));
    TPCircularBufferInit(inBuffer, 4096);
    TPCircularBufferInit(outBuffer, 4096);
    
    [self initAudioUnit:false];
    //initAudio([NSString stringWithUTF8String:[self settings]->fileName]); // deprecated
    
    audioFormat.mSampleRate = [self settings]->fs;
    OSStatus status = AudioUnitSetProperty(audioUnit,
                                           kAudioUnitProperty_StreamFormat,
                                           kAudioUnitScope_Output,
                                           kInputBus,
                                           &audioFormat,
                                           sizeof(audioFormat));
    checkStatus(status);
    
    
    
    status = AudioOutputUnitStart(audioUnit);
    checkStatus(status);
    printf("Audio Started (IosAudioController.m)\n");
    if(settings->doSaveFile)
    {
        NSDateFormatter *formatter;
        NSString *dateString;
        
        formatter = [[NSDateFormatter alloc] init];
        [formatter setDateFormat:@"MM_dd_yyyy_HH_mm_ss"];
        dateString = [formatter stringFromDate:[NSDate date]];

        NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString* documentsDirectory = [paths objectAtIndex:0];
        
        NSString* path1 = [documentsDirectory stringByAppendingPathComponent:[NSString stringWithFormat:@"%@.txt",[NSString stringWithFormat:@"Output_Data_%@",dateString]]];
        [[NSFileManager defaultManager] createFileAtPath:path1 contents:[NSData new] attributes: nil];
        const char *outFile = [ path1 UTF8String];
        f1 = fopen(outFile, "ab");
        printf("%s\n", outFile);
        if(f1==NULL) printf("Output data file not created: NULL\n");
        
        NSString* path2 = [documentsDirectory stringByAppendingPathComponent:[NSString stringWithFormat:@"%@.txt",[NSString stringWithFormat:@"Input_Data_%@",dateString]]];
        [[NSFileManager defaultManager] createFileAtPath:path2 contents:[NSData new] attributes: nil];
        const char *inFile = [ path2 UTF8String];
        f2 = fopen(inFile, "ab");
        printf("%s\n", inFile);
        if(f2==NULL) printf("Input data file not created: NULL\n");
    }
    
    
    
    timer = [NSTimer scheduledTimerWithTimeInterval:0.5
                                            repeats:YES
                                              block: ^(NSTimer* timer){
                                                  settings->processTime = 1000*timeBuffer.movingAverage;
                                              }];
    
    
    tensorflowInference = [TensorFlowInference alloc];
    
    vadTimer = [NSTimer scheduledTimerWithTimeInterval:0.0625
                                               repeats:YES
                                                 block: ^(NSTimer* timer){
                                                     [tensorflowInference predict:vadNoiseRedCom settings:settings];
                                                 }];
    NSString* source = [[NSBundle mainBundle] pathForResource:@"frozen_without_dropout" ofType:@"pb"];
    if(!source){
        NSLog(@"Unable to find file in the bundle");
    }
    else {
        // Load Tensorflow model
        [tensorflowInference loadGraphFromPath:source];
        
        // Create Tensorflow session
        [tensorflowInference createSession];
    }
}

/**
 Stop the audioUnit
 */
- (void) stop {
    OSStatus status = AudioOutputUnitStop(audioUnit);
    checkStatus(status);
    [timer invalidate];
    [vadTimer invalidate];
    fclose(file);
    timer = nil;
    vadTimer = nil;
    destroyVAD_NoiseReduction_Compression(&vadNoiseRedCom);
    if(f1!=NULL) fclose(f1);
    if(f2!=NULL) fclose(f2);
    printf("Audio Stopped (IosAudioController.m)\n");
}

- (void) processStream {
    
    //Frame Size
    UInt32 frameSize = settings->stepSize * sizeof(short);
    int32_t availableBytes;
    int n = 0;
    //Initialize Temporary buffers for processing
    short *tail = (short*)TPCircularBufferTail(inBuffer, &availableBytes);
    
    if (availableBytes > frameSize){
        short *buffer = (short*)malloc(frameSize), *output = (short*)malloc(frameSize);
        
        memcpy(buffer, tail, frameSize);
        
        TPCircularBufferConsume(inBuffer, frameSize);
        
        doNoiseReduction_Compression_withVAD(vadNoiseRedCom, buffer, output, settings);
        
        if(settings->doSaveFile)
        {
            for (n=0 ; n<settings->stepSize ; n++)
            {
               fprintf ( f1, "%f\n", output[n]*S2F);
               fprintf ( f2, "%f\n", buffer[n]*S2F);
            }
        }
        
        TPCircularBufferProduceBytes(outBuffer, output, frameSize);
        
        free(output);
        free(buffer);

        //SAVE VAD CLASS TO SETTINGS
        //settings->classLabel= getClass(vadNoiseRedCom->memoryPointer);
    }
    
}


void openFile()
{
    classifierParams = [NSData dataWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"GMMParam"
                                                                                      ofType:@"txt"]];
}



/*--------------------------Tensorflow Code-----------------------*/

// Input Tensor
//tensorflow::Tensor x(tensorflow::DT_FLOAT,
//                     tensorflow::TensorShape({40,40}));
//
//
//std::vector<std::pair<std::string, tensorflow::Tensor>> inputs = {
//    {"inputs/x-input", x},
//};
//std::vector<std::string> nodes = {
//    {"model/Softmax"}
//};
//std::vector<tensorflow::Tensor> outputs;
//
///**
// Load the tensorflow graph into memory from the specified path
//
// @param path Path to the graph on the machine
// @return If the graph has been loaded or not
// */
//- (BOOL)loadGraphFromPath:(NSString *)path {
//
//    auto status = ReadBinaryProto(tensorflow::Env::Default(), path.fileSystemRepresentation, &graph);
//    if (!status.ok()) {
//        NSLog(@"Error reading graph: %s", status.error_message().c_str());
//        return NO;
//    }
//
//    // This prints out the names of the nodes in the graph.
//    auto nodeCount = graph.node_size();
//    //NSLog(@"Node count: %d", nodeCount);
//    for (auto i = 0; i < nodeCount; ++i) {
//        auto node = graph.node(i);
//        //NSLog(@"Node %d: %s '%s'", i, node.op().c_str(), node.name().c_str());
//    }
//
//    return YES;
//}
//
//
///**
// Start the tensorflow session
//
// @return If the session has been started or not
// */
//- (BOOL)createSession {
//    tensorflow::SessionOptions options;
//    auto status = tensorflow::NewSession(options, &session);
//    if (!status.ok()) {
//        NSLog(@"Error creating session %s",
//              status.error_message().c_str());
//        return NO;
//    }
//
//    status = session->Create(graph);
//    if (!status.ok()) {
//        NSLog(@"Error adding graph to session: %s",
//              status.error_message().c_str());
//        return NO;
//    }
//    return YES;
//}
//
//- (void)predict {
//
//    auto input  = x.tensor<float, 2>();
//
//    for (int i = 0; i < 40; i++) {
//        for (int j = 0; j < 40; j++) {
//            input(i, j) = vadNoiseRedCom->melSpectrogram->melSpectrogramImage[i][j];
//        }
//    }
//
//    auto status = session->Run(inputs, nodes, {}, &outputs);
//    if(!status.ok()) {
//        NSLog(@"Error running model: %s", status.error_message().c_str());
//        return;
//    }
//
//    auto pred = outputs[0].tensor<float, 2>();
//    settings->classLabel = round(pred(0,1)) + 1;
//    NSLog(@"%f", pred(0,1));
//}



@end

