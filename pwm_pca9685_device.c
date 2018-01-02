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

#include "pwm_pca9685_device.h"
#include "pwm_pca9685_regrw.h"
#include "pwm_pca9685_regdefs.h"
#include <ehwe.h>
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#define OSC_CLOCK 25000000

#ifndef ALL_PWM
#define ALL_PWM 16
#endif                          //ALL_PWM

static uint8_t pwm_reg_index(int index);

/* Device-register high-level access functions */
reg_mode1_t get_mode1(pwm_hndl pwm)
{
    if (pwm->reg)
        return pwm->reg->mode1;

    return (reg_mode1_t) reg_read_uint8(pwm, MODE1);
}

reg_mode2_t get_mode2(pwm_hndl pwm)
{
    if (pwm->reg)
        return pwm->reg->mode2;

    return (reg_mode2_t) reg_read_uint8(pwm, MODE2);
}

void set_mode1(pwm_hndl pwm, reg_mode1_t val)
{
    reg_write_uint8(pwm, MODE1, val.raw);

    if (pwm->reg)
        pwm->reg->mode1 = val;
}

void set_mode2(pwm_hndl pwm, reg_mode2_t val)
{
    reg_write_uint8(pwm, MODE2, val.raw);

    if (pwm->reg)
        pwm->reg->mode2 = val;
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

    if (pwm->reg)
        pwm->reg->prescale = prescale;
}

unsigned int get_pwm_freq(pwm_hndl pwm)
{
    uint8_t prescale = 0;

    prescale = reg_read_uint8(pwm, PRE_SCALE);

    return OSC_CLOCK / ((prescale + 1) * 4096);
}

void regs_sync(pwm_hndl pwm)
{
    int regssz = sizeof(struct devregs);

    if (pwm->reg == NULL) {
        pwm->reg = malloc(sizeof(struct devregs));
    }
#ifdef DEVREGS_CONTAIN_RESERVED_REGS
    /* Send which register to access, omit STOP */
    i2c_write(pwm->bus, pwm->addr, (uint8_t[]) {
              MODE1}, 1, 0);

    i2c_read(pwm->bus, pwm->addr, pwm->reg->barray, regssz);
#endif

}

reg_pwm_t get_pwm(pwm_hndl pwm, uint8_t idx)
{
    reg_pwm_t reg_pwm;

    reg_pwm = (reg_pwm_t) reg_read_uint32(pwm, pwm_reg_index(idx));

    return reg_pwm;
}

void set_pwm(pwm_hndl pwm, uint8_t idx, reg_pwm_t reg_pwm)
{
    reg_write_uint32(pwm, pwm_reg_index(idx), reg_pwm.raw);
}

/* Transform a PWM-index into a PWM-register "address".
 * Valid indexes are {0-15, ALL_PWM}.
 * There's no sanity checking, exceeding limits results in fold-over. */
static uint8_t pwm_reg_index(int index)
{
    uint8_t regnum = ALL_PWM_ON_L;

    if (index == ALL_PWM)
        return regnum;

    /* Optimized way of "((index % 16) * 4) + LOWEST_REG_NUMBER" */
    regnum = index & 0xFuL;
    return ((regnum << 2) + PWM0_ON_L);
}
