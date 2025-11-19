/*
 * menu.h
 *
 *  Created on: Jun 25, 2021
 *      Author: Biren
 */

#ifndef UI_MENU_H_
#define UI_MENU_H_

// Configuration
#include "hV_Configuration.h"

struct menu_item_s;

typedef struct menu_s {
	struct menu_s* parent;
//private:
	struct menu_item_s* selected;
	uint8_t len;
	uint8_t line;
} menu_t;

typedef const char* (*menu_item_stringify)(void);
typedef menu_t* (*menu_item_action)(menu_t*, uint32_t);

typedef struct menu_item_s {
	struct {
		const char* text;
		menu_item_stringify value;
		const char* unit;
		menu_item_action action;
	} element;
//private:
	struct menu_item_s* prev;
	struct menu_item_s* next;
} menu_item_t;

menu_t* menu_event(menu_t*, uint32_t event);
void menu_enter(menu_t* menu);
void menu_remove(menu_t* menu, menu_item_t* item);
void menu_insert(menu_t* menu, uint8_t pos, menu_item_t* item);
uint8_t menu_len(menu_t* menu);
void menu_render(menu_t *menu);

#endif /* UI_MENU_H_ */
