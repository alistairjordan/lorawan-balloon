#include "timer.h"
#include "rtc-board.h"

void RtcProcess( void )
{
    // Not used on this platform.
}


uint32_t GettimeToTicks(struct timespec *ts) {
    uint32_t time;
    time += ts->tv_sec * 1000000000;
    time += ts->tv_nsec;
    return time;
}

struct timespec TicksToGettime(uint32_t time) {
    struct timespec ts;
    ts.tv_sec += time / 1000000000;
    ts.tv_nsec += time % 1000000000;
    return ts;
}

TimerTime_t RtcTempCompensation( TimerTime_t period, float temperature )
{
    //aka, not implemented on this board.
    TimerTime_t time = 0;
    return time;
}

uint32_t RtcGetMinimumTimeout( void )
{
    return 0; //( MIN_ALARM_DELAY );
}

uint32_t RtcGetTimerValue(void) {
    uint32_t time;
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    time = GettimeToTicks(&ts);
}

uint32_t RtcTick2Ms(uint32_t clock) {
    return clock / 1000000;
}

uint32_t RtcMs2Tick(uint32_t clock) {
    return clock * 1000000;
}

void RtcSetAlarm( uint32_t timeout ) {
    struct timespec ts = TicksToGettime(timeout);
    nanosleep(&ts, NULL);
}

void RtcStopAlarm(void) {
    //not needed;
}