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
reg_mode1_t get_mode1(pwm_hndl pwm_dev)
{
    if (pwm_dev->reg)
        return pwm_dev->reg->mode1;

    return (reg_mode1_t) reg_read_uint8(pwm_dev, MODE1);
}

reg_mode2_t get_mode2(pwm_hndl pwm_dev)
{
    if (pwm_dev->reg)
        return pwm_dev->reg->mode2;

    return (reg_mode2_t) reg_read_uint8(pwm_dev, MODE2);
}

void set_mode1(pwm_hndl pwm_dev, reg_mode1_t val)
{
    reg_write_uint8(pwm_dev, MODE1, val.raw);

    if (pwm_dev->reg)
        pwm_dev->reg->mode1 = val;
}

void set_mode2(pwm_hndl pwm_dev, reg_mode2_t val)
{
    reg_write_uint8(pwm_dev, MODE2, val.raw);

    if (pwm_dev->reg)
        pwm_dev->reg->mode2 = val;
}

/* PWM-frequency set/get functions in Hz
   Valid frequencies are 24-1526Hz

   Note that device must be in SLEEP mode for frequency changes to take
   effect.
 */
void set_pwm_freq(pwm_hndl pwm_dev, unsigned int freq)
{
    uint8_t prescale = 0;

    assert((freq >= 24) && (freq <= 1526));

    prescale = OSC_CLOCK / (4096 * freq) - 1;
    reg_write_uint8(pwm_dev, PRE_SCALE, prescale);

    if (pwm_dev->reg)
        pwm_dev->reg->prescale = prescale;
}

unsigned int get_pwm_freq(pwm_hndl pwm_dev)
{
    uint8_t prescale = 0;

    prescale = reg_read_uint8(pwm_dev, PRE_SCALE);

    return OSC_CLOCK / ((prescale + 1) * 4096);
}

void regs_sync(pwm_hndl pwm_dev)
{
    if (pwm_dev->reg == NULL) {
        pwm_dev->reg = malloc(sizeof(struct devregs));
    }
#ifdef DEVREGS_CONTAIN_RESERVED_REGS
    int regssz = sizeof(struct devregs);

    i2c_write(pwm_dev->bus, pwm_dev->addr, (uint8_t[]) {
              MODE1}, 1, 0);

    i2c_read(pwm_dev->bus, pwm_dev->addr, pwm_dev->reg->barray, regssz);
#else
	/* First part */
    i2c_write(pwm_dev->bus, pwm_dev->addr, (uint8_t[]) {
              MODE1}, 1, 0);
    i2c_read(pwm_dev->bus, pwm_dev->addr, pwm_dev->reg->barray,
             sizeof(pwm_dev->reg->regs_part1));

	/* Second part */
    i2c_write(pwm_dev->bus, pwm_dev->addr, (uint8_t[]) {
              ALL_PWM_ON_L}, 1, 0);
    i2c_read(pwm_dev->bus, pwm_dev->addr, pwm_dev->reg->barray,
             sizeof(pwm_dev->reg->regs_part2));
#endif

}

reg_pwm_t get_pwm(pwm_hndl pwm_dev, uint8_t idx)
{
    /* Return cashed info if available */
    if (pwm_dev->reg) {
        if (idx == ALL_PWM)
            return pwm_dev->reg->allpwm;

        return pwm_dev->reg->pwm[pwm_reg_index(idx)];
    }

    return (reg_pwm_t) reg_read_uint32(pwm_dev, pwm_reg_index(idx));
}

void set_pwm(pwm_hndl pwm_dev, uint8_t idx, reg_pwm_t reg_pwm)
{
    /* Try to short-cut if possible */
    if (pwm_dev->reg && (idx != ALL_PWM)) {
        if (reg_pwm.raw == pwm_dev->reg->pwm[pwm_reg_index(idx)].raw)
            /* Identical value cache<->device, so cut it short */
            return;
    }

    reg_write_uint32(pwm_dev, pwm_reg_index(idx), reg_pwm.raw);

    if (pwm_dev->reg) {
        if (idx == ALL_PWM)
            pwm_dev->reg->allpwm = reg_pwm;
        else
            pwm_dev->reg->pwm[pwm_reg_index(idx)] = reg_pwm;
    }
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
