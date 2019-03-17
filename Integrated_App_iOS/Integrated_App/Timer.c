#include "Timer.h"

Timer*
newTimer(){
	Timer* newTimer = (Timer*)malloc(sizeof(Timer));
	newTimer->startTime.tv_nsec = 0;
	newTimer->startTime.tv_sec = 0;
	newTimer->stopTime.tv_nsec = 0;
	newTimer->stopTime.tv_sec = 0;
	newTimer->totalTime = 0;
	newTimer->totalRuns = 0;
	return newTimer;
}

void startTimer(Timer* timer){
	//clock_gettime(CLOCK_MONOTONIC, &(timer->startTime));
    timer->startTime.tv_nsec = clock();
}

void stopTimer(Timer* timer){
	//clock_gettime(CLOCK_MONOTONIC, &(timer->stopTime));
    timer->stopTime.tv_nsec = clock();
	timer->totalTime += (timer->stopTime.tv_sec - timer->startTime.tv_sec)*1000000000LL + timer->stopTime.tv_nsec - timer->startTime.tv_nsec;
	timer->totalRuns++;
}

void tellTimerTime(Timer* timer){
	printf("Total Function Calls: %u, Total Processing Time: %llu ns, Average Per Run: %f ms",timer->totalRuns, timer->totalTime, timer->totalTime/(timer->totalRuns*1000000.0));
}


float getTimerMS(Timer* timer){
	return timer->totalTime/(timer->totalRuns*1000000.0);
}

void destroyTimer(Timer** timer){
	free(*timer);
	*timer = NULL;
}
