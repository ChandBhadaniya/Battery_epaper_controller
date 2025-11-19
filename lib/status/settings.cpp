//#include "nvs_header.h"
#include "settings.h"
#include "pair.h"
#include "disp_common.h"
//#include "esp_mac.h"
//#include "esp_random.h"
#include "gfx.h"
//#include "hw_version.h"
#include "ui.h"
#include "status.h"
// #include "mb_slave.h"

#define TAG "SETT"

#define DEFAULT_MODE FLAGS_SENSOR_MODE_STANDALONE
#define DEFAULT_STATUS_INTERVAL 5 // 30
#define DEFAULT_NODE_ID 0
#define DEFAULT_NETWORK_ID 0xFF
#define DEFAULT_CHANNEL 1
#define DEFAULT_PAN 0xabcd
#define DEFAULT_UDP_PORT 1234
#define DEFAULT_SETPOINT_TEMP 225
#define DEFAULT_SETPOINT_TEMP_AWAY 150
#define DEFAULT_SETPOINT_HUMIDITY 400
#define DEFAULT_SETPOINT_CO2 500
#define DEFAULT_DISP_MODE  DISPLAY_REGULAR
pairing_sett_s pairingsett = {};
settings_t settings = {};
union sensor_status_e status = {};
config_s dev_config = {};
ext_settings ext_sett = {0};
pairing_flag_s pairing_flag = {PAIRING_DONE};
uint32_t flag_is_dev_paired = 0;
uint32_t display_mode = 0;
uint16_t temp_setpointTemp = 0, temp_setpointAway = 0;
uint8_t is_setpoint_change = 0;
global_settings_t global_settings;
extern uint32_t old_saved_modbus_baudrate;

void fetching_local_settings();
void read_dev_pair_stat();

void init_settings_and_status(void)
{
//    size_t read_len = 0;
//    esp_err_t err;
//
//    ESP_LOGI(TAG, "Fetching global setting");
//    err = Read_Blob(NAMESPACE_EPC, KEY_GLOBAL_SETTINGS, (uint8_t *)&global_settings, &read_len);
//    if (err != ESP_OK)
//    {
//        memset((uint8_t *)&global_settings, 0x00, sizeof(global_settings_t));
//        old_saved_modbus_baudrate = get_baudrate_value(BAUDRATE_19200);
//    }
//    else
//    {
//        old_saved_modbus_baudrate = global_settings.modbus_baudrate;
//        ESP_LOGI(TAG, "global_settings.modbus_baudrate %d", global_settings.modbus_baudrate);
//    }
//    read_len = 0;
//
//    ESP_LOGI(TAG, "Fetching sensor setting");
//    err = Read_Blob(NAMESPACE_EPC, KEY_SENSOR_SETTINGS, (uint8_t *)&settings, &read_len);
//    if (err != ESP_OK)
//    {
//        settings.node_id = DEFAULT_NODE_ID;
//        settings.setpoint_temp = DEFAULT_SETPOINT_TEMP;
//        settings.setpoint_temp_away = DEFAULT_SETPOINT_TEMP_AWAY;
//        settings.control_input = CONTROLMODE_AIR_TEMP;
//    }
//    temp_setpointTemp = settings.setpoint_temp;
//    temp_setpointAway = settings.setpoint_temp;
//    settings.flags &= ~FLAGS_HEC_SET_RELAY_ON;
//
//    memset((uint8_t *)&status, 0x00, sizeof(sensor_status_e));
//    status.model_no = MN_WIRELESS_EPC_SENSOR;
//    status.hw_ver = (read_hw_version(MAJOR) << 8 |read_hw_version(MINOR));
//    status.fw_ver = ((DEVICE_FW_VERSION_MAJOR << 8) | DEVICE_FW_VERSION_MINOR);
//    status.serial_no = settings.node_id;
//    status.error = 0;
//    status.flag = FLAGD_SENSOR_SETT_UPDATED|FLAGD_GLOBAL_SETT_REQUEST|FLAGD_EXT_SETT_REQUEST|FLAGD_CONFIGURATION_REQUEST|FLAGD_EXT_STATUS_REQUEST;
//    status.air_temp = TEMP_INVALID;
//    status.air_hum = HUM_INVALID;
//    status.air_co2 = CO2_INVALID;
//    status.floor_temp = TEMP_INVALID;
//
//    ESP_LOGI(TAG, "|| Firmware version: %d.%d (%d) ||  Hardware Version: %d.%d (%d) || Node ID: %d ||", (uint32_t)((status.fw_ver>>8)&0xFF), (uint32_t)(status.fw_ver&0xFF), status.fw_ver, (uint32_t)((status.hw_ver>>8)&0xFF), (uint32_t)(status.hw_ver&0xFF), status.hw_ver, settings.node_id);
//    fetching_local_settings();
//    if(!is_dev_paired())
//    {
//        settings.node_id = DEFAULT_NODE_ID;
//    }
}

