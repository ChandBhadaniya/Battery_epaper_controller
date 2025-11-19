#include "status.h"
#include "hvac.h"
#include "buttons.h"
#include "gfx.h"
#include "val2str.h"
#include "home.h"
#include "common_def.h"
//#include "esp_log.h"
#include "hV_GUI.h"
#include "font_render.h"
#include "icons.h"
#include "pair.h"
#include "disp.h"
#include "disp_common.h"
#include "settings.h"
//#include "mbslave.h"

const char* PAIR_TAG = "PAIRING";
#define PAIR_FAILED_TIMEOUT_MS (10 * 60 * 1000)
#define AIROBOT_LOGO_TIMEOUT_MS (30 * 1000)

uint8_t available_zone[MAX_ZONE];
uint8_t is_electric_floor_allowed_pair_only = 0;
GUI myGUI(&myScreen);
void fill_pair_value();
static void update_pair_flow_strings();

struct pair_s pair_t = {
	.airobot_logo = {
		.ui_type = UI_TYPE_IMAGE,
		.next = &pair_t.language,
	},
	.language = {
		.ui_type = UI_TYPE_BUTTON,
		.value = (uint8_t)get_lang(),
		.max = DEF_MAX_VAL,
		.step = DEF_STEP,
		.next = &pair_t.display,
	},
	// .welcome_note = {
	// 	.ui_type = UI_TYPE_TEXT,
	// 	.value = DEF_VALUE,
	// 	.max = DEF_MAX_VAL,
	// 	.step = DEF_STEP,
	// 	.next = &pair_t.display,
	// },
	.display = {
		.ui_type = UI_TYPE_BUTTON,
		.value = DISPLAY_REGULAR,
		.max = DEF_MAX_VAL,
		.step = DEF_STEP,
		.next = &pair_t.controlmode,
	},
	.controlmode = {
		.ui_type = UI_TYPE_BUTTON,
		.value = settings.control_input,
		.max = DEF_MAX_VAL,
		.step = DEF_STEP,
		.next = &pair_t.node_id,
	},
	.node_id = {
		.ui_type = UI_TYPE_SELECT,
		.value = (settings.node_id>0)?settings.node_id:1,
		.max = DEVICE_MODBUS_MAX_ADDRESS+1,						// +1 because they are starting from zero and we skip zero from modbus address
		.step = DEF_STEP,
		.next = &pair_t.pairing_screen,
	},
	.pairing_screen = {
		.ui_type = UI_TYPE_TEXT,
		.value = DEF_VALUE,
		.max = DEF_MAX_VAL,
		.step = DEF_STEP,
		.next = NULL,
	},
	.cooling_application_pair = {
		.ui_type = UI_TYPE_BUTTON,
		.value = DEF_VALUE,
		.max = MAX_COOLING_APP,
		.step = DEF_STEP,
		.next = &pair_t.cooling_method,
	},
	.cooling_method = {
		.ui_type = UI_TYPE_BUTTON,
		.value = DEF_VALUE,
		.max = DEF_MAX_VAL,
		.step = DEF_STEP,
		.next = &pair_t.electric_floor_heating,
	},
	.electric_floor_heating = {
		.ui_type = UI_TYPE_BUTTON,
		.value = DEF_VALUE,
		.max = DEF_MAX_VAL,
		.step = DEF_STEP,
		.next = &pair_t.heating_method,
	},
	.heating_method = {
		.ui_type = UI_TYPE_BUTTON,
		.value = DEF_VALUE,
		.max = MAX_HEATING,
		.step = DEF_STEP,
		.next = &pair_t.zones,
	},
	.zones = {
		.ui_type = UI_TYPE_SELECTION,
		.value = DEF_VALUE,
		.max = MAX_ZONE,
		.step = DEF_STEP,
		.next = &pair_t.fancoil_id,
	},
	.fancoil_id = {
		.ui_type = UI_TYPE_SELECT,
		.value = DEF_VALUE,
		.max = MAX_FANCOIL,
		.step = DEF_STEP,
		.next = &pair_t.connection_stat,
	},
	.connection_stat = {
		.ui_type = UI_TYPE_TEXT,
		.value = DEF_VALUE,
		.max = DEF_MAX_VAL,
		.step = DEF_STEP,
		.next = NULL,
	},
};

