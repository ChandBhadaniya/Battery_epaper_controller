/*
 * ui.c
 *
 *  Created on: Jun 25, 2021
 *      Author: Biren
 */

#include <stddef.h>
#include "stringcnv.h"
//#include "factoryconfig.h"
#include "status.h"
#include "buttons.h"
#include "hvac.h"
#include "gfx.h"
#include "val2str.h"
#include "menu.h"
#include "home.h"
#include "hsm.h"
#include "ui.h"
#include "common_def.h"
#include "settings.h"
#include "pair.h"
//#include "esp_log.h"
//#include "relay.h"
#include "font_render.h"
#include "ui_text.h"
#include "disp.h"

#define MENU_TIMEOUT_MS (20 * 1000)
#define EDIT_TIMEOUT_MS (20 * 1000)
#define PAIR_TIMEOUT_MS (6 * 60  * 1000)

uint8_t ipaddr_changed = 0;
uint8_t is_installer_menu_unlock = 0;
uint32_t last_error_skip_ts = 0;

baudrate_t baudrate_s = BAUDRATE_115200;

static menu_t *ui_toggle_baudrate(menu_t *menu, uint32_t event)
{
	if (!(event & (BTN_MASK_PRS(BTN_OK))))
		return menu;

	baudrate_s = (baudrate_t)((baudrate_s + 1) % BAUDRATE_MAX);

	return menu;
}

static const char *get_baudrate_string()
{
	return text_baudrate_s[get_baudrate_enum(global_settings.modbus_baudrate)][lang]; // Clear the string
}

static const char *get_zone_selection()
{
	return text_common[UI_SELECT][lang]; // Clear the string
}

uint32_t get_baudrate_value(baudrate_t baud_enum)
{
	switch (baud_enum) {
		case BAUDRATE_19200: return BAUDRATE_V_19200;
		case BAUDRATE_115200: return BAUDRATE_V_115200;
		case BAUDRATE_921600: return BAUDRATE_V_921600;
		default: return BAUDRATE_V_115200; // default fallback
	}
}

baudrate_t get_baudrate_enum(uint32_t baudrate)
{
	switch (baudrate) {
		case 19200:   return BAUDRATE_19200;
		case 115200:  return BAUDRATE_115200;
		case 921600:  return BAUDRATE_921600;
		default:      return BAUDRATE_115200; // default fallback
	}
}

uint8_t is_valid_baudrate(uint32_t baudrate)
{
    baudrate_t baud_enum = get_baudrate_enum(baudrate);
    return ((baud_enum < BAUDRATE_MAX) && (get_baudrate_value(baud_enum) == baudrate))?1:0;
}

uint8_t is_floor_cooling_application_true()
{
	return ((global_settings.cooling_application == COOLING_FLOOR_ONLY) || (global_settings.cooling_application == COOLING_FLOOR_AND_FANCOIL))?1:0;
}

static const char *ui_controller_ucfw_uchw()
{
	static char text_fw[10], text_hw[10], text[20];
	ver2str(dev_config.fw_ver, text_fw, sizeof(text_fw));
	ver2str(dev_config.hw_ver, text_hw, sizeof(text_hw));
	sprintf(text, "%s (%s)", text_fw, text_hw);
	return text;
}

static const char *ui_hec_ucfw_uchw()
{
	static char text_fw[10], text_hw[10], text[20];
	ver2str(status.fw_ver, text_fw, sizeof(text_fw));
	ver2str(status.hw_ver, text_hw, sizeof(text_hw));
	sprintf(text, "%s (%s)", text_fw, text_hw);
	return text;
}

static const char *ui_abt_get_zone_name()
{
	if(settings.name[0] == '\0')
	{
		return "-";
	}
	else
	{
		return &settings.name[0];
	}
}

static const char *ui_temp_from_sht()
{
	static char str[10];
	temp2str(status.air_temp, str, sizeof(str));

	return str;
}
static const char *ui_hum_from_sht()
{
	static char str[10];
	hum2str(status.air_hum / 10, str, sizeof(str));

	return str;
}
static const char *ui_co2_from_scd()
{
	static char str[10];
	co2str(status.air_co2, str, sizeof(str));

	return str;
}
static const char *ui_temp_from_floor()
{
	static char str[10];
	temp2str(status.floor_temp, str, sizeof(str));

	return str;
}

static menu_t *ui_reset_config(menu_t *menu, uint32_t event)
{
	if (event & (BTN_MASK_PRS(BTN_OK)))
		hvac_conf_reset();
	return menu;
}

static menu_t *ui_enter_installer(menu_t *menu, uint32_t event)
{
	if (event & (BTN_MASK_PRS(BTN_OK)))
	{
		return &ui.installer_sett_warn.menu;
	}
	else if (event & (BTN_MASK_PRS(BTN_LONGP_OK)))
	{
		return &ui.installer_sett.menu;
	}
	else
	{
		return menu;
	}
}

static menu_t *ui_enter_main_settings(menu_t *menu, uint32_t event)
{
	if (!((event & (BTN_MASK_PRS(BTN_OK))) || (event & (BTN_MASK_PRS(BTN_LONGP_OK)))))
	{
		return menu;
	}
	return &ui.main_settings.menu;
}

static menu_t *ui_enter_disp_settings(menu_t *menu, uint32_t event)
{
	if (!((event & (BTN_MASK_PRS(BTN_OK))) || (event & (BTN_MASK_PRS(BTN_LONGP_OK)))))
	{
		return menu;
	}
	return &ui.disp_sett.menu;
}

static menu_t *ui_enter_system_settings(menu_t *menu, uint32_t event)
{
	if (!((event & (BTN_MASK_PRS(BTN_OK))) || (event & (BTN_MASK_PRS(BTN_LONGP_OK)))))
	{
		return menu;
	}
	return &ui.system_sett.menu;
}

static menu_t *ui_enter_global_settings(menu_t *menu, uint32_t event)
{
	if (!((event & (BTN_MASK_PRS(BTN_OK))) || (event & (BTN_MASK_PRS(BTN_LONGP_OK)))))
	{
		return menu;
	}
	return &ui.global_sett.menu;
}

static menu_t* ui_enter_zone_settings(menu_t *menu, uint32_t event)
{
	if (!((event & (BTN_MASK_PRS(BTN_OK))) || (event & (BTN_MASK_PRS(BTN_LONGP_OK)))))
	{
		return menu;
	}
	return &ui.zone_sett.menu;
}

static menu_t *ui_enter_about(menu_t *menu, uint32_t event)
{
	if (!(event & (BTN_MASK_PRS(BTN_OK))))
	{
		return menu;
	}
	return &ui.about.menu;
}

