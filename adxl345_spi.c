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
#else
#define PRINTF(...) ((void)(0))
#endif

void write_spiram(int address, const char *pattern, int sz, int times)
{
#ifdef NEVER
    int i, j, offset = 0;
    uint32_t naddress;
    uint8_t cmd[4];

    cmd[0] = SPIRAM_WR_CMD;
    naddress = htonl(address);
    cmd[1] = ((uint8_t *)(&naddress))[1];
    cmd[2] = ((uint8_t *)(&naddress))[2];
    cmd[3] = ((uint8_t *)(&naddress))[3];

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) ;
    PRINTF("SPI-RAM: Write {0x%02X,0x%02X,0x%02X,0x%02X}\n", cmd[0], cmd[1],
           cmd[2], cmd[3]);

    for (i = 0; i < times; i++) {
        SPI_I2S_SetCS(SPI1, 0);
        SPI_I2S_SendDataArray_ncs(SPI1, cmd, 4);
        for (j = 0; j < sz; j++) {
            SPI_I2S_SendData_ncs(SPI1, pattern[j]);
        }
        SPI_I2S_SetCS(SPI1, 1);
        PRINTF("%08X: %s\n", address + offset, pattern);
        offset += sz;
    }
#endif //NEVER
}

void read_spiram(int address, char *pattern, int sz, int times)
{
#ifdef NEVER
    int i, j, offset = 0;
    uint32_t naddress;
    uint8_t cmd[4];

    cmd[0] = SPIRAM_RD_CMD;
    naddress = htonl(address);
    cmd[1] = ((uint8_t *)(&naddress))[1];
    cmd[2] = ((uint8_t *)(&naddress))[2];
    cmd[3] = ((uint8_t *)(&naddress))[3];

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) ;

    PRINTF("SPI-RAM: Read {0x%02X,0x%02X,0x%02X,0x%02X}\n", cmd[0], cmd[1],
           cmd[2], cmd[3]);
    for (i = 0; i < times; i++) {

        SPI_I2S_SetCS(SPI1, 0);
        SPI_I2S_SendDataArray_ncs(SPI1, cmd, 4);
        for (j = 0; j < sz; j++) {
            pattern[j] = SPI_I2S_ReceiveData_ncs(SPI1);
        }
        SPI_I2S_SetCS(SPI1, 1);
        PRINTF("%08X: %s\n", address + offset, pattern);
        offset += sz;
    }
#endif //NEVER
}

int main(int argc, char **argv)
{
#ifdef STDLIB_TARGET
    int c;

    opterr = 0;
    while ((c = getopt(argc, argv, "x:a:")) != -1)
        switch (c) {
#ifdef NEVER
            case 'x':
                repetitions = atoi(optarg);
                break;
            case 'a':
                startaddress = atoi(optarg);
                break;
#endif //NEVER
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
#endif
	
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

		/*
		PRINTF("SPI-ADXL345: Write 0x41 to config register\n");
		SPI_I2S_SendDataArray(SPI1, (uint8_t[]) {
							  0x01, 0x41}, 2);

		PRINTF("SPI-ADXL345: Read from config register\n");
		SPI_I2S_SendReceiveData(SPI1, (uint8_t[]) {
								0x05}, 1, &creg, 1);
		PRINTF("SPI-ADXL345: Config register is: 0x%02X\n", creg);
		*/
	}
#endif

    return 0;
}
