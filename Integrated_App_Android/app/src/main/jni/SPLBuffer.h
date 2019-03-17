//
//  SPLBuffer.h
//  SPP_Integrated_App
//
//  Created by Akshay Chitale on 7/28/17.
//  Modified by Chowdhury, Tahsin on 11/02/17 for integration
//  Copyright © 2017 UT Dallas. All rights reserved.
//

#ifndef SPLBuffer_h
#define SPLBuffer_h

#include "Settings.h"

// Use when to buffer
#define SPLBUFFER_MODE_FULL 0


// Do not buffer
#define SPLBUFFER_MODE_NONE -1

typedef struct SPLBuffer {
    
    int bufflen;
    int pos;
    float* buff;
    
    // Adds db value to buffer, returns whether settings has been updated
    int(*buffer)(struct SPLBuffer* spl, float db, Settings* settings);
    
} SPLBuffer;

SPLBuffer* newSPLBuffer(int len, int mode);
void destroySPLBuffer(SPLBuffer**);

#endif /* SPLBuffer_h */

