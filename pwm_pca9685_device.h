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
#ifndef pwm_pca9685_device_h
#define pwm_pca9685_device_h
/*
 * pwm_pca9685 device tier
 */

#include "pwm_pca9685_regdefs.h"
#include "pwm_pca9685_regrw.h"

/* Device specific functions */
reg_mode1_t get_mode1(pwm_hndl pwm);
reg_mode2_t get_mode2(pwm_hndl pwm);
void set_mode1(pwm_hndl pwm, reg_mode1_t val);
void set_mode2(pwm_hndl pwm, reg_mode2_t val);
void set_pwm_freq(pwm_hndl pwm, unsigned int freq);
unsigned int get_pwm_freq(pwm_hndl pwm);

#endif                          //pwm_pca9685_device_h
