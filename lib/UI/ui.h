/*
 * ui.h
 *
 *  Created on: Jun 25, 2021
 *      Author: Biren
 */

#ifndef UI_H_
#define UI_H_

// Configuration
#include "hV_Configuration.h"
//#include "mbslave.h"
#include "menu.h"
#include "hsm.h"
#include "ui_text.h"

#define FANCOIL_MIN_V_MAX  10
#define FANCOIL_MIN_V_MIN  0
#define FANCOIL_MIN_V_STEP 1

#define FANCOIL_MAX_V_MAX  10
#define FANCOIL_MAX_V_MIN  2
#define FANCOIL_MAX_V_STEP 1

#define BAUDRATE_V_19200	19200
#define BAUDRATE_V_115200	115200
#define BAUDRATE_V_921600	921600

#define ONE_MINUTE_MS		(60*1000)
#define ERROR_SKIP_MS       (5*ONE_MINUTE_MS)

extern uint8_t is_installer_menu_unlock;

void ui_init();
void ui_task();

enum
{
	HSM_HVAC_DISCONNECTED, // hvac comm loss
	HSM_HVAC_CONNECTED,	   // hvac comm ok
	HSM_HVAC_OFF,		   // hvac power off
	HSM_HVAC_ON,		   // hvac power on
	HSM_HVAC_ERROR,		   // hvac system error
	HSM_HVAC_OK,		   // hvac operational
	HSM_HOME,			   // render home view
	HSM_MENU,			   // render menus
	HSM_EDIT,			   // edit mode
	HSM_PAIR,		   	   // pair mode
	NUM_HSM,
};

#define CHECK "\xe2\x98\x91"


enum
{
	PAIR_LANGUAGE_SELECT,
	PAIR_WORKING_MODE,
	PAIR_METHOD,
	NUM_PAIR
};

enum
{
	CONFIRM_FACTORY_RESET,
	DENY_FACTORY_RESET,
	NUM_FACTORY_RESET
};

struct ui
{
	hsm_t *task;
	hsm_t *menu_req_task;
	menu_t *menu;
	uint32_t user_activity_ts;
	hsm_t hsm[NUM_HSM];
	struct
	{
		menu_t menu;
		menu_item_t items[NUM_MAIN];
	} main;
	struct
	{
		menu_t menu;
		menu_item_t items[NUM_STATUS];
	} mobileapp;
	struct
	{
		menu_t menu;
		menu_item_t items[NUM_MAIN_SETTINGS];
	} main_settings;

	struct
	{
		menu_t menu;
		menu_item_t items[NUM_DISP_SETTINGS];
	} disp_sett;
	struct
	{
		menu_t menu;
		menu_item_t items[NUM_SYS_SETTINGS];
	} system_sett;
	struct
	{
		menu_t menu;
		menu_item_t items[NUM_INSTALLER_SETTINGS];
	} installer_sett;
	struct
	{
		menu_t menu;
		menu_item_t items[INS_WARNING_2];
	} installer_sett_warn;
	struct
	{
		menu_t menu;
		menu_item_t items[NUM_GLOBAL_SETTINGS];
	} global_sett;
	struct
	{
		menu_t menu;
		menu_item_t items[NUM_ZONE_SETTINGS];
	} zone_sett;
	struct
	{
		menu_t menu;
		menu_item_t items[NUM_ABOUT];
	} about;
	struct
	{
		menu_t menu;
		menu_item_t items[NUM_PAIR];
	} pair;
	struct
	{
		menu_t menu;
		menu_item_t items[NUM_FACTORY_RESET];
	} factory_reset;
	struct
	{
		menu_t menu;
		menu_item_t items[NUM_EDIT_SELECTION];
	} edit_selection;
};

uint32_t get_baudrate_value(baudrate_t baud_enum);
baudrate_t get_baudrate_enum(uint32_t baudrate);
uint8_t is_valid_baudrate(uint32_t baudrate);

extern struct ui ui;


#endif /* UI_H_ */
