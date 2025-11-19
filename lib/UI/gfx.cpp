/*
 * gfx.c
 *
 *  Created on: 23.07.2014
 *      Author: anti
 */
#include "stringcnv.h"
#include "font_render.h"

#include "string.h"
#include "common_def.h"
#include <math.h>
#include "settings.h"
#include "disp_common.h"
#include "icons.h"
//#include "aqi.h"
#include "val2str.h"
#include "status.h"
#include "buttons.h"
#include "ui.h"
#include "pair.h"
#include "home.h"
#include "hV_CodeQR.h"
//#include "mb_ota.h"
#include "gfx.h"

#define MAX_HOMEPAGE_IMEGS      6

extern uint8_t screen_changed;
uint8_t* imgs_to_display[MAX_HOMEPAGE_IMEGS] = {NULL};
uint8_t is_force_lcd_update = 0;
uint8_t is_fancoil_view_enabled = 0, is_co2_view_enabled =0;

void gfx_text_right_unit(const char* val, const char *unit, uint16_t w, uint8_t line, uint8_t sel)
{
    if(sel)
        sel = SEL_INVERT_WITH_BG;
	uint16_t rw = text_render_menu_right(unit, font_16px, w, line, sel);
    if(sel)
        sel = SEL_INVERT_TEXT;
	text_render_menu_right(val, font_16px, rw, line, sel);
}

void gfx_render_clear()
{
	myScreen.clear();
}

uint32_t get_screen_refresh_interval()
{
    if((millis() - get_time_since_last_interaction()) <= USER_INTEREACTION_TIME_MS)    // User-intereaction cooling period
    {
        return REFRESH_TIME_WHILE_INTERACTION_MS;
    }
    else
    {
        return DEFAULT_SCREEN_REFRESH_TIME_MS;
    }
}

void set_lcd_force_update()
{
    is_force_lcd_update = 1;
}

uint8_t is_lcd_force_update_require()
{
    return is_force_lcd_update;
}

void gfx_render_finish()
{
    uint32_t now = millis();
    static uint32_t last_updated = 0;

    if((now - last_updated > get_screen_refresh_interval()) || is_lcd_force_update_require() || (is_buttons_event_updated()) || (millis() < REFRESH_TIME_WHILE_INITIALIZATION_MS) || (get_current_sceen_hsm() != &ui.hsm[HSM_HOME]))
    {
        if(myScreen.ScreenBufferUpdated())
        {
            is_force_lcd_update = 0;
            myScreen.flushMode(UPDATE_FAST);
        }
        last_updated = now;
    }
}

void gfx_display_off()
{
	gfx_render_clear();
//	lcd_display_off();
}

void gfx_display_on()
{
//	lcd_display_on();
}

void gfx_render_menu_settings_item(uint8_t line, const char *text, const char *value, const char *unit, uint8_t sel)
{
    uint16_t font, x, y, w, h, xoff, yoff;
    int8_t yoffset;

    xoff = 12;
	yoff = 62;
	w = 128;
	h = 53;

    if (sel)
    {
        sel = SEL_INVERT_TEXT;
    }

    x = 0;
    font = font_19px;
    yoffset = 4;

    if(sel)
    {
        x = xoff;
        y = (h*line)+yoff;
        myScreen.setPenSolid(true);
        myScreen.dRoundedRectangle(x, y, w, h, 4, myColours.black);
        myScreen.setPenSolid(false);
    }

    x = xoff;
    y = (h*line)+yoff+3;
    text_render_center_horizontal(text, font_16px, w, &x, &y, sel);
    x = xoff;
    y += yoffset;
    text_render_center_horizontal(value, font, w, &x, &y, sel);
}

void gfx_render_menu_cmd(uint8_t line, const char *text, uint8_t sel)
{
    if(sel)
        sel = SEL_INVERT_WITH_BG;
	uint16_t w = text_render_menu_right(">", font_16px, LCD_W, line, sel);
    if(sel)
        sel = SEL_INVERT_TEXT;
	text_render_menu_left(text, font_16px, w, line, sel);
}

