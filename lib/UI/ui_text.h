#ifndef _TEXT_H_
#define _TEXT_H_

#include "hV_Font_Header.h"
#include "settings.h"
#include "status.h"


#define STRING_LEN_10 11
#define STRING_LEN_20 20
#define STRING_LEN_30 30
#define STRING_LEN_100 100
#define STRING_LEN_120 120
#define STRING_LEN_200 200
#define STRING_LEN_60 60

extern uint8_t lang;

typedef enum
{
	LCD_LANG_EN = 0,	//ENGLISH
	LCD_LANG_ET,		//ESTONIAN
	LCD_LANG_MAX,
}language_e;

typedef enum {
    BAUDRATE_19200,
    BAUDRATE_115200,
    BAUDRATE_921600,
    BAUDRATE_MAX
} baudrate_t;
extern baudrate_t baudrate_s;

const char text_baudrate_s[BAUDRATE_MAX][LCD_LANG_MAX][STRING_LEN_20] =
{
	{"19200", "19200"},
	{"115200", "115200"},
	{"921600", "921600"},
};

enum
{
	MAIN_SETTINGS,
	MAIN_MOBILE_APP,
	MAIN_ABOUT,
	NUM_MAIN,
};


const char text_menu[NUM_MAIN][LCD_LANG_MAX][STRING_LEN_20] =
{
	{"Settings", "Seaded"},
	{"Mobile app", "Mobiili" _a "pp"},
	{"About", "Teave"},
};

const char text_cooling_application[COOLING_APP_MAX][LCD_LANG_MAX][STRING_LEN_20] =
{
	{"Off", "V" _a "ljas"},
	{"Floor only", "P" _02 "rand"},
	{"Fan coil only", "Jahutusvent"},
	{"Floor+Fan c.", "P" _02 "rand+Vent"}
};

const char text_fancoil_allowed[COOLING_APP_MAX][LCD_LANG_MAX][STRING_LEN_20] =
{
	{"Cooling only", "K" _u "te"},
	{"All mode", "K" _u "te ja k" _u "te"},
};

enum aqi_e
{
	AQI_INVALID,
	UNHEALTHY,
	BAD,
	BELOW_MODERATE_AQI,
	MODERATE_AQI,
	EXCELLENT_AQI,
	NUM_AQI
};

typedef enum aqi_e aqi_e;
const char text_aqi[NUM_AQI][LCD_LANG_MAX][STRING_LEN_20] =
{
	{"-", "-"},
	{"Very bad AQ", "V" _a "ga halb " _01 "K"},
	{"Bad AQ", "Halb " _01 "K"},
	{"Moderate AQ", "Keskmine " _01 "K"},
	{"Good AQ", "Hea " _01 "K"},
	{"Perfect AQ", "V" _a "ga hea " _01 "K"},
};

enum
{
	STATUS_AIR_TEMP,
	STATUS_AIR_HUMIDITY,
	STATUS_CO2,
	STATUS_FLOOR_TEMP,
	NUM_STATUS,
};
const char text_status[NUM_STATUS][LCD_LANG_MAX][STRING_LEN_20] =
{
	{"Temp air","Temp" _02 "hu"},
	{"Humidity", "" _01 "huniiskus"},
	{"CO2", "CO2"},
	{"Temp floor", "Temp p" _02 "randa"},
};


typedef enum
{
	FLOOR_HEATING_MODE,
	FLOOR_COOLING_MODE,
}floor_opr_mode_e;
const char text_floor_opr_mode[NUM_STATUS][LCD_LANG_MAX][STRING_LEN_30] =
{
	{"Heating", "K" _u "te"},
	{"Cooling", "Jahutus"},
};

typedef enum
{
	STATUS_NOT_CONNECTED,
	STATUS_CONNECTED,
	NUM_STATUS_TEXT,
};
const char text_status_common[NUM_STATUS_TEXT][LCD_LANG_MAX][STRING_LEN_30] =
{
	{"Disconnected", "lahti " _U "hend."},
	{"Connected", _U "hendatud"},
};

