/*!
 * \file      spi-board.c
 *
 * \brief     Target board SPI driver implementation
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

#include <stddef.h>
#include <string.h>
#include "utilities.h"
#include "board.h"
#include "spi-board.h"
#include "spidev_lib/spidev_lib.h"

#define SPI_FILE "/dev/spidev2.0"

spi_config_t spi_config;

void SpiInit( Spi_t *obj, SpiId_t spiId)
{
    spi_config.mode=0;
    spi_config.speed=1000000;
    spi_config.delay=0;
    spi_config.bits_per_word=8;
    obj->SpiId = spiId;
    obj->SpiFd = spi_open(SPI_FILE,spi_config);
}

void SpiDeInit( Spi_t *obj )
{
    spi_close(obj->SpiFd);
}

void SpiFormat( Spi_t *obj, int8_t bits, int8_t cpol, int8_t cpha, int8_t slave )
{
    spi_config.bits_per_word=bits;
}

void SpiFrequency( Spi_t *obj, uint32_t hz )
{
    spi_config.speed=hz;
}

uint16_t SpiInOut( Spi_t *obj, uint16_t outData )
{
    uint8_t tx_buffer[1];
    uint8_t rx_buffer[1];
    spi_xfer(obj->SpiFd,tx_buffer,strlen(tx_buffer),rx_buffer,strlen(tx_buffer));
    return( rx_buffer[0] ); //hmmmmmmmmmmmm this isn't quite 16bits word size........
}

