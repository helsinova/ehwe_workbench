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
#ifndef pwm_pca9685_h
#define pwm_pca9685_h

/* Not strictly true as we will use Nordics higher-level API.
 * Include is a work-around for EHWE not separating them well enough. This
 * will change and might require a change of the include (TBD) */
#include <stm32f10x.h>

/* Forward declaration of instance-type hides details and enforces use of
 * API */
typedef struct pwm_instance *pwm_hndl;

/* 12-bit PWM settings for on- and off-times*/
struct pwm_val {
    uint16_t on_cntr;
    uint16_t off_cntr;
};

pwm_hndl pwm_pca9685_create(I2C_TypeDef * bus);
void pwm_pca9685_destruct(pwm_hndl);
void pwm_pca9685_init(pwm_hndl);
void pwm_pca9685_all_swreset(I2C_TypeDef * bus);
void pwm_pca9685_test(pwm_hndl);

/* Access API of individual PWM:s*/
#define ALL_PWM -1
void pwm_pca9685_set(pwm_hndl, uint8_t index, struct pwm_val);
struct pwm_val pwm_pca9685_get(pwm_hndl, uint8_t index);

#ifdef EHWE
#include "embedded_config.h"
#define main embedded_main
#endif

#endif