enum
{
	HEAT_HYDRONIC,
	HEAT_ELECTRIC,
	HEAT_HYDRONIC_ELECTRIC,
	HEAT_AUTO_SWITCH,
	NUM_HEAT_TYPES,
};
const char text_heat_types[NUM_HEAT_TYPES][LCD_LANG_MAX][STRING_LEN_30] =
{
	{"Hydronic", "Vesi"},
	{"Electric", "Elekter"},
	{"Hydro+Elec.", "Vesi ja elek"},
	{"Automatic", "Automaatne"},
};

enum
{
	GLOBAL_COOLING_APPLICATION,
	GLOBAL_FANCOIL_ALLOWED,
	GLOBAL_BLOCK_COOLING_HUM,
	GLOBAL_COOLING_ACTIVATION,
	GLOBAL_HYSTERESIS,
	NUM_GLOBAL_SETTINGS,
};
const char text_global_settings[NUM_GLOBAL_SETTINGS][LCD_LANG_MAX][STRING_LEN_30] =
{
	{"Cooling application", "Jahutuse rakendus"},
	{"Fan coil allowed", "Jahutusvent. lubatud"},
	{"Block cooling humidity", "Blok. jahutus niiskus"},
	{"Cooling activation", "Jahutuse aktiveerimine"},
	{"Hysteresis", "H" _u "sterees"},
};

enum
{
	ZONE_CONTROL_INPUT,
	ZONE_FLOOR_APPLICATION,
	ZONE_DISABLE_FLOOR_COOLING,
	ZONE_FANCOIL_ID,
	ZONE_FANCOIL_MAX_V,
	ZONE_FANCOIL_MIN_V,
	ZONE_FANCOIL_HYSTERESIS,
	ZONE_RESET_SENSOR,
	ZONE_MB_BAUDRATE,
	ZONE_SELECTION,
	NUM_ZONE_SETTINGS,
};
const char text_zone_settings[NUM_ZONE_SETTINGS][LCD_LANG_MAX][STRING_LEN_30] =
{
	{"Control input", "Juhtsisend"},
	{"Floor application", "P" _02 "randa rakendus"},
	{"Disable floor cooling", "Keela p" _02 "randjahutus"},
	{"Fan coil ID", "Jahutusvent. ID"},
	{"Fan coil max. V", "Jahutusvent. max. V"},
	{"Fan coil min. V", "Jahutusvent. min. V"},
	{"Fan coil hysteresis", "Jahutusvent. h" _u "sterees"},
	{"Reset sensor", "Taasta andur"},
	{"Modbus baudrate", "Modbus bauder"},
	{"Zone selection", "Tsooni valik"},
};

enum
{
	INSTALLER_GLOBAL_SETTINGS,
	INSTALLER_ZONE_SETTINGS,
	NUM_INSTALLER_SETTINGS,
};

const char text_installer_settings[NUM_INSTALLER_SETTINGS][LCD_LANG_MAX][STRING_LEN_20] =
{
	{"Global", "Globaalsed"},
	{"Zone", "Tsooni"},
};

enum
{
	INS_WARNING_1,
	INS_WARNING_2,
	INS_WARNING_3,
	NUM_INS_WARNING,
};

const char text_installer_warning[NUM_INS_WARNING][LCD_LANG_MAX][STRING_LEN_100] =
{
	{"Installer menu access not allowed.", "Paigaldaja men" _u _u "le ligip" _a _a "s pole lubatud."},
	{"Only installer is allowed to do it as system can be damaged otherwise.", "Seda tohib teha ainult paigaldaja, sest s" _u "steemi v" _02 "ib muidu kahjustada."},
	{"Read manual how to access it.", "Lugege juhendit, kuidas men" _u _u "d avada."},
};

