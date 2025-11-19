/*
 * val2str.h
 *
 *  Created on: Oct 29, 2015
 *      Author: andrei
 */

#ifndef UI_VAL2STR_H_
#define UI_VAL2STR_H_

// Configuration
#include "hV_Configuration.h"

void temp2str_1_dec_point(int16_t t, char* str, uint8_t len);
void temp2str(int16_t t, char* str, uint8_t len);
void hum2str(int16_t h, char* str, uint8_t len);
void co2str(int16_t co2, char* str, uint8_t len);
uint16_t bit2index(uint8_t num);
void index2str(int16_t index, char* str, uint8_t len);
void fan2str(int16_t fan, char* str, uint8_t len);
void ver2str(uint16_t ver, char* str, uint8_t len);
void hexstr(uint16_t hex, char* str, uint8_t len);

#endif /* UI_VAL2STR_H_ */
