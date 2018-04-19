/***************************************************************************
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
#ifndef fg_bq27441_device_h
#define fg_bq27441_device_h

/*
 * Commands
 */
#define COMMAND_CONTROL         0x00 /* Control */
#define COMMAND_TEMP            0x02 /* Temperature */
#define COMMAND_VOLTAGE         0x04 /* Voltage */
#define COMMAND_FLAGS           0x06 /* Flags */
#define COMMAND_NOM_CAPACITY    0x08 /* NominalAvailableCapacity */
#define COMMAND_AVAIL_CAPACITY  0x0A /* FullAvailableCapacity */
#define COMMAND_REM_CAPACITY    0x0C /* RemainingCapacity */
#define COMMAND_FULL_CAPACITY   0x0E /* FullChargeCapacity */
#define COMMAND_AVG_CURRENT     0x10 /* AverageCurrent */
#define COMMAND_STDBY_CURRENT   0x12 /* StandbyCurrent */
#define COMMAND_MAX_CURRENT     0x14 /* MaxLoadCurrent */
#define COMMAND_AVG_POWER       0x18 /* AveragePower */
#define COMMAND_SOC             0x1C /* StateOfCharge */
#define COMMAND_INT_TEMP        0x1E /* InternalTemperature */
#define COMMAND_SOH             0x20 /* StateOfHealth */
#define COMMAND_REM_CAP_UNFL    0x28 /* RemainingCapacityUnfiltered */
#define COMMAND_REM_CAP_FIL     0x2A /* RemainingCapacityFiltered */
#define COMMAND_FULL_CAP_UNFL   0x2C /* FullChargeCapacityUnfiltered */
#define COMMAND_FULL_CAP_FIL    0x2E /* FullChargeCapacityFiltered */
#define COMMAND_SOC_UNFL        0x30 /* StateOfChargeUnfiltered */
/*
 * Control sub-commands
 */
#define CONTROL_STATUS          0x00 /* Reports the status of device.*/
#define CONTROL_DEVICE_TYPE     0x01 /* Reports the device type (0x0421).*/
#define CONTROL_FW_VERSION      0x02 /* Reports the firmware version of the device.*/
#define CONTROL_DM_CODE         0x04 /* Reports the Data Memory Code number stored in
                                        NVM.*/
#define CONTROL_PREV_MACWRITE   0x07 /* Returns previous MAC command code.*/
#define CONTROL_CHEM_ID         0x08 /* Reports the chemical identifier of the battery
                                        profile used by the fuel gauge.*/
#define CONTROL_BAT_INSERT      0x0C /* Forces the Flags() [BAT_DET] bit set when the
                                        OpConfig [BIE] bit is 0.*/
#define CONTROL_BAT_REMOVE      0x0D /* Forces the Flags() [BAT_DET] bit clear when
                                        the OpConfig [BIE] bit is 0.*/
#define CONTROL_SET_HIBERNATE   0x11 /* Forces CONTROL_STATUS [HIBERNATE] bit to 1.*/
#define CONTROL_CLEAR_HIBERNATE 0x12 /* Forces CONTROL_STATUS [HIBERNATE] bit to 0.*/
#define CONTROL_SET_CFGUPDATE   0x13 /* Forces Flags() [CFGUPMODE] bit to 1 and gauge
                                        enters CONFIG UPDATE mode.*/
#define CONTROL_SHUTDOWN_ENABLE 0x1B /* Enables device SHUTDOWN mode.*/
#define CONTROL_SHUTDOWN        0x1C /* Commands the device to enter SHUTDOWN mode.*/
#define CONTROL_SEALED          0x20 /* Places the device in SEALED access mode.*/
#define CONTROL_TOGGLE_GPOUT    0x23 /* Commands the device to toggle the GPOUT pin
                                        for 1 ms.*/
#define CONTROL_RESET           0x41 /* Performs a full device reset.*/
#define CONTROL_SOFT_RESET      0x42 /* Gauge exits CONFIG UPDATE mode.*/
#define CONTROL_EXIT_CFGUPDATE  0x43 /* Exits CONFIG UPDATE mode without an OCV
                                        measurement and without resimulating to
                                        update StateOfCharge().*/
#define CONTROL_EXIT_RESIM      0x44 /* Exits CONFIG UPDATE mode without an OCV
                                        measurement and resimulates with the updated
                                        configuration data to update*/
#endif
