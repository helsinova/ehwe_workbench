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

#define DEFLT_REPETITIONS 100
#define DEFLT_PATTERN     "Donald Duck .."

void write_spiram(char *pattern, int sz, int times)
{
    int i, j;

    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) ;

    for (i = 0; i < times; i++) {
        for (j = 0; j < sz; j++) {
            SPI_I2S_SendData(SPI1, pattern[j]);
        }
    }
}

int main(int argc, char **argv)
{
    int repetitions = DEFLT_REPETITIONS;
    char *pattern = DEFLT_PATTERN;
    int pattsz = sizeof(DEFLT_PATTERN);
#ifdef STDLIB_TARGET
    int c;

    opterr = 0;
    while ((c = getopt(argc, argv, "x:")) != -1)
        switch (c) {
            case 'x':
                repetitions = atoi(optarg);
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

    printf("SPI-RAM will be filled with test-pattern [%s] %d number of times\n",
           pattern, repetitions);
#endif
    write_spiram(pattern, pattsz, repetitions);
    return 0;
}
