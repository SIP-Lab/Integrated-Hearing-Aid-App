#ifndef TIMER_H
#define TIMER_H

#include <stdlib.h>
#include <time.h>
#include <stdio.h>

typedef struct Timer {
		unsigned int totalRuns;
		unsigned long long totalTime;
        struct timespec startTime;
        struct timespec stopTime;
} Timer;

Timer* newTimer(void);

void startvoidTimer(Timer* timer);
void stopTimer(Timer* timer);

void tellTimerTime(Timer* timer);

float getTimerMS(Timer* timer);

void destroyTimer(Timer** timer);

#endif
