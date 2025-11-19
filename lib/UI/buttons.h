/*
 * buttons.h
 *
 *  Created on: Jun 25, 2021
 *      Author: Biren
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "capsense.h"
// Configuration
#include "hV_Configuration.h"

#ifdef __cplusplus
extern "C" {
#endif
    
#include <stdint.h>
    
#define BTN_MASK_PRS(x) (x)

void buttons_init();
uint8_t buttons_probe();
uint8_t buttons_get_events();
uint8_t button_scan();
uint8_t buttons_check();
uint8_t is_buttons_event_updated(void);
uint32_t get_time_since_last_interaction();
void reset_button_tp();

#ifdef __cplusplus
} // closing brace for extern "C"
#endif

#endif /* BUTTONS_H_ */