static uint8_t home_pair_value(uint8_t value, uint8_t max, int16_t step) {
	if (max <= 0 || step == 0)
		return value;
	if (step < 0 && value <= 0)
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

	if (value < 0 && step < 0)
		return 0;
	else if (value > max && step > 0)
		return max;
	return value;
}
static void home_pair_item_up(pair_item_t* i) {
	uint8_t temp_val = i->value;
	i->value = home_pair_value(i->value, (i->max)-1, i->step);
}
static void home_pair_item_down(pair_item_t* i) {
	uint8_t temp_val = i->value;
	i->value = home_pair_value(i->value, (i->max)-1, -i->step);
}

void get_available_zones(uint8_t zones)
{
	uint8_t j=0;

	ESP_LOGI(PAIR_TAG, "pairingsett.activezones=%d", pairingsett.activezones);
	for(uint8_t i=0; i<MAX_ZONE; i++)
	{
		if(!(zones & 1<<i))
		{
			available_zone[j] = i+1;
    		ESP_LOGI(PAIR_TAG, "available_zone[%d]=%d", j, available_zone[j]);
			j++;
		}
	}
	pair_t.zones.max = j;
}

void pair_get_available_zones() 
{
	uint8_t zones = pairingsett.activezones;
	get_available_zones(zones);
}

