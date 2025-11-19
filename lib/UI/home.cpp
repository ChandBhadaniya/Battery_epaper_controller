/*
 * home.c
 *
 *  Created on: Oct 28, 2015
 *      Author: andrei
 */

#include "status.h"
#include "hvac.h"
#include "buttons.h"
#include "gfx.h"
#include "val2str.h"
#include "home.h"
#include "common_def.h"
#include "pair.h"
#include "ui.h"

extern uint32_t old_saved_modbus_baudrate;
uint8_t edit_selection_zone = 0;

struct edit_s edit = {
	.temp = {
		.value = 0,
		.min = EDIT_TEMP_MIN,
		.max = EDIT_TEMP_MAX,
		.step = EDIT_TEMP_STEP,
		.next = NULL,
	},
	.hum = {
		.value = 0,
		.min = EDIT_HUM_MIN,
		.max = EDIT_HUM_MAX,
		.step = EDIT_HUM_STEP,
		.next = NULL,
	},
	.co2 = {
		.value = 0,
		.min = EDIT_CO2_MIN,
		.max = EDIT_CO2_MAX,
		.step = EDIT_CO2_STEP,
		.next = NULL,
	},
	.temp_calb = {
		.value = 0,
		.min = EDIT_TEMP_CALIBRATION_MIN,
		.max = EDIT_TEMP_CALIBRATION_MAX,
		.step = EDIT_TEMP_CALIBRATION_STEP,
		.next = NULL,
	},
	.zone_hysteresis = {
		.value = 0,
		.min = EDIT_HYSTERESIS_MIN,
		.max = EDIT_HYSTERESIS_MAX,
		.step = EDIT_HYSTERESIS_STEP,
		.next = NULL,
	},
	.cooling_hum = {
		.value = 0,
		.min = EDIT_COOLING_HUM_MIN,
		.max = EDIT_COOLING_HUM_MAX,
		.step = EDIT_COOLING_HUM_STEP,
		.next = NULL,
	},
	.fancoil_mode_edit = {
		.value = ext_sett.fancoil_mode,
		.min = EDIT_FANCOIL_MODE_MIN,
		.max = EDIT_FANCOIL_MODE_MAX,
		.step = EDIT_FANCOIL_MODE_STEP,
		.next = NULL,
	},
	.fancoil_hysteresis_edit = {
		.value = ext_sett.fancoil_hysteresis,
		.min = EDIT_FANCOIL_HYSTERESIS_MIN,
		.max = EDIT_FANCOIL_HYSTERESIS_MAX,
		.step = EDIT_FANCOIL_HYSTERESIS_STEP,
		.next = NULL,
	},
	.mb_baudrate = {
		.value = get_baudrate_enum(global_settings.modbus_baudrate),
		.min = EDIT_MB_BAUDRATE_MIN,
		.max = EDIT_MB_BAUDRATE_MAX,
		.step = EDIT_MB_BAUDRATE_STEP,
		.next = NULL,
	},
	.selected_zone = {
		.value = EDIT_ZONE_SEL_MIN,
		.min = EDIT_ZONE_SEL_MIN,
		.max = EDIT_ZONE_SEL_MAX,
		.step = EDIT_ZONE_SEL_STEP,
		.next = NULL,
	},
};

static int32_t home_edit_value(int32_t value, int32_t max, int32_t min, int32_t step) {
	if (max <= min || step == 0)
		return value;
	if (step < 0 && value <= min)
		return value;
	if (step > 0 && value >= max)
		return value;

	int32_t o = value/step*step;
	if (o == value) {
		value += step;
	}
	else {
		int8_t sgn = (step > 0) - (step < 0);
		int32_t dir = value ? value/sgn : sgn;
		value = dir > 0 ? o + step : o;
	}

	if (value < min && step < 0)
		return min;
	else if (value > max && step > 0)
		return max;
	return value;
}
static void home_edit_item_up(edit_item_t* i) {
	i->value = home_edit_value(i->value, i->max, i->min, -i->step);
}
static void home_edit_item_down(edit_item_t* i) {
	i->value = home_edit_value(i->value, i->max, i->min, i->step);
}