static menu_t *ui_enter_mobile_app(menu_t *menu, uint32_t event)
{
	if (!(event & (BTN_MASK_PRS(BTN_OK))))
	{
		return menu;
	}
	return &ui.mobileapp.menu;
}


static menu_t *ui_toggle_display_mode(menu_t *menu, uint32_t event)
{
	if (!(event & (BTN_MASK_PRS(BTN_OK))))
	{
		return menu;
	}
	if(get_disp_type() == DISPLAY_REVERTED)
		set_disp(DISPLAY_REGULAR);
	else
		set_disp(DISPLAY_REVERTED);

	return menu;
}

static menu_t *ui_shuffle_modbus_address(menu_t *menu, uint32_t event)
{
	if (!(event & (BTN_MASK_PRS(BTN_OK))))
		return menu;

	if ((settings.node_id & 0xFF) >= DEVICE_MODBUS_MAX_ADDRESS)
	{
		settings.node_id = 1;
	}
	else
	{
		settings.node_id += 1;
	}
	hvac_conf_change(FLAGD_SENSOR_SETT_UPDATED);
	return menu;
}

static menu_t *ui_toggle_language(menu_t *menu, uint32_t event)
{
	if (!(event & (BTN_MASK_PRS(BTN_OK))))
		return menu;

	if(get_lang() == LCD_LANG_ET)
		set_lang(LCD_LANG_EN);
	else if(get_lang() == LCD_LANG_EN)
		set_lang(LCD_LANG_ET);
		
	return menu;
}

static menu_t *ui_toggle_control_input(menu_t *menu, uint32_t event)
{
	if (!(event & (BTN_MASK_PRS(BTN_OK))))
		return menu;

	settings.control_input = (settings.control_input == CONTROLMODE_FLOOR_TEMP)?CONTROLMODE_AIR_TEMP:CONTROLMODE_FLOOR_TEMP;
	hvac_conf_change(FLAGD_SENSOR_SETT_UPDATED);
	return menu;
}

static menu_t *ui_toggle_factoryreset(menu_t *menu, uint32_t event)
{
	if (!(event & (BTN_MASK_PRS(BTN_OK))))
		return menu;

	return &ui.factory_reset.menu;
}

static void ui_handle_reboot(uint32_t event)
{
	if (!(event & (BTN_MASK_PRS(BTN_OK) | BTN_MASK_PRS(BTN_ONOFF))))
	{
		return;
	}

	settings.flags |= FLAGS_HEC_REBOOT;
	hvac_conf_change(FLAGD_SENSOR_SETT_UPDATED);
}

void skip_hec_error()
{
	last_error_skip_ts = millis();
}

static void ui_handle_skip(uint32_t event)
{
	if (!(event & (BTN_MASK_PRS(BTN_OK) | BTN_MASK_PRS(BTN_ONOFF))))
	{
		return;
	}
	skip_hec_error();
}

static const char *ui_modbus_address()
{
	static char address[10];
	dec2str(settings.node_id, address, sizeof(address));
	return address;
}

static const char *ui_get_temp_calibration()
{ 
	static char str[10];
	temp2str_1_dec_point(settings.temp_calibration, str, sizeof(str));
	return str;
}

static const char *ui_get_zone_hysteresis()
{
	static char str[10];
	temp2str_1_dec_point(global_settings.hysteresis_band, str, sizeof(str));
	return str;
}

static const char *ui_get_factoryreset_state()
{
	return (settings.flags & FLAGS_HEC_FACTORY_RESET) ? text_common[UI_DONE][lang] : text_common[UI_ENTER][lang] ;
}

static menu_t *ui_goto_parent(menu_t *menu, uint32_t event)
{
	return menu->parent;
}

static const char* ui_get_floor_heat_mode()
{
	return text_heat_types[settings.floor_heat_mode][lang];
}

static menu_t *ui_toggle_floor_heat_mode(menu_t *menu, uint32_t event)
{
	if (!(event & (BTN_MASK_PRS(BTN_OK))))
		return menu;

	if(settings.floor_heat_mode == HEAT_HYDRONIC_ELECTRIC)
		settings.floor_heat_mode = HEAT_HYDRONIC;
	else
		settings.floor_heat_mode += 1;
	hvac_conf_change(FLAGD_SENSOR_SETT_UPDATED);
	return menu;
}

static const char* ui_get_floor_application()
{
	return text_heat_types[settings.floor_heat_applic][lang];
}

static menu_t *ui_toggle_floor_application(menu_t *menu, uint32_t event)
{
	if (!(event & (BTN_MASK_PRS(BTN_OK))))
		return menu;

	if(settings.floor_heat_applic == HEAT_AUTO_SWITCH)
		settings.floor_heat_applic = HEAT_HYDRONIC;
	else
		settings.floor_heat_applic += 1;
	hvac_conf_change(FLAGD_SENSOR_SETT_UPDATED);
	return menu;
}

// make function for ui_get_disable_floor_cooling
static const char *ui_get_disable_floor_cooling()
{
	return (settings.flags & FLAGS_HEC_COOLING_FLOOR_BLOCKED) ? text_common[UI_YES][lang] : text_common[UI_NO][lang];
}

static menu_t *ui_toggle_disable_floor_cooling(menu_t *menu, uint32_t event)
{
	if (!(event & (BTN_MASK_PRS(BTN_OK))))
		return menu;

	if(settings.flags & FLAGS_HEC_COOLING_FLOOR_BLOCKED)
		settings.flags &= ~FLAGS_HEC_COOLING_FLOOR_BLOCKED;
	else
		settings.flags |= FLAGS_HEC_COOLING_FLOOR_BLOCKED;

	hvac_conf_change(FLAGD_SENSOR_SETT_UPDATED);
	return menu;
}

static const char *ui_get_fancoil_min_v()
{
	static char str[10];
	dec2str(ext_sett.fancoil_min_v, str, sizeof(str));
	return str;
}

static menu_t *ui_set_fancoil_min_v(menu_t *menu, uint32_t event)
{
	if (!(event & (BTN_MASK_PRS(BTN_OK))))
		return menu;

	if ((ext_sett.fancoil_min_v < FANCOIL_MIN_V_MAX) && (ext_sett.fancoil_max_v > ext_sett.fancoil_min_v))
		ext_sett.fancoil_min_v += FANCOIL_MIN_V_STEP;
	else if(ext_sett.fancoil_max_v == 0)
		ext_sett.fancoil_min_v += FANCOIL_MIN_V_STEP;
	else
		ext_sett.fancoil_min_v = FANCOIL_MIN_V_MIN;

	hvac_conf_change(FLAGD_EXT_SETT_UPDATED);
	return menu;
}

static const char *ui_get_fancoil_max_v()
{
	static char str[10];
	dec2str(ext_sett.fancoil_max_v, str, sizeof(str));
	return str;
}

