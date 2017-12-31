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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <assert.h>
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
    I2C_TypeDef *bus;           /* HW-bus: I2C0-I2Cn */
    uint8_t addr;               /* 7-bit write address - read-address implicit */
    struct registers_t *registers;  /* Copy of pca9685 registers or NULL if not in sync */
};

/* Register bit-field struct:s */
#if BYTES_BIG_ENDIAN
typedef union {
    struct {
        uint8_t RESTART:1;
        uint8_t EXTCLK:1;
        uint8_t AI:1;
        uint8_t SLEEP:1;
        uint8_t SUB1:1;
        uint8_t SUB2:1;
        uint8_t SUB3:1;
        uint8_t ALLCALL:1;
    } __attribute__ ((packed));
    uint8_t raw;
} reg_mode1_t;

typedef union {
    struct {
        uint8_t _reserved:3;
        uint8_t INVRT:1;
        uint8_t OCH:1;
        uint8_t OUTDRV:1;
        uint8_t OUTNE:2;
    } __attribute__ ((packed));
    uint8_t raw;
} reg_mode2_t;

typedef union {
    struct {
        uint16_t _reserved:3;
        uint16_t FULL:1;
        uint16_t CNTR:12;
    } __attribute__ ((packed));
    uint8_t barray[2];
    uint16_t raw;
} pwm_counter_t;

typedef union {
    struct {
        pwm_counter_t off;
        pwm_counter_t on;
    } __attribute__ ((packed));
    uint8_t barray[4];
    uint32_t raw;
} pwm_reg_t;

#else
typedef union {
    struct {
        uint8_t ALLCALL:1;
        uint8_t SUB3:1;
        uint8_t SUB2:1;
        uint8_t SUB1:1;
        uint8_t SLEEP:1;
        uint8_t AI:1;
        uint8_t EXTCLK:1;
        uint8_t RESTART:1;
    } __attribute__ ((packed));
    uint8_t raw;
} reg_mode1_t;

typedef union {
    struct {
        uint8_t OUTNE:2;
        uint8_t OUTDRV:1;
        uint8_t OCH:1;
        uint8_t INVRT:1;
        uint8_t _reserved:3;
    } __attribute__ ((packed));
    uint8_t raw;
} reg_mode2_t;

typedef union {
    struct {
        uint16_t CNTR:12;
        uint16_t FULL:1;
        uint16_t _reserved:3;
    } __attribute__ ((packed));
    uint8_t barray[2];
    uint16_t raw;
} pwm_counter_t;

typedef union {
    struct {
        pwm_counter_t on;
        pwm_counter_t off;
    } __attribute__ ((packed));
    uint8_t barray[4];
    uint32_t raw;
} pwm_reg_t;

#endif

struct registers_t {
    reg_mode1_t mode1;
    reg_mode2_t mode2;
};

/* Forward declaration of static functions */
/* Generalized access functions */
static uint8_t reg_read_uint8(pwm_hndl pwm, uint8_t reg);
static uint16_t reg_read_uint16(pwm_hndl pwm, uint8_t reg);
static uint32_t reg_read_uint32(pwm_hndl pwm, uint8_t reg);
static void reg_write_uint8(pwm_hndl pwm, uint8_t reg, uint8_t val);
static void reg_write_uint16(pwm_hndl pwm, uint8_t reg, uint16_t val);
static void reg_write_uint32(pwm_hndl pwm, uint8_t reg, uint32_t val);

/* Driver specific functions */
static uint8_t pwm_reg_index(int index);
static reg_mode1_t get_mode1(pwm_hndl pwm);
static reg_mode2_t get_mode2(pwm_hndl pwm);
static void set_mode1(pwm_hndl pwm, reg_mode1_t val);
static void set_mode2(pwm_hndl pwm, reg_mode2_t val);

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

    pwm->registers = NULL;

    return pwm;
}

void pwm_pca9685_destruct(pwm_hndl pwm)
{
    if (pwm->registers != NULL)
        free(pwm->registers);
    free(pwm);
}

