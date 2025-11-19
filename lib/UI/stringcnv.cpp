/*
 * stringcnv.c
 *
 *  Created on: 16.09.2014
 *      Author: anti
 */

#include "stringcnv.h"

uint32_t dec2str(uint32_t num, char *str, uint32_t len) {
	if (len == 0) return 0;
	str[0] = '\0';

	uint8_t tmp[11] = {0};
	int cnt = 0;
	while(num || cnt == 0) {
		tmp[cnt++] = num % 10;
		num /= 10;
	}
	if (len < cnt+1) return 0;	// does not fit
	uint32_t cnt2 = cnt;
	while(cnt--) {
		*str++ = '0'+tmp[cnt];
	}
	*str++ = 0;
	return cnt2;
}

uint32_t int2str(int32_t val, char *str, uint32_t len) {
	if (len == 0) return 0;
	str[0] = '\0';

	int32_t tmp[11] = {0};
	int32_t num = val;
	int cnt = 0;
	uint32_t cnt2 = cnt;

	if (val < 0) {
		*str++ = '-';
		cnt2++;
		num *= -1;
	}

	while(num != 0 || cnt == 0) {
		tmp[cnt++] = num % 10;
		num /= 10;
	}

	if (len < cnt+1) return 0;	// does not fit

	while(cnt--) {
		*str++ = '0'+tmp[cnt];
	}
	*str++ = 0;
	return cnt2;
}

