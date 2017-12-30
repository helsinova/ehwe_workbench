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
 * pwm_pca9685 user functions library
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <ehwe.h>
#include <stm32f10x.h>
#include "pwm_pca9685.h"

/* 7-bit W/r device default address */
#define DFLT_PCA9685_ADDR           0x40

/* Registers */
#define MODE1                       0x00
#define MODE2                       0x01
#define SUBADR1                     0x02
#define SUBADR2                     0x03
#define SUBADR3                     0x04
#define ALLCALLADR                  0x05

#define PWM0_ON_L                   0x06
#define PWM0_ON_H                   0x07
#define PWM0_OFF_L                  0x08
#define PWM0_OFF_H                  0x09

#define PWM1_ON_L                   0x0A
#define PWM1_ON_H                   0x0B
#define PWM1_OFF_L                  0x0C
#define PWM1_OFF_H                  0x0D

#define PWM2_ON_L                   0x0E
#define PWM2_ON_H                   0x0F
#define PWM2_OFF_L                  0x10
#define PWM2_OFF_H                  0x11

#define PWM3_ON_L                   0x12
#define PWM3_ON_H                   0x13
#define PWM3_OFF_L                  0x14
#define PWM3_OFF_H                  0x15

#define PWM4_ON_L                   0x16
#define PWM4_ON_H                   0x17
#define PWM4_OFF_L                  0x18
#define PWM4_OFF_H                  0x19

#define PWM5_ON_L                   0x1A
#define PWM5_ON_H                   0x1B
#define PWM5_OFF_L                  0x1C
#define PWM5_OFF_H                  0x1D

#define PWM6_ON_L                   0x1E
#define PWM6_ON_H                   0x1F
#define PWM6_OFF_L                  0x20
#define PWM6_OFF_H                  0x21

#define PWM7_ON_L                   0x22
#define PWM7_ON_H                   0x23
#define PWM7_OFF_L                  0x24
#define PWM7_OFF_H                  0x25

#define PWM8_ON_L                   0x26
#define PWM8_ON_H                   0x27
#define PWM8_OFF_L                  0x28
#define PWM8_OFF_H                  0x29

#define PWM9_ON_L                   0x2A
#define PWM9_ON_H                   0x2B
#define PWM9_OFF_L                  0x2C
#define PWM9_OFF_H                  0x2D

#define PWM10_ON_L                  0x2E
#define PWM10_ON_H                  0x2F
#define PWM10_OFF_L                 0x30
#define PWM10_OFF_H                 0x31

#define PWM11_ON_L                  0x32
#define PWM11_ON_H                  0x33
#define PWM11_OFF_L                 0x34
#define PWM11_OFF_H                 0x35

#define PWM12_ON_L                  0x36
#define PWM12_ON_H                  0x37
#define PWM12_OFF_L                 0x38
#define PWM12_OFF_H                 0x39

#define PWM13_ON_L                  0x3A
#define PWM13_ON_H                  0x3B
#define PWM13_OFF_L                 0x3C
#define PWM13_OFF_H                 0x3D

#define PWM14_ON_L                  0x3E
#define PWM14_ON_H                  0x3F
#define PWM14_OFF_L                 0x40
#define PWM14_OFF_H                 0x41

#define PWM15_ON_L                  0x42
#define PWM15_ON_H                  0x43
#define PWM15_OFF_L                 0x44
#define PWM15_OFF_H                 0x45

#define ALL_PWM_ON_L                0xFA
#define ALL_PWM_ON_H                0xFB
#define ALL_PWM_OFF_L               0xFC
#define ALL_PWM_OFF_H               0xFD

#define PRE_SCALE                   0xFE
#define TEST_MODE                   0xFF

struct pwm_instance {
    I2C_TypeDef *bus;
    uint8_t addr;
};

/* Invokes a SW reset-all. 
   Note: this function resets ALL pca9685 devices attached to a certain
   i2c-bus.
 */
void pwm_pca9685_swreset(pwm_hndl pwm)
{
    i2c_write(pwm->bus, 0x00, (uint8_t[]) {
              0x06}, 1, 1);
}

/* Creates a pwm-instance, initializes device and returns handle to it.
 * Whence done with the pwm-instance, the handle can be freed.*/
pwm_hndl pwm_pca9685_init(I2C_TypeDef * busID)
{
    struct pwm_instance *pwm;

    pwm = malloc(sizeof(struct pwm_instance));

    /* bus-id for this instance. */
    pwm->bus = busID;
    pwm->addr = DFLT_PCA9685_ADDR;

    pwm_pca9685_swreset(pwm);

    /* Clear SLEEP-bit */
    i2c_write(pwm->bus, pwm->addr, (uint8_t[]) {
              MODE1, 0x01}, 2, 1);
    usleep(500);

    /* Set AI */
    i2c_write(pwm->bus, pwm->addr, (uint8_t[]) {
              MODE1, 0x21}, 2, 1);

    /* Set a test output */
    i2c_write(pwm->bus, pwm->addr, (uint8_t[]) {
              PWM0_ON_L,
              0x00, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 0x00, 0x03, 0x00,
              0x05, 0x00, 0x08, 0x00, 0x09, 0x00, 0x0F, 0x00, 0x01}, 21, 1);

    return pwm;

}