enum
{
	SETTINGS_SYSTEM,
	SETTINGS_DISPLAY,
	INSTALLER_TEXT,
	NUM_MAIN_SETTINGS,
};

 const char text_main_settings[NUM_MAIN_SETTINGS][LCD_LANG_MAX][STRING_LEN_20] =
{
	{"System", "S" _u "steem"},
	{"Display", "Ekraan"},
	{"Installer", "Paigaldaja"},
};

enum
{
	DISP_SETTINGS_LANGUAGE,
	DISP_SETTINGS_DISPLAY,
	NUM_DISP_SETTINGS,
};
const char text_disp_settings[NUM_DISP_SETTINGS][LCD_LANG_MAX][STRING_LEN_30] =
{
	{"Language", "Keel"},
	{"Display","Ekraan"},
};

enum
{
	SYS_SETTINGS_FLOOR_MODE,
	SYS_SETTINGS_FLOOR_HEATING,
	SYS_SETTINGS_TEMP_CALIBRATION,
	// SYS_SETTINGS_HUM_CALIBRATION,
	// SYS_SETTINGS_CO2_CALIBRATION,
	NUM_SYS_SETTINGS,
};
const char text_sys_settings[NUM_SYS_SETTINGS][LCD_LANG_MAX][STRING_LEN_30] =
{
	{"Mode", "Re" _z "iim"},
	{"Floor heating", "P" _02 "randk" _u "te"},
	{"Sensor temp. calibration", "Anduri temp. kalibr"},
	// {"Hum. calibration", "Hum. kalibr."},
	// {"Co2 calibration", "Co2 kalibr."},
};

enum
{
	ABT_ZONE_NAME,
	ABT_NODEID,
	ABT_FW_HW_VER,
	ABT_MODE,
	ABT_DEV_ID,
	ABT_NETWORK,
	ABT_IP_ADDR,
	ABT_CONTROLLER_FW_HW,
	ABT_MB_BAUDRATE,
	NUM_ABOUT,
};
const char text_about[NUM_ABOUT][LCD_LANG_MAX][STRING_LEN_20] =
{
	{"Name", "Nimi"},
	{"ID", "ID"},
	{"Firmw. (HW)", "Tarkv. (RV)"},
	{"Mode", "Re" _z "iim"},
	{"SN/ID", "SN/ID"},
	{"Network", "V" _02 "rk"},
	{"IP", "IP"},
	{"Firmw. (HW)", "Tarkv. (RV)"},
	{"MB baudrate", "MB baudrate"},
};

typedef enum 
{
    FANCOIL_MODE_OFF                = 0, // no cooling application
	FANCOIL_MODE_LOW				= 1, // fancoil cooling low speed
	FANCOIL_MODE_MEDIUM				= 2, // fancoil cooling medium speed
	FANCOIL_MODE_HIGH				= 3, // fancoil cooling high speed
	FANCOIL_MODE_AUTO               = 11, // auto mode
}fancoil_mode_e;

const char text_fancoil_mode[FANCOIL_MODE_HIGH+2][LCD_LANG_MAX][STRING_LEN_20] =
{
    {"OFF", "V" _A "LJA"},
    {"LOW", "MADAL"},
    {"MID", "KESK."},
    {"HIGH", "KIIRE"},
    {"AUTO", "AUTO"},
};

const char text_fancoil_speed[FANCOIL_MODE_HIGH+2][LCD_LANG_MAX][STRING_LEN_20] =
{
    {"OFF", "V" _A "LJAS"},
    {"LOW", "MADAL"},
    {"MID", "KESKMINE"},
    {"HIGH", "KIIRE"},
    {"AUTO", "AUTO"},
};

// Below is just for the text display, not for the fancoil mode
enum						
{
	FANCOIL_MODE_EXT_AUTO_COOL,
	FANCOIL_MODE_EXT_MANUAL_COOL,
	NUM_FANCOIL_MODE_EXT,
};
const char text_fancoil_mode_ext[NUM_FANCOIL_MODE_EXT][LCD_LANG_MAX][STRING_LEN_20] =
{
	{"AUTO COOL", "AUTO JAHUTUS"},
	{"MANUAL COOL", "MANUAAL JAHUTUS"},
};