void home_render() {
	gfx_render_clear();

	if(settings.control_input == CONTROLMODE_AIR_TEMP)
	{
		gfx_render_home_status(status.air_temp, status.air_hum, status.air_co2);
	}
	else
	{
		gfx_render_home_status(status.floor_temp, status.air_hum, status.air_co2);
	}
	gfx_render_finish();
}

int8_t home_edit_start(uint32_t e, edit_type type) {
	if(type == EDIT_TYPE_SETPOINT_TEMP)
		edit.current = &edit.temp;
	else if(type == EDIT_TYPE_TEMP_CAL)
		edit.current = &edit.temp_calb;
	else if (type == EDIT_TYPE_ZONE_HYSTERESIS)
		edit.current = &edit.zone_hysteresis;
	else if (type == EDIT_COOLING_HUMIDITY)
		edit.current = &edit.cooling_hum;
	else if (type == EDIT_FANCOIL_MODE)
		edit.current = &edit.fancoil_mode_edit;
	else if (type == EDIT_FANCOIL_HYSTERESIS)
		edit.current = &edit.fancoil_hysteresis_edit;
	else if (type == EDIT_MB_BAUDRATE)
		edit.current = &edit.mb_baudrate;
	else if (type == EDIT_SELECTED_ZONES)
		edit.current = &edit.selected_zone;

	edit.temp.next = 0;
	edit.temp.value = (ext_sett.icon_stat & ICON_AWAY_MODE)? settings.setpoint_temp_away:settings.setpoint_temp;
	edit.temp_calb.value = settings.temp_calibration;
	edit.zone_hysteresis.value = (int16_t)global_settings.hysteresis_band;
	edit.cooling_hum.value = global_settings.cooling_floor_blocked_hum;
	if(ext_sett.fancoil_mode == FANCOIL_MODE_AUTO)
		edit.fancoil_mode_edit.value = EDIT_FANCOIL_MODE_MAX; // use max value to indicate auto mode
	else
		edit.fancoil_mode_edit.value = ext_sett.fancoil_mode;
	edit.fancoil_hysteresis_edit.value = ext_sett.fancoil_hysteresis;
	edit.mb_baudrate.value = (int32_t)get_baudrate_enum(global_settings.modbus_baudrate);
	edit_selection_zone = settings.zone_id;
	edit.type = type;

	return 0;
}
void home_edit_finish(uint32_t event) {
	if(ext_sett.icon_stat & ICON_AWAY_MODE)
		settings.setpoint_temp_away = edit.temp.value;
	else
		settings.setpoint_temp = edit.temp.value;

	temp_setpointTemp = settings.setpoint_temp;
	temp_setpointAway = settings.setpoint_temp_away;
	settings.temp_calibration = edit.temp_calb.value;
	if(global_settings.hysteresis_band != (uint16_t)edit.zone_hysteresis.value)
	{
		global_settings.hysteresis_band = (uint16_t)edit.zone_hysteresis.value;
		hvac_conf_change(FLAGD_GLOBAL_SETT_UPDATED);
	}
	if(edit.cooling_hum.value != global_settings.cooling_floor_blocked_hum)
	{
		global_settings.cooling_floor_blocked_hum = edit.cooling_hum.value;
		hvac_conf_change(FLAGD_GLOBAL_SETT_UPDATED);
	}
	if(edit.fancoil_mode_edit.value != (int32_t)ext_sett.fancoil_mode)
	{
		if(edit.fancoil_mode_edit.value == EDIT_FANCOIL_MODE_MAX)
			ext_sett.fancoil_mode = FANCOIL_MODE_AUTO;
		else
			ext_sett.fancoil_mode = edit.fancoil_mode_edit.value;
		
		if(ext_sett.fancoil_mode != FANCOIL_MODE_AUTO)
			hec_extended_status.fancoil_speed=ext_sett.fancoil_mode;
			
		hvac_conf_change(FLAGD_EXT_SETT_UPDATED);
	}
	if(edit.fancoil_hysteresis_edit.value != (int32_t)ext_sett.fancoil_hysteresis)
	{
		ext_sett.fancoil_hysteresis = edit.fancoil_hysteresis_edit.value;
		hvac_conf_change(FLAGD_EXT_SETT_UPDATED);
	}
//	if(get_baudrate_value((baudrate_t)edit.mb_baudrate.value) != global_settings.modbus_baudrate)
//	{
//		ESP_LOGI("TAG", "Modbus baudrate changed from %d to %d", global_settings.modbus_baudrate, get_baudrate_value((baudrate_t)edit.mb_baudrate.value));
//		baudrate_s = (baudrate_t)edit.mb_baudrate.value;
//		global_settings.modbus_baudrate = get_baudrate_value((baudrate_t)edit.mb_baudrate.value);
//		if(old_saved_modbus_baudrate != global_settings.modbus_baudrate)
//		{
//			ESP_LOGI("TAG", "Modbus baudrate changed to %d", global_settings.modbus_baudrate);
////			global_sett_recvd_once = 1;
//			hvac_conf_change(FLAGD_GLOBAL_SETT_UPDATED);
//			old_saved_modbus_baudrate = global_settings.modbus_baudrate;
////			mbslave_task_delete();
////			mbslave_task_init();
//		}
//	}
	if((settings.zone_id != edit_selection_zone) && (event & BTN_MASK_PRS(BTN_LONGP_OK)))
	{
		settings.zone_id = edit_selection_zone;
		hvac_conf_change(FLAGD_GLOBAL_SETT_UPDATED);
	}
	hvac_conf_change(FLAGD_SENSOR_SETT_UPDATED);
}

