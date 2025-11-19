#ifndef _SETTINGS
#define _SETTINGS

#include "common_def.h"
//#include "mb_ota.h"

#define NAME_MAX_LEN        20
#define GLOBAL_SETTINGS_LEN 41

#define REG_INPUT_SENSOR_INFO_START (1001)
#define REG_INPUT_SENSOR_INFO_NREGS MB_SENSOR_STATUS_LEN
#define REG_HOLDING_SENSOR_SETT_START (2001)
#define REG_HOLDING_SENSOR_SETT_NREGS MB_SETTINGS_LEN
#define REG_HOLDING_PAIRING_SETT_START (2101)
#define REG_HOLDING_PAIRING_SETT_NREGS MB_PAIRING_SETT_LEN
#define REG_HOLDING_CONFIGURATION_START (2201)
#define REG_HOLDING_CONFIGURATION_NREGS MB_CONFIGURATION_LEN
#define REG_HOLDING_EXTENDED_SETT_START (2301)
#define REG_HOLDING_EXTENDED_SETT_NREGS MB_EXTENDED_LEN
#define REG_HOLDING_GLOBAL_SETT_START (2401)
#define REG_HOLDING_GLOBAL_SETT_NREGS GLOBAL_SETTINGS_LEN
#define REG_HOLDING_GLOBAL_EXTENDED_STATUS (2501)
#define REG_HOLDING_GLOBAL_EXTENDED_STATUS_NREGS MB_EXTENDED_STATUS_HEC_LEN
#define REG_HOLDING_OTA_HEADER_START (4001)
#define REG_HOLDING_OTA_HEADER_NREGS MODBUS_OTA_HEADER_LENGTH
#define REG_HOLDING_OTA_DATA_START (5001)
#define REG_HOLDING_OTA_DATA_NREGS OTA_BUFFSIZE

#define NAMESPACE_LOCAL_DATA 	"localData"
#define NAMESPACE_EPC           "EPC_NVS"
#define KEY_SENSOR_SETTINGS     "SENSOR_SETTINGS"
#define KEY_GLOBAL_SETTINGS     "GLOBAL_SETTINGS"
#define KEY_DISPLAY             "DISPLAY"
#define KEY_IS_DEV_PAIRED       "isPaired"
#define KEY_LANG 				"lang"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum 
{
	MN_STM32_UART			=0,
	MN_WIRED_EPC_SENSOR		=1,
	MN_WIRED_SENSOR			=2,
	MN_WIRELESS_EPC_SENSOR	=101,
	MN_WIRELESS_SENSOR		=102,
}modelNo;

enum nvs_save_settings_e
{
    NVS_SETTINGS_SENSOR,
    NVS_CONFIGURATION,
    NVS_SETTINGS_ALL,
    NVS_FACTORY_RESET,
    NVS_SETTINGS_GLOBAL,
};
typedef enum nvs_save_settings_e nvs_save_settings_e;

enum setting_src_e
{
	SRC_UI,
	SRC_MODBUS,
	SRC_RF,
};
typedef enum setting_src_e setting_src_e;

typedef enum 
{
	FLAGD_SENSOR_SETT_REQUEST = (1 << 0),
	FLAGD_SENSOR_SETT_UPDATED = (1 << 1),
	FLAGD_CONFIGURATION_REQUEST = (1 << 2),
	FLAGD_ELECTRIC_HEAT_REQ = (1 << 3),
	FLAGD_GLOBAL_SETT_UPDATED = (1 << 4),
	FLAGD_GLOBAL_SETT_REQUEST = (1 << 5),
	FLAGD_EXT_SETT_REQUEST = (1 << 6),
	FLAGD_EXT_SETT_UPDATED = (1 << 7),
	FLAGD_EXT_STATUS_REQUEST = (1 << 8),
}sensor_data_flag_e;