enum
{
	EDIT_SELECTION_SETPOINT_TEMP,
	EDIT_SELECTION_FANCOIL_MODE,
	NUM_EDIT_SELECTION,
};


const char text_edit_selection[NUM_EDIT_SELECTION][LCD_LANG_MAX][STRING_LEN_20] =
{
	{"Temperature", "Temperatuur"},
	{"Cooling", "Jahutus"},
};

enum
{
	UI_SELECT,
	UI_FLOOR,
	UI_AIR,
	UI_TEMPERATURE,
	UI_LOADING,
	UI_SERIAL_ID,
	UI_ENGLISH,
	UI_EESTI,
	UI_SENSOR,
	UI_CONTROLLER,
	UI_CO2_RECALIBRATE,
	UI_DONE,
	UI_ENTER,
	UI_YES,
	UI_NO,
	UI_CONFIRM,
	UI_OK,
	UI_FANCOIL_MODE_HEADER,
	UI_NUM,
};
const char text_common[UI_NUM][LCD_LANG_MAX][STRING_LEN_120] =
{
	{"Select:", "Valige:"},
	{"Target floor", "Soovitud p" _02 "randa"},
	{"Target air", "Soovitud " _02 "hu"},
	{"temperature","temperatuur"},
	{"Loading..","Laadimine.."},
	{"Serial ID","Seeria ID"},
	{"English","English"},
	{"Eesti","Eesti"},
	{"Sensor", "Andur"},
	{"Controller", "Kontroller"},
	{"CO2 sensor calibration in  progress. It will take up to 3 minutes and device will reboot once complete","Toimub CO2 anduri kalibreerimine. See v" _o "tab maksimaalselt 3 minutit ja p" _a "rast seade taask" _a "ivitub"},
	{"Done", "Tehtud"},
	{"Enter", "Kinnita"},
	{"Yes", "Jah"},
	{"No", "Ei"},
	{"Confirm change","Kinnita muudatus"},
	{"Ok","Ok"},
	{"Fan coil cooling mode", "Jahutusvent. re" _z "iim"},
};