void gfx_render_error(const char* header,const char *line1, const char *line2)
{
    char numberstr[50] = "";
    uint16_t x = 0;
    uint16_t y = 0;

    uint8_t xoffset = 5;
    uint8_t yoffset = 6;
    uint8_t rw;
    uint8_t E_ICON_W = 100;
    uint8_t E_ICON_H = 100;
    myScreen.setOrientation(myOrientation);

    x = 0;
    y = 21 - yoffset;
    if(header != NULL)
        text_render_center_horizontal(header, font_30px, LCD_W, &x, &y,SEL_NONE);

    x = 32;
    y = 64;
    myScreen.showImage(x - 7, y, E_ICON_W, E_ICON_H, (uint8_t *)icon_error, myColours.black);

    x = 0;
    y = 166;
    if(line1 != NULL)
        text_render_center_horizontal_multiple_line(line1, font_16px, LCD_W, &x, &y);
    else
        get_error_status();


    x = 0;
    y = 234;
    if(line2 != NULL)
        text_render_center_horizontal_multiple_line(line2, font_16px, LCD_W, &x, &y);
}

void gfx_render_balance_edit(const char* balance)
{

}

void gfx_render_menu_item(uint8_t line, const char *text, const char *value, const char *unit, uint8_t sel)
{
	uint16_t w = LCD_W;
    //hV_HAL_Debug_println("%d, %s, %s, %s, %d", __LINE__, text, value, unit, sel);
    if(sel)
        sel = SEL_INVERT_WITH_BG;
	if (unit) 
    {
        w = text_render_menu_right(unit, font_16px, w, line, sel);
        if(sel)
            sel = SEL_INVERT_TEXT;
    }
	if (value)
    {
        w = text_render_menu_right(value, font_16px, w, line, sel);
        if(sel)
            sel = SEL_INVERT_TEXT;
    }
	if (text)
    {
        text_render_menu_left(text, font_16px, w, line, sel);
    }
}

