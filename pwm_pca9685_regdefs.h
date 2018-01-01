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
#ifndef pwm_pca9685_regdefs_h
#define pwm_pca9685_regdefs_h
/*
 * pwm_pca9685 registers and layout
 */

#include <stdint.h>

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

/* Register layouts as bit-field struct:s */
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

#endif                          //pwm_pca9685_regdefs_h
