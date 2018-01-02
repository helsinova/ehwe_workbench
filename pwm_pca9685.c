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
 * pwm_pca9685 driver
 */

#include <stdint.h>
#include <unistd.h>
#include <malloc.h>
#include <assert.h>
#include <ehwe.h>
#include <stm32f10x.h>
#include <pwm_pca9685.h>
#include "pwm_pca9685_regdefs.h"
#include "pwm_pca9685_regrw.h"
#include "pwm_pca9685_device.h"

/* 7-bit W/r device default address */
#define DFLT_PCA9685_ADDR           0x40

static uint8_t pwm_reg_index(int index);

/* Invokes a SW reset-all.
   Note: This function resets ALL pca9685 devices attached to a certain
   i2c-bus.
 */
void pwm_pca9685_all_swreset(I2C_TypeDef * bus)
{
    i2c_write(bus, 0x00, (uint8_t[]) {
              0x06}, 1, 1);
}

/* Creates a pwm-instance, and returns handle to it. */
pwm_hndl pwm_pca9685_create(I2C_TypeDef * bus)
{
    struct pwm_instance *pwm;

    pwm = malloc(sizeof(struct pwm_instance));

    /* bus-id for this instance. */
    pwm->bus = bus;
    /* Only default all-call address for now (TBD) */
    pwm->addr = DFLT_PCA9685_ADDR;

    pwm->reg = NULL;

    return pwm;
}

void pwm_pca9685_destruct(pwm_hndl pwm)
{
    if (pwm->reg != NULL)
        free(pwm->reg);
    free(pwm);
}

/* Initializes driver for specific pca9685 */
void pwm_pca9685_init(pwm_hndl pwm)
{
    reg_mode1_t reg_mode1;
    reg_mode2_t reg_mode2;

    /* Make sure at least AI is set before doing anything. I.e a small waste
     * of time but it can't be helped */
    reg_mode1 = get_mode1(pwm);
    reg_mode1.AI = 1;
    set_mode1(pwm, reg_mode1);

    /* Query device for all it's registers */
    regs_sync(pwm);

    set_pwm_freq(pwm, 50);      /* Standard analog servo frequency */

    /*Hard-coded for now. Replace with read-up of current value (TBD) */
    reg_mode1.raw = 0;
    reg_mode1.SLEEP = 0;
    reg_mode1.ALLCALL = 1;

    /* Assure bit-fields are oriented correctly for this architecture */
    assert(reg_mode1.raw == 0x01);

    i2c_write(pwm->bus, pwm->addr, (uint8_t[]) {
              MODE1, reg_mode1.raw}, 2, 1);
    usleep(500);

    /* Set also AI */
    reg_mode1.AI = 1;
    /* Assure bit-fields are oriented correctly for this architecture */
    assert(reg_mode1.raw == 0x21);
    set_mode1(pwm, reg_mode1);

    /* Hard-coded as documented defaults for now. Replace with extended
     * arguments for this function (TBD) */
    reg_mode2.raw = 0;
    reg_mode2.OUTDRV = 1;       /* Totem-pole outputs */
    set_mode2(pwm, reg_mode2);

    /* Temporary sanity test of read-back. To be removed/improved when init
     * is more intelligent */
    assert(pwm->reg->mode1.raw == 0x21);
    assert(pwm->reg->mode2.raw == 0x04);
}

/* Set-up PWM0-PWM4 to a pre-defined test-pattern  */
void pwm_pca9685_test(pwm_hndl pwm)
{
    i2c_write(pwm->bus, pwm->addr, (uint8_t[]) {
              PWM0_ON_L,
              0x00, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 0x00, 0x03, 0x00,
              0x05, 0x00, 0x08, 0x00, 0x09, 0x00, 0x0F, 0x00, 0x01}, 21, 1);
}

void pwm_pca9685_set(pwm_hndl pwm, int index, struct pwm_val val)
{
    reg_pwm_t reg_pwm;

    reg_pwm.raw = 0;
    reg_pwm.on.CNTR = val.on_cntr;
    reg_pwm.off.CNTR = val.off_cntr;

    set_pwm(pwm, index, reg_pwm);
}

struct pwm_val pwm_pca9685_get(pwm_hndl pwm, int index)
{
    struct pwm_val val;
    reg_pwm_t reg_pwm;

    reg_pwm = get_pwm(pwm, index);
    val.on_cntr = reg_pwm.on.CNTR;
    val.off_cntr = reg_pwm.off.CNTR;

    return val;
}
