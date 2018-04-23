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
#include <fg_bq27441.h>
#include "fg_bq27441_device.h"

i2c_device_hndl i2c_device = NULL;

void fuelguage_init(void)
{
    i2c_device = i2c_device_open(I2C1, BQ27441_ADDR);
}

void fuelguage_deinit(void)
{
    i2c_device_close(i2c_device);
}

void Control()
{
}

float Temperature()
{
    return (i2c_device_read_uint16(i2c_device, COMMAND_TEMP) / 10.0) - 273.15;
}

float Voltage()
{
    return i2c_device_read_uint16(i2c_device, COMMAND_VOLTAGE) / 1000.0;
}

void Flags()
{
}

void NominalAvailableCapacity()
{
}

void FullAvailableCapacity()
{
}

void RemainingCapacity()
{
}

void FullChargeCapacity()
{
}

float AverageCurrent()
{
    return i2c_device_read_uint16(i2c_device, COMMAND_AVG_CURRENT) / 1000.0;
}

float StandbyCurrent()
{
    return i2c_device_read_uint16(i2c_device, COMMAND_STDBY_CURRENT) / 1000.0;
}

float MaxLoadCurrent()
{
    return i2c_device_read_uint16(i2c_device, COMMAND_MAX_CURRENT) / 1000.0;
}

float AveragePower()
{
    return i2c_device_read_uint16(i2c_device, COMMAND_AVG_POWER) / 1000.0;
}

void StateOfCharge()
{
}

float InternalTemperature()
{
    return (i2c_device_read_uint16(i2c_device, COMMAND_INT_TEMP) / 10.0) -
        273.15;
}

void StateOfHealth()
{
}

void RemainingCapacityUnfiltered()
{
}

void RemainingCapacityFiltered()
{
}

void FullChargeCapacityUnfiltered()
{
}

void FullChargeCapacityFiltered()
{
}

void StateOfChargeUnfiltered()
{
}
