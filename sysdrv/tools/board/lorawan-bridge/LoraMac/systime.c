#include <stddef.h>
#include "systime.h"

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