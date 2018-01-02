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
 * pwm_pca9685 low-level register bus-access functions
 *
 * These are almost i2c-generic - however not quite as register layout may
 * differ from device to device. pwm_pca9685 happens to have a register
 * layout that is LSB.
 */

#include <ehwe.h>
#include <assert.h>
#include "pwm_pca9685_regrw.h"

uint8_t reg_read_uint8(pwm_hndl pwm_dev, uint8_t reg)
{
    uint8_t val = 0;

    /* Send which register to access, omit STOP */
    i2c_write(pwm_dev->bus, pwm_dev->addr, (uint8_t[]) {
              reg}, 1, 0);

    i2c_read(pwm_dev->bus, pwm_dev->addr, &val, sizeof(val));

    return val;
}

uint16_t reg_read_uint16(pwm_hndl pwm_dev, uint8_t reg)
{
    uint16_t val = 0;
    uint8_t buf[2];

    /* Send which register to access, omit STOP */
    i2c_write(pwm_dev->bus, pwm_dev->addr, (uint8_t[]) {
              reg}, 1, 0);

    i2c_read(pwm_dev->bus, pwm_dev->addr, buf, sizeof(val));
    val = *(uint16_t *)buf;

    return val;
}

uint32_t reg_read_uint32(pwm_hndl pwm_dev, uint8_t reg)
{
    uint32_t val = 0;
    uint8_t buf[4];

    /* Send which register to access, omit STOP */
    i2c_write(pwm_dev->bus, pwm_dev->addr, (uint8_t[]) {
              reg}, 1, 0);

    i2c_read(pwm_dev->bus, pwm_dev->addr, buf, sizeof(val));
    val = *(uint32_t *)buf;

    return val;
}

void reg_write_uint8(pwm_hndl pwm_dev, uint8_t reg, uint8_t val)
{
    /* Send which register to access directly followed by value */
    i2c_write(pwm_dev->bus, pwm_dev->addr, (uint8_t[]) {
              reg, val}, sizeof(val) + 1, 1);
}

void reg_write_uint16(pwm_hndl pwm_dev, uint8_t reg, uint16_t val)
{
    uint8_t buf[sizeof(val) + 1];
    buf[0] = reg;
    *(uint16_t *)(&buf[1]) = val;

    assert(sizeof(buf) == 3);

    i2c_write(pwm_dev->bus, pwm_dev->addr, buf, sizeof(val) + 1, 1);
}

void reg_write_uint32(pwm_hndl pwm_dev, uint8_t reg, uint32_t val)
{
    uint8_t buf[sizeof(val) + 1];
    buf[0] = reg;
    *(uint32_t *)(&buf[1]) = val;

    assert(sizeof(buf) == 5);

    i2c_write(pwm_dev->bus, pwm_dev->addr, buf, sizeof(val) + 1, 1);
}