enum
{
	PAIR_LANG_HEADER,
	PAIR_METHOD_HEADER,
	PAIR_METHOD_WIRELESS,
	PAIR_METHOD_WIRED,
	PAIR_HEADER,
	PAIR_OK,
	PAIR_ZONES,
	PAIR_MB_ADDR,
	PAIR_CONTROL_MODE_HEADER,
	PAIR_AIR_TEMP,
	PAIR_AIR_TEMP_MSG,
	PAIR_FLOOR_TEMP,
	PAIR_FLOOR_TEMP_MSG,
	PAIR_DISPLAY_HEADER,
	PAIR_DISPLAY_REGULAR,
	PAIR_DISPLAY_REGULAR_MSG,
	PAIR_DISPLAY_REVERTED,
	PAIR_DISPLAY_REVERTED_MSG,
	PAIR_SELECTED,
	PAIR_PRESS_ENTER,
	PAIR_TEMP_CALIB_MSG,
	PAIR_CONTINUE,
	PAIR_SUCCESS,
	PAIR_NEXT,
	PAIR_CONNECTING,
	PAIR_SELECTED_ID,
	PAIR_CONFIRM,
	PAIR_TIMEOUT,
	PAIR_NO_ZONES_AVAILABLE,
	PAIR_HOLD_OK_BTN,
	PAIR_CONF_NODEID,
	PAIR_WELCOME_NOTE1,
	PAIR_WELCOME_NOTE2,
	PAIR_FLOOR_APPLICATION_HEADER,
	PAIR_COOLING_METHOD_HEADER,
	PAIR_COOLING_METHOD_MANUAL,
	PAIR_COOLING_METHOD_AUTO,
	PAIR_ELECTRIC_FLOOR_HEATING_HEADER,
	PAIR_FLOOR_HEATING_METHOD_HEADER,
	PAIR_HEATING_METHOD_HYDRONIC,
	PAIR_HEATING_METHOD_ELECTRIC,
	PAIR_HEATING_METHOD_HYDRONIC_ELECTRIC,
	PAIR_HEATING_METHOD_AUTO,
	PAIR_FLOOR_COOLING_ALLOWED_HEADER,
	PAIR_CONFIRM_OK_BTN,
	PAIR_SELECT_FANCOIL,
	PAIR_NUM,
};
const char text_pair[PAIR_NUM][LCD_LANG_MAX][STRING_LEN_120] =
{
    {"Language", "Keel"},
	{"Pairing method","Sidumise meetod"},
	{"Wireless","Juhtmeta"},
	{"Wired","Juhtmega"},
	{"Pairing","Sidumine"},
	{"Press OK to continue", "J" _a "tkamiseks vajutage OK"},
	{"Zones","Tsoonid"},
	{"Sensor ID","Anduri ID"},
	{"Control input", "Juhtsisend"},
	{"Air temp.",  _01 "hu temp."},
	{"Air temperature is used to control temperature.", "Juhtimine toimib " _02 "hutemperatuuri j" _a "rgi"},
	{"Floor temp", "P" _02 "randa temp"},
	{"Floor sensor is used to control temperature","Juhtimine toimub p" _02 "randaanduriga p" _02 "randatemperatuuri j" _a "rgi"},
	{"Display", "Ekraan"},
	{"Regular", "Tavaline"},
	{"Regular background", "Tavaline ekraani vaade"},
	{"Black", "Must"},
	{"Black background", "Must ekraanikuva"},
	{"Selected:", "Valitud:"},
	{"ID needs to be unique for each room sensor. Press OK to select", "ID peab olema iga ruumianduri puhul unikaalne. Valimiseks vajutage OK"},
	{"Selected value is added or subtracted to real temp. reading","Valitud v" _a _a "rtus liidetakse/lahutatakse temp. anduri n" _a "idust"},
	{"Pairing..", "Sidumine.."},
	{"Pairing complete", "Sidumine l" _02 "petatud"},
	{"You can continue to next room sensor or if all done then exit room controller pairing mode", "V" _02 "ite seadistada j" _a "rgmise ruumianduri v" _02 "i kui k" _02 "ik on tehtud, siis l" _02 "petage ruumianduri sidumisre" _z "iim"},
	{"Connecting with room controller..", _U"hendamine ruumikontrolleriga.."},
	{"Selected ID:", "Valitud ID:"},
	{"Confirm","Kinnita"},
	{"Pairing has failed. Make sure room controller is also in pairing mode.","Sidumine eba" _02 "nnestus. Veenduge, et ruumikontroller oleks ka sidumisre" _z "iimis."},
	{"Pairing has failed. There is no available zones.","Sidumine eba" _02 "nnestus. Saadaolevaid tsoone pole."},
	{"Hold OK to confirm", "Hoia OK kinnitamiseks"},
	{"Make sure this is unique for each room sensor.", "Veenduge, et see oleks iga ruumianduri jaoks unikaalne."},
	{"Welcome, please complete initial", "Tere tulemast, palun t" _a "itke initsiaal"},
	{"Open user manual while doing setup", "Avage seadistamise ajal kasutusjuhend"},
	{"Cooling application", "Jahutuse rakendus"},
	{"Cooling activ. method", "Jahutuse aktiveerimise meetod"},
	{"Manual", "Manuaal"},
	{"Auto - GPI", "Auto - GPI"},
	{"Set up electrical floor heating?", "Seadista elektriline p" _02 "randak" _u "te?"},
	{"Heat. method", "K" _u "tte meetod"},
	{"Hydronic", "Vesi"},
	{"Electrical", "Elektriline"},
	{"Hydro+Elec.", "Vesi ja elekt."},
	{"Auto switchover", "Automaatne " _u "mberl" _u "litus"},
	{"Is floor cooling allowed in this zone?", "Kas p" _02 "randajahutus on selles tsoonis lubatud?"},
	{"Press OK to confirm", "Vajutage OK kinnitamiseks"},
	{"Select fan coil","Valige jahutusvent."},
};    