void gfx_render_home_status(uint16_t temp, uint16_t hum, uint16_t co2)
{
    char numberstr[50] = "";
    uint16_t x = 10;
    uint16_t y = 10;
    uint16_t dyUnit = 10;
    uint16_t dyRead = 20;
    uint16_t offset = 0;
    uint8_t img_cnt = 0;
    uint8_t aqi_level = MODERATE_AQI;//get_aqi_level(co2);

    if((temp == 0)&&(hum == 0)&&(co2 == 0))
    {
        return;
    }

    is_fancoil_view_enabled = (hec_extended_status.hec_flags_dup & STATUS_COOLING_FANCOIL) ? 1 : 0;
    is_co2_view_enabled = (co2 != CO2_INVALID) ? 1 : 0;

    myScreen.selectFont(font_16px);
    offset = myScreen.characterSizeY()/2;

    myScreen.setOrientation(myOrientation);

    //ICONs at upper side
    for(uint8_t i = 0; i<MAX_HOMEPAGE_IMEGS; i++)
    {
        imgs_to_display[i] = NULL;
    }

    if(ext_sett.icon_stat & ICON_HEATING)
    {
        imgs_to_display[img_cnt++] = (uint8_t *)&icon_heating[0];
    }

    if(ext_sett.icon_stat & ICON_AWAY_MODE)
    {
        imgs_to_display[img_cnt++] = (uint8_t *)&icon_away[0];
    }

    if(ext_sett.icon_stat & ICON_COOLING)
    {
        imgs_to_display[img_cnt++] = (uint8_t *)&icon_cooling[0];
    }

    if(ext_sett.icon_stat & ICON_HEAT_ELEC)
    {
        imgs_to_display[img_cnt++] = (uint8_t *)&icon_heat_elec[0];
    }

    if (ext_sett.icon_stat & ICON_WINDOW_OPEN)
    {
        imgs_to_display[img_cnt++] = (uint8_t *)&icon_window_open[0];
    }

    if(ext_sett.icon_stat & ICON_HUM_BLOCKED)
    {
        imgs_to_display[img_cnt++] = (uint8_t *)&icon_block[0];
    }
    
    x = (LCD_W - (img_cnt*(ICON_W+7)))/2;
    y = (is_co2_view_enabled && is_fancoil_view_enabled)? 10 : 18;

    // Show icons
    while(img_cnt>0)
    {
        x += 3;
        myScreen.showImage(x, y, ICON_W, ICON_H, imgs_to_display[img_cnt-1], myColours.black);
        imgs_to_display[img_cnt-1] = NULL;
        x += ICON_W + 4;
        img_cnt --;
    }

    // Temperature 
    memset(numberstr, 0, 50);
    temp2str(temp, numberstr, sizeof(numberstr));
    x = 0;
    if(is_co2_view_enabled && is_fancoil_view_enabled) 
    {
        y = 32;
    }
    else if(is_co2_view_enabled || is_fancoil_view_enabled)
    {
        y = 55;
    }
    else
    {
        y = 90;
    }
    text_render_center_horizontal(numberstr, font_L_65px, LCD_W, &x, &y, 0);
    x = 55;

    y -= (is_co2_view_enabled && is_fancoil_view_enabled) ? 11 : 8; // Adjust y position based on CO2 view enabled
    if(settings.control_input == CONTROLMODE_FLOOR_TEMP)
    {
        myScreen.showImage(x, y, ICON_W, ICON_H, (uint8_t *)&icon_floor[0], myColours.black);
        x = 10;
        text_render_center_horizontal("\xb0""C", font_16px, LCD_W-10, &x, &y, 0);
    }
    else
    {
        x = 0;
        text_render_center_horizontal("\xb0""C", font_16px, LCD_W, &x, &y, 0);
    }

    // FAN coil speed
    if(is_fancoil_view_enabled)
    {
        uint16_t height = 16, width = 8;

        x = (LCD_W - (3*(width+4)))/2;
        y += 24;

        const uint8_t* bars[3][3] = {
            {icon_bar_w8_h16, icon_bar_filled_w8_h16},
            {icon_bar_w8_h24, icon_bar_filled_w8_h24},
            {icon_bar_w8_h32, icon_bar_filled_w8_h32}
        };
        for(int i=0; i<3; i++) 
        {
            const uint8_t* icon = (hec_extended_status.fancoil_speed > i) ? bars[i][1] : bars[i][0];
            myScreen.showImage(x, y, width, height, (uint8_t*)icon, myColours.black);
            x += 12; // spacing between bars
            y -= 8;
            height += 8;
        }
        y += 40;
        x = 0;
        if(ext_sett.fancoil_mode == FANCOIL_MODE_AUTO)
        {
            text_render_center_horizontal(text_fancoil_mode_ext[FANCOIL_MODE_EXT_AUTO_COOL][lang], font_16px, LCD_W, &x, &y, 0);
        }
        else
        {
            text_render_center_horizontal(text_fancoil_mode_ext[FANCOIL_MODE_EXT_MANUAL_COOL][lang], font_16px, LCD_W, &x, &y, 0);
        }
    }

    // Co2
    if(is_co2_view_enabled)
    {
        memset(numberstr, 0, 50);
        co2str(co2, numberstr, sizeof(numberstr));
        x = 0;
        y += 6;
        text_render_center_horizontal(numberstr, font_30px, LCD_W, &x, &y, 0);
        y -= 5;
        x = 0;
        text_render_center_horizontal("ppm", font_16px, LCD_W, &x, &y, 0);
    }

    // Humidity 
    memset(numberstr, 0, 50);
    hum2str(hum/10, numberstr, sizeof(numberstr));
    x = 0;
    y += 14;
    y -= (is_co2_view_enabled || is_fancoil_view_enabled) ? 8 : 0;
    text_render_center_horizontal(numberstr, font_30px, LCD_W, &x, &y, 0);
    y -= (is_co2_view_enabled && is_fancoil_view_enabled)? 5 : 3;
    x = 0;
    text_render_center_horizontal("%", font_16px, LCD_W, &x, &y, 0);

    // AQI level
    if(is_co2_view_enabled)
    {
        y += 5;
        x = 16;
        y = (y < 261) ? 261 : y;

        for(int i=0; i<aqi_level; i++)
        {
            myScreen.showImage(x, y, ICON_W, ICON_H, (uint8_t *)&icon_aqi_select[0], myColours.black);
            x += 6;
        }
        for(int i=0; i<(5-aqi_level); i++)
        {
            myScreen.showImage(x, y, ICON_W, ICON_H, (uint8_t *)&icon_aqi_unselect[0], myColours.black);
            x += 6;
        }
        x = 72;
//        text_render_left(get_aqi_level_str((aqi_e)aqi_level), font_16px, LCD_W, x, y);
    }
}