static menu_t *ui_set_fancoil_max_v(menu_t *menu, uint32_t event)
{
	if (!(event & (BTN_MASK_PRS(BTN_OK))))
		return menu;

	if ((ext_sett.fancoil_max_v < FANCOIL_MAX_V_MAX) && (ext_sett.fancoil_min_v < ext_sett.fancoil_max_v))
		ext_sett.fancoil_max_v += FANCOIL_MAX_V_STEP;
	else if((ext_sett.fancoil_min_v >= FANCOIL_MAX_V_MIN) && (ext_sett.fancoil_min_v < FANCOIL_MAX_V_MAX))
		ext_sett.fancoil_max_v = ext_sett.fancoil_min_v+1;
	else
		ext_sett.fancoil_max_v = FANCOIL_MAX_V_MIN;

	hvac_conf_change(FLAGD_EXT_SETT_UPDATED);
	return menu;
}

static const char *ui_get_fancoil_hysteresis()
{
	static char str[10];
	temp2str_1_dec_point(ext_sett.fancoil_hysteresis, str, sizeof(str));
	return str;
}

static const char *ui_get_fancoil_id()
{
	static char str[10];
	index2str(bit2index(ext_sett.fancoil_id), str, sizeof(str));
	return str;
}

static menu_t *ui_toggle_fancoil_id(menu_t *menu, uint32_t event)
{
	if (!(event & (BTN_MASK_PRS(BTN_OK))))
		return menu;

	if(ext_sett.fancoil_id == 32)
		ext_sett.fancoil_id = 0;
	else if(ext_sett.fancoil_id == 0)
		ext_sett.fancoil_id = 1;
	else
		ext_sett.fancoil_id *= 2;

	hvac_conf_change(FLAGD_EXT_SETT_UPDATED);
	return menu;
}

static const char* ui_get_floor_cooling_hum()
{
	static char humidity[10];
	if(global_settings.cooling_floor_blocked_hum == 0)
	{
		memcpy(humidity, "OFF", sizeof("OFF"));
	}
	else
	{
		dec2str(global_settings.cooling_floor_blocked_hum/10, humidity, sizeof(humidity));
		strcat(humidity, " %");
	}
	return humidity;
}

static const char* ui_get_cooling_activation()
{
	return (global_settings.floor_cooling_act_method) ? text_pair[PAIR_COOLING_METHOD_MANUAL][lang] : text_pair[PAIR_COOLING_METHOD_AUTO][lang];
}

static menu_t *ui_toggle_cooling_activation(menu_t *menu, uint32_t event)
{
	if (!(event & (BTN_MASK_PRS(BTN_OK))))
		return menu;

	if(global_settings.floor_cooling_act_method)
		global_settings.floor_cooling_act_method = COOLING_METHOD_AUTO;
	else
		global_settings.floor_cooling_act_method = COOLING_METHOD_MANUAL;

	hvac_conf_change(FLAGD_GLOBAL_SETT_UPDATED);
	return menu;
}

static const char* ui_get_floor_operation_mode()
{
	return (global_settings.floor_opr_mode == FLOOR_HEATING_MODE) ? text_floor_opr_mode[FLOOR_HEATING_MODE][lang] : text_floor_opr_mode[FLOOR_COOLING_MODE][lang];
}

static const char* ui_get_cooling_application()
{
	return text_cooling_application[global_settings.cooling_application][lang];
}

static const char* ui_get_fancoil_allowed()
{
	return text_fancoil_allowed[global_settings.fancoil_allowed][lang];
}

static menu_t *ui_toggle_floor_operation_mode(menu_t *menu, uint32_t event)
{
	if (!(event & (BTN_MASK_PRS(BTN_OK))))
		return menu;

	if(global_settings.floor_cooling_act_method == COOLING_METHOD_AUTO)
		return menu;

	if(global_settings.floor_opr_mode == FLOOR_HEATING_MODE)
		global_settings.floor_opr_mode = FLOOR_COOLING_MODE;
	else
		global_settings.floor_opr_mode = FLOOR_HEATING_MODE;

	hvac_conf_change(FLAGD_GLOBAL_SETT_UPDATED);
	return menu;
}

static menu_t *ui_toggle_cooling_application(menu_t *menu, uint32_t event)
{
	if (!(event & (BTN_MASK_PRS(BTN_OK))))
		return menu;

	if(global_settings.cooling_application == COOLING_FLOOR_AND_FANCOIL)
		global_settings.cooling_application = COOLING_NONE;
	else
		global_settings.cooling_application += 1;

	hvac_conf_change(FLAGD_GLOBAL_SETT_UPDATED);
	return menu;
}

static menu_t *ui_toggle_fancoil_allowed(menu_t *menu, uint32_t event)
{
	if (!(event & (BTN_MASK_PRS(BTN_OK))))
		return menu;

	if(global_settings.fancoil_allowed == FANCOIL_ALLOWED_COOLING_ONLY)
		global_settings.fancoil_allowed = FANCOIL_ALLOWED_ANY_MODE;
	else
	global_settings.fancoil_allowed = FANCOIL_ALLOWED_COOLING_ONLY;

	hvac_conf_change(FLAGD_GLOBAL_SETT_UPDATED);
	return menu;
}

const char *ui_get_device_id()
{
	static char device_id[10];
	if(dev_config.deviceid != 0)
		sprintf(&device_id[0], "H%08X", dev_config.deviceid);
	else
		sprintf(&device_id[0], "-");
	return device_id;
}

const char *ui_get_network_connected()
{
	return (dev_config.network_stat == true) ? text_status_common[STATUS_CONNECTED][lang] : text_status_common[STATUS_NOT_CONNECTED][lang];
}

const char *ui_get_ip_address()
{
	static char ip_address[20];
	if(dev_config.ip_addr != 0)
		sprintf(&ip_address[0], "%d.%d.%d.%d", dev_config.ip_addr&0xFF, (dev_config.ip_addr>>8)&0xFF, (dev_config.ip_addr>>16)&0xFF, (dev_config.ip_addr>>24)&0xFF);
	else
		sprintf(&ip_address[0], "-");
	return ip_address;
}

