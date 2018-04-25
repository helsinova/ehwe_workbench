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

#include <stdint.h>
#include <ctype.h>

void pm_init(void);
void pm_deinit(void);

//void clear_faults();
//void restore_default_all();
uint8_t capability();
//void iout_oc_warn_limit();
//void vin_ov_warn_limit();
//void vin_uv_warn_limit();
//void pin_op_warn_limit();
//void status();
//void status_word();
//void status_iout();
//void status_input();
//void status_cml();
//void status_mfr_specific();
//void read_ein();
double read_vin();
//void read_in();
//void read_vout();
//void read_iout();
//void read_pout();
//void read_pin();
uint16_t mfr_id();
void mfr_model(char model[6]);
uint16_t mfr_revision();
uint16_t get_mfr_adc_config();
void set_mfr_adc_config(uint16_t);
//void mfr_read_vshunt();
//void mfr_alert_mask();
//void mfr_calibration();
//void mfr_device_config();
//void clear_ein();
int16_t ti_mfr_id();
int16_t ti_mfr_model();
int16_t ti_mfr_revision();

#endif
