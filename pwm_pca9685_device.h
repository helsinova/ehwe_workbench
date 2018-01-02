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

#define DEVREGS_CONTAIN_RESERVED_REGS   /* Used as padding to map device reg-map
                                           exactly if complete reg-dups are 
                                           faster */

struct devregs {
    union {
        union {
            struct {
                reg_mode1_t mode1;
                reg_mode2_t mode2;
                uint8_t subadr1;
                uint8_t subadr2;
                uint8_t subadr3;
                uint8_t allcalladdr;
                reg_pwm_t pwm_cntr[16];
            };
            uint8_t regs_part1[70];
        };

#ifdef DEVREGS_CONTAIN_RESERVED_REGS
        uint8_t regs_reserved[180];
#endif

        union {
            struct {
                reg_pwm_t allpwm_cntr;
                uint8_t prescale;
                uint8_t testmode;
            };
            uint8_t regs_part2[6];
        };
        uint8_t barray[256];
    };
};

/* Device specific functions */
reg_mode1_t get_mode1(pwm_hndl);
reg_mode2_t get_mode2(pwm_hndl);
reg_pwm_t get_pwm(pwm_hndl pwm, uint8_t idx);

void set_mode1(pwm_hndl, reg_mode1_t);
void set_mode2(pwm_hndl, reg_mode2_t);
void set_pwm(pwm_hndl, uint8_t idx, reg_pwm_t val);

void regs_sync(pwm_hndl);
void set_pwm_freq(pwm_hndl, unsigned int freq);
unsigned int get_pwm_freq(pwm_hndl);

#endif                          //pwm_pca9685_device_h
