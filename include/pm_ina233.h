/***************************************************************************
 *   Copyright (C) 2015 by Michael Ambrus                                  *
 *   ambrmi09@gmail.com                                                    *
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
#ifndef fg_bq27441_h
#define fg_bq27441_h

#include <stdint.h>
#include <ctype.h>

void pm_init(void);
void pm_deinit(void);

/* Register layouts as bit-field struct:s */

/* Note regarding Field overlapping byte boundary:
 *
 * This should be OK on Android/ARM now as warning: "note: offset of packed
 * bit-field ‘VBUSCT’ has changed in GCC 4.4" means layout error is now
 * fixed. See: https://gcc.gnu.org/gcc-4.4/changes.html
 *
 * I.e. add to build options: -Wno-packed-bitfield-compat
 */
#if BYTES_BIG_ENDIAN
typedef union {
    struct {
        uint8_t __not_used:4;
        uint8_t AVG:3;          /* Averaging */
        uint8_t VBUSCT:3;       /* Bus voltage conversion time */
        uint8_t VSHCT:3;        /* Shunt voltage conversion time */
        uint8_t shunt:1;        /* Operation modes (3) */
        uint8_t bus:1;
        uint8_t continuous:1;
    } __attribute__ ((packed));
    uint16_t raw_val;
    uint8_t barray[2];
} reg_mfr_adc_config;

#else
typedef union {
    struct {
        uint8_t continuous:1;
        uint8_t bus:1;
        uint8_t shunt:1;
        uint8_t VSHCT:3;
        uint8_t VBUSCT:3;
        uint8_t AVG:3;
        uint8_t __not_used:4;
    } __attribute__ ((packed));
    uint16_t raw_val;
    uint8_t barray[2];
} reg_mfr_adc_config;

#endif

void clear_faults();
//void restore_default_all();
uint8_t capability();
//void iout_oc_warn_limit();
//void vin_ov_warn_limit();
//void vin_uv_warn_limit();
//void pin_op_warn_limit();
//void status();
//void status_word();
//void status_iout();
//void status_input();
//void status_cml();
//void status_mfr_specific();
//void read_ein();
double read_vin();
//void read_in();
//void read_vout();
//void read_iout();
//void read_pout();
//void read_pin();
uint16_t mfr_id();
void mfr_model(char model[6]);
uint8_t mfr_revision();
reg_mfr_adc_config get_mfr_adc_config();
void set_mfr_adc_config(reg_mfr_adc_config);
//void mfr_read_vshunt();
//void mfr_alert_mask();
//void mfr_calibration();
//void mfr_device_config();
//void clear_ein();
int16_t ti_mfr_id();
int16_t ti_mfr_model();
int16_t ti_mfr_revision();

#endif
