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
#ifndef pwm_pca9685_regrw_h
#define pwm_pca9685_regrw_h

#include <ctype.h>
struct pwm_instance {
    I2C_TypeDef *bus;           /* HW-bus: I2C0-I2Cn */
    uint8_t addr;               /* 7-bit write address - read-address implicit */
    struct registers_t *registers;  /* Copy of pca9685 registers or NULL if not in sync */
};
typedef struct pwm_instance *pwm_hndl;

uint8_t reg_read_uint8(pwm_hndl pwm, uint8_t reg);
uint16_t reg_read_uint16(pwm_hndl pwm, uint8_t reg);
uint32_t reg_read_uint32(pwm_hndl pwm, uint8_t reg);
void reg_write_uint8(pwm_hndl pwm, uint8_t reg, uint8_t val);
void reg_write_uint16(pwm_hndl pwm, uint8_t reg, uint16_t val);
void reg_write_uint32(pwm_hndl pwm, uint8_t reg, uint32_t val);


#endif //pwm_pca9685_regrw_h
