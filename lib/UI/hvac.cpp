/*
 * hvac.c
 *
 *  Created on: Jun 25, 2021
 *      Author: Biren
 */

#include "hvac.h"
#include "ui.h"
//#include "sht4x.h"
//#include "scd4x.h"
#include "settings.h"
//#include "ntc_temp_sensor.h/"
#include "pair.h"
#include "disp.h"
#include "status.h"

#define AVG_NO_OF_SAMPLES       5
#define CFG_HOLD_OFF_MS 10000
#define CFG_REQUEST_INTERVAL_MS (60 * 60 * 1000)
#define STATUS_TIMEOUT_MS (1 * 70 * 1000)
#define HARDCODED_AIR_TEMP_CALIBRATION 	-6

uint8_t DATA_REQUEST_INTERVAL = SHT4X_INITIAL_SCAN_RATE;
uint8_t is_dev_status_timeout = 0;
uint8_t is_calibration_started =0;
struct
{
	struct
	{
		uint32_t last_sent_ts;
		uint32_t last_change_ts;
		uint32_t last_ts;
	} conf;
	struct
	{
		uint32_t last_ts;
	} status;
} static hvac;

typedef enum
{
	STATE_NONE = 0,
	READ_STATUS,
	READ_CONFIG,
	READ_SETTINGS,
	UPDATE_SETTINGS,
	READ_FW_VER,
} app_state;

uint8_t is_dev_connection_error()
{
	return is_dev_status_timeout;
}

void hvac_report_conf()
{
	hvac.conf.last_ts = millis();
}

void hvac_conf_change(uint16_t settings_type)
{
	ESP_LOGI("TAG", "hvac_conf_change: %d", settings_type);
	if(settings_type == FLAGD_GLOBAL_SETT_UPDATED)
	{
//		if(global_sett_recvd_once == 0)
//		{
//			ESP_LOGI("TAG", "global_sett_recvd_once: %d", global_sett_recvd_once);
//			status.flag |= FLAGD_GLOBAL_SETT_REQUEST;
//			return;
//		}
	}
	settings_updated(SRC_UI, settings_type);
}

void hvac_conf_changes_sent()
{
	hvac.conf.last_sent_ts = millis();
}

void hvac_report_status()
{
	is_dev_status_timeout = 0;
	hvac.status.last_ts = millis();
}

void hvac_conf_reset()
{
	settings.flags |= FLAGS_HEC_FACTORY_RESET;
	hvac_conf_change(FLAGD_SENSOR_SETT_UPDATED);
}

uint8_t hvac_is_available()
{
	return 1;
	// return hvac.conf.available && hvac.status.available;
}

void hvac_task()
{
	if (millis() - hvac.conf.last_ts > (CFG_REQUEST_INTERVAL_MS+(settings.node_id*60*1000)))
	{
		settings_request();
		configuration_request();
		hvac.conf.last_ts += CFG_REQUEST_INTERVAL_MS;
	}

//	if ((millis() - hvac.status.last_ts > STATUS_TIMEOUT_MS) && (is_dev_paired()) && (ota_started == false))
	{
		is_dev_status_timeout = 1;
		settings_request();
		hvac.status.last_ts = millis();
	}
}

uint16_t avg_air_temp(float latest_temp)
{
	static uint16_t air_temp_arr[AVG_NO_OF_SAMPLES] = {0};
	static uint8_t air_temp_index = 0, index_count = 0;
//	uint16_t air_temperature = 0;
//	index_count = (++index_count<AVG_NO_OF_SAMPLES)?index_count:AVG_NO_OF_SAMPLES;
//	air_temp_arr[air_temp_index] = (uint16_t)(latest_temp * 10);
//	air_temp_index = ((++air_temp_index)<AVG_NO_OF_SAMPLES)?air_temp_index:0;
//	for(uint8_t i=0; i<index_count; i++)
//	{
//		air_temperature += air_temp_arr[i];
//	}
//	air_temperature /= index_count;
	return 1;
}

uint16_t avg_humidity(float latest_hum)
{
	static uint16_t humid_arr[AVG_NO_OF_SAMPLES] = {0};
	static uint8_t humid_index = 0, humid_index_count = 0;
//	uint16_t humidity = 0;
//	humid_index_count = (++humid_index_count<AVG_NO_OF_SAMPLES)?humid_index_count:AVG_NO_OF_SAMPLES;
//	humid_arr[humid_index] = (uint16_t)(latest_hum * 10);
//	humid_index = ((++humid_index)<AVG_NO_OF_SAMPLES)?humid_index:0;
//	for(uint8_t i=0; i<humid_index_count; i++)
//	{
//		humidity += humid_arr[i];
//	}
//	humidity /= humid_index_count;
	return 1;
}

