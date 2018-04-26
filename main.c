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
 * Test program: i2c  Voltage/Apmp/Power INA233
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#include <pm_ina233.h>

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

int main(int argc, char **argv)
{
    int c, i, x = DEFLT_X;
    char bbuf[32];
    reg_mfr_adc_config adc_config;

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

    pm_init();

    mfr_model(bbuf);
    fprintf(stderr, "Fuel guage chip model=[%s], capabilities=[0x%01X]\n", bbuf,
            capability());
    fprintf(stderr, "Manufaturer ID=[0x%04X], revision=[0x%02X]\n", mfr_id(),
            mfr_revision());
    fprintf
        (stderr,
         "TI manufaturer ID=[0x%04X], TI model=[0x%04X], TI revision=[0x%04X]\n",
         ti_mfr_id(), ti_mfr_revision(), ti_mfr_revision());
    fprintf(stderr, "Status=[0x%02X], Status Word=[0x%04X]\n", status().raw_val,
            status_word().raw_val);

    adc_config = get_mfr_adc_config();
    fprintf(stderr, "ADC config=[0x%04X]\n", adc_config.raw_val);
    fprintf(stderr, "  AVG=[%d]\n", adc_config.AVG);
    fprintf(stderr, "  VBUSCT=[%d]\n", adc_config.VBUSCT);
    fprintf(stderr, "  VSHCT=[%d]\n", adc_config.VSHCT);
    fprintf(stderr, "  MODE.shunt=[%d]\n", adc_config.shunt);
    fprintf(stderr, "  MODE.bus=[%d]\n", adc_config.bus);
    fprintf(stderr, "  MODE.continuous=[%d]\n", adc_config.continuous);

    adc_config.AVG = 3;         /* val=0-7 => (val << 2) + 1; */
    set_mfr_adc_config(adc_config);

    adc_config = get_mfr_adc_config();
    fprintf(stderr, "ADC config=[0x%04X]\n", adc_config.raw_val);
    fprintf(stderr, "  AVG=[%d]\n", adc_config.AVG);
    fprintf(stderr, "  VBUSCT=[%d]\n", adc_config.VBUSCT);
    fprintf(stderr, "  VSHCT=[%d]\n", adc_config.VSHCT);
    fprintf(stderr, "  MODE.shunt=[%d]\n", adc_config.shunt);
    fprintf(stderr, "  MODE.bus=[%d]\n", adc_config.bus);
    fprintf(stderr, "  MODE.continuous=[%d]\n", adc_config.continuous);
    clear_faults();
    restore_default_all();

    for (i = 0; i < x; i++) {
        PRINTF("%f\n", read_vin());
        FFLUSH(stdout);
    }

    pm_deinit();
    return 0;
}