enum 
{
    SET_FLAG_OTA_BETA_SET =         1<<0,
    SET_FLAG_DEVICE_REBOOT =        1<<1,
    SET_FLAG_FACTORY_RESET =        1<<2,
    SET_FLAG_CLEAR_BLACKLIST =      1<<3,
    SET_FLAG_OTA_UPDATE_DONE =      1<<4,
    RESERVED_FLAG_ALLOW_FLOOR_COOLING =  1<<5,
    RESERVED_FLAG_ALLOW_ELECTRICAL_FLOOR=1<<6,
    SET_FLAG_GLOBAL_SETT_CONFIGURED=1<<7,
};

enum sensor_sett_flag_e
{
	FLAGS_HEC_REBOOT = 1 << 0,
	FLAGS_HEC_FACTORY_RESET = 1 << 1,
	FLAGS_HEC_CO2_FORCED_RECALIBRATION = 1<< 2,
    DEPRECIATED_FLAGS_HEC_AWAY_MODE_DISABLE_COOLING = 1<<3,			// Depreciated: Not using anymore
    FLAGS_HEC_COOLING_FLOOR_BLOCKED = 1<<4,
    FLAGS_HEC_SET_RELAY_ON = 1<<5,
};

typedef enum 
{
	PAIR_FLAG_EXT_CONNECTED	= (1 << 0),
}pair_flags_e;

enum PAIR_e {
    PAIRING_STARTED,
    PAIRING_DONE
};

enum sensorError
{
	ERROR_NONE,
	ERROR_AIRTEMP = 1 << 1,
	ERROR_FLOOR_TEMP = 1 << 2,
	ERROR_CO2 = 1 << 3,
	ERROR_DISPLAY = 1 << 4,
};

typedef enum 
{
    NO_MODE =       0,
    MODE_AUTO =     1<<0,
    MODE_AWAY =     1<<1,
    MODE_BOOST =    1<<2,
    MODE_COOL =     1<<3,
}working_mode_e;

enum								// Enum for extended settings icons
{
	ICON_HEATING 		= 1<<0,
	ICON_AWAY_MODE 		= 1<<1,
	ICON_COOLING		= 1<<2,
	ICON_HEAT_ELEC		= 1<<3,
	ICON_WINDOW_OPEN	= 1<<4,
	ICON_HUM_BLOCKED	= 1<<5,
};


typedef enum 
{
    FANCOIL_SPEED_OFF                	= 0, // no cooling application
	FANCOIL_SPEED_LOW					= 1, // fancoil cooling low speed
	FANCOIL_SPEED_MEDIUM				= 2, // fancoil cooling medium
	FANCOIL_SPEED_HIGH					= 3, // fancoil cooling high speed
}fancoil_speed_e;

typedef enum 
{
    COOLING_NONE                = 0, // no cooling application
    COOLING_FLOOR_ONLY          = 1, // floor cooling only
    COOLING_FANCOIL_ONLY        = 2, // fancoil cooling only
    COOLING_FLOOR_AND_FANCOIL   = 3, // floor + fancoil cooling
	COOLING_APP_MAX				= 4, // max value for cooling application
}cooling_app_e;

typedef enum
{
    FANCOIL_ALLOWED_COOLING_ONLY	= 0, // only cooling mode allowed
    FANCOIL_ALLOWED_ANY_MODE		= 1, // both cooling and heating mode allowed
	FANCOIL_ALLOWED_MAX				= 2, // max value for fancoil allowed
}fancoil_allowed_e;

// Sensor settings
#define MB_SETTINGS_LEN 		21
typedef union
{
	uint16_t data[MB_SETTINGS_LEN];
	struct __attribute__((__packed__))
	{
		uint16_t node_id;				// Mb addr
		uint16_t zone_id;				// Selected zones
		uint16_t setpoint_temp; 			// Unit: 0.1 °C
		uint16_t setpoint_temp_away;
		int16_t  temp_calibration;
		uint16_t control_input;
		uint16_t flags;
		char     name[20];
		uint8_t floor_heat_applic;
		uint8_t floor_heat_mode;
		int16_t  hum_calibration;
		int16_t  co2_calibration;
		int16_t  zone_hysteresis;
	};
} settings_t;