uint16_t avg_co2(uint16_t latest_co2)
{
	static uint16_t co2_readings_arr[AVG_NO_OF_SAMPLES] = {0};
	static uint8_t co2_readings_index = 0, co2_index_count = 0;
//	uint16_t co2_val = 0;
//	co2_index_count = (++co2_index_count<AVG_NO_OF_SAMPLES)?co2_index_count:AVG_NO_OF_SAMPLES;
//	co2_readings_arr[co2_readings_index] = (latest_co2);
//	co2_readings_index = ((++co2_readings_index)<AVG_NO_OF_SAMPLES)?co2_readings_index:0;
//	for(uint8_t i=0; i<co2_index_count; i++)
//	{
//		co2_val += co2_readings_arr[i];
//	}
//	co2_val /= co2_index_count;
	return 1;
}

uint8_t hvac_read_sensors()
{
	static uint8_t scd4x_read_retry = 0, sht4x_read_retry = 0;
	static uint16_t CO2;
	static uint32_t scd4x_read_counter = 0, sht4x_read_counter = 0;
	static float Temp, Hum;

if((settings.flags & FLAGS_HEC_CO2_FORCED_RECALIBRATION) && (is_scd4x_sensor_present == 1))
{
//	is_calibration_started = 1;
//	settings.flags &= ~FLAGS_HEC_CO2_FORCED_RECALIBRATION;
//	forced_co2_calibration_task_init();
}
else if(wait_for_s(DATA_REQUEST_INTERVAL, &sht4x_read_counter))
	{
		//SHT for temp & humidity
//		if(sht4x_read(&Temp, &Hum))
//		{
//			status.air_temp = avg_air_temp(Temp) + settings.temp_calibration;
//			status.air_temp += HARDCODED_AIR_TEMP_CALIBRATION;
//			status.air_hum = avg_humidity(Hum) + (settings.hum_calibration);
//			status.error &= ~ERROR_AIRTEMP;
//			sht4x_read_retry = 0;
//			DATA_REQUEST_INTERVAL = SHT4X_NORMAL_SCAN_RATE;
//		}
//		else
		{
			DATA_REQUEST_INTERVAL = SHT4X_ERROR_SCAN_RATE;
			status.air_temp = TEMP_INVALID;
			status.air_hum = HUM_INVALID;
			sht4x_read_retry++;
		}
//		if(sht4x_read_retry > MAX_SHT4x_READ_ERR_COUNT)
//		{
//			status.air_temp = TEMP_INVALID;
//			status.air_hum = HUM_INVALID;
//			status.error |= ERROR_AIRTEMP;
//			sht4x_read_retry = 0;
//		}
#ifdef DEBUG_LOGS
		hV_HAL_Debug_println("Temp:%d Hum:%d", status.air_temp, status.air_hum);
#endif

		// for co2 sensor
//		if(is_scd4x_sensor_present == 1)
//		{
//			if(is_scd4x_sensor_data_ready())
//			{
//				if(scd4x_read_co2(&CO2, &Temp, &Hum))
//				{
//					status.air_co2 = avg_co2(CO2)+ settings.co2_calibration;
//					// sensor_data.aqi = get_aqi_level(CO2);
//					status.error &= ~ERROR_CO2;
//					scd4x_read_retry = 0;
//				}
//				else
//				{
//					scd4x_read_retry++;
//				}
//			}
//			else
//			{
//				scd4x_read_retry++;
//			}
//			if(scd4x_read_retry > MAX_SCD4x_READ_ERR_COUNT)
//			{
//				status.air_co2 = CO2_INVALID;
//				status.error |= ERROR_CO2;
//				scd4x_read_retry = 0;
//			}
//		}			
//		else
//		{
//			status.air_co2 = CO2_INVALID;
//			status.error &= ~ERROR_CO2;
//		}
#ifdef DEBUG_LOGS
			hV_HAL_Debug_println("Co2: %d", sensor_data.air_co2);
#endif

		//for floor sensor
//		if (settings.control_input != CONTROLMODE_FLOOR_TEMP)
//		{
//			status.floor_temp = TEMP_INVALID;
//			status.error &= ~ERROR_FLOOR_TEMP;
//		}
//		else
//		{
//			read_floor_sensor();
//		}
	}
	return 0;
}