int8_t home_pair_handle(uint32_t e) {
static uint32_t pair_failed_timout = 0;
	if (e & BTN_MASK_PRS(BTN_OK))
	{
		if(pair_t.current->ui_type == UI_TYPE_SELECTION)
		{
			pair_t.current->selection ^= (1<<(available_zone[pair_t.current->value]-1));
		}
		else if((pair_t.current == &pair_t.cooling_application_pair) || (pair_t.current == &pair_t.cooling_method) || 
				(pair_t.current == &pair_t.electric_floor_heating) || (pair_t.current == &pair_t.heating_method) || (pair_t.current == &pair_t.fancoil_id) ||
				((pair_t.current == &pair_t.connection_stat) && ((pairingsett.status != PS_PAIRED))))
		{
			// do nothing
		}
		else
		{
			if(pair_t.current == &pair_t.display)
			{
				set_disp(pair_t.display.value);
			}
			else if(pair_t.current == &pair_t.language)
			{
				set_lang((language_e)pair_t.current->value);
				update_pair_flow_strings();
			}
			if((pair_t.current != &pair_t.pairing_screen) && (pair_t.current != &pair_t.node_id))
			{
				pair_t.current = pair_t.current->next;
			}
		}
		pair_t.flash = 1;
		pair_t.ts = millis();
		return pair_t.current ? 1 : 0;
	}
	
	if(pair_t.current ==  &pair_t.pairing_screen)
	{
		if(millis() - pair_failed_timout > PAIR_FAILED_TIMEOUT_MS)
		{
			pair_t.pairing_screen.text[0] = text_pair[PAIR_TIMEOUT][lang];
		}
		if(pairingsett.status == PS_PAIRING)
		{
			if(pairingsett.activezones != 255)
			{
				if(global_settings.flags & SET_FLAG_GLOBAL_SETT_CONFIGURED)
				{
					pair_get_available_zones();
					if((pair_t.controlmode.value == CONTROLMODE_FLOOR_TEMP) && (is_electric_floor_allowed_pair_only))
					{
						pair_t.current->next = &pair_t.heating_method;
					}
					else
					{
						pair_t.current->next = &pair_t.zones;
					}
				}
				else
				{
					pair_t.cooling_application_pair.max = (pairingsett.pair_flags & PAIR_FLAG_EXT_CONNECTED)?MAX_COOLING_APP:MAX_COOLING_APP_ONLY_FLOOR;
					pair_t.current->next = &pair_t.cooling_application_pair;
				}
				pair_t.current = pair_t.current->next;
			}
			else
			{
				pair_t.pairing_screen.text[0] = text_pair[PAIR_NO_ZONES_AVAILABLE][lang];
			}
		}
		else if(pairingsett.status == PS_READY)
		{
			pair_get_available_zones();
			pair_t.current->next = &pair_t.connection_stat;
			pair_t.current = pair_t.current->next;
		}
		return 1;
	}
	else if((pair_t.current ==  &pair_t.connection_stat) && (pairingsett.status == PS_PAIRED))
	{
		pair_t.connection_stat.text[0] = text_pair[PAIR_SUCCESS][lang];
		pair_t.connection_stat.text[1] = text_pair[PAIR_NEXT][lang];
	}
	else if(pair_t.current ==  &pair_t.airobot_logo)
	{
		if(millis() > AIROBOT_LOGO_TIMEOUT_MS)
		{
			pair_t.current = pair_t.current->next;
		}
	}

	if (e & BTN_MASK_PRS(BTN_LONGP_OK))
	{
		if((pair_t.current == &pair_t.zones) && (pair_t.current->selection > 0))
		{
			if(pairingsett.pair_flags & PAIR_FLAG_EXT_CONNECTED)
			{
				pair_t.current = &pair_t.fancoil_id;
			}
			else
			{
				pair_t.current = &pair_t.connection_stat;
				fill_pair_value();
			}
			pair_t.flash = 1;
			pair_t.ts = millis();
			return pair_t.current ? 1 : 0;
		}
		else if(pair_t.current == &pair_t.fancoil_id)
		{
			pair_t.current = &pair_t.connection_stat;
			pair_t.flash = 1;
			pair_t.ts = millis();
			fill_pair_value();
			return pair_t.current ? 1 : 0;
		}
		else if(pair_t.current == &pair_t.node_id)
		{
			settings.node_id = pair_t.current->value;
//			mbslave_task_delete();
//			mbslave_task_init();
			pairingsett.status = PS_UNPAIRED;
			pair_failed_timout = millis();
			pair_t.current = pair_t.current->next;
			pair_t.flash = 1;
			pair_t.ts = millis();
			return pair_t.current ? 1 : 0;
		}
		else if((pair_t.current == &pair_t.cooling_application_pair) || (pair_t.current == &pair_t.cooling_method) || (pair_t.current == &pair_t.electric_floor_heating) || (pair_t.current == &pair_t.heating_method))
		{
			if(pair_t.current == &pair_t.electric_floor_heating)
			{
				if((is_electric_floor_allowed_pair_only) || (pair_t.electric_floor_heating.value == ELECTRIC_FLOOR_HEATING_ON))
				{
					pair_t.current->next = &pair_t.heating_method;
				}
				else
				{
					pair_t.heating_method.value = HEAT_HYDRONIC;
					pair_get_available_zones();
					pair_t.current->next = &pair_t.zones;
				}
			}
			else if(pair_t.current == &pair_t.heating_method)
			{
				pair_get_available_zones();
				pair_t.current->next = &pair_t.zones;
			}
			else if(pair_t.current == &pair_t.cooling_application_pair)
			{
				if((pair_t.cooling_application_pair.value > COOLING_NONE))
				{
					pair_t.current->next = &pair_t.cooling_method;
				}
				else
				{
					pair_t.current->next = &pair_t.electric_floor_heating;
				}				
			}
			pair_t.current = pair_t.current->next;
			pair_t.flash = 1;
			pair_t.ts = millis();
			return pair_t.current ? 1 : 0;
		}
	}

	if (e & (BTN_MASK_PRS(BTN_RIGHT))) 
	{
		if(pair_t.current->ui_type == UI_TYPE_IMAGE)
		{
			pair_t.current = pair_t.current->next;
		}
		else
		{
			home_pair_item_up(pair_t.current);
			pair_t.flash = 0;
			pair_t.ts = millis();
		}
	}
	else if (e & (BTN_MASK_PRS(BTN_LEFT))) 
	{
		if(pair_t.current->ui_type == UI_TYPE_IMAGE)
		{
			pair_t.current = pair_t.current->next;
		}
		else
		{
			home_pair_item_down(pair_t.current);
			pair_t.flash = 0;
			pair_t.ts = millis();
		}
	}

	if(pair_t.current == &pair_t.display)
	{
		set_disp(pair_t.current->value);
	}

	return 1;
}

