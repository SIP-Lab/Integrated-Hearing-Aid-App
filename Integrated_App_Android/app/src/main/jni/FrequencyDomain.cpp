#include <jni.h>
#include <stdlib.h>
#include <SuperpoweredFrequencyDomain.h>
#include <AndroidIO/SuperpoweredAndroidAudioIO.h>
#include <SuperpoweredAdvancedAudioPlayer.h>
#include <SuperpoweredSimple.h>
#include <SuperpoweredCPU.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_AndroidConfiguration.h>
#include <fstream>
#include <SuperpoweredRecorder.h>

extern "C" {
#include "Timer.h"
#include "SpeechProcessing.h"
}



static SuperpoweredFrequencyDomain *frequencyDomain;
static float  *inputBufferFloat, *data;

//Changes made by Tahsin Ahmed Chowdhury for integration

static float *left, *right, *output;
SuperpoweredAndroidAudioIO *audioIO;
VADNoiseReductionCompression *vadNoiseRedCom;
Settings *settings;

//Variables for Timing
static size_t numOfFrames;
struct timespec begin, end;
static float timeTaken = 0.0f;
static uint64_t *timeBuffer;

bool storeIOData = false;

const char *filePathIn, *filePathOut;
FILE* fileIDIn, *fileIDOut;


// This is called periodically by the media server.
static bool audioProcessing(void * __unused clientdata, short int *audioInputOutput, int numberOfSamples, int __unused samplerate) {

    //start(timer);
    clock_gettime(CLOCK_MONOTONIC_RAW, &begin);

    SuperpoweredShortIntToFloat(audioInputOutput, inputBufferFloat, (unsigned int)numberOfSamples); // Converting the 16-bit integer samples to 32-bit floating point.
    SuperpoweredDeInterleave(inputBufferFloat, left, right, (unsigned int)numberOfSamples);

    if(storeIOData) {//Storing input audio
        fwrite(audioInputOutput, sizeof(short int), numberOfSamples*2, fileIDIn);
    }

    output = (float*)calloc(numberOfSamples, sizeof(float));
    doNoiseReduction_Compression_withVAD(vadNoiseRedCom, left, output, settings);
    SuperpoweredInterleave(output, output, inputBufferFloat, (unsigned int) numberOfSamples);
    SuperpoweredFloatToShortInt(inputBufferFloat, audioInputOutput, (unsigned int)numberOfSamples);

    if(storeIOData) {//Storing processed/output audio
        fwrite(audioInputOutput, sizeof(short int), numberOfSamples*2, fileIDOut);
    }

    if(output!=NULL){
        free(output);
        output = NULL;
    }

    //stop(timer);

    clock_gettime(CLOCK_MONOTONIC_RAW,&end);
    memmove(timeBuffer, timeBuffer+1, 2*(numOfFrames - 1)* sizeof(timeBuffer));
    timeBuffer[numOfFrames - 1] = (end.tv_sec - begin.tv_sec) * 1000000000LL + (end.tv_nsec - begin.tv_nsec);
    timeTaken += (float)(timeBuffer[numOfFrames - 1])/(numOfFrames*1000000.0) - (float)(timeBuffer[0])/(numOfFrames*1000000.0);

    return true;
}