typedef union
{
	uint16_t data[GLOBAL_SETTINGS_LEN];
	struct __attribute__((__packed__))
    {
        uint16_t mode;
        uint32_t status_interval;
        uint32_t mode_timeout;
        uint16_t saved_mode;
        uint16_t control_logic;
        uint16_t hysteresis_band;
        uint16_t floor_opr_mode;
        uint16_t actuator_type;
        uint32_t flags;
        char device_name[NAME_MAX_LEN];
        uint16_t floor_cooling_act_method;
        uint16_t gpi_mode;
        uint16_t cooling_floor_blocked_hum;
        uint32_t modbus_baudrate;
        uint16_t cooling_application;
        uint8_t cooling_dead_band;
        uint8_t fancoil_allowed;
        uint8_t reserve[4];
    };
}global_settings_t;
extern global_settings_t global_settings;

// Extended sensor settings
#define MB_EXTENDED_LEN			4
typedef union
{
	uint16_t data[MB_EXTENDED_LEN];
	struct __attribute__((__packed__))
	{
		uint16_t icon_stat;
		uint8_t fancoil_id;
		uint8_t fancoil_mode;
		uint8_t fancoil_min_v;
		uint8_t fancoil_max_v;
		int8_t fancoil_hysteresis;
		uint8_t reserve;
	};
} ext_settings;

// Pairing time settings
#define MB_PAIRING_SETT_LEN 		5
typedef union
{
	uint16_t data[MB_PAIRING_SETT_LEN];
	struct __attribute__((__packed__))
	{
		uint16_t activezones;
		uint16_t status;
		uint8_t pair_flags;
		uint8_t reserve[5];
	};
} pairing_sett_s;

//enum sensorError
//{
//	ERROR_NONE,
//	ERROR_AIRTEMP = 1 << 1,
//	ERROR_FLOOR_TEMP = 1 << 2,
//	ERROR_CO2 = 1 << 3,
//	ERROR_RF = 1 << 4,
//};

// HCB Configuration 
#define DEVICE_PASSWORD_LEN         20
#define MB_CONFIGURATION_LEN 		DEVICE_PASSWORD_LEN+3
typedef union
{
	uint16_t data[MB_CONFIGURATION_LEN];
	struct __attribute__((__packed__))
	{
		uint16_t is_device_configured;
		uint32_t deviceid;
		char     password[DEVICE_PASSWORD_LEN];
		uint16_t hw_ver;
		uint16_t fw_ver;
		uint16_t network_stat;
		uint32_t ip_addr;
		uint16_t reserve[5];
	};
} config_s;

typedef struct __attribute__((__packed__))
{
	uint8_t flags;
} pairing_flag_s;

extern pairing_flag_s pairing_flag;
extern union sensor_status_e status;
extern settings_t settings;
extern pairing_sett_s pairingsett;
extern config_s dev_config;
extern ext_settings ext_sett;
extern uint16_t temp_setpointTemp, temp_setpointAway;

void init_settings_and_status(void);
void settings_request();
void settings_updated(setting_src_e src, uint16_t settings_type);
void settings_fetched(sensor_data_flag_e src);
void settings_sensor_paired(setting_src_e src);
void sensor_settings_update(settings_t src);
void settings_save_to_nvs(nvs_save_settings_e src);
void set_disp(uint32_t disp_mode);
uint32_t get_serial_number(void);
void configuration_request();
uint32_t is_dev_paired(void);
void set_dev_pair_stat(uint32_t pair);
uint32_t get_disp_type();
void read_disp_mode();
uint8_t is_setpoint_update_from_server();
void setpoint_screen_popup_success();

#ifdef __cplusplus
} // closing brace for extern "C"
#endif

#endif //_SETTINGS
