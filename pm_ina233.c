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
#include <ctype.h>
#include <stddef.h>

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

void clear_faults()
{
}

void restore_default_all()
{
}

void capability()
{
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

void mfr_id()
{
}

void mfr_model()
{
}

void mfr_revision()
{
}

void mfr_adc_config()
{
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

void ti_mfr_id()
{
}

void ti_mfr_model()
{
}

void ti_mfr_revision()
{
}