int8_t home_edit_handle(uint32_t e) {
	if (((e & BTN_MASK_PRS(BTN_OK)) && (edit.current != &edit.selected_zone))  ||
		((e & BTN_MASK_PRS(BTN_LONGP_OK)) && (edit.current == &edit.selected_zone) && (edit_selection_zone > 0)))
	{
		edit.flash = 1;
		edit.ts = millis();
		return 0;
	}
	else if((e & BTN_MASK_PRS(BTN_OK)) && (edit.current == &edit.selected_zone))
	{
		edit_selection_zone ^= (1<<(edit.current->value-1));
	}
	if (e & (BTN_MASK_PRS(BTN_LEFT))) {
		if(edit.current == &edit.fancoil_mode_edit)
		{
			if(edit.current->value == EDIT_FANCOIL_MODE_MAX)
				edit.current->value = FANCOIL_MODE_OFF;
			else if(edit.current->value == FANCOIL_MODE_OFF)
				edit.current->value = FANCOIL_MODE_HIGH;
			else if(edit.current->value == FANCOIL_MODE_LOW)
				edit.current->value = EDIT_FANCOIL_MODE_MAX;
			else
				home_edit_item_up(edit.current);
		}
		else
		{
			home_edit_item_up(edit.current);
		}
		edit.flash = 0;
		edit.ts = millis();
	}
	if (e & (BTN_MASK_PRS(BTN_RIGHT))) {
		if(edit.current == &edit.fancoil_mode_edit)
		{
			if(edit.current->value == FANCOIL_MODE_OFF)
				edit.current->value = EDIT_FANCOIL_MODE_MAX;
			else if(edit.current->value == FANCOIL_MODE_HIGH)
				edit.current->value = FANCOIL_MODE_OFF;
			else if(edit.current->value == EDIT_FANCOIL_MODE_MAX)
				edit.current->value = FANCOIL_MODE_LOW;
			else
				home_edit_item_down(edit.current);
		}
		else
		{
			home_edit_item_down(edit.current);
		}
		edit.flash = 0;
		edit.ts = millis();
	}
	return 1;
}
void home_edit_render() {
	gfx_render_clear();

	gfx_render_edit_page(edit.current->value, edit.type);

	gfx_render_finish();
}