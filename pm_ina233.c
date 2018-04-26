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
#define  CAL                 (1.0)

/* Constants and derived constants */
#define  VOLTAGE_LSB         (1.25E-3)
#define  VOLTAGE_SHUNT_LSB   (2.5E-6)
#define  CURRENT_LSB         (5.12E-3 / (CAL*R_SHUNT))
#define  POWER_LSB           (CURRENT_LSB / 25)

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
}

void restore_default_all()
{
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

void status()
{
}

void status_word()
{
}

void status_iout()
{
}

void status_input()
{
}

void status_cml()
{
}

void status_mfr_specific()
{
}

/* Note: Block read */
void read_ein()
{
}

double read_vin()
{
    uint16_t tvoltage;

    tvoltage = i2c_device_read_uint16(i2c_device, CMD_READ_VIN);

    /* Handle undocumented special case */
    if (tvoltage == 0xFF00) {
        tvoltage = 0;
    }

    return ((int16_t)tvoltage) * VOLTAGE_LSB;
}

void read_vout()
{
}

void read_iout()
{
}

void read_pout()
{
}

void read_pin()
{
}

/* Note: Block read */
uint16_t mfr_id()
{
    uint8_t buf[3] = { 0 };

    i2c_device_read_bytes(i2c_device, CMD_MFR_MODEL, buf, sizeof(buf));

    return *((uint16_t *)(&buf[1]));
}

/* Note: Block read */
void mfr_model(char model[6])
{
    uint8_t buf[7] = { 0 };

    i2c_device_read_bytes(i2c_device, CMD_MFR_MODEL, buf, sizeof(buf));
    memcpy(model, &buf[1], 6);
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

void mfr_read_vshunt()
{
}

void mfr_alert_mask()
{
}

void mfr_calibration()
{
}

void mfr_device_config()
{
}

void clear_ein()
{
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