// Main entry to Native Portion of the app, initalizes module variables
extern "C" JNIEXPORT void Java_com_superpowered_IntegratedApp_MainActivity_FrequencyDomain(JNIEnv *javaEnvironment,
                                                                                           jobject __unused obj,
                                                                                           jint samplerate,
                                                                                           jint buffersize,
                                                                                           jboolean storeAudioFlag,
                                                                                           jstring fileIn,
                                                                                           jstring fileOut
                                                                                             ) {

    vadNoiseRedCom = initVAD_NoiseReduction_Compression(settings);
    //timer = newTimer();

    inputBufferFloat = (float *)malloc(buffersize * sizeof(float) * 2 + 128);
    left = (float *)malloc(buffersize * sizeof(float) + 128);
    right = (float *)malloc(buffersize * sizeof(float) + 128);
    data  = (float*)malloc(sizeof(float)*1600);

    storeIOData=storeAudioFlag;
    if (storeIOData) {
        filePathIn = javaEnvironment->GetStringUTFChars(fileIn,JNI_FALSE);
        filePathOut = javaEnvironment->GetStringUTFChars(fileOut,JNI_FALSE);
        fileIDIn = createWAV(filePathIn, samplerate, 2);
        fileIDOut = createWAV(filePathOut, samplerate, 2);
    }

    //calculates number of frames within the gui update interval to provide average processing time
    numOfFrames = (size_t)(samplerate*settings->guiUpdateInterval/buffersize);
    timeBuffer = (uint64_t *)calloc(numOfFrames, sizeof(uint64_t));
    timeTaken = 0.0f;

    SuperpoweredCPU::setSustainedPerformanceMode(true);
    audioIO = new SuperpoweredAndroidAudioIO(samplerate, buffersize, true, true, audioProcessing, javaEnvironment, -1, SL_ANDROID_STREAM_MEDIA, buffersize * 2); // Start audio input/output.
}

// Stops audio media and detroys variables from allocated memory
extern "C" JNIEXPORT
void Java_com_superpowered_IntegratedApp_MainActivity_StopAudio(JNIEnv* javaEnvironment, jobject __unused obj, jstring fileIn, jstring fileOut){

    if(inputBufferFloat != NULL){
        if(audioIO!= nullptr) {
            delete audioIO;
        }
        destroyVAD_NoiseReduction_Compression(&vadNoiseRedCom);
        free(inputBufferFloat);
        if(left!=NULL); {
            free(left);
            left=NULL;
        }
        if(right!=NULL); {
            free(right);
            right=NULL;
        }
        //destroy(&timer);
        inputBufferFloat = NULL;
        if(storeIOData) {
            closeWAV(fileIDIn);
            closeWAV(fileIDOut);
            javaEnvironment->ReleaseStringUTFChars(fileIn, filePathIn);
            javaEnvironment->ReleaseStringUTFChars(fileOut, filePathOut);
        }
    }
}

//Returns frames processing time to noise reduction settings view
extern "C" JNIEXPORT float
Java_com_superpowered_IntegratedApp_NoiseReductionSettings_getExecutionTime(JNIEnv* __unused env,
                                                                            jobject  __unused instance) {

//    if(timer!=NULL){
//        return getMS(timer);
//    } else {
//        return 0.0f;
//    }
    return timeTaken;
}


// settings is loaded by MainActivity onCreate() method
extern "C" JNIEXPORT void
Java_com_superpowered_IntegratedApp_MainActivity_loadSettings(JNIEnv* __unused env, jobject __unused instance) {

    settings = newSettings();
}

// settings is destroyed by MainActivity onDestroy() method
extern "C" JNIEXPORT void
Java_com_superpowered_IntegratedApp_MainActivity_destroySettings(JNIEnv* __unused env, jobject __unused instance) {

    destroySettings(settings);
}

//Returns the Sampling Frequency to display in noise reduction settings view
extern "C" JNIEXPORT int
Java_com_superpowered_IntegratedApp_NoiseReductionSettings_getFs(JNIEnv* __unused env, jobject __unused instance) {

    if (settings != NULL) {
        return settings->fs;
    } else {
        return 0;
    }

}

//Returns the Sampling Frequency to display in noise reduction settings view
extern "C" JNIEXPORT int
Java_com_superpowered_IntegratedApp_MainActivity_getFs(JNIEnv* __unused env, jobject __unused instance) {

    if (settings != NULL) {
        return settings->fs;
    } else {
        return 0;
    }
}

extern "C" JNIEXPORT int
Java_com_superpowered_IntegratedApp_MainActivity_getStepSize(JNIEnv* __unused env, jobject __unused instance) {

    if (settings != NULL) {
        return settings->stepSize;
    } else {
        return 0;
    }

}


