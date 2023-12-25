/*!
 * \file      board.c
 *
 * \brief     Target board general functions implementation
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2017 Semtech
 *
 * \endcode
 *
 * \author    Alistair Jordan 
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 */
#include <stdio.h>
#include "utilities.h"
#include "spi.h"
#include "board-config.h"
#include "sx1276-board.h"
#include "board.h"
#include <stdlib.h>
#include <time.h>

void BoardCriticalSectionBegin( uint32_t *mask )
{
}

void BoardCriticalSectionEnd( uint32_t *mask )
{
}

void BoardInitPeriph( void )
{

}

void BoardInitMcu( void )
{

    SpiInit( &SX1276.Spi, SPI_1);
    SX1276IoInit( );

    SX1276IoDbgInit( );
    SX1276IoTcxoInit( );

}

void BoardResetMcu( void )
{
}

void BoardDeInitMcu( void )
{
    SpiDeInit( &SX1276.Spi );
    SX1276IoDeInit( );
}

uint32_t BoardGetRandomSeed( void )
{
    srand(time(NULL));   // Initialization, should only be called once.
    int r = rand();      // Returns a pseudo-random integer between 0 and RAND_MAX.
    return (uint32_t)r;
}

void BoardGetUniqueId( uint8_t *id )
{
    *id = 127;
}

uint16_t BoardBatteryMeasureVoltage( void )
{
    return 0;
}

uint32_t BoardGetBatteryVoltage( void )
{
    return 0;
}

uint8_t BoardGetBatteryLevel( void )
{
    return 0;
}

uint8_t GetBoardPowerSource( void )
{
    return 255;
}

/**
  * \brief Enters Low Power Stop Mode
  *
  * \note ARM exists the function when waking up
  */
void LpmEnterStopMode( void)
{
}

/*!
 * \brief Exists Low Power Stop Mode
 */
void LpmExitStopMode( void )
{
}

/*!
 * \brief Enters Low Power Sleep Mode
 *
 * \note ARM exits the function when waking up
 */
void LpmEnterSleepMode( void)
{
}

void BoardLowPowerHandler( void )
{
}

/*
 * Function Name  : assert_failed
 * Description    : Reports the name of the source file and the source line number
 *                  where the assert_param error has occurred.
 * Input          : - file: pointer to the source file name
 *                  - line: assert_param error line source number
 * Output         : None
 * Return         : None
 */
void assert_failed( uint8_t* file, uint32_t line )
{
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %lu\n", file, line) */

    printf( "Wrong parameters value: file %s on line %u\n", ( const char* )file, line );
    /* Infinite loop */
    while( 1 )
    {
    }
}