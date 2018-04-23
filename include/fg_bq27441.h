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
#ifndef fg_bq27441_h
#define fg_bq27441_h

void fuelguage_init(void);
void fuelguage_deinit(void);

//void Control();
float Temperature();
float Voltage();
//void Flags();
//void NominalAvailableCapacity();
//void FullAvailableCapacity();
//void RemainingCapacity();
//void FullChargeCapacity();
float AverageCurrent();
float StandbyCurrent();
float MaxLoadCurrent();
float AveragePower();
//void StateOfCharge();
float InternalTemperature();
//void StateOfHealth();
//void RemainingCapacityUnfiltered();
//void RemainingCapacityFiltered();
//void FullChargeCapacityUnfiltered();
//void FullChargeCapacityFiltered();
//void StateOfChargeUnfiltered();

#endif
