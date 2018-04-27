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
 *
 * Shorts-forms:
 *  * [NS]:   Not Supported
 *  * W.H.O:  Warning Has Occured
 *  * H.O:    Has Occured
 */
#if BYTES_BIG_ENDIAN

typedef union {
    struct {
        uint8_t BUSY:1;         /* [NS] 0 */
        uint8_t OFF:1;          /* [NS] 0 */
        uint8_t VOUT_OC:1;      /* [NS] 0 */
        uint8_t IOUT_OC:1;      /* [NS] 0 */
        uint8_t VIN_UV:1;       /* [NS] 0 */
        uint8_t TEMPERATURE:1;  /* [NS] 0 */
        uint8_t CML:1;          /* A communication fault H.O. 0 */
        uint8_t NONE_OF_THE_ABOVE:1;    /*  A fault or warning not listed in
                                           bits[7:1] H.O. */
    } __attribute__ ((packed));
    uint8_t raw_val;
} reg_status_byte;

typedef union {
    struct {
        uint16_t VOUT:1;         /* [NS] 0 */
        uint16_t IOUT_POUT:1;    /* An output current or power W.H.O. 0 */
        uint16_t INPUT:1;        /* An input voltage, current, or power W.H.O. 0
                                 */
        uint16_t MFR:1;          /* A manufacturer-specific fault or W.H.O. 1 */
        uint16_t POWER_GOOD:1;   /* [NS] 0 */
        uint16_t FANS:1;         /* [NS] 0 */
        uint16_t OTHER:1;        /* [NS] 0 */
        uint16_t UNKNOWN:1;      /* [NS] 0 */

        uint16_t BUSY:1;         /* [NS] 0 */
        uint16_t OFF:1;          /* [NS] 0 */
        uint16_t VOUT_OC:1;      /* [NS] 0 */
        uint16_t IOUT_OC:1;      /* [NS] 0 */
        uint16_t VIN_UV:1;       /* [NS] 0 */
        uint16_t TEMPERATURE:1;  /* [NS] 0 */
        uint16_t CML:1;          /* A communication fault H.O. 0 */
        uint16_t NONE_OF_THE_ABOVE:1;    /* A fault or warning not listed in
                                           bits[7:1] H.O. */
    } __attribute__ ((packed));
    uint16_t raw_val;
    uint8_t barray[2];
} reg_status_word;

typedef union {
    struct {
        uint8_t E_IOUT_OC:1;    /* IOUT_OC fault [NS] 0 */
        uint8_t W_IOUT_OC_POFF:1;   /* IOUT_OC fault with LV shutdown [NS] 0 */
        uint8_t W_IOUT_OC:1;    /* IOUT_OC_WARN An input undercurrent W.H.O. 0 */
        uint8_t E_IOUT_UC:1;    /* IOUT_UC fault [NS] 0 */
        uint8_t CURRENT_SHARE:1;    /* Current share fault [NS] 0 */
        uint8_t IN_PWR_LMT:1;   /* In power-limiting mode [NS] 0 */
        uint8_t E_POUT_OP:1;    /* POUT_OP fault [NS] 0 */
        uint8_t W_POUT_OP:1;    /* POUT_OP_WARN [NS] 0 */
    } __attribute__ ((packed));
    uint8_t raw_val;
} reg_status_iout;

typedef union {
    struct {
        uint8_t E_VIN_OV:1;     /* VIN_OV fault [NS] 0 */
        uint8_t W_VIN_OV:1;     /* VIN_OV_WARN An input overvoltage W.H.O. 0 */
        uint8_t W_VIN_UV:1;     /* VIN_UV_WARN An input undervoltage W.H.O. 0 */
        uint8_t E_VIN_UV:1;     /* VIN_UV fault [NS] 0 */
        uint8_t E_VBAD:1;       /* Insufficient voltage [NS] 0 */
        uint8_t E_IIN_OC:1;     /* IIN_OC fault [NS] 0 */
        uint8_t W_IIN_OC:1;     /* IIN_OC_WARN An input overcurrent W.H.O. 0 */
        uint8_t W_PIN_OP:1;     /* PIN_OP_WARN An input overpower W.H.O. 0 */
    } __attribute__ ((packed));
    uint8_t raw_val;
} reg_status_input;

typedef union {
    struct {
        uint8_t E_CMD;          /* Invalid or unsupported command received 0 */
        uint8_t __NA6:1;        /* [NS] 0 */
        uint8_t E_PACKET:1;     /* Packet error check failed 0 */
        uint8_t E_MEM:1;        /* Memory fault detected (trim fuse CRC failed,
                                   ECC active) 0 */
        uint8_t __NA3:1;        /* [NS] 0 */
        uint8_t __NA2:1;        /* Reserved 0 */
        uint8_t __NA1:1;        /* [NS] 0 */
        uint8_t __NA0:1;        /* [NS] 0 */
    } __attribute__ ((packed));
    uint8_t raw_val;
} reg_status_cml;

