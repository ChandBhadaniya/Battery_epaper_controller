///
/// @file hV_Serial.h
/// @brief File library for Pervasive Displays iTC monochrome and colour screens
///
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @author Rei Vilo
/// @date 21 Jun 2023
/// @version 611
///
/// @copyright (c) Rei Vilo, 2010-2023
/// @copyright All rights reserved
///
/// * Evaluation edition: for professionals or organisations, no commercial usage
/// * Commercial edition: for professionals or organisations, for commercial usage
///

#ifndef hV_SERIAL_RELEASE
///
/// @brief Release
///
#define hV_SERIAL_RELEASE 611

/// @name Speed
///
/// Configuration
///
/// + Panel iTC 2.71" 176x264
/// + Micro-SD card Samsung EVO HC1 Class 1, FAT
/// + Board MSP42P401R LaunchPad with micro-SD BoosterPack
///
/// SPI at 4 MHz
///
/// Format | Write (ms) | Size (kB) | Read (ms)
/// ---- | ---- | ---- | ----
/// BMP | 5881 | 93.0 | 4143
/// Image | 2571 | 38.9 | -
/// P4 | 511 | 5.8 | 238
//
/// SPI at 8 MHz
//
/// Format | Write (ms) | Size (kB) | Read (ms)
/// ---- | ---- | ---- | ----
/// BMP | 5375 | 93.0 | 4097
/// Image | 2360 | 38.9 | -
/// P4 | 384 | 5.8 | 235
///

// Other libraries

// hV_Screen_Virtual
#include "hV_Serial_Image16.h"

#if defined(WITH_FAST_FRIENDS)

// Screen_EPD_EXT3_Fast
#include "hV_Serial_Image.h"
#include "hV_Serial_P4.h"

#endif // WITH_FAST_FRIENDS

#endif // hV_SERIAL_RELEASE