extern "C" JNIEXPORT float
Java_com_superpowered_IntegratedApp_NoiseReductionSettings_getWindowSize(JNIEnv* __unused env,
                                                                         jobject __unused instance) {

    if (settings != NULL ) {
        return (1.0f*settings->frameSize/(1.0f*settings->fs))*1000.0f;
    } else {
        return 0.0f;
    }
}

extern "C" JNIEXPORT float
Java_com_superpowered_IntegratedApp_NoiseReductionSettings_getOverlapSize(JNIEnv* __unused env,
                                                                          jobject __unused instance) {

    if (settings != NULL ) {
        return (1.0f*settings->stepSize/(1.0f*settings->fs))*1000.0f;
    } else {
        return 0.0f;
    }

}

extern "C" JNIEXPORT float
Java_com_superpowered_IntegratedApp_NoiseReductionSettings_getNoiseUpdateRate(JNIEnv* __unused env,
                                                                              jobject __unused instance) {

    if (settings != NULL ) {
        return settings->noiseEstimateTime;
    } else {
        return 0.0f;
    }

}

extern "C" JNIEXPORT float
Java_com_superpowered_IntegratedApp_NoiseReductionSettings_getGuiUpdateRate(JNIEnv* __unused env,
                                                                            jobject __unused instance) {

    if (settings != NULL ) {
        return settings->guiUpdateInterval;
    } else {
        return 0.0f;
    }

}

//This returns gui update time in Miliseconds in main activity
extern "C" JNIEXPORT float
Java_com_superpowered_IntegratedApp_MainActivity_getGuiUpdateRate(JNIEnv* __unused env, jobject __unused instance) {

    if (settings != NULL ) {
        return (settings->guiUpdateInterval)*1000.0f;
    } else {
        return 0.0f;
    }

}

extern "C" JNIEXPORT float
Java_com_superpowered_IntegratedApp_NoiseReductionSettings_getSPLCalibration(JNIEnv* __unused env,
                                                                             jobject __unused instance) {

    if (settings != NULL ) {
        return settings->calibration;
    } else {
        return 0.0f;
    }

}

extern "C" JNIEXPORT void
Java_com_superpowered_IntegratedApp_MainActivity_updateSettingsAmplification(JNIEnv* __unused env,
                                                                             jobject __unused instance,
                                                                             jfloat ampValue) {
    if (settings != NULL ) {
        settings->amplification = ampValue;
    }
}

extern "C" JNIEXPORT void
Java_com_superpowered_IntegratedApp_NoiseReductionSettings_updateSettingsSPLCalibration(JNIEnv* __unused env,
                                                                                        jobject __unused instance,
                                                                                        jfloat calibration) {

    if (settings != NULL ) {
        settings->calibration = calibration;
    }

}

extern "C" JNIEXPORT void
Java_com_superpowered_IntegratedApp_NoiseReductionSettings_updateSettingsGUIUpdateRate(JNIEnv* __unused env,
                                                                                       jobject __unused instance,
                                                                                       jfloat guiUpdateRate) {

    if (settings != NULL ) {
        settings->guiUpdateInterval = guiUpdateRate;
    }

}

extern "C" JNIEXPORT void
Java_com_superpowered_IntegratedApp_NoiseReductionSettings_updateSettingsNoiseUpdateRate(
        JNIEnv* __unused  env, jobject __unused instance, jfloat noiseUpdateRate) {

    if (settings != NULL ) {
        settings->noiseEstimateTime = noiseUpdateRate;
    }

}

extern "C" JNIEXPORT jfloatArray
Java_com_superpowered_IntegratedApp_CompressionSettings_getCompressionSettingsData(JNIEnv *env,
                                                                                   jobject __unused instance) {

    jfloatArray dataArray;
    dataArray = env->NewFloatArray(20);
    env->SetFloatArrayRegion(dataArray, 0, 20, dataIn);
    return dataArray;
}

