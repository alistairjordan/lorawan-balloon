#ifndef SYSTIME_S
#define SYSTIME_S

#include <stdint.h>

#ifndef TimerTime_t
typedef uint32_t TimerTime_t;
#define TIMERTIME_T_MAX                             ( ( uint32_t )~0 )
#endif

typedef struct SysTime_s {
    uint32_t Seconds;
    uint32_t SubSeconds;
}SysTime_t;

/*!
 * \brief Sets new system time
 *
 * \param  sysTime    New seconds/sub-seconds since UNIX epoch origin
 */
void SysTimeSet( SysTime_t sysTime );

/*!
 * \brief Gets current system time
 *
 * \retval sysTime    Current seconds/sub-seconds since UNIX epoch origin
 */
SysTime_t SysTimeGet( void );
SysTime_t SysTimeAdd( SysTime_t a, SysTime_t b);
SysTime_t SysTimeSub( SysTime_t a, SysTime_t b);

/*!
 * \brief Gets current MCU system time
 *
 * \retval sysTime    Current seconds/sub-seconds since Mcu started
 */
SysTime_t SysTimeGetMcuTime( void );

/*!
 * Converts the given SysTime to the equivalent RTC value in milliseconds
 *
 * \param [IN] sysTime System time to be converted
 * 
 * \retval timeMs The RTC converted time value in ms
 */
TimerTime_t SysTimeToMs( SysTime_t sysTime );

/*!
 * Converts the given RTC value in milliseconds to the equivalent SysTime
 *
 * \param [IN] timeMs The RTC time value in ms to be converted
 * 
 * \retval sysTime Converted system time
 */
SysTime_t SysTimeFromMs( TimerTime_t timeMs );

/*!
 * \brief Convert a calendar time into time since UNIX epoch as a uint32_t.
 *
 * \param [IN] localtime Pointer to the object containing the calendar time
 * \retval     timestamp The calendar time as seconds since UNIX epoch.
 */
uint32_t SysTimeMkTime( const struct tm* localtime );

/*!
 * \brief Converts a given time in seconds since UNIX epoch into calendar time.
 *
 * \param [IN]  timestamp The time since UNIX epoch to convert into calendar time.
 * \param [OUT] localtime Pointer to the calendar time object which will contain
                          the result of the conversion.
 */
void SysTimeLocalTime( const uint32_t timestamp, struct tm *localtime );

/*!
 * \brief Number of seconds elapsed between Unix and GPS epoch
 */
#define UNIX_GPS_EPOCH_OFFSET                       315964800



#endif