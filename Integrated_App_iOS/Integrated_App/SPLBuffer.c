//
//  SPLBuffer.c
//  SPP_Integrated_App
//
//  Created by Akshay Chitale on 7/28/17.
//  Copyright © 2017 UT Dallas. All rights reserved.
//

#include "SPLBuffer.h"

int _SPLBuffer_buffer_full(SPLBuffer* spl, float db, Settings* settings);
int _SPLBuffer_buffer_none(SPLBuffer* spl, float db, Settings* settings);

SPLBuffer* newSPLBuffer(int len, int mode) {
    SPLBuffer* newSPLBuffer = (SPLBuffer*)malloc(sizeof(SPLBuffer));
    newSPLBuffer->bufflen = (len > 0 ? len : 1);
    newSPLBuffer->pos = 0;
    newSPLBuffer->buff = (float*)calloc(newSPLBuffer->bufflen, sizeof(float));
    switch(mode) {
        case SPLBUFFER_MODE_FULL:
            newSPLBuffer->buffer = _SPLBuffer_buffer_full;
            break;
        case SPLBUFFER_MODE_NONE:
        default:
            newSPLBuffer->buffer = _SPLBuffer_buffer_none;
    }
    return newSPLBuffer;
}

/// Return whether buffer is full (bool)
int _SPLBuffer_add_to_buffer(SPLBuffer* spl, float db) {
    if(spl->pos >= spl->bufflen) {
        spl->pos = 0;
        return 1;
    }
    spl->buff[spl->pos] = db;
    spl->pos++;
    if(spl->pos >= spl->bufflen) {
        spl->pos = 0;
        return 1;
    }
    return 0;
}

/// Buffers, returns when ready or -1 if not ready
///
int _SPLBuffer_buffer_full(SPLBuffer* spl, float db, Settings* settings) {
    if(_SPLBuffer_add_to_buffer(spl, db)) {
        // Buffer full and set dbpower
        int i;
        float dbavg = 0;
        for(i = 0; i < spl->bufflen; i++) {
            dbavg += spl->buff[i];
        }
        dbavg = dbavg / spl->bufflen;

        // Store avg power,
        settings->dbpower = dbavg;
        return 0;
    }
    return -1;
}

/// Never buffers anything, always returns false
int _SPLBuffer_buffer_none(SPLBuffer* spl, float db, Settings* settings) {
    return -1;
}

void destroySPLBuffer(SPLBuffer**);