/* Initializes device and synchronizes driver with pca9685 device */
void pwm_pca9685_init(pwm_hndl pwm)
{
    reg_mode1_t reg_mode1;
    reg_mode2_t reg_mode2;

    assert(sizeof(reg_mode1_t) == 1);

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

    pwm->registers = malloc(sizeof(struct registers_t));
    pwm->registers->mode1 = get_mode1(pwm);
    pwm->registers->mode2 = get_mode2(pwm);

    /* Temporary sanity test of read-back. To be removed/improved when init
     * is more intelligent */
    assert(pwm->registers->mode1.raw == 0x21);
    assert(pwm->registers->mode2.raw == 0x04);
}

/* Set-up PWM0-PWM4 to a pre-defined test-pattern  */
void pwm_pca9685_test(pwm_hndl pwm)
{
    i2c_write(pwm->bus, pwm->addr, (uint8_t[]) {
              PWM0_ON_L,
              0x00, 0x00, 0x00, 0x08, 0x00, 0x08, 0x00, 0x00, 0x00, 0x03, 0x00,
              0x05, 0x00, 0x08, 0x00, 0x09, 0x00, 0x0F, 0x00, 0x01}, 21, 1);
}

void pwm_pca9685_set(pwm_hndl pwm, uint8_t index, struct pwm_val val)
{
    pwm_reg_t pwm_formatted;

    pwm_formatted.raw = 0;
    pwm_formatted.on.CNTR = val.on_cntr;
    pwm_formatted.off.CNTR = val.off_cntr;

    reg_write_uint32(pwm, pwm_reg_index(index), pwm_formatted.raw);
}

struct pwm_val pwm_pca9685_get(pwm_hndl pwm, uint8_t index)
{
    struct pwm_val val;
    uint32_t pwm_tmp;
    pwm_reg_t pwm_formatted;

    pwm_tmp = reg_read_uint32(pwm, pwm_reg_index(index));
    pwm_formatted = *((pwm_reg_t *) & pwm_tmp);

    val.on_cntr = pwm_formatted.on.CNTR;
    val.off_cntr = pwm_formatted.off.CNTR;

    return val;
}

/* Static functions */

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

/* i2c generic low-level register access functions */
uint8_t reg_read_uint8(pwm_hndl pwm, uint8_t reg)
{
    uint8_t val = 0;

    /* Send which register to access, omit STOP */
    i2c_write(pwm->bus, pwm->addr, (uint8_t[]) {
              reg}, 1, 0);

    i2c_read(pwm->bus, pwm->addr, &val, sizeof(val));

    return val;
}

uint16_t reg_read_uint16(pwm_hndl pwm, uint8_t reg)
{
    uint16_t val = 0;
    uint8_t buf[2];

    /* Send which register to access, omit STOP */
    i2c_write(pwm->bus, pwm->addr, (uint8_t[]) {
              reg}, 1, 0);

    i2c_read(pwm->bus, pwm->addr, buf, sizeof(val));
    val = *(uint16_t *)buf;

    return val;
}

uint32_t reg_read_uint32(pwm_hndl pwm, uint8_t reg)
{
    uint32_t val = 0;
    uint8_t buf[4];

    /* Send which register to access, omit STOP */
    i2c_write(pwm->bus, pwm->addr, (uint8_t[]) {
              reg}, 1, 0);

    i2c_read(pwm->bus, pwm->addr, buf, sizeof(val));
    val = *(uint32_t *)buf;

    return val;
}

void reg_write_uint8(pwm_hndl pwm, uint8_t reg, uint8_t val)
{
    /* Send which register to access directly followed by value */
    i2c_write(pwm->bus, pwm->addr, (uint8_t[]) {
              reg, val}, sizeof(val) + 1, 1);
}

void reg_write_uint16(pwm_hndl pwm, uint8_t reg, uint16_t val)
{
    uint8_t buf[sizeof(val) + 1];
    buf[0] = reg;
    *(uint16_t *)(&buf[1]) = val;

    assert(sizeof(buf) == 3);

    i2c_write(pwm->bus, pwm->addr, buf, sizeof(val) + 1, 1);
}

void reg_write_uint32(pwm_hndl pwm, uint8_t reg, uint32_t val)
{
    uint8_t buf[sizeof(val) + 1];
    buf[0] = reg;
    *(uint32_t *)(&buf[1]) = val;

    assert(sizeof(buf) == 5);

    i2c_write(pwm->bus, pwm->addr, buf, sizeof(val) + 1, 1);
}

/* Specific register high-level access functions */
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
