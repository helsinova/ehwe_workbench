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
#include <stddef.h>
#include <string.h>

#include <ehwe_i2c_device.h>
#include <pm_ina233.h>
#include "pm_ina233_device.h"

/* Change the following according to HW-design */
#define  R_SHUNT             (1.0E-3)
#define  CAL                 (2555.0)   /* Multiplication factor
                                           (value of calibration register) */
#define  SCAL                (32.9) /* Soft (fine tune) calibration
                                       Note: Inverse multiplication */

/* Constants and derived constants */
#define  VOLTAGE_LSB         (1.25E-3)
#define  VOLTAGE_SHUNT_LSB   (2.5E-6)
#define  CURRENT_LSB         (5.12E-3 / (CAL*R_SHUNT))
#define  POWER_LSB           (CURRENT_LSB * 25.0 / SCAL)

/* Convert internal (ADC) values to SI-units */
#define ADC2VOLTAGE( V )  ((  ((double)(int16_t) V )) * VOLTAGE_LSB)
#define ADC2CURRENT( I )  ((( ((double)(int16_t) I )) * CURRENT_LSB) / SCAL)
#define ADC2POWER( P )    ((  ((double)(int16_t) P )) * POWER_LSB)

i2c_device_hndl i2c_device = NULL;

void pm_init(void)
{
    i2c_device = i2c_device_open(I2C1, ADDR_INA233);
}

void pm_deinit(void)
{
    i2c_device_close(i2c_device);
}

/* Device commands */

void clear_faults()
{
    i2c_device_write_bytes(i2c_device, CMD_CLEAR_FAULTS, NULL, 0);
}

void restore_default_all()
{
    i2c_device_write_bytes(i2c_device, CMD_RESTORE_DEFAULT_ALL, NULL, 0);
}

uint8_t capability()
{
    return i2c_device_read_uint8(i2c_device, CMD_CAPABILITY);
}

void iout_oc_warn_limit()
{
}

void vin_ov_warn_limit()
{
}

void vin_uv_warn_limit()
{
}

void pin_op_warn_limit()
{
}

/* Overview status */
reg_status_byte status_byte()
{
    return (reg_status_byte) i2c_device_read_uint8(i2c_device, CMD_STATUS_BYTE);
}

reg_status_word status_word()
{
    return (reg_status_word) i2c_device_read_uint16(i2c_device,
                                                    CMD_STATUS_WORD);
}

/* Specific statuses: set/get - use set_X::bit to clear */
reg_status_iout get_status_iout()
{
    return (reg_status_iout) i2c_device_read_uint8(i2c_device, CMD_STATUS_IOUT);
}

reg_status_input get_status_input()
{
    return (reg_status_input) i2c_device_read_uint8(i2c_device,
                                                    CMD_STATUS_INPUT);
}

reg_status_cml get_status_cml()
{
    return (reg_status_cml) i2c_device_read_uint8(i2c_device, CMD_STATUS_CML);
}

reg_status_mfr_specific get_status_mfr_specific()
{
    return (reg_status_mfr_specific) i2c_device_read_uint8(i2c_device,
                                                           CMD_STATUS_MFR_SPECIFIC);
}

void set_status_iout(reg_status_iout status)
{
    i2c_device_write_uint8(i2c_device, CMD_STATUS_IOUT, status.raw_val);
}

void set_status_input(reg_status_input status)
{
    i2c_device_write_uint8(i2c_device, CMD_STATUS_INPUT, status.raw_val);
}

void set_status_cml(reg_status_input status)
{
    i2c_device_write_uint8(i2c_device, CMD_STATUS_CML, status.raw_val);
}

void set_status_mfr_specific(reg_status_mfr_specific status)
{
    i2c_device_write_uint8(i2c_device, CMD_STATUS_MFR_SPECIFIC, status.raw_val);
}

