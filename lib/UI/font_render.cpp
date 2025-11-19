/*
 * font_render.c
 *
 * Renders proportional width fonts to video memory.
 *
 *      Author: anti
 */

#include "font_render.h"
#include "hV_Screen_Buffer.h"
#include "hV_Font_Header.h"

#include "PDLS_EXT3_Advanced_Fast.h"
extern Screen_EPD_EXT3_Fast myScreen;

#define DOTS_CHARS	3
#define TEXT_ALLOW_OVERFLOW	3
static const char dots[] = "...";
uint8_t font_16px, font_19px, font_30px, font_45px, font_L_65px, font_EL_65px;

// =================================================================
// Public interface
uint16_t text_render_menu_left(const char *text, unsigned int font, uint16_t w, uint8_t line, uint8_t sel)
{
	char print_str[30] = "";
	uint16_t x,dx,y,dotx,t;
	
	myScreen.selectFont(font);
	dotx = myScreen.stringSizeX(dots);
	dx = myScreen.stringSizeX(text);
	x = 0;
	y = myScreen.characterSizeY() * line;

	if(dx >= w)
	{
		t=myScreen.stringLengthToFitX(text, w-dotx);
		//hV_HAL_Debug_println("%d, %s, %d, %d", __LINE__, text, w-dotx, t);
		strncpy(print_str, text,  t);
		dx = myScreen.stringSizeX(print_str);
		text_render(x, y, print_str, font, sel);
		x += dx;

		text_render(x, y, dots, font, sel);
		dx = myScreen.stringSizeX(dots);
		x += dx;
	}
	else
	{
		text_render(x, y, text, font, sel);
		dx = myScreen.stringSizeX(text);
		x += dx;
	}

	return x;
}

uint16_t text_render_wrap_left(const char *text, unsigned int font, uint16_t w, uint8_t sel, uint16_t x, uint16_t y)
{
	char print_str[30] = "";
	// uint16_t x, dx, y, dotx, t;
	uint16_t dx, dotx, t;

	myScreen.selectFont(font);
	dotx = myScreen.stringSizeX(dots);
	dx = myScreen.stringSizeX(text);
	// x = 0;
	// y = myScreen.characterSizeY() * line;

	if (dx >= w)
	{
		t = myScreen.stringLengthToFitX(text, w - dotx);
		// hV_HAL_Debug_println("%d, %s, %d, %d", __LINE__, text, w-dotx, t);
		strncpy(print_str, text, t);
		dx = myScreen.stringSizeX(print_str);
		text_render(x, y, print_str, font, sel);
		x += dx;

		text_render(x, y, dots, font, sel);
		dx = myScreen.stringSizeX(dots);
		x += dx;
	}
	else
	{
		text_render(x, y, text, font, sel);
		dx = myScreen.stringSizeX(text);
		x += dx;
	}

	return x;
}

uint16_t text_render_custom_menu_right(const char *text, unsigned int font, uint16_t w, uint8_t sel, uint16_t x, uint16_t y)
{
	uint16_t dx;

	myScreen.selectFont(font);
	dx = myScreen.stringSizeX(text);
	x = w - dx - x;
	// y = myScreen.characterSizeY() * line;
	// y = (LCD_H * 2 / 3) + 25;
	text_render(x, y, text, font, sel);
	return x;
}

uint16_t text_render_menu_center(const char *text, unsigned int font, uint16_t w, uint8_t line, uint8_t sel)
{
	uint16_t x,dx,y;
	
	myScreen.selectFont(font);
	dx = myScreen.stringSizeX(text);
	x = w - dx;
	x = x/2;
	y = myScreen.characterSizeY() * line;
	
	text_render(x, y, text, font, sel);
	x += dx;

	return x;
}

uint16_t text_render_menu_right(const char *text, unsigned int font, uint16_t w, uint8_t line, uint8_t sel)
{
	uint16_t x,dx,y;

	myScreen.selectFont(font);
	dx = myScreen.stringSizeX(text);
	x = w - dx;
	y = myScreen.characterSizeY() * line;
	//y = (LCD_H * 2 / 3) + 25;
	text_render(x, y, text, font, sel);

	return x;
}

