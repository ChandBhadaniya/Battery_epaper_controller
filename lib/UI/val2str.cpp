/*
 * val2str.c
 *
 *  Created on: Oct 29, 2015
 *      Author: andrei
 */

#include "stringcnv.h"

#include "val2str.h"

void temp2str_1_dec_point(int16_t t, char* str, uint8_t len) {
	if (len < 6) return;
	if (t > 999 || t < -999) {
		str[0] = '-';
		str[1] = '\0';
		return;
	}
	if (t < 0) {
		*str++ = '-';
		t = -t;
	}

	if(t>99)
		*str++ = '0'+((t / 100) % 100);
	*str++ = '0'+((t / 10) % 10);
	*str++ = '.';
	*str++ = '0'+((t) % 10);
	*str = '\0';
}

void temp2str(int16_t t, char* str, uint8_t len) {
	if (len < 6) return;
	if (t > 999 || t < -999) {
		str[0] = '-';
		str[1] = '\0';
		return;
	}
	if (t < 0) {
		*str++ = '-';
		t = -t;
	}

	*str++ = '0'+((t / 100) % 100);
	*str++ = '0'+((t / 10) % 10);
	*str++ = '.';
	*str++ = '0'+((t) % 10);
	*str = '\0';
}

void hum2str(int16_t h, char* str, uint8_t len) {
	if (len < 6) return;
	if (h > 999 || h < 0) {
		str[0] = '-';
		str[1] = '\0';
		return;
	}
	co2str(h, str, len);
}

void co2str(int16_t co2, char* str, uint8_t len) {
	if (len < 6) return;
	if (co2 < 0 || co2 > 10000) {
		str[0] = '-';
		str[1] = '\0';
		return;
	}
	dec2str((uint32_t)co2, str, len);
}

uint16_t bit2index(uint8_t num) {
    for (int i = 0; i < 8; i++) {
        if (num & (1 << i)) {
            return i+1;  // Return the index of the first ON bit
        }
    }
    return 0;  // This line should never be reached for non-zero input
}


void index2str(int16_t index, char* str, uint8_t len) {
	if (len < 6) return;
	if (index < 0 || index > 500) {
		str[0] = '-';
		str[1] = '\0';
		return;
	}
	dec2str((uint32_t)index, str, len);
}

void fan2str(int16_t fan, char* str, uint8_t len) {
	if (len < 6) return;
	dec2str((uint32_t)fan, str, len);
}

void ver2str(uint16_t ver, char* str, uint8_t len) {
	if (len < 8) return;
	if (ver == 0xffff) {
		str[0] = '-';
		str[1] = '\0';
		return;
	}
	uint8_t d = dec2str(ver >> 8, str, len);
	str[d++] = '.';
	dec2str(ver & 0xff, str + d, len - d + 1);
}

void hexstr(uint16_t hex, char* str, uint8_t len) {
	if (len < 8) return;
	if (hex == 0xffff) {
		str[0] = '-';
		str[1] = '\0';
		return;
	}
	str[0] = (uint8_t)(((hex >> 12) & 0x000F) + '0');
	str[1] = (uint8_t)(((hex >>  8) & 0x000F) + '0');
	str[2] = (uint8_t)(((hex >>  4) & 0x000F) + '0');
	str[3] = (uint8_t)(((hex >>  0) & 0x000F) + '0');
	str[4] = '\0';
}
