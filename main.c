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
#include <assure.h>

#include <stm32f10x.h>
#include <driver.h>
#include <pwm_pca9685.h>
#include "pwm_pca9685_device.h"
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

#ifndef SYNC_FILE
#define SYNC_FILE "/tmp/pwm.regs"
#endif

/* Symbolic bus-interface ehwe has given this embedded work-bench */
#define I2CN I2C1

/* Assume option argument-strings do not exceed this value */
#define ARGSTR_MAX 80

/* Parse pwm option as formatted string
 *
 * Format is number[:]number
 *
 * Each "number" is a valid numeric string in base {8,10,16} and "[:]" is a
 * delimiter which is anything BUT a alpha-numeric character */
struct pwm_val parse_pwm_opt(char *sarg)
{
    struct pwm_val pwm_val;
    int i, found;
    char *on_str = sarg;
    char *off_str = "0\0";

    for (i = 0, found = 0; i < ARGSTR_MAX && !found; i++) {
        if (!isalnum(sarg[i])) {
            found = 1;
            i--;
        }
    }

    if (found) {
        sarg[i] = 0;
        off_str = &sarg[i + 1];
    }

    sscanf(on_str, "%i", &pwm_val.on_cntr);
    sscanf(off_str, "%i", &pwm_val.off_cntr);

    /* Returning a stack-variable which is OK when passed (returned) as value */
    return pwm_val;
}

int main(int argc, char **argv)
{
    int c, i, pwm_idx = 0;
    pwm_hndl pwm_dev;
    struct pwm_val pwm_val;
    FILE *regsf;
    char *sync_fn;

    sync_fn = getenv("PCA9685_REGS_FILENAME");
    if (sync_fn == NULL)
        sync_fn = SYNC_FILE;

    ASSURE(regsf = fopen(sync_fn, "w"));

    pwm_dev = pwm_pca9685_create(I2CN);
    pwm_pca9685_init(pwm_dev);

    opterr = 0;
    while ((c = getopt(argc, argv, "i:p:PRrdDsf:FtX")) != -1) {
        switch (c) {
            case 'i':
                /* PWM index to refer */
                pwm_idx = atoi(optarg);
                break;
            case 'p':
                /* Set PWM at current index and increment index */
                pwm_pca9685_set(pwm_dev, pwm_idx, parse_pwm_opt(optarg));
                pwm_idx++;
                break;
            case 'P':
                /* Read and print PWM at current index and increment index */
                pwm_val = pwm_pca9685_get(pwm_dev, pwm_idx);
                printf("pwm[%-2d]: 0x%03X 0x%03X\n", pwm_idx, pwm_val.on_cntr,
                       pwm_val.off_cntr);
                pwm_idx++;
                break;
            case 'r':
                for (i = 0; i < 17; i++) {
                    pwm_val = pwm_pca9685_get(pwm_dev, i);
                    printf("pwm[%-2d]: 0x%03X 0x%03X\n", i, pwm_val.on_cntr,
                           pwm_val.off_cntr);
                }
                break;
            case 'R':
                /* Reset all pca9685 on bus */
                pwm_pca9685_all_swreset(I2CN);
                break;
            case 'd':
                /* Dump registers in human readable form for pca9685 */
                regs_dump(pwm_dev, fprintf, stderr);
                break;
            case 'D':
                /* Hex-dump registers for pca9685 */
                regs_dump_hex(pwm_dev, fprintf, stderr);
                break;
            case 's':
                /* Explicitly sync device regs copy */
                regs_sync(pwm_dev);
                break;
            case 'X':
                /* Explicitly un-sync device regs copy */
                if (regs_insync(pwm_dev)) {
                    regs_unsync(pwm_dev);
                    if (regsf)
                        fclose(regsf);
                    unlink(sync_fn);
                }
                break;
            case 'f':
                /* Set PWM-frequency 24-1526 Hz */
                set_pwm_freq(pwm_dev, atoi(optarg));
                break;
            case 'F':
                /* Print device PWM-frequency */
                printf("PWM-frequency (Hz): %d\n", get_pwm_freq(pwm_dev));
                break;
            case 't':
                /* Set a pre-defined PWM test-pattern PWM-0 - PWM-5 */
                pwm_pca9685_test(pwm_dev);

                /* Set also PWM-6. The time-difference on analyzer shows
                 * execution delay for various bus HW-interfaces */
                pwm_pca9685_set(pwm_dev, 6, (struct pwm_val) {
                                0x0500, 0x0550});
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

    /* Save for next invocation */
    if (regs_insync(pwm_dev))
        regs_dump_hex(pwm_dev, fprintf, regsf);

    pwm_pca9685_destruct(pwm_dev);

    printf("Embedded work-bench exits normally...\n\n");
#ifdef DEVICE_BUSPIRATE
    /* Workaround due to that BP when switching mode back to bit-bang
     * (BBIOx) on it's way to terminal-mode will also send a reset to all
     * 12C peripherals. I.e. free-running PWM will stop, which is not a
     * desired behaviour. */

    /* Also check that it really is a BusPirate bound to the symbolic
     * bus-interface */
    if (I2CN->device->devid == BUSPIRATE)
        exit(0);
#endif

    return 0;
}