enum{
	ERROR_AIR_SENSOR,
	ERROR_FLOOR_SENSOR,
	ERROR_CO2_SENSOR,
	ERROR_RF_SENSOR,
	ERROR_CODE,
	ERROR_REBOOT_MSG,
	ERROR_DEVICE_DISABLED,
	ERROR_CONTACT_MSG,
	ERROR_TEXT,
	ERROR_POWEROFF_TEXT,
	ERROR_TURNON_TEXT,
	ERROR_NOCONNECTION_MSG,
	ERROR_SKIP_MSG,
	ERROR_NO_EXT_CONNECTION,
	ERROR_ZONES_OVERLAP_MSG,
	ERROR_NUM,
};

const char text_error[ERROR_NUM][LCD_LANG_MAX][STRING_LEN_60] =
{
	{"Air temp error!", _01 "hu temp. anduri h" _a "ire"},
	{"Floor temp error!", "P" _02 "randa anduri h" _a "ire"},
	{"CO2 error!", "CO2 h" _a "ire"},
	{"RF error!", "RF h" _a "ire"},
	{"Code(dec)", "Kood(dec)"},
	{"Press OK to reboot", "Taask" _a"ivitamiseks vajuta OK "},
	{"Device Disabled", "Seade keelatud"},
	{"Contact www.airobothome.com", "Veeb www.airobothome.com"},
	{"Error","H" _a "ire"},
	{"Power Off","Lülita v" _a "lja"},
	{"Turn on", "L" _u "lita sisse"},
	{"Connection lost with room controller", "Puudub " _u "hendus ruumikontrolleriga"},
	{"Press OK to skip", "J" _a "tkamiseks vajutage OK"},
	{"No connection with extension board", "Puudub " _u "hendus laiendigaardiga"},
	{"Zones overlap detected", "Tsoonide kattumine tuvastatud"},
};

enum
{
	PAGE_MOBILE_APP,
	NUM_MOBILE_APP,
};

enum
{
	MOBILE_APP_ID,
	MOBILE_APP_PASS,
	MOBILE_APP_STEP_1,
	MOBILE_APP_STEP_2,
	MOBILE_APP_MSG_1,
	MOBILE_APP_MSG_2,
	MOBILE_APP_ERROR_MSG,
	MOBILE_APP_NUM
};

const char text_mobileapp[MOBILE_APP_NUM][LCD_LANG_MAX][STRING_LEN_100] =
{
	{"ID:", "ID:"},
	{"Password:", "Parool:"},
	{"Step 1:", "Samm 1:"},
	{"Step 2:", "Samm 2:"},
	{"Open Airobot app","Avage Airobot mobiilirakendus"},
	{"Scan QR code to add device to your account","Skanneerige QR kood ja lisage enda kontole"},
	{"Room controller is not registered yet. You need to connect it to the internet network","Ruumikontroller ei ole veel registreeritud. Peate selle " _u "hendama internetiv" _02"rku"},
};

enum
{
    OTA_STAT_NONE,
    OTA_STAT_INPROGRESS,
    OTA_STAT_COMPLETED,
	OTA_STAT_DOWNLOAD,
	OTA_STAT_TITLE,
	OTA_STAT_NUM,
};

const char text_OTA[OTA_STAT_NUM][LCD_LANG_MAX][STRING_LEN_60]=
{
    {"Updating None","Uuendan MITTE" _U"KSKI"},
    {"Firmware update in progess..","Toimub p" _u "sivara uuendamine.."},
    {"Restarting..", "Taask" _a "ivitamine.."},
    {"Progress..","Edenemine.."},
    {"Updating","Uuendan"},
};

language_e get_lang();
void set_lang(language_e language);
void read_language();

#endif /* _TEXT_H_ */