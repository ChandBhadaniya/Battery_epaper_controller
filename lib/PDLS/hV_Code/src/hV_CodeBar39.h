///
/// @file hV_CodeBar39.h
/// @brief Bar-code 39 library for Pervasive Displays iTC monochrome and colour screens
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

// SDK
#include "hV_HAL_Peripherals.h"

// Configuration
#include "hV_Configuration.h"

#ifndef hV_CODEBAR39_RELEASE
///
/// @brief Release
///
#define hV_CODEBAR39_RELEASE 605

// Other libraries
#include "hV_Screen_Virtual.h"

///
/// @brief Generate and display a barcode
/// @param screen &screen to display the barcode on
/// @param x0 top-left x coordinate
/// @param y0 top-left y coordinate
/// @param text text to convert, string or char array
/// @param height size of each dot, default = 32
/// @param frontColour 16-bit colour, default = black
/// @param backColour 16-bit colour, default = white
/// @param displayText display text below, default = false
/// @return RESULT_SUCCESS = success, otherwise RESULT_ERROR = overflow
/// @note Text maximum length is 39
///
#if (STRING_MODE == USE_STRING_OBJECT)

bool codeBar39(hV_Screen_Virtual * screen, uint16_t x0, uint16_t y0, String text, uint16_t height = 32, uint16_t frontColour = myColours.black, uint16_t backColour = myColours.white, bool displayText = false);

#elif (STRING_MODE == USE_CHAR_ARRAY)

bool codeBar39(hV_Screen_Virtual * screen, uint16_t x0, uint16_t y0, const char * text, uint16_t height = 32, uint16_t frontColour = myColours.black, uint16_t backColour = myColours.white, bool displayText = false);

#endif // STRING_MODE

///
/// @brief Get the barcode size
/// @param text text to convert, string or char array
/// @return size, in pixels, including leading and ending *
///
#if (STRING_MODE == USE_STRING_OBJECT)

uint16_t getSizeCodeBar39(String text);

#elif (STRING_MODE == USE_CHAR_ARRAY)

uint16_t getSizeCodeBar39(const char * text);

#endif // STRING_MODE

#endif // hV_CODEBAR39_RELEASE
