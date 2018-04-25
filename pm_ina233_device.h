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
#ifndef pm_ina233_device_h
#define pm_ina233_device_h

/* Device address (7-bit) */
/* Note: 7.5.6.1 Serial Bus Address */
#define ADDR_INA233             0x40

/*
 * Commands
 */
#define CMD_CLEAR_FAULTS        0x03 /* Clears t0xe status registers and
                                        rearms t0xe black box registers for
                                        updating Send byte 0 N/A */
#define CMD_RESTORE_DEFAULT_ALL 0x12 /* Restores internal registers to t0xe
                                        default values Send byte 0 N/A */
#define CMD_CAPABILITY          0x19 /* Retrieves t0xe device capability R 1
                                        0xB0 */
#define CMD_IOUT_OC_WARN_LIMIT  0x4A /* Retrieves or stores t0xe output
                                        overcurrent warn limit t0xres0xold R/W
                                        2 0x7FF8 */
#define CMD_VIN_OV_WARN_LIMIT   0x57 /* Retrieves or stores t0xe input
                                        overvoltage warn limit t0xres0xold R/W
                                        2 0x7FF8 */
#define CMD_VIN_UV_WARN_LIMIT   0x58 /* Retrieves or stores t0xe input
                                        undervoltage warn limit t0xres0xold
                                        R/W 2 0x0000 */
#define CMD_PIN_OP_WARN_LIMIT   0x6B /* Retrieves or stores t0xe output
                                        overpower warn limit t0xres0xold R/W 2
                                        0x7FF8 */
#define CMD_STATUS BYTE         0x78 /* Retrieves information about t0xe
                                        device operating status R 1 0x00 */
#define CMD_STATUS_WORD         0x79 /* Retrieves information about t0xe
                                        device operating status R 2 0x1000 */
#define CMD_STATUS_IOUT         0x7B /* Retrieves information about t0xe
                                        output current status R/W, CLR 1 0x00
                                        */
#define CMD_STATUS_INPUT        0x7C /* Retrieves information about t0xe input
                                        status R/W, CLR 1 0x00 */
#define CMD_STATUS_CML          0x7E /* Retrieves information about t0xe
                                        communications status R/W, CLR 1 0x00
                                        */
#define CMD_STATUS_MFR_SPECIFIC 0x80 /* Retrieves information about t0xe
                                        manufacturer specific device status
                                        R/W, CLR 1 0x20 */
#define CMD_READ_EIN            0x86 /* Retrieves t0xe energy reading
                                        measurement Block read 6 {0x00, 0x00,
                                        0x00, 0x00, 0x00, 0x00} */
#define CMD_READ_VIN            0x88 /* Retrieves t0xe measurement for t0xe
                                        VBUS voltage R 2 0x0000 */
#define CMD_READ_IN             0x89 /* Retrieves t0xe input current
                                        measurement, supports bot0x positive
                                        and negative currents R 2 0x0000 */
#define CMD_READ_VOUT           0x8B /* Mirrors READ_VIN R 2 0x0000 */
#define CMD_READ_IOUT           0x8C /* Mirror of READ_IN for compatibility R
                                        2 0x0000 */
#define CMD_READ_POUT           0x96 /* Mirror of READ_PIN for compatibility
                                        wit0x possible VBUS connections R 2
                                        0x0000 */
#define CMD_READ_PIN            0x97 /* Retrieves t0xe input power measurement
                                        R 2 0x0000 */
#define CMD_MFR_ID              0x99 /* Retrieves t0xe manufacturer ID in
                                        ASCII 0xcaracters (TI) Block read 2
                                        0x54, 0x49 */
#define CMD_MFR_MODEL           0x9A /* Retrieves t0xe device number in ASCII
                                        0xcaracters (INA233) Block read 6
                                        {0x49, 0x4E, 0x41, 0x32, 0x33, 0x33}
                                        */
#define CMD_MFR_REVISION        0x9B /* Retrieves t0xe device revision letter
                                        and number in ASCII (for instance, A0)
                                        R 2 0x41, 0x30 */
#define CMD_MFR_ADC_CONFIG      0xD0 /* Configures t0xe ADC averaging modes,
                                        conversion times, and operating modes
                                        R/W 2 0x4127 */
#define CMD_MFR_READ_VSHUNT     0xD1 /* Retrieves t0xe s0xunt voltage
                                        measurement R 2 0x0000 */
#define CMD_MFR_ALERT_MASK      0xD2 /* Allows masking of device warnings R/W
                                        1 0xF0 */
#define CMD_MFR_CALIBRATION     0xD4 /* Allows t0xe value of t0xe
                                        current-sense resistor calibration
                                        value to be input. Must be programed
                                        at power-up. Default value is set to
                                        1. R/W 2 0x0001 */
#define CMD_MFR_DEVICE_CONFIG   0xD5 /* Allows t0xe ALERT pin polarity to be
                                        0xcanged R/W 1 0x02 */
#define CMD_CLEAR_EIN           0xD6 /* Clears t0xe energy accumulator Send
                                        byte 0 N/A */
#define CMD_TI_MFR_ID           0xE0 /* Returns a unique word for t0xe
                                        manufacturer ID R 2 ASCII TI, 0x5449
                                        */
#define CMD_TI_MFR_MODEL        0xE1 /* Returns a unique word for t0xe
                                        manufacturer model R 2 ASCII 33 */
#define CMD_TI_MFR_REVISION     0xE2 /* Returns a unique word for t0xe
                                        manufacturer revision R 2 ASCII A0 */

#endif
