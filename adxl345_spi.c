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
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stm32f10x.h>
#include "spiram.h"
#include <arpa/inet.h>

#define DEFLT_REPETITIONS 100
#define DEFLT_START_ADDRESS 0
#define DEFLT_PATTERN     "Donald Duck .."
#define SPIx SPI1
#define SPIRAM_WR_CMD 0x02
#define SPIRAM_RD_CMD 0x03

#ifdef HAS_PRINTF
#define PRINTF printf
#else
#define PRINTF(...) ((void)(0))
#endif

void write_spiram(int address, const char *pattern, int sz, int times)
{
    int i, j, offset = 0;
    uint32_t naddress;
    uint8_t cmd[4];

    cmd[0] = SPIRAM_WR_CMD;
    naddress = htonl(address);
    cmd[1] = ((uint8_t *)(&naddress))[1];
    cmd[2] = ((uint8_t *)(&naddress))[2];
    cmd[3] = ((uint8_t *)(&naddress))[3];

    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET) ;
    PRINTF("SPI-RAM: Write {0x%02X,0x%02X,0x%02X,0x%02X}\n", cmd[0], cmd[1],
           cmd[2], cmd[3]);

    for (i = 0; i < times; i++) {
        SPI_I2S_SetCS(SPIx, 0);
        SPI_I2S_SendDataArray_ncs(SPIx, cmd, 4);
        for (j = 0; j < sz; j++) {
            SPI_I2S_SendData_ncs(SPIx, pattern[j]);
        }
        SPI_I2S_SetCS(SPIx, 1);
        PRINTF("%08X: %s\n", address + offset, pattern);
        offset += sz;
    }
}

void read_spiram(int address, char *pattern, int sz, int times)
{
    int i, j, offset = 0;
    uint32_t naddress;
    uint8_t cmd[4];

    cmd[0] = SPIRAM_RD_CMD;
    naddress = htonl(address);
    cmd[1] = ((uint8_t *)(&naddress))[1];
    cmd[2] = ((uint8_t *)(&naddress))[2];
    cmd[3] = ((uint8_t *)(&naddress))[3];

    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET) ;

    PRINTF("SPI-RAM: Read {0x%02X,0x%02X,0x%02X,0x%02X}\n", cmd[0], cmd[1],
           cmd[2], cmd[3]);
    for (i = 0; i < times; i++) {

        SPI_I2S_SetCS(SPIx, 0);
        SPI_I2S_SendDataArray_ncs(SPIx, cmd, 4);
        for (j = 0; j < sz; j++) {
            pattern[j] = SPI_I2S_ReceiveData_ncs(SPIx);
        }
        SPI_I2S_SetCS(SPIx, 1);
        PRINTF("%08X: %s\n", address + offset, pattern);
        offset += sz;
    }
}

int main(int argc, char **argv)
{
    int repetitions = DEFLT_REPETITIONS;
    int startaddress = DEFLT_START_ADDRESS;
    char *pattern = DEFLT_PATTERN;
    int pattsz = sizeof(DEFLT_PATTERN);
#ifdef STDLIB_TARGET
    int c;
    uint8_t creg;
    char *r_pattern;

    opterr = 0;
    while ((c = getopt(argc, argv, "x:a:")) != -1)
        switch (c) {
            case 'x':
                repetitions = atoi(optarg);
                break;
            case 'a':
                startaddress = atoi(optarg);
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
        pattern = argv[optind];
        pattsz = strlen(pattern);
    }
#endif
#ifdef TEST_CS
    {
        int i = 0;
        for (i = 0; i < 1000; i++) {
            SPI_I2S_SetCS(SPIx, 1);
            usleep(1000);
            SPI_I2S_SetCS(SPIx, 0);
            usleep(10000);
        }
    }
#endif

    r_pattern = strdup(pattern);    /*Just allocate the same size */

#ifdef ONEBYTE
    PRINTF("SPI-RAM: Read from config register\n");
    SPI_I2S_SendReceiveData(SPIx, (uint8_t[]) {
                            0x05}, 1, &creg, 1);
    PRINTF("SPI-RAM: Config register is: 0x%02X\n", creg);

    PRINTF("SPI-RAM: Write 0x41 to config register\n");
    SPI_I2S_SendDataArray(SPIx, (uint8_t[]) {
                          0x01, 0x41}, 2);

    PRINTF("SPI-RAM: Read from config register\n");
    SPI_I2S_SendReceiveData(SPIx, (uint8_t[]) {
                            0x05}, 1, &creg, 1);
    PRINTF("SPI-RAM: Config register is: 0x%02X\n", creg);
#else
    PRINTF("SPI-RAM_2: Read from config register\n");
    SPI_I2S_SetCS(SPIx, 0);
    SPI_I2S_SendDataArray_ncs(SPIx, (uint8_t[]) {
                              0x05}, 1);
    SPI_I2S_ReceiveDataArray_ncs(SPIx, &creg, 1);
    SPI_I2S_SetCS(SPIx, 1);
    PRINTF("SPI-RAM_2: Config register is: 0x%02X\n", creg);

    PRINTF("SPI-RAM_2: Write 0x41 to config register\n");
    SPI_I2S_SetCS(SPIx, 0);
/*
    SPI_I2S_SendDataArray_ncs(SPIx, (uint8_t[]) {
                              0x01}, 1);
    SPI_I2S_SendDataArray_ncs(SPIx, (uint8_t[]) {
                              0x41}, 1);
*/
    SPI_I2S_SendDataArray_ncs(SPIx, (uint8_t[]) {
                              0x01, 0x41}, 2);
    SPI_I2S_SetCS(SPIx, 1);

    PRINTF("SPI-RAM_2: Read from config register\n");
    SPI_I2S_SetCS(SPIx, 0);
    SPI_I2S_SendDataArray_ncs(SPIx, (uint8_t[]) {
                              0x05}, 1);
    SPI_I2S_ReceiveDataArray_ncs(SPIx, &creg, 1);
    SPI_I2S_SetCS(SPIx, 1);
    PRINTF("SPI-RAM_2: Config register is: 0x%02X\n", creg);
#endif

    PRINTF
        ("SPI-RAM: will be filled with test-pattern [%s] %d number of times\n",
         pattern, repetitions);
    write_spiram(startaddress, pattern, pattsz, repetitions);

    PRINTF
        ("SPI-RAM: Read test-pattern [%s] %d number of times\n",
         pattern, repetitions);
    memset(r_pattern, 0, pattsz);
    read_spiram(startaddress, r_pattern, pattsz, repetitions);

    free(r_pattern);
    return 0;
}
