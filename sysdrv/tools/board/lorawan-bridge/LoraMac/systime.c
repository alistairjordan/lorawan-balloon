#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include "systime.h"

SysTime_t SysTimeGet( void ) {
    struct timespec time_local;
    SysTime_t ret;
    clock_gettime(CLOCK_REALTIME, &time_local);
    ret.Seconds = time_local.tv_sec;
    ret.SubSeconds = time_local.tv_nsec / 1000;
    return ret;
}

void SysTimeSet( SysTime_t time_local ) {
// Hmmmm, shall we really ne doing this??
    printf("Should we really be setting system time?");
    struct timespec time_out;
    time_out.tv_sec = time_local.Seconds;
    time_out.tv_nsec = time_local.SubSeconds * 1000;
}

SysTime_t SysTimeAdd( SysTime_t a, SysTime_t b) {
    SysTime_t ret;
    ret.Seconds = (a.Seconds + b.Seconds);
    ret.SubSeconds = (a.SubSeconds + b.SubSeconds);
    return ret;
}

SysTime_t SysTimeSub( SysTime_t a, SysTime_t b) {
    SysTime_t ret;
    ret.Seconds = (a.Seconds - b.Seconds);
    ret.SubSeconds = (a.SubSeconds - b.SubSeconds);
    return ret;
}

SysTime_t SysTimeGetMcuTime( void ) {
    struct timespec time_mcu;
    SysTime_t ret;
    clock_gettime(CLOCK_REALTIME, &time_mcu);
    ret.Seconds = time_mcu.tv_sec;
    ret.SubSeconds = time_mcu.tv_nsec / 1000;
    return ret;   
}