// static const char *ui_get_ext1_setting() { return (settings.flags & FLAGS_EXT1_ON) ? "ON" : "OFF"; }
static const char *ui_get_language() { return get_lang() ? text_common[UI_EESTI][lang] : text_common[UI_ENGLISH][lang]; }
static const char *ui_display_mode() { return (get_disp_type() == DISPLAY_REVERTED) ? text_pair[PAIR_DISPLAY_REVERTED][lang] : text_pair[PAIR_DISPLAY_REGULAR][lang] ; }
static const char *ui_get_control_input() { return (settings.control_input == CONTROLMODE_FLOOR_TEMP) ? text_pair[PAIR_FLOOR_TEMP][lang] : text_pair[PAIR_AIR_TEMP][lang]; }
static hsm_t *ui_hvac_connection(uint32_t event)
{
	if (hvac_is_available())
	{
		return &ui.hsm[HSM_HVAC_CONNECTED];
	}
	gfx_render_clear();
	gfx_render_ui_disabled();
	gfx_render_finish();
	return &ui.hsm[HSM_HVAC_DISCONNECTED];
}

static hsm_t *ui_hvac_power(uint32_t event)
{
	return &ui.hsm[HSM_HVAC_ON];
}

static hsm_t *ui_hvac_status(uint32_t event)
{
	uint16_t err = status.error;
	// return &ui.hsm[HSM_HVAC_OK];
	if(is_dev_connection_error())
	{
		ui_handle_reboot(event);
		gfx_render_clear();
		gfx_render_error(text_error[ERROR_TEXT][lang],text_error[ERROR_NOCONNECTION_MSG][lang], text_error[ERROR_REBOOT_MSG][lang]);
		gfx_render_finish();
		return &ui.hsm[HSM_HVAC_ERROR];
	}

//	if(ota_started)
//	{
//		gfx_render_clear();
//		gfx_render_ota();
//		gfx_render_finish();
//		return &ui.hsm[HSM_HVAC_ERROR];
//	}

	if (err)
	{
		ui_handle_reboot(event);
		char tmp[32];
		char numberstr[50] = "";
		memset(numberstr, 0, 50);
		dec2str(err, tmp, sizeof(tmp));
		gfx_render_clear();
		if(err <32)
			gfx_render_error(text_error[ERROR_TEXT][lang],NULL, text_error[ERROR_REBOOT_MSG][lang]);
		else
		{
			sprintf(numberstr,"%s %s",text_error[ERROR_CODE][lang],tmp);
			gfx_render_error(text_error[ERROR_TEXT][lang],numberstr, text_error[ERROR_REBOOT_MSG][lang]);
		}
		gfx_render_finish();
		return &ui.hsm[HSM_HVAC_ERROR];
	}
	else if((hec_extended_status.hec_error_dup > 0) && ((last_error_skip_ts == 0) || ((millis() - last_error_skip_ts) > ERROR_SKIP_MS)))
	{
		if (event & (BTN_MASK_PRS(BTN_OK)))
		{
			last_error_skip_ts =  millis();
			home_render();
			return &ui.hsm[HSM_HVAC_OK];
		}
		else
		{
			last_error_skip_ts = 0;
		}
		gfx_render_clear();
		if(hec_extended_status.hec_error_dup & EXT_DISCONNECT_ERROR)
			gfx_render_error(text_error[ERROR_TEXT][lang],text_error[ERROR_NO_EXT_CONNECTION][lang], text_error[ERROR_SKIP_MSG][lang]);
		else if(hec_extended_status.hec_error_dup & ZONES_OVERLAP_ERROR)
			gfx_render_error(text_error[ERROR_TEXT][lang],text_error[ERROR_ZONES_OVERLAP_MSG][lang], text_error[ERROR_SKIP_MSG][lang]);
		else
			gfx_render_error(text_error[ERROR_TEXT][lang],NULL, text_error[ERROR_SKIP_MSG][lang]);
		gfx_render_finish();
		return &ui.hsm[HSM_HVAC_ERROR];
	}
	return &ui.hsm[HSM_HVAC_OK];
}

static hsm_t *ui_home(uint32_t event)
{
	if (event & BTN_MASK_PRS(BTN_OK))
	{
		menu_enter(&ui.main.menu);
		ui.menu = &ui.main.menu;
		return &ui.hsm[HSM_MENU];
	}

	if(!is_dev_paired())
	{
		if (home_pair_start(event) == 0)
		{
			return &ui.hsm[HSM_PAIR];
		}
	}

	if(is_calibration_started && (is_scd4x_sensor_present == 1))
	{
		gfx_render_clear();
	    uint16_t x = 5;
        uint16_t y = LCD_H/3;
        text_render_center_horizontal_multiple_line(text_common[UI_CO2_RECALIBRATE][lang], font_16px, LCD_W-5, &x, &y);
		gfx_render_finish();
		return &ui.hsm[HSM_HOME];
	}

	if ((event & (BTN_MASK_PRS(BTN_LEFT))) || (event & (BTN_MASK_PRS(BTN_RIGHT))))
	{
		if(is_fancoil_view_enabled)
		{
			menu_enter(&ui.edit_selection.menu);
			ui.menu = &ui.edit_selection.menu;
			return &ui.hsm[HSM_MENU];
		}
		else
		{
			if (home_edit_start(event, EDIT_TYPE_SETPOINT_TEMP) == 0)
			{
				ui.user_activity_ts = millis();
				return &ui.hsm[HSM_EDIT];
			}
		}
		if(is_setpoint_update_from_server())
		{
			if (home_edit_start(event, EDIT_TYPE_SETPOINT_TEMP) == 0)
			{
				ui.user_activity_ts = millis();
				return &ui.hsm[HSM_EDIT];
			}
		}
	}
	home_render();
	return &ui.hsm[HSM_HOME];
}

static void show_zone_settings()
{
	if(is_floor_cooling_application_true())
		menu_insert(&ui.zone_sett.menu, ZONE_DISABLE_FLOOR_COOLING, &ui.zone_sett.items[ZONE_DISABLE_FLOOR_COOLING]);
	else
		menu_remove(&ui.zone_sett.menu, &ui.zone_sett.items[ZONE_DISABLE_FLOOR_COOLING]);
}

static void show_global_settings()
{
	if(is_floor_cooling_application_true())
	{
		menu_insert(&ui.global_sett.menu, GLOBAL_BLOCK_COOLING_HUM, &ui.global_sett.items[GLOBAL_BLOCK_COOLING_HUM]);
		menu_insert(&ui.global_sett.menu, GLOBAL_COOLING_ACTIVATION, &ui.global_sett.items[GLOBAL_COOLING_ACTIVATION]);
	}
	else
	{
		menu_remove(&ui.global_sett.menu, &ui.global_sett.items[GLOBAL_BLOCK_COOLING_HUM]);
		menu_remove(&ui.global_sett.menu, &ui.global_sett.items[GLOBAL_COOLING_ACTIVATION]);
	}
}

static void show_system_settings()
{
	if(settings.floor_heat_applic == HEAT_HYDRONIC_ELECTRIC)
	{
		menu_insert(&ui.system_sett.menu, SYS_SETTINGS_FLOOR_HEATING, &ui.system_sett.items[SYS_SETTINGS_FLOOR_HEATING]);
	}
	else
	{
		menu_remove(&ui.system_sett.menu, &ui.system_sett.items[SYS_SETTINGS_FLOOR_HEATING]);
	}
}

