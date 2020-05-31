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
#include "max30105_device.h"
#include <assert.h>
#include <stdint.h>
#include <unistd.h>

i2c_device_hndl i2c_device = NULL;

void device_init(void)
{
    i2c_device = i2c_device_open(I2C1, MAX30105_ADDR);
}

void device_deinit(void)
{
    assert(i2c_device);
    i2c_device_close(i2c_device);
}

float Temperature()
{
    uint16_t ttemp;

    i2c_device_write_uint8(i2c_device, TEMP_CONF, 0x01);
    sleep(1);
    ttemp = i2c_device_read_uint16(i2c_device, TEMP);
    return (ttemp << 4);
}