/* Note: Block read */
reg_read_ein read_ein()
{
    uint8_t buf[7] = { 0 };
    reg_read_ein ein;

    /* Somewhat architecture portable */
    i2c_device_read_bytes(i2c_device, CMD_READ_EIN, buf, sizeof(buf));
    ein.power_accumulator = *((uint16_t*)(&(buf[1])));
    ein.count.rollover = buf[3];
    ein.barray[5] = buf[6];
    ein.barray[4] = buf[5];
    ein.barray[3] = buf[4];

    return ein;
}

double read_vin()
{
    uint16_t tvoltage;

    tvoltage = i2c_device_read_uint16(i2c_device, CMD_READ_VIN);

    /* Handle undocumented special case */
    if (tvoltage == 0xFF00) {
        tvoltage = 0;
    }

    return ADC2VOLTAGE(tvoltage);
}

double read_vout()
{
    uint16_t tvoltage;

    tvoltage = i2c_device_read_uint16(i2c_device, CMD_READ_VOUT);

    /* Handle undocumented special case */
    if (tvoltage == 0xFF00) {
        tvoltage = 0;
    }

    return ADC2VOLTAGE(tvoltage);
}

double read_iin()
{

    return ADC2CURRENT(i2c_device_read_uint16(i2c_device, CMD_READ_IIN));
}

double read_iout()
{
    return ADC2CURRENT(i2c_device_read_uint16(i2c_device, CMD_READ_IOUT));
}

double read_pout()
{
    return ADC2POWER(i2c_device_read_uint16(i2c_device, CMD_READ_POUT));
}

double read_pin()
{
    return ADC2POWER(i2c_device_read_uint16(i2c_device, CMD_READ_PIN));
}

/* Note: Block read */
uint16_t mfr_id()
{
    uint8_t buf[3] = { 0 };

    i2c_device_read_bytes(i2c_device, CMD_MFR_MODEL, buf, sizeof(buf));

    return *((uint16_t *)(&buf[1]));
}

/* Note: Block read */
char *mfr_model(char model[6])
{
    uint8_t buf[7] = { 0 };

    i2c_device_read_bytes(i2c_device, CMD_MFR_MODEL, buf, sizeof(buf));
    memcpy(model, &buf[1], 6);

    return model;
}

uint8_t mfr_revision()
{
    return i2c_device_read_uint8(i2c_device, CMD_MFR_REVISION);
}

reg_mfr_adc_config get_mfr_adc_config()
{
    uint16_t tmp;
    tmp = i2c_device_read_uint16(i2c_device, CMD_MFR_ADC_CONFIG);

    return (reg_mfr_adc_config) tmp;
}

void set_mfr_adc_config(reg_mfr_adc_config config)
{
    i2c_device_write_uint16(i2c_device, CMD_MFR_ADC_CONFIG, config.raw_val);
}

/* Return VShunt ADC-value*/
uint16_t mfr_read_vshunt_ADC()
{
    return i2c_device_read_uint16(i2c_device, CMD_MFR_READ_VSHUNT);
}

double mfr_read_vshunt()
{
    return ADC2VOLTAGE(i2c_device_read_uint16(i2c_device, CMD_MFR_READ_VSHUNT));
}

void mfr_alert_mask()
{
}

void set_mfr_calibration(uint16_t cal)
{
    i2c_device_write_uint16(i2c_device, CMD_MFR_CALIBRATION, cal);
}

uint16_t get_mfr_calibration()
{
    return i2c_device_read_uint16(i2c_device, CMD_MFR_CALIBRATION);
}

void mfr_device_config()
{
}

void clear_ein()
{
    i2c_device_write_bytes(i2c_device, CMD_CLEAR_EIN, (uint8_t[]){0}, 1);
}

int16_t ti_mfr_id()
{
    return i2c_device_read_uint16(i2c_device, CMD_TI_MFR_ID);
}

int16_t ti_mfr_model()
{
    return i2c_device_read_uint16(i2c_device, CMD_TI_MFR_MODEL);
}

int16_t ti_mfr_revision()
{
    return i2c_device_read_uint16(i2c_device, CMD_TI_MFR_REVISION);
}
