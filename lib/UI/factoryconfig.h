/*
 * factoryconfig.h
 *
 *  Created on: Apr 17, 2021
 *      Author: Biren
 */

#ifndef CONFIG_INC_FACTORYCONFIG_H_
#define CONFIG_INC_FACTORYCONFIG_H_

#define MODBUS_CONFIG_BUF_LENGTH	10
#define CONFIG_COMM_SIZE sizeof(factorycfg_t)

typedef union
{
	uint16_t data[MODBUS_CONFIG_BUF_LENGTH];
	struct __attribute__((packed, aligned(4)))
	{
		uint32_t deviceid;
		uint8_t macaddr[6];
		uint16_t model;
		uint16_t uchw;
		uint32_t date;
	};
}factorycfg_t;

int factorycfg_init();

#endif /* CONFIG_INC_FACTORYCONFIG_H_ */