static hsm_t *ui_menu(uint32_t event)
{
	if (millis() - ui.user_activity_ts > MENU_TIMEOUT_MS)
	{
		ui.menu = 0;
	}

	if((event & BTN_MASK_PRS(BTN_OK)) && (ui.menu->selected == &ui.installer_sett.items[INSTALLER_ZONE_SETTINGS]))
	{
		show_zone_settings();
	}
	else if((event & BTN_MASK_PRS(BTN_OK)) && (ui.menu->selected == &ui.installer_sett.items[INSTALLER_GLOBAL_SETTINGS]))
	{
		show_global_settings();
	}
	else if((event & BTN_MASK_PRS(BTN_OK)) && (ui.menu->selected == &ui.main_settings.items[SETTINGS_SYSTEM]))
	{
		show_system_settings();
	}

	ui.menu = menu_event(ui.menu, event);
	if (ui.menu == 0)
	{
		is_installer_menu_unlock = 0;
		if (ui.menu_req_task)
		{
			return ui.menu_req_task;
		}
		return &ui.hsm[HSM_HOME];
	}

	if((ui.menu->selected == &ui.system_sett.items[SYS_SETTINGS_TEMP_CALIBRATION]))
	{
		if(event & BTN_MASK_PRS(BTN_OK))
		{
			if (home_edit_start(event, EDIT_TYPE_TEMP_CAL) == 0)
			{
				return &ui.hsm[HSM_EDIT];
			}
		}
	}

	if((ui.menu->selected == &ui.global_sett.items[GLOBAL_HYSTERESIS]))
	{
		if(event & BTN_MASK_PRS(BTN_OK))
		{
			if (home_edit_start(event, EDIT_TYPE_ZONE_HYSTERESIS) == 0)
			{
				return &ui.hsm[HSM_EDIT];
			}
		}
	}

	if((ui.menu->selected == &ui.global_sett.items[GLOBAL_BLOCK_COOLING_HUM]))
	{
		if(event & BTN_MASK_PRS(BTN_OK))
		{
			if (home_edit_start(event, EDIT_COOLING_HUMIDITY) == 0)
			{
				return &ui.hsm[HSM_EDIT];
			}
		}
	}

	if((ui.menu->selected == &ui.zone_sett.items[ZONE_MB_BAUDRATE]))
	{
		if(event & BTN_MASK_PRS(BTN_OK))
		{
			if (home_edit_start(event, EDIT_MB_BAUDRATE) == 0)
			{
				return &ui.hsm[HSM_EDIT];
			}
		}
	}

	if(ui.menu->selected == &ui.edit_selection.items[EDIT_SELECTION_SETPOINT_TEMP])
	{
		if(event & BTN_MASK_PRS(BTN_OK))
		{
			if (home_edit_start(event, EDIT_TYPE_SETPOINT_TEMP) == 0)
			{
				return &ui.hsm[HSM_EDIT];
			}
		}
	}
	else if(ui.menu->selected == &ui.edit_selection.items[EDIT_SELECTION_FANCOIL_MODE])
	{
		if(event & BTN_MASK_PRS(BTN_OK))
		{
			if (home_edit_start(event, EDIT_FANCOIL_MODE) == 0)
			{
				return &ui.hsm[HSM_EDIT];
			}
		}
	}

	if((ui.menu->selected == &ui.zone_sett.items[ZONE_FANCOIL_HYSTERESIS]))
	{
		if(event & BTN_MASK_PRS(BTN_OK))
		{
			if (home_edit_start(event, EDIT_FANCOIL_HYSTERESIS) == 0)
			{
				return &ui.hsm[HSM_EDIT];
			}
		}
	}

	if((ui.menu->selected == &ui.zone_sett.items[ZONE_SELECTION]))
	{
		if(event & BTN_MASK_PRS(BTN_OK))
		{
			if (home_edit_start(event, EDIT_SELECTED_ZONES) == 0)
			{
				return &ui.hsm[HSM_EDIT];
			}
		}
	}

	menu_render(ui.menu);
	return &ui.hsm[HSM_MENU];
}

static hsm_t *ui_edit(uint32_t event)
{
	if ((event & BTN_MASK_PRS(BTN_BACK)) || (home_edit_handle(event) == 0))
	{
		home_edit_finish(event);
		if((edit.current == &edit.temp) || (edit.current == &edit.fancoil_mode_edit))
		{
			return &ui.hsm[HSM_HOME];
		}
		else if(edit.current == &edit.temp_calb)
		{
			ui.menu = &ui.system_sett.menu;
			return &ui.hsm[HSM_MENU];
		}
		else if(edit.current == &edit.zone_hysteresis)
		{
			ui.menu = &ui.global_sett.menu;
			return &ui.hsm[HSM_MENU];
		}
		else if(edit.current == &edit.cooling_hum)
		{
			ui.menu = &ui.global_sett.menu;
			return &ui.hsm[HSM_MENU];
		}
		else if(edit.current == &edit.mb_baudrate)
		{
			ui.menu = &ui.zone_sett.menu;
			return &ui.hsm[HSM_MENU];
		}
		else if(edit.current == &edit.fancoil_hysteresis_edit)
		{
			ui.menu = &ui.zone_sett.menu;
			ui.menu->selected = &ui.zone_sett.items[ZONE_FANCOIL_HYSTERESIS];
			return &ui.hsm[HSM_MENU];
		}
		else if(edit.current == &edit.selected_zone)
		{
			ui.menu = &ui.zone_sett.menu;
			ui.menu->selected = &ui.zone_sett.items[ZONE_SELECTION];
			return &ui.hsm[HSM_MENU];
		}
		else 
		{
			ui.menu = &ui.global_sett.menu;
			return &ui.hsm[HSM_MENU];
		}
	}
	if (millis() - ui.user_activity_ts > EDIT_TIMEOUT_MS)
	{
		set_lcd_force_update();
		return &ui.hsm[HSM_HOME];
	}

	home_edit_render();
	return &ui.hsm[HSM_EDIT];
}

static hsm_t *ui_pair(uint32_t event)
{
	if (event & BTN_MASK_PRS(BTN_BACK))
	{
		if(!is_dev_paired())
		{
//			mbslave_task_delete();
		}
		else
		{
			hvac_conf_change(FLAGD_SENSOR_SETT_UPDATED);
		}
		home_pair_finish();
		return &ui.hsm[HSM_HOME];
	}

	if (home_pair_handle(event) == 0)
	{
		home_pair_finish();
		return &ui.hsm[HSM_HOME];
	}
	home_pair_render();
	return &ui.hsm[HSM_PAIR];
}