extern "C" JNIEXPORT void
Java_com_superpowered_IntegratedApp_CompressionSettings_updateCompressionSettingsData(JNIEnv* __unused env,
                                                                                      jobject __unused instance,
                                                                                      jint index,
                                                                                      jint data) {
    dataIn[index] = (float)data;
    switch(index)
    {
        case 0:
        case 1:
        case 2:
        case 3:
            dRC1_not_empty = (boolean_T) false;
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            dRC2_not_empty = (boolean_T) false;
            break;

        case 8:
        case 9:
        case 10:
        case 11:
            dRC3_not_empty = (boolean_T) false;
            break;

        case 12:
        case 13:
        case 14:
        case 15:
            dRC4_not_empty = (boolean_T) false;
            break;

        case 16:
        case 17:
        case 18:
        case 19:
            dRC5_not_empty = (boolean_T) false;
            break;

        default:
            break;
    }

}

extern "C" JNIEXPORT int
Java_com_superpowered_IntegratedApp_MainActivity_getDetectedClass(JNIEnv* __unused env, jobject __unused instance) {

    if(settings != NULL){
        return settings->classLabel;
    } else {
        return -1;
    }
}

extern "C" JNIEXPORT float
Java_com_superpowered_IntegratedApp_NoiseReductionSettings_getdbPower(JNIEnv* __unused env,
                                                                      jobject __unused instance) {

    if(settings != NULL){
        return settings->dbpower;
    } else {
        return 0.0f;
    }

}

extern "C" JNIEXPORT void
Java_com_superpowered_IntegratedApp_MainActivity_noiseReductionOn(JNIEnv* __unused env, jobject __unused instance,
                                                                  jboolean on) {

    if (settings != NULL ) {
        settings->noiseReductionOutputType = on;
    }

}

extern "C" JNIEXPORT void
Java_com_superpowered_IntegratedApp_MainActivity_compressionOn(JNIEnv*  __unused env, jobject __unused instance,
                                                               jboolean on) {

    if (settings != NULL ) {
        settings->compressionOutputType = on;
    }

}

extern "C" JNIEXPORT void
Java_com_superpowered_IntegratedApp_MainActivity_setAudioPlay(JNIEnv* __unused env, jobject __unused instance, jint on) {

    if (settings != NULL ) {
        settings->playAudio = on;
    }

}

extern "C" JNIEXPORT int
Java_com_superpowered_IntegratedApp_NoiseReductionSettings_getPlayAudioStatus(JNIEnv* __unused env,
                                                                              jobject __unused instance) {

    if (settings != NULL){
        return settings->playAudio;
    }
    else {
        return 0;
    }


}

extern "C"
JNIEXPORT jfloatArray Java_com_superpowered_IntegratedApp_MainActivity_GetMelImage(JNIEnv * __unused env, jobject __unused instance){
    jfloatArray result;
    result = env->NewFloatArray(1600);


    for (int i = 0; i < 40; i++) {
        for (int j = 0; j < 40; j++) {
            data[40*i+j] = vadNoiseRedCom->melSpectrogram->melSpectrogramImage[i][j];
        }
    }

    env->SetFloatArrayRegion(result, 0, 1600, data);
    return result;
}

extern "C"
JNIEXPORT void JNICALL Java_com_superpowered_IntegratedApp_MainActivity_setDetectedClass(JNIEnv* __unused env, jobject __unused instance, jfloat detectedClass) {
    if (settings!=NULL)
    {
        settings->classLabel = (int)round(detectedClass);
    }
}

extern "C" JNIEXPORT float
Java_com_superpowered_IntegratedApp_MainActivity_getdbPower(JNIEnv* __unused env,
                                                                      jobject __unused instance) {

    if(settings != NULL){
        return settings->dbpower;
    } else {
        return 0.0f;
    }

}