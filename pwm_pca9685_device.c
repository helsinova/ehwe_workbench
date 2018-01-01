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
 * pwm_pca9685 device tier
 */

#include "pwm_pca9685_regdefs.h"
#include "pwm_pca9685_regrw.h"
#include <assert.h>

#define OSC_CLOCK 25000000

/* Register high-level access functions */
reg_mode1_t get_mode1(pwm_hndl pwm)
{
    return (reg_mode1_t) reg_read_uint8(pwm, MODE1);
}

reg_mode2_t get_mode2(pwm_hndl pwm)
{
    return (reg_mode2_t) reg_read_uint8(pwm, MODE2);
}

void set_mode1(pwm_hndl pwm, reg_mode1_t val)
{
    reg_write_uint8(pwm, MODE1, val.raw);
}

void set_mode2(pwm_hndl pwm, reg_mode2_t val)
{
    reg_write_uint8(pwm, MODE2, val.raw);
}

/* PWM-frequency set/get functions in Hz
   Valid frequencies are 24-1526Hz

   Note that device must be in SLEEP mode for frequency changes to take
   effect.
 */
void set_pwm_freq(pwm_hndl pwm, unsigned int freq)
{
    uint8_t prescale = 0;

    assert((freq >= 24) && (freq <= 1526));

    prescale = OSC_CLOCK / (4096 * freq) - 1;
    reg_write_uint8(pwm, PRE_SCALE, prescale);
}

unsigned int get_pwm_freq(pwm_hndl pwm)
{
    uint8_t prescale = 0;

    prescale = reg_read_uint8(pwm, PRE_SCALE);

    return OSC_CLOCK / ((prescale + 1) * 4096);
}
