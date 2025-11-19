/*
 * home.h
 *
 *  Created on: Oct 28, 2015
 *      Author: andrei
 */

#ifndef UI_HOME_H_
#define UI_HOME_H_

// Configuration
#include "hV_Configuration.h"

typedef enum
{
    EDIT_TYPE_SETPOINT_TEMP = 1,
    EDIT_TYPE_TEMP_CAL,
    EDIT_TYPE_HUM_CAL,
    EDIT_TYPE_CO2_CAL,
    EDIT_TYPE_ZONE_HYSTERESIS,
    EDIT_COOLING_HUMIDITY,
    EDIT_FANCOIL_MODE,
    EDIT_FANCOIL_HYSTERESIS,
    EDIT_MB_BAUDRATE,
    EDIT_SELECTED_ZONES,
}edit_type;

// 0.1 °C
#define EDIT_TEMP_MIN		50
#define EDIT_TEMP_MAX		300
#define EDIT_TEMP_STEP		5

// 1 ppm
#define EDIT_CO2_MIN		450
#define EDIT_CO2_MAX		2000
#define EDIT_CO2_STEP		10

// 0.1 %
#define EDIT_HUM_MIN		50
#define EDIT_HUM_MAX		950
#define EDIT_HUM_STEP		10

#define EDIT_TEMP_CALIBRATION_MIN	-50
#define EDIT_TEMP_CALIBRATION_MAX	 50
#define EDIT_TEMP_CALIBRATION_STEP	 1

#define FAN_LEVEL_X 190
#define FAN_LEVEL_Y 35

#define EDIT_FLASH_INTERVAL_MS 500
#define EDIT_FLASH_HOLD_OFF_MS 500

#define EDIT_COOLING_HUM_MIN	 	0
#define EDIT_COOLING_HUM_MAX	 	1000
#define EDIT_COOLING_HUM_STEP		50

#define EDIT_HYSTERESIS_MIN		 	0
#define EDIT_HYSTERESIS_MAX	 		30
#define EDIT_HYSTERESIS_STEP	 	1

#define EDIT_FANCOIL_MODE_MIN		0
#define EDIT_FANCOIL_MODE_MAX	 	4
#define EDIT_FANCOIL_MODE_STEP	 	1

#define EDIT_FANCOIL_HYSTERESIS_MIN		0
#define EDIT_FANCOIL_HYSTERESIS_MAX		30
#define EDIT_FANCOIL_HYSTERESIS_STEP	1

#define EDIT_MB_BAUDRATE_MIN		0
#define EDIT_MB_BAUDRATE_MAX	 	BAUDRATE_MAX-1
#define EDIT_MB_BAUDRATE_STEP	 	1

#define EDIT_ZONE_SEL_MIN		1
#define EDIT_ZONE_SEL_MAX	 	MAX_ZONE
#define EDIT_ZONE_SEL_STEP	 	1

enum {
	HOME_AUTO,
	HOME_MAN,
	HOME_BUILDING,
	HOME_AWAY,
	HOME_BOOST,
	HOME_OVERPRS,
	HOME_VENT,
	HOME_UNKNOWN,
};
typedef struct edit_item_s {
	int32_t value;
	int32_t min;
	int32_t max;
	int32_t step;
	struct edit_item_s* next;
} edit_item_t;

struct edit_s{
	edit_item_t temp;
	edit_item_t hum;
	edit_item_t co2;
	edit_item_t temp_calb;
	edit_item_t zone_hysteresis;
	edit_item_t cooling_hum;
	edit_item_t fancoil_mode_edit;
	edit_item_t fancoil_hysteresis_edit;
	edit_item_t mb_baudrate;
	edit_item_t selected_zone;
	edit_item_t* current;
	uint32_t ts;
	uint8_t flash;
	uint8_t type;
};
extern struct edit_s edit;

extern uint8_t edit_selection_zone;

void home_render();

int8_t home_edit_handle(uint32_t e);
void home_edit_render();
int8_t home_edit_start(uint32_t e, edit_type type);
void home_edit_finish(uint32_t event);

#endif /* UI_HOME_H_ */
