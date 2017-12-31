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
/*
 * Main program - pwm
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include <stm32f10x.h>
#include <pwm_pca9685.h>
#include <math.h>
#include <devices.h>
#include <interfaces.h>

#ifdef HAS_PRINTF
#define PRINTF printf
#define FFLUSH fflush
#else
#define PRINTF(...) ((void)(0))
#define FFLUSH(...) ((void)(0))
#endif

int main(int argc, char **argv)
{
    int i, c, x = 0;
    pwm_hndl pwm;
    struct pwm_val pwm_val;

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

    pwm_pca9685_all_swreset(I2C1);
    pwm = pwm_pca9685_create(I2C1);

    pwm_pca9685_init(pwm);
    pwm_pca9685_test(pwm);

    pwm_val.on_cntr = 0x0500;
    pwm_val.off_cntr = 0x0550;
    pwm_pca9685_set(pwm, 5, pwm_val);

    for (i = 0; i < 5; i++) {
        pwm_val = pwm_pca9685_get(pwm, i);
        printf("pwm[%d]: 0x%03X 0x%03X\n", i, pwm_val.on_cntr,
               pwm_val.off_cntr);
    }

    pwm_pca9685_destruct(pwm);

    return 0;
}