void read_disp_mode()
{
//    esp_err_t err;

//    err = Read_Byte(NAMESPACE_LOCAL_DATA, KEY_DISPLAY, &display_mode);
//    if (err != ESP_OK)
//        display_mode = DEFAULT_DISP_MODE;
}

uint32_t get_disp_type()
{
    return 1;
}

void fetching_local_settings()
{
//    read_dev_pair_stat();
//    read_disp_mode();
//    set_disp(display_mode);
//    read_language(); 
}

void settings_save_to_nvs(nvs_save_settings_e src)
{
    switch (src)
    {
//    case NVS_SETTINGS_SENSOR:
//        ESP_LOGI(TAG, "Saving NVS_SETTINGS_SENSOR %d", global_settings.modbus_baudrate);
//        Write_Blob(NAMESPACE_EPC, KEY_SENSOR_SETTINGS, (uint8_t *)&settings, sizeof(settings));
//        break;
//    case NVS_SETTINGS_ALL:
//        ESP_LOGI(TAG, "Saving NVS_SETTINGS_ALL %d", global_settings.modbus_baudrate);
//        Write_Blob(NAMESPACE_EPC, KEY_SENSOR_SETTINGS, (uint8_t *)&settings, sizeof(settings));
//        Write_Blob(NAMESPACE_EPC, KEY_GLOBAL_SETTINGS, (uint8_t *)&global_settings, sizeof(global_settings_t));
//        break;
//    case NVS_FACTORY_RESET:
//        ESP_LOGI(TAG, "Saving NVS_FACTORY_RESET %d", global_settings.modbus_baudrate);
//        Erase_Flash();
//        break;
//    case NVS_SETTINGS_GLOBAL:
//        ESP_LOGI(TAG, "Saving global_settings.modbus_baudrate %d", global_settings.modbus_baudrate);
//        Write_Blob(NAMESPACE_EPC, KEY_GLOBAL_SETTINGS, (uint8_t *)&global_settings, sizeof(global_settings_t));
//        break;
    }
}

void settings_sensor_paired(setting_src_e src)
{
//    switch (src)
//    {
//    case SRC_MODBUS:
//        pairingsett.status = PS_PAIRED;
//        break;
//    case SRC_RF:
//        pairingsett.status = PS_PAIRED;
//        break;
//    }
//    set_dev_pair_stat(1);
}

void settings_request()
{
//    status.flag |= FLAGD_SENSOR_SETT_REQUEST;
//    status.flag |= FLAGD_GLOBAL_SETT_REQUEST;
//    status.flag |= FLAGD_EXT_SETT_REQUEST;
//    status.flag |= FLAGD_EXT_STATUS_REQUEST;
}

void configuration_request()
{
//    status.flag |= FLAGD_CONFIGURATION_REQUEST;
}

uint8_t is_setpoint_update_from_server()
{
    return 1;
}

void setpoint_screen_popup_success()
{
//    is_setpoint_change = 0;
}