void gfx_render_edit_page(int32_t value, uint8_t type)
{
    char numberstr[50] = "";
    uint16_t x = 10;
    uint16_t y = 10;
    uint16_t dyRead = 20;
    uint16_t offset = 0;

    myScreen.selectFont(font_16px);
    offset = myScreen.characterSizeY()/2;
    myScreen.setOrientation(myOrientation);
    memset(numberstr, 0x00, sizeof(numberstr));

    if(type == EDIT_TYPE_SETPOINT_TEMP)
    {
        sprintf(numberstr, "%.1f", (float)value/10);
        if (settings.control_input == CONTROLMODE_FLOOR_TEMP)
        {
            x = 0; y = 20;
            text_render_center_horizontal(text_common[UI_FLOOR][lang], font_16px, LCD_W, &x, &y, 0);
        }
        else
        {
            x = 0; y = 20;
            text_render_center_horizontal(text_common[UI_AIR][lang], font_16px, LCD_W, &x, &y, 0);
        }
        x = 0; y = 35;
        text_render_center_horizontal(text_common[UI_TEMPERATURE][lang], font_16px, LCD_W, &x, &y, 0);
    }
    else if(type == EDIT_TYPE_TEMP_CAL)
    {
        sprintf(numberstr, "%.1f", (float)value/10);
        x = 0; y=35;
        text_render_center_horizontal(text_sys_settings[SYS_SETTINGS_TEMP_CALIBRATION][lang], font_16px, LCD_W, &x, &y, 0);
        x = 0;
        y = 220;
        text_render_center_horizontal_multiple_line(text_pair[PAIR_TEMP_CALIB_MSG][lang],font_16px, LCD_W, &x, &y);
    }
    else if(type == EDIT_COOLING_HUMIDITY)
    {
        sprintf(numberstr, "%d", value/10);
        x = 0; y=35;
        text_render_center_horizontal(text_global_settings[GLOBAL_BLOCK_COOLING_HUM][lang], font_16px, LCD_W, &x, &y, 0);
    }
    else if(type == EDIT_TYPE_ZONE_HYSTERESIS)
    {
        sprintf(numberstr, "%.1f", (float)value/10);
        x = 0; y=35;
//        text_render_center_horizontal(text_global_settings[GLOBAL_HYSTERESIS][lang], font_16px, LCD_W, &x, &y, 0);
    }
    else if(type == EDIT_FANCOIL_MODE)
    {
        x = 0; y=35;
        text_render_center_horizontal(text_common[UI_FANCOIL_MODE_HEADER][lang], font_16px, LCD_W, &x, &y, 0);
    }
    else if(type == EDIT_FANCOIL_HYSTERESIS)
    {
        sprintf(numberstr, "%.1f", (float)value/10);
        x = 0; y=35;
        text_render_center_horizontal(text_zone_settings[ZONE_FANCOIL_HYSTERESIS][lang], font_16px, LCD_W, &x, &y, 0);
    }
    else if(type == EDIT_MB_BAUDRATE)
    {
        sprintf(numberstr, "%d", get_baudrate_value((baudrate_t)value));
        x = 0; y=35;
        text_render_center_horizontal(text_zone_settings[ZONE_MB_BAUDRATE][lang], font_16px, LCD_W, &x, &y, 0);
    }
    else if(type == EDIT_SELECTED_ZONES)
    {
        x = 10;
        y = 30;
        text_render_center_horizontal_multiple_line(text_pair[PAIR_ZONES][lang], font_19px, LCD_W-20, &x, &y);

        gfx_render_arrows();

        x = 10;
        y = 210;
        text_render_center_horizontal(text_pair[PAIR_SELECTED][lang], font_16px, LCD_W-20, &x, &y,0);

        uint8_t j = 0;
        for(uint8_t i=0; i<MAX_ZONE; i++)
        {
            if(edit_selection_zone & 1<<i)
            {
                sprintf(&numberstr[j], "%d ", i+1);
                j += 2;
            }
        }
        x = 0;
        text_render_center_horizontal(numberstr, font_19px, LCD_W, &x, &y,0);

        if(edit_selection_zone > 0)
        {
            x = 10;
            y += 5;
            text_render_center_horizontal_multiple_line(text_pair[PAIR_HOLD_OK_BTN][lang], font_16px, LCD_W-20, &x, &y);
        }

        memset(numberstr, 0x00, sizeof(numberstr));
        sprintf(numberstr, "%d", edit.current->value);
    }
    
    x = 0;
    y = 90;
    if(type == EDIT_FANCOIL_MODE)
    {
        text_render_center_horizontal(text_fancoil_mode[value][lang], font_45px, LCD_W, &x, &y, 0);
    }
    else if(type == EDIT_MB_BAUDRATE)
    {
        text_render_center_horizontal(numberstr, font_45px, LCD_W, &x, &y,0);
    }
    else
    {
        text_render_center_horizontal(numberstr, font_L_65px, LCD_W, &x, &y, 0);
    }
        
    x = 0;
    if(type == EDIT_COOLING_HUMIDITY)
    {
        text_render_center_horizontal("%", font_16px, LCD_W, &x, &y,0);
    }
    else if((type == EDIT_TYPE_ZONE_HYSTERESIS) || (type == EDIT_FANCOIL_MODE) || (type == EDIT_MB_BAUDRATE) || (type == EDIT_SELECTED_ZONES))
    {
        // nothing to print
    }
    else
    {
        text_render_center_horizontal("\xb0""C", font_16px, LCD_W, &x, &y,0);
    }
    setpoint_screen_popup_success();
}