static void ui_hvac_off_enter(uint32_t event)
{
	gfx_display_off();
}

static void ui_hvac_off_exit(uint32_t event)
{
	gfx_display_on();
}

static void ui_hvac_disable_enter(uint32_t event)
{
	gfx_display_on();
}

static void ui_hvac_disable_exit(uint32_t event)
{
}

static void ui_hvac_ok_enter(uint32_t event)
{
	ui.hsm[HSM_HVAC_OK].sub = &ui.hsm[HSM_HOME];
}

void ui_init()
{
	menu_item_t *it;
	menu_item_t *end;
	menu_t *menu = &ui.main.menu;

	for (it = ui.main.items, end = ui.main.items + NUM_MAIN; it != end; ++it)
	{
		menu_insert(menu, menu_len(menu), it);
	}

	menu = &ui.mobileapp.menu;
	for (it = ui.mobileapp.items, end = ui.mobileapp.items + NUM_MOBILE_APP; it != end; ++it)
	{
		menu_insert(menu, menu_len(menu), it);
	}

	menu = &ui.main_settings.menu;
	for (it = ui.main_settings.items, end = ui.main_settings.items + NUM_MAIN_SETTINGS; it != end; ++it)
	{
		menu_insert(menu, menu_len(menu), it);
	}

	menu = &ui.disp_sett.menu;
	for (it = ui.disp_sett.items, end = ui.disp_sett.items + NUM_DISP_SETTINGS; it != end; ++it)
	{
		menu_insert(menu, menu_len(menu), it);
	}

	menu = &ui.system_sett.menu;
	for (it = ui.system_sett.items, end = ui.system_sett.items + NUM_SYS_SETTINGS; it != end; ++it)
	{
		menu_insert(menu, menu_len(menu), it);
	}

	menu = &ui.installer_sett.menu;
	for (it = ui.installer_sett.items, end = ui.installer_sett.items + NUM_INSTALLER_SETTINGS; it != end; ++it)
	{
		menu_insert(menu, menu_len(menu), it);
	}

	menu = &ui.installer_sett_warn.menu;
	for (it = ui.installer_sett_warn.items, end = ui.installer_sett_warn.items + INS_WARNING_2; it != end; ++it)
	{
		menu_insert(menu, menu_len(menu), it);
	}

	menu = &ui.global_sett.menu;
	for (it = ui.global_sett.items, end = ui.global_sett.items + NUM_GLOBAL_SETTINGS; it != end; ++it)
	{
		menu_insert(menu, menu_len(menu), it);
	}

	menu = &ui.zone_sett.menu;
	for (it = ui.zone_sett.items, end = ui.zone_sett.items + NUM_ZONE_SETTINGS; it != end; ++it)
	{
		menu_insert(menu, menu_len(menu), it);
	}

	menu = &ui.about.menu;
	for (it = ui.about.items, end = ui.about.items + NUM_ABOUT; it != end; ++it)
	{
		menu_insert(menu, menu_len(menu), it);
	}

	menu = &ui.pair.menu;
	for (it = ui.pair.items, end = ui.pair.items + NUM_PAIR; it != end; ++it)
	{
		menu_insert(menu, menu_len(menu), it);
	}

	menu = &ui.factory_reset.menu;
	for (it = ui.factory_reset.items, end = ui.factory_reset.items + NUM_FACTORY_RESET; it != end; ++it)
	{
		menu_insert(menu, menu_len(menu), it);
	}

	menu = &ui.edit_selection.menu;
	for (it = ui.edit_selection.items, end = ui.edit_selection.items + NUM_EDIT_SELECTION; it != end; ++it)
	{
		menu_insert(menu, menu_len(menu), it);
	}

	gfx_display_on();
}

#define NO_EVENT_WAIT_TIME 2 * 60 * 1000

uint32_t No_Event_wait_count = 0;

uint8_t Wait_for_2m()
{
	if (No_Event_wait_count == 0)
	{
		No_Event_wait_count = millis();
	}
	else
	{
		if ((millis() - No_Event_wait_count) > NO_EVENT_WAIT_TIME)
		{
			No_Event_wait_count = 0;
			return 1;
		}
	}
	return 0;
}

uint32_t Lcd_Flesh_wait_count = 0;
uint8_t Wait_for_2s()
{
	if (Lcd_Flesh_wait_count == 0)
	{
		Lcd_Flesh_wait_count = millis();
	}
	else
	{
		if ((millis() - Lcd_Flesh_wait_count) > 2000)
		{
			Lcd_Flesh_wait_count = 0;
			return 1;
		}
	}
	return 0;
}

uint8_t Detect_event_Flag = 0;

void ui_task()
{
	uint32_t event = buttons_check();
	if (event)
	{
		ui.user_activity_ts = millis();
		Detect_event_Flag = 1;
	}
	else
	{
		if (Detect_event_Flag == 0)
		{
		}
		else
		{
			Detect_event_Flag = 0;
			No_Event_wait_count = 0;
		}
	}
	ui.task = hsm_run(ui.task, event);
}

