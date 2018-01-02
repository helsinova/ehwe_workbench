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

void pwm_pca9685_destruct(pwm_hndl pwm_dev)
{
    if (pwm_dev->reg != NULL)
        free(pwm_dev->reg);
    free(pwm_dev);
}

/* Initializes driver for specific pca9685 */
void pwm_pca9685_init(pwm_hndl pwm_dev)
{
    reg_mode1_t reg_mode1;
    reg_mode2_t reg_mode2;

    /* Make sure at least AI is set before doing anything else. */
    reg_mode1 = get_mode1(pwm_dev);
    if (!reg_mode1.AI) {
        reg_mode1.AI = 1;
        set_mode1(pwm_dev, reg_mode1);
    }

    /* Hard-coded for now. Replace with read-up of current value (TBD) */
    reg_mode1.raw = 0;
    reg_mode1.SLEEP = 0;
    reg_mode1.ALLCALL = 1;

    /* Assure bit-fields are oriented correctly for this architecture */
    assert(reg_mode1.raw == 0x01);

    i2c_write(pwm_dev->bus, pwm_dev->addr, (uint8_t[]) {
              MODE1, reg_mode1.raw}, 2, 1);
    usleep(500);

    /* Set also AI */
    reg_mode1.AI = 1;
    /* Assure bit-fields are oriented correctly for this architecture */
    assert(reg_mode1.raw == 0x21);
    set_mode1(pwm_dev, reg_mode1);

    /* Hard-coded as documented defaults for now. Replace with extended
     * arguments for this function (TBD) */
    reg_mode2.raw = 0;
    reg_mode2.OUTDRV = 1;       /* Totem-pole outputs */
    set_mode2(pwm_dev, reg_mode2);
}

/* Set-up PWM0-PWM4 to a pre-defined test-pattern  */
void pwm_pca9685_test(pwm_hndl pwm_dev)
{
    /* Predefined pattern */
    reg_pwm_t pwm[] = {
        {
         .on.FULL = 0,
         .off.FULL = 0,
         .on.CNTR = 0,
         .off.CNTR = 0x800,
         },
        {
         .on.FULL = 0,
         .off.FULL = 0,
         .on.CNTR = 0x800,
         .off.CNTR = 0,
         },
        {
         .on.FULL = 0,
         .off.FULL = 0,
         .on.CNTR = 0x300,
         .off.CNTR = 0x500,
         },
        {
         .on.FULL = 0,
         .off.FULL = 0,
         .on.CNTR = 0x800,
         .off.CNTR = 0x900,
         },
        {
         .on.FULL = 0,
         .off.FULL = 0,
         .on.CNTR = 0xF00,
         .off.CNTR = 0x100,
         },
    };

    /* Set several pwm:s at once */
    i2c_write(pwm_dev->bus, pwm_dev->addr, (uint8_t[]) {
              PWM0_ON_L,
              pwm[0].barray[0], pwm[0].barray[1], pwm[0].barray[1],
              pwm[0].barray[3],
              pwm[1].barray[0], pwm[1].barray[1], pwm[1].barray[1],
              pwm[1].barray[3],
              pwm[2].barray[0], pwm[2].barray[1], pwm[2].barray[1],
              pwm[2].barray[3],
              pwm[3].barray[0], pwm[3].barray[1], pwm[3].barray[1],
              pwm[3].barray[3],
              pwm[4].barray[0], pwm[4].barray[1], pwm[4].barray[1],
              pwm[4].barray[3]
              }, 21, 1);

    /* Update regs-cache */
    if (pwm_dev->reg != NULL) {
        int i, j = sizeof(pwm) / sizeof(reg_pwm_t);

        for (i = 0; i < j; i++) {
            pwm_dev->reg->pwm[i] = pwm[i];
        }
    }

}

void pwm_pca9685_set(pwm_hndl pwm_dev, int index, struct pwm_val val)
{
    reg_pwm_t reg_pwm;

    reg_pwm.raw = 0;
    reg_pwm.on.CNTR = val.on_cntr;
    reg_pwm.off.CNTR = val.off_cntr;

    set_pwm(pwm_dev, index, reg_pwm);
}

struct pwm_val pwm_pca9685_get(pwm_hndl pwm_dev, int index)
{
    struct pwm_val val;
    reg_pwm_t reg_pwm;

    reg_pwm = get_pwm(pwm_dev, index);
    val.on_cntr = reg_pwm.on.CNTR;
    val.off_cntr = reg_pwm.off.CNTR;

    return val;
}
