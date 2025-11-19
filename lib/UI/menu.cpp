/*
 * menu.c
 *
 *  Created on: Jun 25, 2021
 *      Author: Biren
 */

#include <stddef.h>

#include "buttons.h"
#include "gfx.h"
#include "menu.h"
#include "ui.h"

#include "disp_common.h"
#include "font_render.h"

#define MENU_BUTTON_X		118
#define MENU_BUTTON_Y		27
#define MENU_BUTTON_SPACE	4

#define UI_MAX_LINES 9

static GUI myGUI(&myScreen);
void menu_render_about_ui(menu_t* menu);


static void menu_render_item(uint8_t line, const menu_item_t* item, uint8_t sel)
{
	const char* val = item->element.value ? item->element.value() : 0;
	gfx_render_menu_item(line, &item->element.text[lang * STRING_LEN_20], val, item->element.unit, sel);
}

static void menu_render_center_button(uint8_t line, const menu_item_t* item, uint8_t len, uint8_t sel)
{
	uint16_t x, y, dx, dy;

	//ESP_LOGI("MENU", "line: = %d, len = %d, sel = %d, text = %s", line, len, sel, item->element.text);

	myGUI.delegate(false);

	x = (myScreen.screenSizeX() - MENU_BUTTON_X)/2;
	dy = (MENU_BUTTON_Y * len) + (MENU_BUTTON_SPACE * (len-1));
	y = ((myScreen.screenSizeY() - dy)/2) + (line * (MENU_BUTTON_Y+MENU_BUTTON_SPACE));

	dx = MENU_BUTTON_X;
	dy = MENU_BUTTON_Y;

	Button myButton(&myGUI);
	// ESP_LOGI("MENU", "x = %d, y = %d, dx = %d, dy = %d", x, y, dx, dy);
	myButton.dDefine(x, y, dx, dy, setItem(line, &(item->element.text[lang*STRING_LEN_20])), font_19px);
	myButton.setOption(optionStandard);
	myButton.setState(stateEnabled);
	myButton.setStyle(styleRounded);
	if(sel == 1)
		myButton.draw(fsmTouched);
	else
		myButton.draw(fsmReleased);   

	myGUI.delegate(true);
}

static void menu_up(menu_t* menu)
{
	if (menu == 0 || menu->selected->prev == 0)
	{
		return;
	}
	menu->selected = menu->selected->prev;
	if (menu->line == 1 && menu->selected->prev)
	{
		return;
	}
	menu->line--;
}

static void menu_down(menu_t* menu)
{
	if (menu == 0 || menu->selected->next == 0)
	{
		return;
	}
	menu->selected = menu->selected->next;
	if (menu->line == UI_MAX_LINES - 2 && menu->selected->next)
	{
		return;
	}
	menu->line++;
}

static menu_t* menu_item_event(menu_t* menu, uint32_t event)
{
	if (menu == 0)
	{
		return 0;
	}
	if (event & BTN_MASK_PRS(BTN_BACK))
	{
		return menu->parent;
	}

	const menu_item_t* it = menu->selected;
	if (!( (event & BTN_MASK_PRS(BTN_OK)) || (event & BTN_MASK_PRS(BTN_LONGP_OK)) ))
	{
		return menu;
	}
	menu_t* active = it->element.action ? it->element.action(menu, event) : menu;
	if (active != menu)
	{
		menu_enter(active);
	}
	return active;
}

static int8_t menu_find_item(menu_t* menu, menu_item_t* item)
{
	if (menu == 0 || menu->len == 0 || item == 0)
	{
		return -1;
	}
	menu_item_t* it;
	for (it = menu->selected; it; it = it->prev)
	{
		if (it != item)
		{
			continue;
		}
		int8_t i;
		for (i = 0; it; it = it->prev, ++i);
		return i;
	}
	for (it = menu->selected->next; it; it = it->next)
	{
		if (it != item)
		{
			continue;
		}
		int8_t i;
		for (i = 0; it; it = it->next, ++i);
		return menu->len - i;
	}
	return -1;
}