#if 1
struct ui ui = {
	.task = &ui.hsm[HSM_HVAC_DISCONNECTED],
	.menu_req_task = NULL,
	.menu = NULL,
	.user_activity_ts = 0,
	.hsm = {
		[HSM_HVAC_DISCONNECTED] = {
			.enter = NULL,
			.task = ui_hvac_connection,
			.exit = NULL,
			.sub = NULL,
		},
		[HSM_HVAC_CONNECTED] = {
			.enter = NULL,
			.task = ui_hvac_connection,
			.exit = NULL,
			.sub = &ui.hsm[HSM_HVAC_ON],
		},
		[HSM_HVAC_OFF] = {
			.enter = ui_hvac_off_enter,
			.task = ui_hvac_power,
			.exit = ui_hvac_off_exit,
			.sub = NULL,
		},
		[HSM_HVAC_ON] = {
			.enter = NULL,
			.task = ui_hvac_power,
			.exit = NULL,
			.sub = &ui.hsm[HSM_HVAC_OK],
		},
		[HSM_HVAC_ERROR] = {
			.enter = NULL,
			.task = ui_hvac_status,
			.exit = NULL,
			.sub = NULL,
		},
		[HSM_HVAC_OK] = {
			.enter = ui_hvac_ok_enter,
			.task = ui_hvac_status,
			.exit = NULL,
			.sub = NULL,
		},
		[HSM_HOME] = {
			.enter = NULL,
			.task = ui_home,
			.exit = NULL,
			.sub = NULL,
		},
		[HSM_MENU] = {
			.enter = NULL,
			.task = ui_menu,
			.exit = NULL,
			.sub = NULL,
		},
		[HSM_EDIT] = {
			.enter = NULL,
			.task = ui_edit,
			.exit = NULL,
			.sub = NULL,
		},
		[HSM_PAIR] = {
			.enter = NULL,
			.task = ui_pair,
			.exit = NULL,
			.sub = NULL,
		},
	},
	.main = {
		.menu = {
			.parent = NULL,
			.selected = NULL,
			.len = 0,
			.line = 0,
		},
		.items = {
			[MAIN_SETTINGS] = {
				.element = {
					.text =  text_menu[MAIN_SETTINGS][lang],
					.value = NULL,
					.unit = NULL,
					.action = ui_enter_main_settings,
				},
			},
			[MAIN_MOBILE_APP] = {
				.element = {
					.text = text_menu[MAIN_MOBILE_APP][lang],
					.value = NULL,
					.unit = ">",
					.action = ui_enter_mobile_app,
				},
			},
			[MAIN_ABOUT] = {
				.element = {
					.text =  text_menu[MAIN_ABOUT][lang],
					.value = NULL,
					.unit = NULL,
					.action = ui_enter_about,
				},
			},
		},
	},
	.mobileapp = {
		.menu = {
			.parent = &ui.main.menu,
			.selected = NULL,
			.len = 0,
			.line = 0,
		},
		.items = {
			[PAGE_MOBILE_APP] = {
				.element = {
					.text =  text_menu[MAIN_MOBILE_APP][lang],
					.value = NULL,
					.unit = NULL,
					.action = NULL,
				},
			},
		},
	},
	.main_settings = {
		.menu = {
			.parent = &ui.main.menu,
			.selected = NULL,
			.len = 0,
			.line = 0,
		},
		.items = {
			[SETTINGS_SYSTEM] = {
				.element = {
					.text = text_main_settings[SETTINGS_SYSTEM][lang],
					.value = NULL,
					.unit = NULL,
					.action = ui_enter_system_settings,
				},
			},
			[SETTINGS_DISPLAY] = {
				.element = {
					.text = text_main_settings[SETTINGS_DISPLAY][lang],
					.value = NULL,
					.unit = NULL,
					.action = ui_enter_disp_settings,
				},
			},
			[INSTALLER_TEXT] = {
				.element = {
					.text = text_main_settings[INSTALLER_TEXT][lang],
					.value = NULL,
					.unit = NULL,
					.action = ui_enter_installer,
				},
			},
		},
	},
	.disp_sett = {
		.menu = {
			.parent = &ui.main_settings.menu,
			.selected = NULL,
			.len = 0,
			.line = 0,
		},
		.items = {
			[DISP_SETTINGS_LANGUAGE] = {
				.element = {
					.text = text_disp_settings[DISP_SETTINGS_LANGUAGE][lang],
					.value = ui_get_language,
					.unit = NULL,
					.action = ui_toggle_language,
				},
			},
			[DISP_SETTINGS_DISPLAY] = {
				.element = {
					.text = text_disp_settings[DISP_SETTINGS_DISPLAY][lang],
					.value = ui_display_mode,
					.unit = NULL,
					.action = ui_toggle_display_mode,
				},
			},
		},
	},
	.system_sett = {
		.menu = {
			.parent = &ui.main_settings.menu,
			.selected = NULL,
			.len = 0,
			.line = 0,
		},
		.items = {
			[SYS_SETTINGS_FLOOR_MODE] = {
				.element = {
					.text = text_sys_settings[SYS_SETTINGS_FLOOR_MODE][lang],
					.value = ui_get_floor_operation_mode,
					.unit = NULL,
					.action = ui_toggle_floor_operation_mode,
				},
			},
			[SYS_SETTINGS_FLOOR_HEATING] = {
				.element = {
					.text = text_sys_settings[SYS_SETTINGS_FLOOR_HEATING][lang],
					.value = ui_get_floor_heat_mode,
					.unit = NULL,
					.action = ui_toggle_floor_heat_mode,
				},
			},
			[SYS_SETTINGS_TEMP_CALIBRATION] = {
				.element = {
					.text = text_sys_settings[SYS_SETTINGS_TEMP_CALIBRATION][lang],
					.value = ui_get_temp_calibration,
					.unit = NULL,
					.action = NULL,
				},
			},
		},
	},
	.installer_sett = {
		.menu = {
			.parent = &ui.main_settings.menu,
			.selected = NULL,
			.len = 0,
			.line = 0,
		},
		.items = {
			[INSTALLER_GLOBAL_SETTINGS] = {
				.element = {
					.text = text_installer_settings[INSTALLER_GLOBAL_SETTINGS][lang],
					.value = NULL,
					.unit = NULL,
					.action = ui_enter_global_settings,
				},
			},
			[INSTALLER_ZONE_SETTINGS] = {
				.element = {
					.text = text_installer_settings[INSTALLER_ZONE_SETTINGS][lang],
					.value = NULL,
					.unit = NULL,
					.action = ui_enter_zone_settings,
				},
			},
		},
	},
	.installer_sett_warn = {
		.menu = {
			.parent = &ui.main_settings.menu,
			.selected = NULL,
			.len = 0,
			.line = 0,
		},
		.items = {
			[INS_WARNING_1] = {
				.element = {
					.text = NULL,
					.value = NULL,
					.unit = NULL,
					.action = ui_enter_main_settings,
				},
			},
		},
	},
	.global_sett
	{
		.menu = {
			.parent = &ui.installer_sett.menu,
			.selected = NULL,
			.len = 0,
			.line = 0,
		},
		.items = {
			[GLOBAL_COOLING_APPLICATION] = {
				.element = {
					.text = text_global_settings[GLOBAL_COOLING_APPLICATION][lang],
					.value = ui_get_cooling_application,
					.unit = NULL,
					.action = ui_toggle_cooling_application,
				},
			},
			[GLOBAL_FANCOIL_ALLOWED] = {
				.element = {
					.text = text_global_settings[GLOBAL_FANCOIL_ALLOWED][lang],
					.value = ui_get_fancoil_allowed,
					.unit = NULL,
					.action = ui_toggle_fancoil_allowed,
				},
			},
			[GLOBAL_BLOCK_COOLING_HUM] = {
				.element = {
					.text = text_global_settings[GLOBAL_BLOCK_COOLING_HUM][lang],
					.value = ui_get_floor_cooling_hum,
					.unit = NULL,
					.action = NULL,
				},
			},
			[GLOBAL_COOLING_ACTIVATION] = {
				.element = {
					.text = text_global_settings[GLOBAL_COOLING_ACTIVATION][lang],
					.value = ui_get_cooling_activation,
					.unit = NULL,
					.action = ui_toggle_cooling_activation,
				},
			},
			[GLOBAL_HYSTERESIS] = {
				.element = {
					.text = text_global_settings[GLOBAL_HYSTERESIS][lang],
					.value = ui_get_zone_hysteresis,
					.unit = NULL,
					.action = NULL,
				},
			},
		},
	},
	.zone_sett = {
		.menu = {
			.parent = &ui.installer_sett.menu,
			.selected = NULL,
			.len = 0,
			.line = 0,
		},
		.items = {
			[ZONE_CONTROL_INPUT] = {
				.element = {
					.text = text_zone_settings[ZONE_CONTROL_INPUT][lang],
					.value = ui_get_control_input,
					.unit = NULL,
					.action = ui_toggle_control_input,
				},
			},
			[ZONE_FLOOR_APPLICATION] = {
				.element = {
					.text = text_zone_settings[ZONE_FLOOR_APPLICATION][lang],
					.value = ui_get_floor_application,
					.unit = NULL,
					.action = ui_toggle_floor_application,
				},
			},
			[ZONE_DISABLE_FLOOR_COOLING] = {
				.element = {
					.text = text_zone_settings[ZONE_DISABLE_FLOOR_COOLING][lang],
					.value = ui_get_disable_floor_cooling,
					.unit = NULL,
					.action = ui_toggle_disable_floor_cooling,
				},
			},
			[ZONE_FANCOIL_ID] = {
				.element = {
					.text = text_zone_settings[ZONE_FANCOIL_ID][lang],
					.value = ui_get_fancoil_id,
					.unit = NULL,
					.action = ui_toggle_fancoil_id,
				},
			},
			[ZONE_FANCOIL_MAX_V] = {
				.element = {
					.text = text_zone_settings[ZONE_FANCOIL_MAX_V][lang],
					.value = ui_get_fancoil_max_v,
					.unit = NULL,
					.action = ui_set_fancoil_max_v,
				},
			},	
			[ZONE_FANCOIL_MIN_V] = {
				.element = {
					.text = text_zone_settings[ZONE_FANCOIL_MIN_V][lang],
					.value = ui_get_fancoil_min_v,
					.unit = NULL,
					.action = ui_set_fancoil_min_v,
				},
			},
			[ZONE_FANCOIL_HYSTERESIS] = {
				.element = {
					.text = text_zone_settings[ZONE_FANCOIL_HYSTERESIS][lang],
					.value = ui_get_fancoil_hysteresis,
					.unit = NULL,
					.action = NULL,
				},
			},
			[ZONE_RESET_SENSOR] = {
				.element = {
					.text = text_zone_settings[ZONE_RESET_SENSOR][lang],
					.value = ui_get_factoryreset_state,
					.unit = NULL,
					.action = ui_toggle_factoryreset,
				},
			},
			[ZONE_MB_BAUDRATE] = {
				.element = {
					.text = text_zone_settings[ZONE_MB_BAUDRATE][lang],
					.value = get_baudrate_string,
					.unit = NULL,
					.action = NULL,
				},
			},
			[ZONE_SELECTION] = {
				.element = {
					.text = text_zone_settings[ZONE_SELECTION][lang],
					.value = get_zone_selection,
					.unit = NULL,
					.action = NULL,
				},
			},
		},
	},
	.about = {
		.menu = {
			.parent = &ui.main.menu,
			.selected = NULL,
			.len = 0,
			.line = 0,
		},
		.items = {
			[ABT_ZONE_NAME] = {
				.element = {
					.text = text_about[ABT_ZONE_NAME][lang],
					.value = ui_abt_get_zone_name,
					.unit = NULL,
					.action = NULL,
				},
			},
			[ABT_NODEID] = {
				.element = {
					.text = text_about[ABT_NODEID][lang],
					.value = ui_modbus_address,
					.unit = NULL,
					.action = NULL,
				},
			},
			[ABT_FW_HW_VER] = {
				.element = {
					.text =text_about[ABT_FW_HW_VER][lang],
					.value = ui_hec_ucfw_uchw,
					.unit = NULL,
					.action = NULL,
				},
			},
			[ABT_MODE] = {
				.element = {
					.text = text_about[ABT_MODE][lang],
					.value = ui_get_floor_operation_mode,
					.unit = NULL,
					.action = NULL,
				},
			},
			[ABT_DEV_ID] = {
				.element = {
					.text = text_about[ABT_DEV_ID][lang],
					.value = ui_get_device_id,
					.unit = NULL,
					.action = NULL,
				},
			},
			[ABT_NETWORK] =	{
				.element = {
					.text = text_about[ABT_NETWORK][lang],
					.value = ui_get_network_connected,
					.unit = NULL,
					.action = NULL,
				},
			},
			[ABT_IP_ADDR] = {
				.element = {
					.text = text_about[ABT_IP_ADDR][lang],
					.value = ui_get_ip_address,
					.unit = NULL,
					.action = NULL,
				},
			},
			[ABT_CONTROLLER_FW_HW] = {
				.element = {
					.text = text_about[ABT_CONTROLLER_FW_HW][lang],
					.value = ui_controller_ucfw_uchw,
					.unit = NULL,
					.action = NULL,
				},
			},
			[ABT_MB_BAUDRATE] = {
				.element = {
					.text = text_about[ABT_MB_BAUDRATE][lang],
					.value = get_baudrate_string,
					.unit = NULL,
					.action = NULL,
				},
			},
		},
	},
	.factory_reset = {
		.menu = {
			.parent = &ui.main_settings.menu,
			.selected = NULL,
			.len = 0,
			.line = 0,
		},
		.items = {
			[CONFIRM_FACTORY_RESET] = {
				.element = {
					.text = text_common[UI_YES][lang],
					.value = NULL,
					.unit = NULL,
					.action = ui_reset_config,
				},
			},
			[DENY_FACTORY_RESET] = {
				.element = {
					.text = text_common[UI_NO][lang],
					.value = NULL,
					.unit = NULL,
					.action = ui_goto_parent,
				},
			},
		},
	},
	.edit_selection = {
		.menu = {
			.parent = NULL,
			.selected = NULL,
			.len = 0,
			.line = 0,
		},
		.items = {
			[EDIT_SELECTION_SETPOINT_TEMP] = {
				.element = {
					.text = text_edit_selection[EDIT_SELECTION_SETPOINT_TEMP][lang],
					.value = NULL,
					.unit = NULL,
					.action = NULL,
				},
			},
			[EDIT_SELECTION_FANCOIL_MODE] = {
				.element = {
					.text = text_edit_selection[EDIT_SELECTION_FANCOIL_MODE][lang],
					.value = NULL,
					.unit = NULL,
					.action = NULL,
				},
			},
		},
	},
};
#endif