static void update_pair_flow_strings()
{
	pair_t.language.header = text_pair[PAIR_LANG_HEADER][lang];
    pair_t.language.text[LCD_LANG_ET] = text_common[UI_EESTI][lang];
    pair_t.language.text[LCD_LANG_EN] = text_common[UI_ENGLISH][lang];

	pair_t.node_id.header = text_pair[PAIR_MB_ADDR][lang];
	pair_t.node_id.subtext[0] = text_pair[PAIR_CONF_NODEID][lang];
	pair_t.node_id.subtext[1] =text_pair[PAIR_HOLD_OK_BTN][lang];
	pair_t.node_id.value = settings.node_id;

  	pair_t.pairing_screen.header = text_pair[PAIR_HEADER][lang];
    pair_t.pairing_screen.text[0] = text_pair[PAIR_CONNECTING][lang];

    pair_t.zones.header = text_pair[PAIR_ZONES][lang];
    pair_t.zones.text[0] = text_pair[PAIR_SELECTED][lang];
    pair_t.zones.text[1] = text_pair[PAIR_HOLD_OK_BTN][lang];

	pair_t.fancoil_id.header = text_pair[PAIR_SELECT_FANCOIL][lang];
	// pair_t.fancoil_id.subtext[0] = text_pair[PAIR_HOLD_OK_BTN][lang];
	pair_t.fancoil_id.subtext[1] = text_pair[PAIR_HOLD_OK_BTN][lang];
	pair_t.fancoil_id.value = 0;

    pair_t.controlmode.header = text_pair[PAIR_CONTROL_MODE_HEADER][lang];
    pair_t.controlmode.text[CONTROLMODE_AIR_TEMP] = text_pair[PAIR_AIR_TEMP][lang];
	pair_t.controlmode.subtext[CONTROLMODE_AIR_TEMP] = text_pair[PAIR_AIR_TEMP_MSG][lang];
    pair_t.controlmode.text[CONTROLMODE_FLOOR_TEMP] = text_pair[PAIR_FLOOR_TEMP][lang];
	pair_t.controlmode.subtext[CONTROLMODE_FLOOR_TEMP] = text_pair[PAIR_FLOOR_TEMP_MSG][lang];

    pair_t.display.header =  text_pair[PAIR_DISPLAY_HEADER][lang];
    pair_t.display.text[DISPLAY_REGULAR] =  text_pair[PAIR_DISPLAY_REGULAR][lang];
	pair_t.display.subtext[DISPLAY_REGULAR] =  text_pair[PAIR_DISPLAY_REGULAR_MSG][lang];
    pair_t.display.text[DISPLAY_REVERTED] =  text_pair[PAIR_DISPLAY_REVERTED][lang];
	pair_t.display.subtext[DISPLAY_REVERTED] =  text_pair[PAIR_DISPLAY_REVERTED_MSG][lang];

    pair_t.connection_stat.header = text_pair[PAIR_HEADER][lang];
    pair_t.connection_stat.text[0] = text_pair[PAIR_CONTINUE][lang];
    pair_t.connection_stat.text[1] = NULL;
	pair_t.connection_stat.text[2] = NULL;
    pair_t.connection_stat.subtext[0] = text_pair[PAIR_OK][lang];
	pair_t.connection_stat.subtext[1] = NULL;
	pair_t.connection_stat.subtext[2] = NULL;

	pair_t.cooling_application_pair.header = text_pair[PAIR_FLOOR_APPLICATION_HEADER][lang];
	pair_t.cooling_application_pair.text[COOLING_NONE] = text_cooling_application[COOLING_NONE][lang];
	pair_t.cooling_application_pair.text[COOLING_FLOOR_ONLY] = text_cooling_application[COOLING_FLOOR_ONLY][lang];
	pair_t.cooling_application_pair.text[COOLING_FANCOIL_ONLY] = text_cooling_application[COOLING_FANCOIL_ONLY][lang];
	pair_t.cooling_application_pair.text[COOLING_FLOOR_AND_FANCOIL] = text_cooling_application[COOLING_FLOOR_AND_FANCOIL][lang];
	pair_t.cooling_application_pair.subtext[0] =text_pair[PAIR_HOLD_OK_BTN][lang];
	pair_t.cooling_application_pair.subtext[1] =text_pair[PAIR_HOLD_OK_BTN][lang];
	pair_t.cooling_application_pair.subtext[2] =text_pair[PAIR_HOLD_OK_BTN][lang];
	pair_t.cooling_application_pair.subtext[3] =text_pair[PAIR_HOLD_OK_BTN][lang];

	pair_t.cooling_method.header = text_pair[PAIR_COOLING_METHOD_HEADER][lang];
	pair_t.cooling_method.text[COOLING_METHOD_MANUAL] = text_pair[PAIR_COOLING_METHOD_MANUAL][lang];
	pair_t.cooling_method.text[COOLING_METHOD_AUTO] = text_pair[PAIR_COOLING_METHOD_AUTO][lang];
	pair_t.cooling_method.subtext[0] =text_pair[PAIR_HOLD_OK_BTN][lang];
	pair_t.cooling_method.subtext[1] =text_pair[PAIR_HOLD_OK_BTN][lang];

	pair_t.electric_floor_heating.header = text_pair[PAIR_ELECTRIC_FLOOR_HEATING_HEADER][lang];
	pair_t.electric_floor_heating.text[ELECTRIC_FLOOR_HEATING_OFF] = text_common[UI_NO][lang];
	pair_t.electric_floor_heating.text[ELECTRIC_FLOOR_HEATING_ON] = text_common[UI_YES][lang];
	pair_t.electric_floor_heating.subtext[0] =text_pair[PAIR_HOLD_OK_BTN][lang];
	pair_t.electric_floor_heating.subtext[1] =text_pair[PAIR_HOLD_OK_BTN][lang];

	pair_t.heating_method.header = text_pair[PAIR_FLOOR_HEATING_METHOD_HEADER][lang];
	pair_t.heating_method.text[HEATING_METHOD_HYDRONIC] = text_pair[PAIR_HEATING_METHOD_HYDRONIC][lang];
	pair_t.heating_method.text[HEATING_METHOD_ELECTRIC] = text_pair[PAIR_HEATING_METHOD_ELECTRIC][lang];
	pair_t.heating_method.text[HEATING_METHOD_HYDRONIC_ELECTRIC] = text_pair[PAIR_HEATING_METHOD_HYDRONIC_ELECTRIC][lang];
	pair_t.heating_method.text[HEATING_METHOD_AUTO] = text_pair[PAIR_HEATING_METHOD_AUTO][lang];
	pair_t.heating_method.subtext[0] =text_pair[PAIR_HOLD_OK_BTN][lang];
	pair_t.heating_method.subtext[1] =text_pair[PAIR_HOLD_OK_BTN][lang];
	pair_t.heating_method.subtext[2] =text_pair[PAIR_HOLD_OK_BTN][lang];
	pair_t.heating_method.subtext[3] =text_pair[PAIR_HOLD_OK_BTN][lang];
}

