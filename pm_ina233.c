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
#include <pm_ina233.h>
#include "pm_ina233_device.h"

i2c_device_hndl i2c_device = NULL;

void pm_init(void)
{
    i2c_device = i2c_device_open(I2C1, INA233_ADDR);
}

void pm_deinit(void)
{
    i2c_device_close(i2c_device);
}

void Control()
{
}

void Temperature()
{
}

float Voltage()
{
    return i2c_device_read_uint16(i2c_device, READ_VIN) / 1000.0;
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

void AverageCurrent()
{
}

void StandbyCurrent()
{
}

void MaxLoadCurrent()
{
}

void AveragePower()
{
}

void StateOfCharge()
{
}

void InternalTemperature()
{
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
