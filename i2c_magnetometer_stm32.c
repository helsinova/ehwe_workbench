/***************************************************************************
 *   Copyright (C) 2015 by Michael Ambrus                                  *
 *   ambrmi09@gmail.com                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
/*
 * Test program: i2c-magnetometer HMC5883L using stm32 API
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stm32f10x.h>
#include "i2c_magnetometer.h"
#include <arpa/inet.h>

#define CONFIGURATION_REGISTER_A	0x00    /* Read/Write */
#define CONFIGURATION_REGISTER_B	0x01    /* Read/Write */
#define MODE_REGISTER				0x02    /* Read/Write */
#define DATA_OUTPUT_X_MSB_REGISTER	0x03    /* Read */
#define DATA_OUTPUT_X_LSB_REGISTER	0x04    /* Read */
#define DATA_OUTPUT_Z_MSB_REGISTER	0x05    /* Read */
#define DATA_OUTPUT_Z_LSB_REGISTER	0x06    /* Read */
#define DATA_OUTPUT_Y_MSB_REGISTER	0x07    /* Read */
#define DATA_OUTPUT_Y_LSB_REGISTER	0x08    /* Read */
#define STATUS_REGISTER				0x09    /* Read */
#define IDENTIFICATION_REGISTER_A	0x0A    /* Read */
#define IDENTIFICATION_REGISTER_B	0x0B    /* Read */
#define IDENTIFICATION_REGISTER_C	0x0C    /* Read */

#define DEFLT_X     1

#ifdef HAS_PRINTF
#define PRINTF printf
#else
#define PRINTF(...) ((void)(0))
#endif

void i2c_write(I2C_TypeDef * bus, uint8_t dev_addr, const uint8_t *buffer,
               int len)
{
    int i;

    while (I2C_GetFlagStatus(bus, I2C_FLAG_BUSY)) ;

    /* Send START condition */
    I2C_GenerateSTART(bus, ENABLE);

    /* Test on EV5 and clear it */
    while (!I2C_CheckEvent(bus, I2C_EVENT_MASTER_MODE_SELECT)) ;

    /* Send device address for write */
    I2C_Send7bitAddress(bus, dev_addr, I2C_Direction_Transmitter);

    /* Test on EV6 and clear it */
    while (!I2C_CheckEvent(bus, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) ;

    for (i = 0; i < len; i++) {
        I2C_SendData(bus, buffer[i]);
        while (!I2C_CheckEvent(bus, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) ;
    }

    /* Close Communication */
    I2C_GenerateSTOP(bus, ENABLE);
}

void i2c_read(I2C_TypeDef * bus, uint8_t dev_addr, uint8_t *buffer, int len)
{
    int i;

    while (I2C_GetFlagStatus(bus, I2C_FLAG_BUSY)) ;

    /* Send START condition */
    I2C_GenerateSTART(bus, ENABLE);

    /* Test on EV5 and clear it */
    while (!I2C_CheckEvent(bus, I2C_EVENT_MASTER_MODE_SELECT)) ;

    /* Send IC address for read */
    I2C_Send7bitAddress(bus, dev_addr, I2C_Direction_Receiver);

    /* Test on EV6 and clear it */
    while (!I2C_CheckEvent(bus, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) ;

    /* Read all but the last with auto ACK */
    for (i = 0; i < (len - 1); i++) {
        while (!I2C_CheckEvent(bus, I2C_EVENT_MASTER_BYTE_RECEIVED)) ;
        buffer[i] = I2C_ReceiveData(bus);
    }

    I2C_AcknowledgeConfig(bus, DISABLE);
    /* Close Communication */
    I2C_GenerateSTOP(bus, ENABLE);

    /* Read the last one */
    while (!I2C_CheckEvent(bus, I2C_EVENT_MASTER_BYTE_RECEIVED)) ;
    buffer[i] = I2C_ReceiveData(bus);

    /* Re-enable HW auto ACK for next time */
    I2C_AcknowledgeConfig(bus, ENABLE);
}

int main(int argc, char **argv)
{
    int i, j, x = DEFLT_X;
#ifdef STDLIB_TARGET
    int c;
    uint8_t data[6];

    opterr = 0;
    while ((c = getopt(argc, argv, "x:")) != -1) {
        switch (c) {
            case 'x':
                x = atoi(optarg);
                break;
            case '?':
                if (optopt == 'x')
                    fprintf(stderr, "Option -%c requires an argument.\n",
                            optopt);
                else if (isprint(optopt))
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf(stderr,
                            "Unknown option character `\\x%x'.\n", optopt);
                return 1;
            default:
                abort();
        }
    }

    if (optind < argc) {
        //somevar = argv[optind];
    }
#endif

    //Set measurement mode to 8-average, 15Hz
    i2c_write(I2C1, 0x3C, (uint8_t[]) {
              0x00, 0x70}, 2);

    //Set gain to 5
    i2c_write(I2C1, 0x3C, (uint8_t[]) {
              0x01, 0xa0}, 2);

    //Set to single-measurement mode
    i2c_write(I2C1, 0x3C, (uint8_t[]) {
              0x02, 0x01}, 2);

    for (i = 0; i < x; i++) {
        i2c_read(I2C1, 0x3D, data, sizeof(data));

        for (j = 0; j < sizeof(data); j++) {
            PRINTF("%3d", data[j]);
            if (j < sizeof(data))
                PRINTF(" ");
            else
                PRINTF("\n");
        }

    }

    PRINTF("I2C mag: Variable [x]: 0x%02X\n", x);

    return 0;
}
