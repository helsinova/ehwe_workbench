/***************************************************************************
 *   Copyright (C) 2017 by Michael Ambrus                                  *
 *   michael@helsinova.se                                                  *
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
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stm32f10x.h>
#include "adxl345_spi.h"

#ifdef HAS_PRINTF
#define PRINTF printf
#define FFLUSH fflush
#else
#define PRINTF(...) ((void)(0))
#define FFLUSH(...) ((void)(0))
#endif

/* ADXL345 register map */
#define  THRESH_TAP      0x1D
#define  OFSX            0x1E
#define  OFSY            0x1F
#define  OFSZ            0x20
#define  DUR             0x21
#define  Latent          0x22
#define  Window          0x23
#define  THRESH_ACT      0x24
#define  THRESH_INACT    0x25
#define  TIME_INACT      0x26
#define  ACT_INACT_CTL   0x27
#define  THRESH_FF       0x28
#define  TIME_FF         0x29
#define  TAP_AXES        0x2A
#define  ACT_TAP_STATUS  0x2B
#define  BW_RATE         0x2C
#define  POWER_CTL       0x2D
#define  INT_ENABLE      0x2E
#define  INT_MAP         0x2F
#define  INT_SOURCE      0x30
#define  DATA_FORMAT     0x31
#define  DATAX0          0x32
#define  DATAX1          0x33
#define  DATAY0          0x34
#define  DATAY1          0x35
#define  DATAZ0          0x36
#define  DATAZ1          0x37
#define  FIFO_CTL        0x38
#define  FIFO_STATUS     0x39

int main(int argc, char **argv)
{
    int c;
#ifdef WORKBENCH_LOOP_MEASURE
    int repetitions;
#endif

    opterr = 0;
    while ((c = getopt(argc, argv, "x:a:")) != -1)
        switch (c) {
#ifdef WORKBENCH_LOOP_MEASURE
            case 'x':
                repetitions = atoi(optarg);
                break;
#endif
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

#ifdef NEVER
    if (optind < argc) {
        pattern = argv[optind];
        pattsz = strlen(pattern);
    }
#endif //NEVER

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) ;

	/* Various tests below. Enable using cmake */

#ifdef WORKBENCH_TEST_CS
    {
    	PRINTF("SPI-ADXL345: Toggle CS 10x - begin\n");
        int i = 0;
        for (i = 0; i < 10; i++) {
            SPI_I2S_SetCS(SPI1, 1);
            usleep(1000);
            SPI_I2S_SetCS(SPI1, 0);
            usleep(10000);
        }
    	PRINTF("SPI-ADXL345: Toggle CS 10x - end\n");
    }
#endif


#ifdef WORKBENCH_STATUS_CHECK
	{
		uint8_t creg;

		PRINTF("SPI-ADXL345: Read from config register (0x80)\n");
		SPI_I2S_SendReceiveData(SPI1, (uint8_t[]) {
								0x80}, 1, &creg, 1);
		PRINTF("SPI-ADXL345: Config register (0x80) is: 0x%02X\n", creg);

	}
#endif

#ifdef WORKBENCH_LOOP_MEASURE
	{
		uint8_t xdata[2];
		uint8_t ydata[2];
		uint8_t zdata[2];
		uint16_t xraw;
		uint16_t yraw;
		uint16_t zraw;
		int16_t X;
		int16_t Y;
		int16_t Z;
		//double vectlen;
		int loop;

		fprintf(stderr, "SPI-ADXL345: Loop readings test\n");

		/* Tell chip to leave idle mode */
		SPI_I2S_SendDataArray(SPI1, (uint8_t[]) {
								POWER_CTL, 0x08}, 2);

		for (loop=0; loop<repetitions; loop++) {
			SPI_I2S_SendReceiveData(SPI1, (uint8_t[]) {
								0x80 | 0x40 | DATAX0}, 1, xdata, 2);
			SPI_I2S_SendReceiveData(SPI1, (uint8_t[]) {
								0x80 | 0x40 | DATAY0}, 1, ydata, 2);
			SPI_I2S_SendReceiveData(SPI1, (uint8_t[]) {
								0x80 | 0x40 | DATAZ0}, 1, zdata, 2);

			/*
			printf("%3d %3d ", 	xdata[0], xdata[1]);
			printf("%3d %3d ", 	ydata[0], ydata[1]);
			printf("%3d %3d -- ",	zdata[0], zdata[1]);
			*/

			xraw = (((uint16_t)xdata[1]) << 8) | xdata[0];
			yraw = (((uint16_t)ydata[1]) << 8) | ydata[0];
			zraw = (((uint16_t)zdata[1]) << 8) | zdata[0];

			xraw = xdata[1];
			xraw <<= 8;
			xraw |= xdata[0];

			yraw = ydata[1];
			yraw <<= 8;
			yraw |= ydata[0];

			zraw = zdata[1];
			zraw <<= 8;
			zraw |= zdata[0];

			/*
			printf("%6d %6d %6d -- ", xraw, yraw, zraw);
			*/

			X = xraw;
			Y = yraw;
			Z = zraw;

			//vectlen = sqrt(X * X + Y * Y + Z * Z);
			//PRINTF("%-6d %-6d %-6d %-.3f\n", X, Y, Z, vectlen);
			PRINTF("%-6d %-6d %-6d \n", X, Y, Z);
			FFLUSH(stdout);
		}
	}
#endif

    return 0;
}
