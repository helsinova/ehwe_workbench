/***************************************************************************
 *   Copyright (C) 2018 by Michael Ambrus                                  *
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
/*
 * Test program: i2c PI3HDX414 HDMI splitter
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <ehwe_i2c_device.h>
#include <ehwe.h>

#ifdef EHWE
#include "embedded_config.h"
#define main embedded_main
#endif

#define DEFLT_X     100

#ifdef HAS_PRINTF
#define PRINTF printf
#define FFLUSH fflush
#else
#define PRINTF(...) ((void)(0))
#define FFLUSH(...) ((void)(0))
#endif

/* Device address (7-bit) */
#define PI3HDX414_ADDR 0x5F
struct device;

void read_print_chunk(i2c_device_hndl i2c_device)
{
    uint8_t buf[8] = { 0 };

    i2c_read(i2c_device_bus(i2c_device),
             i2c_device_addr(i2c_device), buf, sizeof(buf));
    PRINTF("%02X ", buf[0]);
    PRINTF("%02X ", buf[1]);
    PRINTF("%02X ", buf[2]);
    PRINTF("%02X ", buf[3]);
    PRINTF("%02X ", buf[4]);
    PRINTF("%02X ", buf[5]);
    PRINTF("%02X ", buf[6]);
    PRINTF("%02X ", buf[7]);
    PRINTF("\n");
    FFLUSH(stdout);
}

void read_print_bybyte(i2c_device_hndl i2c_device)
{
    PRINTF("%02X ", i2c_device_read_uint8(i2c_device, 0));
    PRINTF("%02X ", i2c_device_read_uint8(i2c_device, 1));
    PRINTF("%02X ", i2c_device_read_uint8(i2c_device, 2));
    PRINTF("%02X ", i2c_device_read_uint8(i2c_device, 3));
    PRINTF("%02X ", i2c_device_read_uint8(i2c_device, 4));
    PRINTF("%02X ", i2c_device_read_uint8(i2c_device, 5));
    PRINTF("%02X ", i2c_device_read_uint8(i2c_device, 6));
    PRINTF("%02X ", i2c_device_read_uint8(i2c_device, 7));
    PRINTF("\n");
}

int main(int argc, char **argv)
{
    int c, i, x = DEFLT_X;
    i2c_device_hndl i2c_device = NULL;

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
                    fprintf(stderr, "Unknown option character `\\x%x'.\n",
                            optopt);
                return 1;
            default:
                abort();
        }
    }

    if (optind < argc) {
        //somevar = argv[optind];
    }

    i2c_device = i2c_device_open(I2C1, PI3HDX414_ADDR);
    i2c_write(i2c_device_bus(i2c_device),
              i2c_device_addr(i2c_device), (uint8_t[]) {
              0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78, 0x89
              }
              , 8, 1);

    for (i = 0; i < x; i++) {
        read_print_chunk(i2c_device);
        read_print_chunk(i2c_device);
        read_print_bybyte(i2c_device);
        read_print_bybyte(i2c_device);
    }

    i2c_device_close(i2c_device);

    return 0;
}