uint16_t text_render_left(const char *text, unsigned int font, uint16_t w, uint16_t x, uint16_t y)
{
	char print_str[30] = "";
	uint16_t dx,t;
	
	myScreen.selectFont(font);
	dx = myScreen.stringSizeX(text);
	if(dx >= w)
	{
		t=myScreen.stringLengthToFitX(text, w);
		strncpy(print_str, text,  t);
		dx = myScreen.stringSizeX(print_str);
		text_render(x, y, print_str, font, 0);
		x += dx;
	}
	else
	{
		text_render(x, y, text, font, 0);
		x += dx;
	}

	return (x+dx);
}

uint16_t text_render_center_horizontal(const char *text, unsigned int font, uint16_t w, uint16_t *x, uint16_t *y, uint8_t sel)
{
	uint16_t x0,dx,dy;
	
	myScreen.selectFont(font);
	dx = myScreen.stringSizeX(text);
	x0 = w - dx;
	x0 = x0/2;
	*x += x0;
	dy = myScreen.characterSizeY();

	text_render(*x, *y, text, font, sel);
	*x += dx;
	*y += dy;

	return *x;
}

uint16_t text_render_custom_center_horizontal(const char *text, unsigned int font, uint16_t w, uint16_t x, uint16_t y, uint8_t sel)
{
	uint16_t x0, dx, dy;

	myScreen.selectFont(font);
	dx = myScreen.stringSizeX(text);
	x0 = w - dx;
	x0 = x0 / 2;
	x += x0;
	dy = myScreen.characterSizeY();

	text_render_custom_ui(x, y, text, font, sel, 1);

	x += dx;
	y += dy;

	return dy;
}

uint16_t text_render_center_vertical(const char *text, unsigned int font, uint16_t h, uint16_t *x, uint16_t *y)
{
	uint16_t y0,dx,dy;
	
	myScreen.selectFont(font);
	dx = myScreen.stringSizeX(text);
	dy = myScreen.characterSizeY();
	y0 = *y + h/2 - dy/2;
	*y = y0;

	text_render(*x, *y, text, font, 0);
	*x += dx;
	*y += dy;

	return *x;
}

uint16_t text_render_center_horizontal_vertical(const char *text, unsigned int font, uint16_t w, uint16_t h, uint16_t *x, uint16_t *y)
{
	uint16_t x0,y0,dx,dy;
	
	myScreen.selectFont(font);
	dx = myScreen.stringSizeX(text);
	x0 = w - dx;
	x0 = x0/2;
	*x += x0;
	dy = myScreen.characterSizeY();
	y0 = *y + h/2 - dy/2;
	*y = y0;
	text_render(*x, *y, text, font, 0);
	*x += dx;
	*y += dy;

	return *x;
}

void text_render_center_vertical_horizontal_multiple_line(const char *text, unsigned int font, uint16_t w, uint16_t h, uint16_t *x, uint16_t *y)
{

	char print_str[30] = "";
	uint16_t dx, dy, ddy, tx_strlen, tx_index = 0, len = 0;
	dy = *x;
	myScreen.selectFont(font);
	dx = myScreen.stringSizeX(text);
	ddy = myScreen.characterSizeY();
	len = strlen(text);
	if (dx >= w)
	{

		do
		{
			tx_strlen = myScreen.stringLengthToFitX(&text[tx_index], w);

			if (tx_strlen > 0)
			{
				if (tx_strlen < strlen(&text[tx_index]))
				{
					for (int i = (tx_strlen + tx_index); i > tx_index; i--)
					{
						if (text[i] == ' ')
						{
							tx_strlen = (i == (tx_strlen + tx_index)) ? (tx_strlen) : (i - tx_index + 1);
							break;
						}
					}
				}
				memset(print_str, 0x00, sizeof(print_str));
				strncpy(print_str, &text[tx_index], tx_strlen);

				*x = dy;
				*y = (h) + (tx_index / 2);

				text_render_center_horizontal_vertical(print_str, font, w, ddy, x, y);
				tx_index += tx_strlen;
			}
		} while (tx_index < len);
	}
	else
	{

		text_render_center_horizontal_vertical(text, font, w, ddy, x, y);
	}
}

