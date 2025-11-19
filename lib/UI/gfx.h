/*
 * gfx.h
 *
 *  Created on: 23.07.2014
 *      Author: anti
 */

#ifndef GFX_H_
#define GFX_H_

#include "common_def.h"
#include "home.h"

extern uint8_t gfxbuf[LCD_W * LCD_H / 8];
extern uint8_t is_fancoil_view_enabled;

uint8_t* gfxbuf_get_ptr();

void gfx_display_off();
void gfx_display_on();

void gfx_render_clear();
void gfx_render_finish();
void gfx_render_menu_cmd(uint8_t line, const char *text);
void gfx_render_menu_item(uint8_t line, const char *text, const char *value, const char *unit, uint8_t sel);

void gfx_render_home_status(uint16_t temp, uint16_t hum, uint16_t co2);
void gfx_render_edit_page(int32_t value, uint8_t type);
void gfx_render_home_mode(const char *mode);
void gfx_render_home_fan(uint16_t x, uint16_t y, uint8_t level, uint8_t max_level, uint8_t arrows);
void gfx_render_home_disconnected();

void gfx_render_balance_edit(const char* balance);

void gfx_render_error(const char* header,const char *line1, const char *line2);
void gfx_render_ui_disabled();
void gfx_render_serial_no();

void gfx_grayout_line(uint8_t line);
void gfx_highlight_line(uint8_t line);
void gfx_highlight_line1(uint8_t line);

void gfx_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void gfx_render_menu_settings_item(uint8_t line, const char *text, const char *value, const char *unit, uint8_t sel);

void gfx_render_home();
void gfx_render_dots(uint16_t x, uint16_t y, uint16_t current_value, uint16_t max);
void gfx_render_arrows();
void gfx_render_left_arrow(uint16_t x, uint16_t y);
void gfx_render_right_arrow(uint16_t x, uint16_t y);
void gfx_render_mobile_app(void);
void set_lcd_force_update();
uint8_t is_lcd_force_update_require();
void gfx_render_ota();

#endif /* GFX_H_ */
