/*
 * font_render.h
 *
 *  Created on: 23.07.2014
 *      Author: anti
 */

#ifndef FONT_RENDER_H_
#define FONT_RENDER_H_

#include "common_def.h"

extern uint8_t font_16px, font_19px, font_30px, font_45px, font_L_65px, font_EL_65px;

// Font list
enum {
	FONT_NORMAL,
	FONT_SMALL,
	FONT_TINY,
	FONT_DROID_SANS_BOLD_10,
	FONT_DROID_SANS_BOLD_20,
	FONT_BOLD,
	FONT_CLOCK,
	FONT_CNT,
};

enum {
	SEL_NONE = 0,
	SEL_INVERT_WITH_BG,
	SEL_INVERT_TEXT,
};


// Initialize text_state_t, perform alignment calc.
uint16_t text_render_menu_left(const char* text, unsigned int font, uint16_t w, uint8_t line, uint8_t sel);
uint16_t text_render_menu_center(const char* text, unsigned int font, uint16_t w, uint8_t line, uint8_t sel);
uint16_t text_render_menu_right(const char* text, unsigned int font, uint16_t w, uint8_t line, uint8_t sel);
uint16_t text_render_left(const char* text, unsigned int font, uint16_t w, uint16_t x, uint16_t y);
uint16_t text_render_right(const char* text, unsigned int font, uint16_t w, uint16_t x, uint16_t y);
uint16_t text_render_center_horizontal(const char* text, unsigned int font, uint16_t w, uint16_t *x, uint16_t *y, uint8_t sel);
void text_render_center_horizontal_multiple_line(const char* text, unsigned int font, uint16_t w, uint16_t *x, uint16_t *y);
uint16_t text_render_center_vertical(const char* text, unsigned int font, uint16_t h, uint16_t *x, uint16_t *y);
uint16_t text_render_center_horizontal_vertical(const char* text, unsigned int font, uint16_t w, uint16_t h, uint16_t *x, uint16_t *y);
uint8_t text_render(uint16_t x0, uint16_t y0, const char * text, unsigned int font, uint8_t sel);
void text_font_init();

uint8_t text_render_custom_ui(uint16_t x0, uint16_t y0, const char *text, unsigned int font, uint8_t sel, uint8_t type);
uint16_t text_render_custom_right(const char *text, unsigned int font, uint16_t w, uint16_t x, uint16_t y, uint8_t sel);
uint16_t text_render_wrap_left(const char *text, unsigned int font, uint16_t w, uint8_t sel, uint16_t x, uint16_t y);
uint16_t text_render_custom_menu_right(const char *text, unsigned int font, uint16_t w, uint8_t sel, uint16_t x, uint16_t y);
uint16_t text_render_custom_center_horizontal(const char *text, unsigned int font, uint16_t w, uint16_t x, uint16_t y, uint8_t sel);
void text_render_center_vertical_horizontal_multiple_line(const char *text, unsigned int font, uint16_t w, uint16_t h, uint16_t *x, uint16_t *y);

#endif /* FONT_RENDER_H_ */
