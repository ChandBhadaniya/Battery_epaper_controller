/*
 * hvac.h
 *
 *  Created on: Jun 25, 2021
 *      Author: Biren
 */

#ifndef COMM_CNX_H_
#define COMM_CNX_H_

// Configuration
#include "hV_Configuration.h"

#include "common_def.h"
#include "settings.h"

#define HVAC_TEMP_INVALID		0x7FFF
#define HVAC_CO2_PPM_INVALID	0xFFFF
#define HVAC_HUM_INVALID		0xFFFF
#define HVAC_PM_INVALID			0xFFFF
#define SHT4X_INITIAL_SCAN_RATE		(2) //all in seconds
#define SHT4X_ERROR_SCAN_RATE		(5)
#define SHT4X_NORMAL_SCAN_RATE		(30)

extern uint8_t DATA_REQUEST_INTERVAL;
extern uint8_t is_calibration_started;
uint8_t hvac_is_available();
void hvac_report_status(); 			//from hvac
void hvac_report_conf(); 			//from hvac
void hvac_report_sys(); 			//to hvac
void hvac_conf_reset();
void hvac_conf_change(uint16_t settings_type); 			//to hvac
uint8_t is_hvac_conf_changed();
void hvac_conf_change_done();
void hvac_conf_changes_sent();
uint8_t hvac_read_sensors();
uint8_t is_dev_connection_error();
void hvac_task();

#endif /* COMM_CNX_H_ */
