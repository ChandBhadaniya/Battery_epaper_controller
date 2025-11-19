/*
 * stringcnv.h
 *
 *  Created on: 16.09.2014
 *      Author: anti
 */

#ifndef STRINGCNV_H_
#define STRINGCNV_H_

// Configuration
#include "hV_Configuration.h"

/* Converts number to string */
uint32_t dec2str(uint32_t num, char *str, uint32_t len);
uint32_t int2str(int32_t val, char *str, uint32_t len);

#endif /* STRINGCNV_H_ */
