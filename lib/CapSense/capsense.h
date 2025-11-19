/*
 * buttons.h
 *
 *  Created on: Jun 25, 2021
 *      Author: Biren
 */

#ifndef CAPSENSE_H_
#define CAPSENSE_H_

#include "common_def.h"

typedef enum 
{
	BTN_NONE 	= 0x00,
	BTN_BACK 	= 0x10,
	BTN_LEFT 	= 0x20,
	BTN_OK 		= 0x40,
	BTN_RIGHT 	= 0x80,
	BTN_ONOFF 	= 0x08,
	NUM_BTNS
}buttons_e;

typedef enum{
	BTN_LONGP_NONE 	 	= 0x00,
	BTN_LONGP_BACK  	= 256,
	BTN_LONGP_LEFT		= 512,
	BTN_LONGP_OK	 	= 1024,
	BTN_LONGP_RIGHT  	= 2048,
	BTN_LONGP_LEFTRIGHT	= 4096,
}buttons_longpress_e;

#define TOUCH_BUTTON_NUM    			5

void touch_button_read_task_init(void);
uint8_t capsense_get_touch(void);
void capsense_clear_touch();
void tp_example_set_thresholds(void);

#endif /* CAPSENSE_H_ */
