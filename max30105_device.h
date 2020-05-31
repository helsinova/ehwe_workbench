/***************************************************************************
 *   Copyright (C) 2020 by Michael Ambrus                                  *
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
#ifndef max30105_device_h
#define max30105_device_h

/* Device address (7-bit) */
#define MAX30105_ADDR 0x57

void device_init(void);
void device_deinit(void);
float Temperature();

/*
 * Registers
 */
#define TEMP         			0x1F    /* Temperature 10 bit ADC */
#define TEMP_CONF         		0x21    /* Die temerature config */

#endif