static menu_item_t* menu_get_item(menu_t* menu, uint8_t pos)
{
	if (menu == 0 || pos >= menu->len)
	{
		return 0;
	}
	menu_item_t* it;
	for (it = menu->selected; it->prev; it = it->prev);
	for (; pos; it = it->next, --pos);
	return it;
}

void menu_enter(menu_t* menu)
{
	if (menu == 0)
	{
		return;
	}
	menu->line = 0;
	menu->selected = menu_get_item(menu, 0);
}

void menu_remove(menu_t* menu, menu_item_t* item)
{
	if (menu == 0 || item == 0 || menu->len == 0)
	{
		return;
	}
	int8_t i = menu_find_item(menu, item);
	if (i < 0)
	{
		return;
	}
	int8_t s = menu_find_item(menu, menu->selected);
	if (item->next)
	{
		item->next->prev = item->prev;
	}
	if (item->prev)
	{
		item->prev->next = item->next;
	}
	if (item == menu->selected)
	{
		menu->selected = s == 0 ? item->next : item->prev;
	}
	if ((i <= s) && (s < UI_MAX_LINES) && (s != 0))
	{
		menu->line--;
	}
	menu->len--;
	item->next = 0;
	item->prev = 0;
}

void menu_insert(menu_t* menu, uint8_t pos, menu_item_t* item)
{
	if (menu == 0 || item == 0 || (menu_find_item(menu, item) >= 0))
	{
		return;
	}
	if (pos >= menu->len)
	{
		menu_item_t* it = menu_get_item(menu, menu->len - 1);
		++menu->len;
		item->prev = it;
		if (it != 0)
		{
			it->next = item;
			return;
		}
		menu->selected = item;
		menu->line = 0;
		return;
	}
	menu_item_t* it = menu_get_item(menu, pos);
	int8_t s = menu_find_item(menu, menu->selected);
	item->next = it;
	item->prev = it->prev;
	if (it->prev)
	{
		it->prev->next = item;
	}
	it->prev = item;
	++menu->len;
	if (pos <= s && s < UI_MAX_LINES)
	{
		--menu->line;
	}
}

uint8_t menu_len(menu_t* menu)
{
	return menu ? menu->len : 0;
}

menu_t* menu_event(menu_t* menu, uint32_t event)
{
	if (event == 0 || menu == 0 || menu->len == 0)
	{
		return menu;
	}
	if((menu == &ui.about.menu)|| (menu == &ui.mobileapp.menu))
		return menu_item_event(menu, event);

	if (event & (BTN_MASK_PRS(BTN_LEFT)))
	{
		menu_up(menu);
	}
	if (event & (BTN_MASK_PRS(BTN_RIGHT)))
	{
		menu_down(menu);
	}
	return menu_item_event(menu, event);
}

static void menu_render_text_screen(const char* text1, const char* text2, const char* text3)
{
	uint16_t x, y;
	char num_to_str[50] = {0};

    myScreen.setOrientation(myOrientation);
    
	y = 30;
	uint8_t i = 0;
	if(text1 != NULL)
	{
		x = 10;
		text_render_center_horizontal_multiple_line(text1, font_16px, LCD_W-20, &x, &y);
		y += 10;
	}
	if(text2 != NULL)
	{
		x = 10;
		text_render_center_horizontal_multiple_line(text2, font_16px, LCD_W-20, &x, &y);
		y += 10;
	}
	if(text3 != NULL)
	{
		x = 10;
		text_render_center_horizontal_multiple_line(text3, font_16px, LCD_W-20, &x, &y);
		y += 10;
	}
}

static void menu_render_button(uint8_t line, const menu_item_t* item, uint8_t len,uint16_t x,uint16_t y, uint8_t sel)
{
	myGUI.delegate(false);
	uint16_t dx, dy;
	x = (myScreen.screenSizeX() - MENU_BUTTON_X)/2;
	y= y + (line * (MENU_BUTTON_Y+MENU_BUTTON_SPACE));
	dx = MENU_BUTTON_X;
	dy = MENU_BUTTON_Y;

		Button myButton(&myGUI);
        myButton.dDefine(x,y, dx, dy, setItem(line,&(item->element.text[lang*STRING_LEN_120])), font_19px);
        myButton.setOption(optionStandard);
        myButton.setState(stateEnabled);
        myButton.setStyle(styleRounded);
        if(sel == 1)
            myButton.draw(fsmTouched);
        else
            myButton.draw(fsmReleased);
	myGUI.delegate(true);
}

