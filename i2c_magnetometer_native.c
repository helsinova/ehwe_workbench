/***************************************************************************
 *   Copyright (C) 2016 by Michael Ambrus                                  *
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
 * Test program: i2c-magnetometer HMC5883L using native API
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stm32f10x.h>
#include "i2c_magnetometer.h"
#include <arpa/inet.h>
#include "../devices/buspirate/local.h"
#include <assert.h>

#define HMC5883L_ADDR				0x1E
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

#define BUS( B ) (B->ddata)
#define SEND_ADDR( A ) (A<<1)
#define READ_ADDR( A ) ((A<<1) | 0x01)

void i2c_write(I2C_TypeDef * bus, uint8_t dev_addr, const uint8_t *buffer,
               int len, int send_stop)
{
    int ack;

    assert(dev_addr < 0x80);

    /* Send START condition */
    bpi2c_start(BUS(bus));

    /* Send device address for write */
    ack = bpi2c_sendByte(BUS(bus), SEND_ADDR(dev_addr));
    assert(ack == 1);

    /* Send the rest */
    bpi2c_sendData(BUS(bus), buffer, len);

    if (send_stop) {
        /* Close Communication */
        bpi2c_stop(BUS(bus));
    }
}

void i2c_read(I2C_TypeDef * bus, uint8_t dev_addr, uint8_t *buffer, int len)
{
    int ack;

    assert(dev_addr < 0x80);

    /* Send START condition */
    bpi2c_start(BUS(bus));

    /* Send IC address for read */
    ack = bpi2c_sendByte(BUS(bus), READ_ADDR(dev_addr));
    assert(ack == 1);

    /* Read all but the last with auto ACK */
    bpi2c_receiveData(BUS(bus), buffer, len);

    /* Close Communication */
    bpi2c_stop(BUS(bus));
}

int main(int argc, char **argv)
{
    int c, i, X, Y, Z, x = DEFLT_X;
    uint8_t data[6] = { 0 };

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

    /* Set measurement mode to 8-average, 15Hz */
    i2c_write(I2C1, HMC5883L_ADDR, (uint8_t[]) {
              CONFIGURATION_REGISTER_A, 0x70}, 2, 1);

    /* Set gain to 5 */
    i2c_write(I2C1, HMC5883L_ADDR, (uint8_t[]) {
              CONFIGURATION_REGISTER_B, 0xa0}, 2, 1);

    /* Set to continuous mode */
    i2c_write(I2C1, HMC5883L_ADDR, (uint8_t[]) {
              MODE_REGISTER, 0x00}, 2, 1);

    usleep(6000);

    for (i = 0; i < x; i++) {
        /* Point at first value register-set again */
        i2c_write(I2C1, HMC5883L_ADDR, (uint8_t[]) {
                  DATA_OUTPUT_X_MSB_REGISTER}, 1, 1);
        usleep(67000);

        /* Read values */
        i2c_read(I2C1, HMC5883L_ADDR, data, sizeof(data));
        X = (data[0] << 8) + data[1];
        Z = (data[2] << 8) + data[3];
        Y = (data[4] << 8) + data[5];

        PRINTF("%-6d %-6d %-6d\n", X, Y, Z);

    }
    return 0;
}