void text_render_center_horizontal_multiple_line(const char *text, unsigned int font, uint16_t w, uint16_t *x, uint16_t *y)
{

	char print_str[30] = "";
	uint16_t dx, dy, tx_strlen, tx_index = 0, len = 0;
	
	dy = *x;
	myScreen.selectFont(font);
	dx = myScreen.stringSizeX(text);
	len = strlen(text);
	if(dx >= w)
	{
		do
		{
			tx_strlen=myScreen.stringLengthToFitX(&text[tx_index], w);
			if(tx_strlen>0)
			{
				if(tx_strlen < strlen(&text[tx_index]))
				{
					for(int i=(tx_strlen+tx_index); i>tx_index; i--)
					{
						if(text[i]==' ')
						{
							tx_strlen = (i==(tx_strlen+tx_index)) ? (tx_strlen) : (i-tx_index+1);
							break;
						}
					}
				}
				memset(print_str, 0x00, sizeof(print_str));
				strncpy(print_str, &text[tx_index], tx_strlen);
				*x = dy;
				text_render_center_horizontal(print_str, font, w, x, y, 0);
				tx_index += tx_strlen;
			}
		}while(tx_index<len);
	}
	else
	{
		text_render_center_horizontal(text, font, w, x, y, 0);
	}
}

uint16_t text_render_right(const char *text, unsigned int font, uint16_t w, uint16_t x, uint16_t y)
{
	uint16_t dx;

	myScreen.selectFont(font);
	dx = myScreen.stringSizeX(text);
	x += w - dx;

	text_render(x, y, text, font, 0);
	x += dx;

	return x;
}

uint8_t text_render(uint16_t x0, uint16_t y0, const char * text, unsigned int font, uint8_t sel)
{
	myScreen.selectFont(font);
	switch(sel)
	{
		case SEL_INVERT_WITH_BG:
			myScreen.setPenSolid(true);
			myScreen.dRectangle(0, y0, myScreen.screenSizeX(), myScreen.characterSizeY(), myColours.black);
			myScreen.setPenSolid(false);
		case SEL_INVERT_TEXT:
			myScreen.gText(x0, y0, text, myColours.white, myColours.black);
			break;
		case SEL_NONE:
			myScreen.gText(x0, y0, text, myColours.black, myColours.white);
			break;
	}
	return 1;
}

uint16_t text_render_custom_right(const char *text, unsigned int font, uint16_t w, uint16_t x, uint16_t y, uint8_t sel)
{
	uint16_t dx;

	myScreen.selectFont(font);
	dx = myScreen.stringSizeX(text);
	x += w - dx;

	text_render_custom_ui(x, y, text, font, sel, 0);
	x += dx;

	return x;
}

uint8_t text_render_custom_ui(uint16_t x0, uint16_t y0, const char *text, unsigned int font, uint8_t sel, uint8_t type)
{
	uint8_t x_axis, y_axis, l, h, r, offset;
	if (type == 0)
	{
		x_axis = 16;
		l = 120;
		h = 22;
		r = 4;
		y_axis = y0;
		offset = 2;
	}
	else if (type == 1)
	{
		x_axis = 12;
		l = 128;
		h = 55;
		r = 4;
		y_axis = y0;
		offset = 6;
	}
	myScreen.selectFont(font);
	switch (sel)
	{
	case SEL_INVERT_WITH_BG:
		myScreen.setPenSolid(true);
		myScreen.dRoundedRectangle(x_axis, y_axis, l, h, r, myColours.black);
		myScreen.gText(x0, y0 + offset, text, myColours.white, myColours.black);
		myScreen.setPenSolid(false);
	case SEL_INVERT_TEXT:
		myScreen.gText(x0, y0 + offset, text, myColours.white, myColours.black);
		break;
	case SEL_NONE:
		myScreen.gText(x0, y0 + offset, text, myColours.black, myColours.white);
		break;
	}
	return 1;
}

void text_font_init()
{
	font_16px = myScreen.addFont(Font_PXSansRegular16);
	font_19px = myScreen.addFont(Font_InterMedium24);
	font_30px = myScreen.addFont(Font_InterMedium36);
	font_45px = myScreen.addFont(Font_InterRegular55);
	font_L_65px = myScreen.addFont(Font_InterLight78);
	// font_EL_65px = myScreen.addFont(Font_InterExtralight65);

	font_16px -= (font_16px > 0) ? 1 : 0;
	font_19px -= (font_19px > 0) ? 1 : 0;
	font_30px -= (font_30px > 0) ? 1 : 0;
	font_45px -= (font_45px > 0) ? 1 : 0;
	font_L_65px -= (font_L_65px > 0) ? 1 : 0;
	// font_EL_65px -= (font_EL_65px > 0) ? 1 : 0;
}