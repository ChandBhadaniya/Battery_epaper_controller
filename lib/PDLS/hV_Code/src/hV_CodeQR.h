///
/// @file hV_CodeQR.h
/// @brief QR-code library for Pervasive Displays iTC monochrome and colour screens
///
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @author Rei Vilo
/// @date 24 Jan 2023
/// @version 605
///
/// @copyright (c) Rei Vilo, 2010-2023
/// @copyright All rights reserved
///
/// * Evaluation edition: for professionals or organisations, no commercial usage
/// * Commercial edition: for professionals or organisations, for commercial usage
///
/// @warning This library requires the external QRcode library.
/// @warning * https://www.arduino.cc/reference/en/libraries/qrcode/
/// @warning * https://github.com/ricmoo/QRCode
///

// SDK
#include "hV_HAL_Peripherals.h"

// Configuration
#include "hV_Configuration.h"

#ifndef hV_CODEQR_RELEASE
///
/// @brief Release
///
#define hV_CODEQR_RELEASE 605

// Other libraries
#include "hV_Screen_Virtual.h"

/// @name Options
/// @{
/// @see https://github.com/ricmoo/QRCode#what-is-version-error-correction-and-mode
#define QR_MODE MODE_ALPHANUMERIC ///< recommended
#define QR_ECC ECC_MEDIUM ///< error correction
#define QR_VERSION 4 ///< version
/// @see https://github.com/ricmoo/QRCode#data-capacities
#define QR_MAX_LENGTH 90 ///<
/// @}

///
/// @brief Generate and display a QR-code
/// @param screen &screen to display the QR-code on
/// @param x0 top-left x coordinate
/// @param y0 top-left y coordinate
/// @param text text to convert, string or char array
/// @param multiplier size of each dot, default = 2 recommended
/// @param frontColour 16-bit colour, default = black
/// @param backColour 16-bit colour, default = white
/// @return RESULT_SUCCESS = success, otherwise RESULT_ERROR = overflow
/// @note Text maximum length is set by TABLES_DEFINED in QRframes.h
/// * 11 for 1.1 = 19 characters in 21x21 square
/// * 12 for 1.2 = 34 characters in 25x25 square
/// * 16 for 1.6 = 68 characters in 41x41 square
/// @note QR-code square size = (41 + 2) * multiplier
///
#if (STRING_MODE == USE_STRING_OBJECT)

bool codeQR(hV_Screen_Virtual * screen, uint16_t x0, uint16_t y0, String text, uint8_t multiplier = 2, uint16_t frontColour = myColours.black, uint16_t backColour = myColours.white);

#elif (STRING_MODE == USE_CHAR_ARRAY)

bool codeQR(hV_Screen_Virtual * screen, uint16_t x0, uint16_t y0, const char * text, uint8_t multiplier = 2, uint16_t frontColour = myColours.black, uint16_t backColour = myColours.white);

#endif // STRING_MODE

///
/// @brief Get the QR-code square size
/// @param multiplier size of each dot, default = 2 recommended
/// @return size, in pixels
///
uint16_t getSizeCodeQR(uint8_t multiplier = 2);

#endif // hV_CODEQR_RELEASE