void gfx_render_confirm_page_ui(menu_t* menu,const char * text)
{
	uint16_t x, y;
	int i, j=1;
	const menu_item_t* it;
	myGUI.begin();
	// Header
	x = 10;
	y = 80;
	text_render_center_horizontal_multiple_line(text,font_16px, LCD_W-20, &x, &y);
	x=20;
	y=200;
	for (it = menu->selected, i = menu->line; i > -1; it = it->prev, --i)
	{
		if(j == 1)
		{
			menu_render_button(i, it, menu->len, x,y,1);
			j = 0;
		}
		else
			menu_render_button(i, it, menu->len,x,y, 0);
	}
	for (it = menu->selected->next, i = menu->line + 1; it && i < menu->len; it = it->next, ++i)
	{
		menu_render_button(i, it, menu->len, x,y,0);
	}
	gfx_highlight_line(menu->line);
}

static void menu_render_setting_item(uint8_t line, const menu_item_t* item, uint8_t sel)
{
	const char* val = item->element.value ? item->element.value() : 0;
	gfx_render_menu_settings_item(line,&(item->element.text[lang * STRING_LEN_30 ]), val, item->element.unit, sel);
}

void menu_render_about_ui(menu_t* menu)
{
	const menu_item_t* it;
    char numberstr[50] = "";
    uint16_t x, y, i;

    uint8_t xoffset = 5;
    uint8_t yoffset = 6;
    uint8_t rw;

    x = 18;
    y = 20;
    text_render_left(text_menu[MAIN_ABOUT][lang], font_30px, LCD_W, x, y - yoffset);
    y = 62;
    yoffset = 4;
    text_render_left(text_common[UI_SENSOR][lang], font_16px, LCD_W, x, y - yoffset);

    y = 74;
    myScreen.setPenSolid(true);
    myScreen.dRectangle(x - 1, y, 116, 1, myColours.black);
    myScreen.setPenSolid(false);

	y = y + 7;
	for (it = menu->selected, i = menu->line; it && i <= ABT_FW_HW_VER; it = it->next, ++i)
	{
		const char* val = it->element.value ? it->element.value() : 0;
    	rw = text_render_custom_menu_right(val, font_16px, LCD_W, 0, x, y - yoffset);
    	text_render_wrap_left(&(it->element.text[lang * STRING_LEN_20]),font_16px, rw - x, 0, x, y - yoffset);
		y = y + 16;
	}

    y = y+10;
    yoffset = 4;
    text_render_left(text_common[UI_CONTROLLER][lang], font_16px, LCD_W, x, y - yoffset);

    y = y+12;
    myScreen.setPenSolid(true);
    myScreen.dRectangle(x - 1, y, 116, 1, myColours.black);
    myScreen.setPenSolid(false);

	y = y + 7;
	for (; it && i < menu->len; it = it->next, ++i)
	{
		const char* val = it->element.value ? it->element.value() : 0;
    	rw = text_render_custom_menu_right(val, font_16px, LCD_W, 0, x, y - yoffset);
    	text_render_wrap_left(&(it->element.text[lang * STRING_LEN_20]),font_16px, rw - x, 0, x, y - yoffset);
		y = y + 16;
	}
}