void gfx_render_mobile_app(void)
{
    char numberstr[50] = "";
    uint16_t x = 0;
    uint16_t y = 0;
    uint16_t dz = 0;
    bool result;
    char id[12]= "-";
    char pass[10]= "-";
    myScreen.setOrientation(myOrientation);

	// Header
    x = 5;
    y = 20;
    text_render_center_horizontal(text_menu[MAIN_MOBILE_APP][lang], font_19px, LCD_W-10, &x, &y, 0);

    if(dev_config.is_device_configured)
    {  
        x = 10;
        y = 50;
	    text_render_center_horizontal_multiple_line(text_mobileapp[MOBILE_APP_STEP_1][lang],font_16px, LCD_W-20, &x, &y);

        x = 10;
        text_render_center_horizontal_multiple_line(text_mobileapp[MOBILE_APP_MSG_1][lang], font_16px, LCD_W-20, &x, &y);

        x = 10;
        y = 100;
	    text_render_center_horizontal_multiple_line(text_mobileapp[MOBILE_APP_STEP_2][lang],font_16px, LCD_W-20, &x, &y);

        x = 10;
        text_render_center_horizontal_multiple_line(text_mobileapp[MOBILE_APP_MSG_2][lang], font_16px, LCD_W-20, &x, &y);

        x = myScreen.screenSizeX();
        y += 15;
        myScreen.selectFont(font_16px);
        sprintf(numberstr, "H%08X;%s", dev_config.deviceid, dev_config.password);
        dz = getSizeCodeQR(2);
        myScreen.setFontSolid(true);
        result = codeQR(&myScreen, (x - dz) / 2, y, numberstr, 2);
        sprintf(&id[0], "H%08X", dev_config.deviceid);
        sprintf(&pass[0], "%s", dev_config.password);

        x = 10;
        y = 250;
        sprintf(numberstr, "%s %s",text_mobileapp[MOBILE_APP_ID][lang], &id[0]);
        text_render_center_horizontal_multiple_line(
            numberstr,
            font_16px, LCD_W-20, &x, &y);
        x = 10;
        sprintf(numberstr, "%s %s",text_mobileapp[MOBILE_APP_PASS][lang], &pass[0]);
        text_render_center_horizontal_multiple_line(numberstr, font_16px, LCD_W-20, &x, &y);
    }
    else
    {
        x = 10;
        y = 120;
	    text_render_center_horizontal_multiple_line(text_mobileapp[MOBILE_APP_ERROR_MSG][lang],font_16px, LCD_W-20, &x, &y);
    }
}