int8_t home_pair_start(uint32_t e) {
	pair_t.current = &pair_t.airobot_logo;

	update_pair_flow_strings();

	pairingsett.status = PS_UNPAIRED;
	e &= ~(BTN_MASK_PRS(BTN_OK));
	home_pair_handle(e);
    myGUI.begin();
	return 0;
}

void fill_global_settings()
{
	if(!(global_settings.flags & SET_FLAG_GLOBAL_SETT_CONFIGURED))
	{
		global_settings.floor_cooling_act_method = pair_t.cooling_method.value;
		global_settings.cooling_application = pair_t.cooling_application_pair.value;

		if(pair_t.electric_floor_heating.value == ELECTRIC_FLOOR_HEATING_ON)
			is_electric_floor_allowed_pair_only = 1;
		else
			is_electric_floor_allowed_pair_only = 0;
		settings.floor_heat_applic = pair_t.heating_method.value;
	}
}

void fill_pair_value(){
	settings.control_input = pair_t.controlmode.value;
	settings.zone_id = pair_t.zones.selection;
	if(pair_t.fancoil_id.value > 0)
		ext_sett.fancoil_id = (uint8_t)(1<<(pair_t.fancoil_id.value-1));
	fill_global_settings();

	hvac_conf_change(FLAGD_SENSOR_SETT_UPDATED);
	hvac_conf_change(FLAGD_EXT_SETT_UPDATED);
	pairingsett.status = PS_READY;
	pair_t.zones.selection = 0;
	pair_t.zones.value = 0;
    ESP_LOGI(PAIR_TAG, "Pair value filled");
}

