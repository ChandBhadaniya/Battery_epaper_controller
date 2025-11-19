#ifndef UI_PAIR_H_
#define UI_PAIR_H_

// Configuration
#include "hV_Configuration.h"
#include "ui_text.h"

#define DEF_VALUE		0
#define DEF_STEP		1
#define DEF_MAX_VAL		2
#define MAX_TEXT		4
#define MAX_HEATING		4
#define MAX_COOLING_APP	4
#define MAX_COOLING_APP_ONLY_FLOOR	2
#define MAX_ZONE 		8
#define MAX_FANCOIL 	6

extern uint8_t available_zone[MAX_ZONE];

enum
{
	UI_TYPE_BUTTON,
	UI_TYPE_SELECTION,
	UI_TYPE_SELECT,
	UI_TYPE_TEXT,
	UI_TYPE_IMAGE,
};

enum
{
	CONTROLMODE_AIR_TEMP,
	CONTROLMODE_FLOOR_TEMP,
}controlmode_e;

enum
{
	FLOOR_COOLING_ALLOWED_OFF,
	FLOOR_COOLING_ALLOWED_ON,
}floor_cooling_allowed_e;

enum
{
	ELECTRIC_FLOOR_HEATING_OFF,
	ELECTRIC_FLOOR_HEATING_ON,
}electric_floor_heating_e;

enum
{
	COOLING_METHOD_AUTO,
	COOLING_METHOD_MANUAL,
}floor_cooling_method_e;

enum
{
	HEATING_METHOD_HYDRONIC,
	HEATING_METHOD_ELECTRIC,
	HEATING_METHOD_HYDRONIC_ELECTRIC,
	HEATING_METHOD_AUTO,
}heating_method_e;

enum
{
	DISPLAY_REGULAR,
	DISPLAY_REVERTED,
}display_e;

enum
{
	PS_NONE,
	PS_UNPAIRED,
	PS_PAIRING,
	PS_READY,
	PS_PAIRED,
}pairingStatus_e;

typedef struct pair_item_s {
	uint8_t ui_type;
	uint8_t selection;
	uint8_t value;
	uint8_t max;
	int16_t step;
	const char* header;
	const char* text[MAX_TEXT];
	const char* subtext[MAX_TEXT];
	struct pair_item_s* next;
} pair_item_t;

struct pair_s{
	pair_item_t airobot_logo;
	pair_item_t language;
	// pair_item_t welcome_note;
	pair_item_t display;
	pair_item_t controlmode;
	pair_item_t node_id;
	pair_item_t pairing_screen;
	pair_item_t cooling_application_pair;
	pair_item_t cooling_method;
	pair_item_t electric_floor_heating;
	pair_item_t heating_method;
	pair_item_t zones;
	pair_item_t fancoil_id;
	pair_item_t connection_stat;
	pair_item_t* current;
	uint32_t ts;
	uint8_t flash;
};

void home_pair_finish();
void home_pair_render();
int8_t home_pair_handle(uint32_t e);
int8_t home_pair_start(uint32_t e);
void get_available_zones(uint8_t zones);

#endif /* UI_PAIR_H_ */
