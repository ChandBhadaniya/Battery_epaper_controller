#ifndef _STATUS_H_
#define _STATUS_H_

#include <stdint.h>
//#include "common_def.h"

enum                                        // Per HEC error flags
{   
    SENSOR_DISCONNECT           = 1<<0,         // No sensor data received
    AIR_TEMP_SENSOR_ERROR       = 1<<1,         // Invalid Temperature/Humidity received
    FLOOR_TEMP_SENSOR_ERROR     = 1<<2,         // Invalid Floor temp received    
    EXT_DISCONNECT_ERROR        = 1<<3,         // Extension not connected   
    ZONES_OVERLAP_ERROR         = 1<<4         // Two sensors assigned to same zone   
}hec_error_flag_e;

enum 
{
    STATUS_HEC_FLAG_HEATING_REQUESTED       = 1<<0,
    STATUS_HEC_FLAG_ACTUATOR_ON             = 1<<1,
    STATUS_COOLING_BLOCKED_AWAY_MODE        = 1<<2,
    STATUS_COOLING_BLOCKED_HIGH_HUMIDITY    = 1<<3,
    STATUS_COOLING_REQ_HYDRONIC             = 1<<4,
    STATUS_HEAT_REQ_ELECTRIC                = 1<<5,
    STATUS_HEAT_REQ_HYDRONIC                = 1<<6,
    STATUS_COOLING_FLOOR_BLOCKED            = 1<<7,
    STATUS_COOLING_FANCOIL                  = 1<<8,
}statusFlags_hec_e;

#define MB_EXTENDED_STATUS_HEC_LEN		15
typedef union hec_extended_status_s
{
	uint16_t data[MB_EXTENDED_STATUS_HEC_LEN];
	struct __attribute__((__packed__))
	{
		uint8_t 	fancoil_speed;
		uint8_t 	fancoil_output_v;
		uint16_t 	fancoil_opr_status;
        uint16_t    hec_error_dup;
		uint16_t	hec_flags_dup;
		uint8_t 	reserve[22];
	};
} ;
extern hec_extended_status_s hec_extended_status;

// Sensor Status
#define MB_SENSOR_STATUS_LEN 		23
union sensor_status_e
{
	uint16_t data[MB_SENSOR_STATUS_LEN];
	struct __attribute__((__packed__))
	{
		uint32_t serial_no;
		uint16_t model_no;
		uint16_t hw_ver;
		uint16_t fw_ver;
		int16_t  air_temp;
		uint16_t air_hum;
		int16_t  floor_temp;
		uint16_t air_co2;
		uint16_t aqi;
		uint32_t device_uptime;
		uint32_t relay_uptime;
		uint32_t reset_reason;
		uint16_t flag;
		uint16_t error;
		uint16_t fallback;
		uint16_t reserve[4];
	};
} ;


void get_error_status();

#endif /* _STATUS_H_ */