void home_pair_finish() {
    ESP_LOGI(PAIR_TAG, "home_pair_finished");
}

void gfx_render_pair_page(void)
{
    uint16_t x, y;
	char num_to_str[50] = {0};

    myScreen.setOrientation(myOrientation);
    
	// Header
    x = 10;
	if(pair_t.current->max == 4)
   		y = 25;
	else 
		y = 30;
		
	if(pair_t.current->header != NULL)
	{
    	text_render_center_horizontal_multiple_line(pair_t.current->header, font_19px, LCD_W-20, &x, &y);
	}

	switch(pair_t.current->ui_type)
	{
		case UI_TYPE_BUTTON:
		{
			// Buttons
			x = LCD_W / 10;
			y = LCD_H / 8;
    		myGUI.delegate(false);
			for(int i=0; i<(pair_t.current->max); i++)
			{
				Button myButton(&myGUI);
				myButton.dDefine(x, (((LCD_H-(pair_t.current->max * y))/2)+(y*i)), x*8, y-2, setItem(i, pair_t.current->text[i]), font_19px);
				myButton.setOption(optionStandard);
				myButton.setState(stateEnabled);
				myButton.setStyle(styleRounded);
				if(pair_t.current->value == i)
					myButton.draw(fsmTouched);
				else
					myButton.draw(fsmReleased);   
			}
    		myGUI.delegate(true);

			// SubText
			if(pair_t.current->subtext[pair_t.current->value] != NULL)
			{
				x = 10;
				y = LCD_H / 8;
				y = ((LCD_H-(pair_t.current->max * y))/2)+(y*pair_t.current->max);
				y += 20;
				if(!(y>220 && y<260))
				{
					y = 220;
				}
				text_render_center_horizontal_multiple_line(pair_t.current->subtext[pair_t.current->value], font_16px, LCD_W-20, &x, &y);
			}
		}
		break;

		case UI_TYPE_SELECTION:
		{
			x = 0;
			y = 0;
//			text_render_center_horizontal_vertical(itoa(available_zone[pair_t.current->value], num_to_str, 10), font_L_65px, LCD_W, LCD_H, &x, &y);
			
			x = 10;
			y = (LCD_H - ICON_H)/2;
			myScreen.showImage(x, y, ICON_W, ICON_H, (uint8_t *)icon_arrow_left, myColours.black);

			x=LCD_W - ICON_W - 10;
			myScreen.showImage(x, y, ICON_W, ICON_H, (uint8_t *)icon_arrow_right, myColours.black);

			x = 10;
			y = 210;
			text_render_center_horizontal(pair_t.current->text[0], font_16px, LCD_W-20, &x, &y,0);

			memset(num_to_str, 0x00, sizeof(num_to_str));
			uint8_t j = 0;
			for(uint8_t i=0; i<MAX_ZONE; i++)
			{
				if(pair_t.current->selection & 1<<i)
				{
					sprintf(&num_to_str[j], "%d ", i+1);
					j += 2;
				}
			}
			x = 0;
			text_render_center_horizontal(num_to_str, font_19px, LCD_W, &x, &y,0);

			if(pair_t.current->selection > 0)
			{
				x = 10;
				y += 5;
				text_render_center_horizontal_multiple_line(pair_t.current->text[1], font_16px, LCD_W-20, &x, &y);
			}
		}
		break;

		case UI_TYPE_SELECT:
		{
			x = 0;
			y = 0;
			if(pair_t.current == &pair_t.node_id)
				pair_t.current->value = (pair_t.current->value>0)?pair_t.current->value:1;
			
//			if((pair_t.current->value == 0) && (pair_t.current == &pair_t.fancoil_id))
				text_render_center_horizontal_vertical("None", font_L_65px, LCD_W, LCD_H, &x, &y);
//			else
//				itoa(pair_t.current->value, num_to_str, 10);

			text_render_center_horizontal_vertical(num_to_str, font_L_65px, LCD_W, LCD_H, &x, &y);

			if(pair_t.current->subtext[0] != NULL)
			{
				x = 10;
				y = 60;
				text_render_center_horizontal_multiple_line(pair_t.current->subtext[0], font_16px, LCD_W-20, &x, &y);
			}

			if(pair_t.current->subtext[1] != NULL)
			{
				x = 10;
				y = 230;
				text_render_center_horizontal_multiple_line(pair_t.current->subtext[1], font_16px, LCD_W-20, &x, &y);
			}
		
			x = 10;
			y = (LCD_H - ICON_H)/2;
			myScreen.showImage(x, y, ICON_W, ICON_H, (uint8_t *)icon_arrow_left, myColours.black);

			x=LCD_W - ICON_W - 10;
			myScreen.showImage(x, y, ICON_W, ICON_H, (uint8_t *)icon_arrow_right, myColours.black);
		}
		break;

		case UI_TYPE_TEXT:
		{			
			y = 60;
			uint8_t i = 0;
			for(i=0; i<pair_t.current->max; i++ )
			{
				if(pair_t.current->text[i] != NULL)
				{	
					x = 10;
					text_render_center_horizontal_multiple_line(pair_t.current->text[i], font_16px, LCD_W-20, &x, &y);
					y += 10;
				}
			}
			i=0;
			y = (y<250)?250:(y+20);
			for(i=0; i<pair_t.current->max; i++ )
			{
				if(pair_t.current->subtext[i] != NULL)
				{	
					x = 10;
					text_render_center_horizontal_multiple_line(pair_t.current->subtext[0], font_16px, LCD_W-20, &x, &y);
				}
			}
		}
		break;

		case UI_TYPE_IMAGE:
		{
			x = (LCD_W - LOGO_W)/2;
			y = (LCD_H - LOGO_H)/2;
			myScreen.showImage(x, y, LOGO_W, LOGO_H, (uint8_t *)airobot_logo, myColours.black);
			x = 10;
			y += LOGO_H;
			if(is_scd4x_sensor_present)
				text_render_center_horizontal_multiple_line("Room Sensor SE1-AQ", font_16px, LCD_W-20, &x, &y);
			else
				text_render_center_horizontal_multiple_line("Room Sensor SE1", font_16px, LCD_W-20, &x, &y);

			static char fw[10];
			ver2str(status.fw_ver, fw, sizeof(fw));
			x = 10;
			text_render_center_horizontal_multiple_line(fw, font_16px, LCD_W-20, &x, &y);
		}
		break;

		default:
		break;

	};
}

void home_pair_render() {
	gfx_render_clear();

	gfx_render_pair_page();

	gfx_render_finish();
}