void menu_render_settings_ui(menu_t* menu, const char* header)
{
	uint8_t button_cnt, button_srt;
	uint8_t xoffset = 5;
	uint8_t yoffset = 4;
	uint16_t dy;
	uint16_t x, y, w, h;
	int i, j=1;
	const menu_item_t* it;
	uint8_t total_page ,current_page;

	x = 0;
	y = 0;
	w = LCD_W;
	h = 62;
	if(header != NULL)
		dy = text_render_center_horizontal_vertical(header, font_19px, w, h, &x, &y);

	if(menu->len % 4 == 0)
		total_page = menu->len / 4 ;
	else
		total_page = menu->len / 4  + 1;

	current_page = (menu->line / 4) + 1;

	y = 275;
	if((total_page == current_page ) && total_page > 1)
	{
		x = 18;
		gfx_render_left_arrow(x, y);
	}
	else if((total_page > current_page ) && current_page > 1)
	{
		x = 18;
		gfx_render_left_arrow(x, y);
		x = 122;
		gfx_render_right_arrow(x, y);
	}
	else if((total_page > current_page ) && current_page == 1)
	{
		x = 122;
		gfx_render_right_arrow(x, y);
	}
		x = LCD_W / 2 - 17;
		gfx_render_dots(x, y, current_page, total_page);
	

	uint8_t ln = 0, lnst = 0, lnend = 0;
	ln = menu->line;
	for(char z = 3, lnst = 0; z < menu->len;)
	{
		if(ln <= z)
		{
			lnend = z;
			break;
		}
		if(z < menu->len-1)
		{
			lnst += 4;
			z += 4;
			if(z > menu->len-1)
				z = menu->len-1;
		}
		else
		{
			z = menu->len;
		}
	}
	for (it = menu->selected, i = (ln%4); i > -1; it = it->prev, --i)
	{
		if(j == 1)
		{
			menu_render_setting_item(i, it, 1);
			j = 0;
		}
		else
			menu_render_setting_item(i, it, 0);
	}
	for (it = menu->selected->next, i = (ln%4) + 1; it && i < 4; it = it->next, ++i)
	{
		menu_render_setting_item(i, it, 0);
	}
}

void menu_render_ui_button_list(menu_t* menu, const char* header)
{
	uint16_t x, y;
	int i, j=1;
	const menu_item_t* it;
	myGUI.begin();

	// Header
	x = 10;
	y = 30;
	if(header != NULL)
		text_render_center_horizontal(header, font_16px, LCD_W-20, &x, &y,0);

	for (it = menu->selected, i = menu->line; i > -1; it = it->prev, --i)
	{
		if(j == 1)
		{
			menu_render_center_button(i, it, menu->len, 1);
			j = 0;
		}
		else
			menu_render_center_button(i, it, menu->len, 0);
	}
	for (it = menu->selected->next, i = menu->line + 1; it && i < UI_MAX_LINES; it = it->next, ++i)
	{
		menu_render_center_button(i, it, menu->len, 0);
	}
	gfx_highlight_line(menu->line);
}

void menu_render(menu_t* menu)
{
	if (menu == 0)
	{
		return;
	}

	gfx_render_clear();
	if (menu->len == 0)
	{
		gfx_render_finish();
		return;
	}

	if(menu == &ui.main.menu)
		menu_render_ui_button_list(menu, text_common[UI_SELECT][lang]);

	else if(menu == &ui.main_settings.menu)
		menu_render_ui_button_list(menu, text_menu[MAIN_SETTINGS][lang]);

	else if(menu == &ui.installer_sett.menu)
		menu_render_ui_button_list(menu, text_main_settings[INSTALLER_TEXT][lang]);
		
	else if(menu == &ui.installer_sett_warn.menu)
		menu_render_text_screen(text_installer_warning[INS_WARNING_1][lang], text_installer_warning[INS_WARNING_2][lang], text_installer_warning[INS_WARNING_3][lang]);

	else if(menu == &ui.disp_sett.menu)
		menu_render_settings_ui(menu, text_main_settings[SETTINGS_DISPLAY][lang]);
	
	else if(menu == &ui.system_sett.menu)
		menu_render_settings_ui(menu, text_main_settings[SETTINGS_SYSTEM][lang]);

	else if(menu == &ui.global_sett.menu)
		menu_render_settings_ui(menu, text_installer_settings[INSTALLER_GLOBAL_SETTINGS][lang]);

	else if(menu == &ui.zone_sett.menu)
		menu_render_settings_ui(menu, text_installer_settings[INSTALLER_ZONE_SETTINGS][lang]);

	else if(menu == &ui.edit_selection.menu)
		menu_render_ui_button_list(menu, NULL);

	else if(menu == &ui.mobileapp.menu)
		gfx_render_mobile_app();

	else if(menu == &ui.about.menu)
		menu_render_about_ui(menu);

	else if(menu == &ui.factory_reset.menu)
		gfx_render_confirm_page_ui(menu,text_common[UI_CONFIRM][lang]);

	gfx_render_finish();
}
