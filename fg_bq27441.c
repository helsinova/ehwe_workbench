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

#include <ehwe.h>
#include <fg_bq27441.h>
#include "fg_bq27441_device.h"

void Control()
{
}

float Temperature()
{
    uint8_t data[2] = { 0 };

    i2c_write(I2C1, BQ27441_ADDR, (uint8_t[]) {
              COMMAND_TEMP}, 1, 0);

    i2c_read(I2C1, BQ27441_ADDR, data, sizeof(data));
    return (((data[1] << 8) + data[0]) / 100.0);
}

float Voltage()
{
    uint8_t data[2] = { 0 };

    i2c_write(I2C1, BQ27441_ADDR, (uint8_t[]) {
              COMMAND_VOLTAGE}, 1, 0);

    i2c_read(I2C1, BQ27441_ADDR, data, sizeof(data));
    return (((data[1] << 8) + data[0]) / 1000.0);
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
    uint8_t data[2] = { 0 };

    i2c_write(I2C1, BQ27441_ADDR, (uint8_t[]) {
              COMMAND_AVG_CURRENT}, 1, 0);

    i2c_read(I2C1, BQ27441_ADDR, data, sizeof(data));
    return (((data[1] << 8) + data[0]) / 1000.0);
}

float StandbyCurrent()
{
    uint8_t data[2] = { 0 };

    i2c_write(I2C1, BQ27441_ADDR, (uint8_t[]) {
              COMMAND_STDBY_CURRENT}, 1, 0);

    i2c_read(I2C1, BQ27441_ADDR, data, sizeof(data));
    return (((data[1] << 8) + data[0]) / 1000.0);
}

float MaxLoadCurrent()
{
    uint8_t data[2] = { 0 };

    i2c_write(I2C1, BQ27441_ADDR, (uint8_t[]) {
              COMMAND_MAX_CURRENT}, 1, 0);

    i2c_read(I2C1, BQ27441_ADDR, data, sizeof(data));
    return (((data[1] << 8) + data[0]) / 1000.0);
}

float AveragePower()
{
    uint8_t data[2] = { 0 };

    i2c_write(I2C1, BQ27441_ADDR, (uint8_t[]) {
              COMMAND_AVG_POWER}, 1, 0);

    i2c_read(I2C1, BQ27441_ADDR, data, sizeof(data));
    return (((data[1] << 8) + data[0]) / 1000.0);
}

void StateOfCharge()
{
}

float InternalTemperature()
{
    uint8_t data[2] = { 0 };

    i2c_write(I2C1, BQ27441_ADDR, (uint8_t[]) {
              COMMAND_INT_TEMP}, 1, 0);

    i2c_read(I2C1, BQ27441_ADDR, data, sizeof(data));
    return (((data[1] << 8) + data[0]) / 100.0);
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
