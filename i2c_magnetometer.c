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
 * Test program: i2c-magnetometer HMC5883L
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

#define DEFLT_X     0
#define I2Cx SPI1	//To be used changed to I2C1 (TBD)
					//===============================
#ifdef HAS_PRINTF
#define PRINTF printf
#else
#define PRINTF(...) ((void)(0))
#endif

int main(int argc, char **argv)
{
    int x = DEFLT_X;
#ifdef STDLIB_TARGET
    int c;
    uint8_t creg;

    opterr = 0;
    while ((c = getopt(argc, argv, "x:")) != -1)
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

    if (optind < argc) {
        //somevar = argv[optind];
    }
#endif
#ifdef TEST_CS
    {
        int i = 0;
        for (i = 0; i < 1000; i++) {
            SPI_I2S_SetCS(I2Cx, 1);
            usleep(1000);
            SPI_I2S_SetCS(I2Cx, 0);
            usleep(10000);
        }
    }
#endif

#ifndef ONEBYTE
    PRINTF("SPI-RAM_2: Read from config register\n");
    SPI_I2S_SetCS(I2Cx, 0);
    SPI_I2S_SendDataArray_ncs(I2Cx, (uint8_t[]) {
                              0x05}, 1);
    SPI_I2S_ReceiveDataArray_ncs(I2Cx, &creg, 1);
    SPI_I2S_SetCS(I2Cx, 1);
    PRINTF("SPI-RAM_2: Config register is: 0x%02X\n", creg);

    PRINTF("SPI-RAM_2: Write 0x41 to config register\n");
    SPI_I2S_SetCS(I2Cx, 0);
/*
    SPI_I2S_SendDataArray_ncs(I2Cx, (uint8_t[]) {
                              0x01}, 1);
    SPI_I2S_SendDataArray_ncs(I2Cx, (uint8_t[]) {
                              0x41}, 1);
*/
    SPI_I2S_SendDataArray_ncs(I2Cx, (uint8_t[]) {
                              0x01, 0x41}, 2);
    SPI_I2S_SetCS(I2Cx, 1);

    PRINTF("SPI-RAM_2: Read from config register\n");
    SPI_I2S_SetCS(I2Cx, 0);
    SPI_I2S_SendDataArray_ncs(I2Cx, (uint8_t[]) {
                              0x05}, 1);
    SPI_I2S_ReceiveDataArray_ncs(I2Cx, &creg, 1);
    SPI_I2S_SetCS(I2Cx, 1);
    PRINTF("SPI-RAM_2: Config register is: 0x%02X\n", creg);
#else
    //Alternative interface
    SPI_I2S_SendReceiveData(I2Cx, (uint8_t[]) {
                            0x05}, 1, &creg, 1);

    SPI_I2S_SendDataArray(I2Cx, (uint8_t[]) {
                          0x01, 0x41}, 2);

    SPI_I2S_SendReceiveData(I2Cx, (uint8_t[]) {
                            0x05}, 1, &creg, 1);
#endif

    PRINTF("I2C mag: Variable [x]: 0x%02X\n", x);

    return 0;
}