typedef union {
    struct {
        uint8_t ADC_RDY:1;      /* Conversion ready 0 */
        uint8_t ADC_OWFLW:1;    /* Arithmetic overflow flag. If the bit is set
                                   to 1 then an arithmetic operation results
                                   from an overflow error. This bit indicate
                                   that either the current or power data is
                                   invalid. 0 */
        uint8_t PON_DETECT:1;   /* Power-on-reset event detected. To detect
                                   power-on or power glitch events, this bit
                                   must be cleared after initial power up. If
                                   power is interrupted this bit is reset to
                                   the default value of 1 */
        uint8_t E_COM_MEM:1;    /* Communications or memory fault (or of
                                   STATUS_CML) 0 */
        uint8_t W_IN_OPWR:1;    /* Input overpower warning 0 */
        uint8_t W_IN_UPWR:1;    /* Input overcurrent warning 0 */
        uint8_t W_IN_OV:1;      /* Input overvoltage warning 0 */
        uint8_t W_IN_UV:1;      /* Input undervoltage warning 0 */
    } __attribute__ ((packed));
    uint8_t raw_val;
} reg_status_mfr_specific;

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
        uint8_t NONE_OF_THE_ABOVE:1;
        uint8_t CML:1;
        uint8_t TEMPERATURE:1;
        uint8_t VIN_UV:1;
        uint8_t IOUT_OC:1;
        uint8_t VOUT_OC:1;
        uint8_t OFF:1;
        uint8_t BUSY:1;
    } __attribute__ ((packed));
    uint8_t raw_val;
} reg_status_byte;

typedef union {
    struct {
        uint16_t NONE_OF_THE_ABOVE:1;
        uint16_t CML:1;
        uint16_t TEMPERATURE:1;
        uint16_t VIN_UV:1;
        uint16_t IOUT_OC:1;
        uint16_t VOUT_OC:1;
        uint16_t OFF:1;
        uint16_t BUSY:1;

        uint16_t UNKNOWN:1;
        uint16_t OTHER:1;
        uint16_t FANS:1;
        uint16_t POWER_GOOD:1;
        uint16_t MFR:1;
        uint16_t INPUT:1;
        uint16_t IOUT_POUT:1;
        uint16_t VOUT:1;
    } __attribute__ ((packed));
    uint16_t raw_val;
    uint8_t barray[2];
} reg_status_word;

typedef union {
    struct {
        uint8_t W_POUT_OP:1;
        uint8_t E_POUT_OP:1;
        uint8_t IN_PWR_LMT:1;
        uint8_t CURRENT_SHARE:1;
        uint8_t E_IOUT_UC:1;
        uint8_t W_IOUT_OC:1;
        uint8_t W_IOUT_OC_POFF:1;
        uint8_t E_IOUT_OC:1;
    } __attribute__ ((packed));
    uint8_t raw_val;
} reg_status_iout;

typedef union {
    struct {
        uint8_t W_PIN_OP:1;
        uint8_t W_IIN_OC:1;
        uint8_t E_IIN_OC:1;
        uint8_t E_VBAD:1;
        uint8_t E_VIN_UV:1;
        uint8_t W_VIN_UV:1;
        uint8_t W_VIN_OV:1;
        uint8_t E_VIN_OV:1;
    } __attribute__ ((packed));
    uint8_t raw_val;
} reg_status_input;

typedef union {
    struct {
        uint8_t __NA0:1;
        uint8_t __NA1:1;
        uint8_t __NA2:1;
        uint8_t __NA3:1;
        uint8_t E_MEM:1;
        uint8_t E_PACKET:1;
        uint8_t __NA6:1;
        uint8_t E_CMD;
    } __attribute__ ((packed));
    uint8_t raw_val;
} reg_status_cml;

typedef union {
    struct {
        uint8_t W_IN_UV:1;
        uint8_t W_IN_OV:1;
        uint8_t W_IN_UPWR:1;
        uint8_t W_IN_OPWR:1;
        uint8_t E_COM_MEM:1;
        uint8_t PON_DETECT:1;
        uint8_t ADC_OWFLW:1;
        uint8_t ADC_RDY:1;
    } __attribute__ ((packed));
    uint8_t raw_val;
} reg_status_mfr_specific;

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
void restore_default_all();
uint8_t capability();

//void iout_oc_warn_limit();
//void vin_ov_warn_limit();
//void vin_uv_warn_limit();
//void pin_op_warn_limit();

reg_status_byte status_byte();
reg_status_word status_word();

reg_status_iout get_status_iout();
reg_status_input get_status_input();
reg_status_cml get_status_cml();
reg_status_mfr_specific get_status_mfr_specific();
void set_status_iout(reg_status_iout status);
void set_status_input(reg_status_input status);
void set_status_cml(reg_status_input status);
void set_status_mfr_specific(reg_status_mfr_specific status);

double read_vin();
double read_iin();
double read_pin();

double read_vout();
double read_iout();
double read_pout();

//void read_ein();

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