void gfx_render_home_mode(const char *mode)
{
//	text_render_menu_left(mode, FONT_BOLD, LCD_W, gfxbuf + LCD_W + 3);
}

void gfx_render_ui_disabled()
{
	uint32_t t = millis() % 4000;
	if (t < 3000)
	{
		myScreen.selectFont(font_16px);
		text_render_menu_center(text_common[UI_LOADING][lang], font_16px, LCD_W, 6, 0);
	}
	//hV_HAL_Debug_println("%s:%d\r\n",__func__,__LINE__);
}

void gfx_render_serial_no()
{
    char numberstr[50] = "";
	uint32_t t = millis() % 4000;
	if (t < 3000)
	{
		myScreen.selectFont(font_16px);
        sprintf(numberstr, "%X", status.serial_no);
		text_render_menu_center(text_common[UI_SERIAL_ID][lang], font_16px, LCD_W, 5, 0);
        text_render_menu_center(numberstr, font_16px, LCD_W, 6, 0);
	}
}

void gfx_highlight_line(uint8_t line)
{

}
void gfx_render_dots(uint16_t x, uint16_t y, uint16_t current_value, uint16_t max)
{
    for (uint8_t i = 1; i <= max; i++)
    {
        myScreen.showImage(x + 7 * i, y, ICON_W, ICON_H, (uint8_t *)icon_aqi_unselect, myColours.black);
    }
    myScreen.showImage(x + 7 * current_value, y, ICON_W, ICON_H, (uint8_t *)icon_aqi_select, myColours.black);
}

void gfx_render_ota()
{
    uint16_t x ,y;
    uint8_t yoffset = 6;
    char numberstr[50] = "";
    uint8_t x_logo=70;
    uint8_t y_logo=70;
    uint16_t download_percentage = 0;
     
    x = 0;
    y = 21 - yoffset;
    text_render_center_horizontal(text_OTA[OTA_STAT_TITLE][lang], font_30px, LCD_W, &x, &y,0);

    x =  (LCD_W - x_logo) / 2;
    y =  (LCD_H - y_logo) / 2;
    myScreen.showImage(x, y-20, x_logo, y_logo, (uint8_t *)ota_logo, myColours.black);

    x = 10;
    y = 210 - yoffset;
//    text_render_center_horizontal_multiple_line(text_OTA[get_ota_status(&download_percentage)][lang], font_16px, LCD_W - 14, &x, &y);

    x = 10;
    y = 240 - yoffset;
    memset(numberstr, 0, 50);
    sprintf(numberstr, "%s %d%s", text_OTA[OTA_STAT_DOWNLOAD][lang],download_percentage,"%");
    text_render_center_horizontal_multiple_line(numberstr, font_16px, LCD_W - 14, &x, &y);
}

void gfx_render_arrows()
{
    uint16_t x = 10;
    uint16_t y = 122; // (LCD_H - ICON_H) / 2;
    myScreen.showImage(x, y, ICON_W, ICON_H, (uint8_t *)icon_arrow_left, myColours.black);

    x=LCD_W - ICON_W - 10;
    myScreen.showImage(x, y, ICON_W, ICON_H, (uint8_t *)icon_arrow_right, myColours.black);
}

void gfx_render_left_arrow(uint16_t x, uint16_t y)
{

    myScreen.showImage(x, y, ICON_W, ICON_H, (uint8_t *)icon_arrow_left, myColours.black);
}

void gfx_render_right_arrow(uint16_t x, uint16_t y)
{
    myScreen.showImage(x, y, ICON_W, ICON_H, (uint8_t *)icon_arrow_right, myColours.black);
}