void settings_updated(setting_src_e src, uint16_t settings_type)
{
//    switch (src)
//    {
//    case SRC_UI:
//        status.flag |= settings_type;
//        ESP_LOGI("TAG", "status.flag: %d", status.flag);
//        break;
//    case SRC_MODBUS:
//        {
//            if(((temp_setpointTemp != settings.setpoint_temp) && (!(ext_sett.icon_stat & ICON_AWAY_MODE)) && (temp_setpointTemp != 0)) || 
//                ((temp_setpointAway != settings.setpoint_temp_away) && (ext_sett.icon_stat & ICON_AWAY_MODE) && (temp_setpointAway != 0)))
//            {
//                if(is_dev_paired())
//                {
//                    is_setpoint_change = 1;
//                }
//                set_lcd_force_update();
//            }
//            temp_setpointTemp = settings.setpoint_temp;
//            temp_setpointAway = settings.setpoint_temp_away;
//        }
//        break;
//    }
//    if (settings.flags & FLAGS_HEC_FACTORY_RESET)
//    {
//        settings.flags &= ~FLAGS_HEC_FACTORY_RESET;
//        settings_save_to_nvs(NVS_FACTORY_RESET);
////        esp_restart();
//        return;
//    }
//    else if (settings.flags & FLAGS_HEC_REBOOT)
//    {
//        settings.flags &= ~FLAGS_HEC_REBOOT;
////        esp_restart();
//        // reboot
//    }
//
//    if((settings_type & FLAGD_SENSOR_SETT_REQUEST))
//    {
//        ESP_LOGI("TAG", "1FLAGD_SENSOR_SETT_REQUEST: %d", settings_type);
//        settings_save_to_nvs(NVS_SETTINGS_ALL);
//    }
//    else
//    {
//        if(status.flag & FLAGD_SENSOR_SETT_UPDATED)
//        {
//            ESP_LOGI("TAG", "2FLAGD_SENSOR_SETT_UPDATED: %d", settings_type);
//            settings_save_to_nvs(NVS_SETTINGS_SENSOR);
//        }
//    }
//
//    if((settings_type & FLAGD_GLOBAL_SETT_REQUEST))
//    {
//        ESP_LOGI("TAG", "3FLAGD_GLOBAL_SETT_REQUEST: %d", settings_type);
//        settings_save_to_nvs(NVS_SETTINGS_ALL);
//    }
//    else
//    {
//        if(status.flag & FLAGD_GLOBAL_SETT_UPDATED)
//        {
//            ESP_LOGI("TAG", "4FLAGD_GLOBAL_SETT_UPDATED: %d", settings_type);
//            settings_save_to_nvs(NVS_SETTINGS_GLOBAL);
//        }
//    }
}

void settings_fetched(sensor_data_flag_e src)
{
//    status.flag &= ~src;
}

void sensor_settings_update(settings_t src)
{
//    settings.node_id = src.node_id;
//    settings.flags = src.flags;
//
//    settings_save_to_nvs(NVS_SETTINGS_SENSOR);
}

uint32_t get_serial_number(void)
{
//    uint8_t base_mac_addr[6] = {0};
//    esp_err_t ret = ESP_OK;
//
//    ret = esp_efuse_mac_get_default(base_mac_addr);
//    if (ret != ESP_OK) 
//    {
//        ESP_LOGE(TAG, "Failed to get base MAC address from EFUSE BLK0. (%s)", esp_err_to_name(ret));
//    } 
//    else 
//    {
//        ESP_LOGI(TAG, "Base MAC Address read from EFUSE BLK0");
//    }
//
//    return (uint32_t)(base_mac_addr[2]<<24 | base_mac_addr[3]<<16 | base_mac_addr[4]<<8 | base_mac_addr[5]);
}

void set_disp(uint32_t disp_mode)
{

	if(disp_mode == DISPLAY_REVERTED)
    {
		myScreen.invert(true);
    }
	else
    {
        myScreen.invert(false);
    }
     if(display_mode != disp_mode)
     {
        display_mode = disp_mode;
//        Write_Byte(NAMESPACE_LOCAL_DATA, KEY_DISPLAY, (uint32_t)display_mode);
     }
    return;
}

uint32_t is_dev_paired(void)
{
    return 1;
}

void read_dev_pair_stat()
{
//	if(Read_Byte(NAMESPACE_LOCAL_DATA, KEY_IS_DEV_PAIRED, &flag_is_dev_paired))
//	{
//		flag_is_dev_paired = 0;
//	}
}

void set_dev_pair_stat(uint32_t pair)
{
//    flag_is_dev_paired = pair;
//    Write_Byte(NAMESPACE_LOCAL_DATA, KEY_IS_DEV_PAIRED, flag_is_dev_paired);
}
