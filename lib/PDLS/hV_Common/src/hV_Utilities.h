///
/// @file hV_Utilities.h
/// @brief Utilities for highView Library Suite
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

#ifndef hV_UTILITIES_RELEASE
///
/// @brief Library release number
///
#define hV_UTILITIES_RELEASE 611

// SDK
#include "hV_HAL_Peripherals.h"

// Configuration
#include "hV_Configuration.h"

#if (hV_HAL_PERIPHERALS_RELEASE < 610)
#error Required hV_HAL_PERIPHERALS_RELEASE 610
#endif // hV_HAL_PERIPHERALS_RELEASE

// #ifndef min
// #define min(a, b) ((a) < (b) ? (a) : (b))
// #endif
// #ifndef max
// #define max(a, b) ((a) > (b) ? (a) : (b))
// #endif

///
/// @name Orientation constants
/// @{
///
#define ORIENTATION_PORTRAIT 6 ///< portrait or vertical, higher than large
#define ORIENTATION_LANDSCAPE 7 ///< landscape or horizontal, larger than high
/// @}
///

///
/// @name Text format functions
/// @brief Utilities to format float, 64-bit unsigned integer, hexadecimal and period into string
/// @{

///
/// @brief UTF-8 to ISO-8859-1 Converter
/// @param s UTF-8 string, input
/// @return ISO-8859-1 string, output
///
/// @see The Unicode Consortium. The Unicode Standard, Version 6.2.0,
/// (Mountain View, CA: The Unicode Consortium, 2012. ISBN 978-1-936213-07-8)
/// http://www.unicode.org/versions/Unicode6.2.0/
///
#if (STRING_MODE == USE_STRING_OBJECT)
String utf2iso(String s);
#elif (STRING_MODE == USE_CHAR_ARRAY)
char * utf2iso(const char * s);
#endif // STRING_MODE

///
/// @brief Format string or char array
/// @details Based on vsprint
/// @param format format with standard codes
/// @param ... list of values
/// @return string or character array with values formatted
/// @see http://www.cplusplus.com/reference/cstdio/printf/?kw=printf for codes
///
#if (STRING_MODE == USE_STRING_OBJECT)
String formatString(const char * format, ...);
#elif (STRING_MODE == USE_CHAR_ARRAY)
char * formatString(const char * format, ...);
#endif // STRING_MODE

///
/// @brief Remove leading and ending characters
/// @param text input text
/// @return trimmed text
/// @note Removed characters are LF CR TAB SPACE '
///
#if (STRING_MODE == USE_STRING_OBJECT)

String trimString(String text);

#elif (STRING_MODE == USE_CHAR_ARRAY)

char * trimString(const char * text);

#endif // STRING_MODE

/// @}

/// @name Numeric functions
/// @details Trigonometric functions, range check and management
/// @{

///
/// @brief Cosinus
/// @param degreesX100 angle in degrees, x100
/// @return cosinus value, x100
/// @note This function uses integers only.
///
int32_t cos32x100(int32_t degreesX100);

///
/// @brief Sinus
/// @param degreesX100 angle in degrees, x100
/// @return sinus value, x100
/// @note This function uses integers only.
///
int32_t sin32x100(int32_t degreesX100);

///
/// @brief Number of decimal places
///
/// @param unit 1 = default, 10, 100, 1000
/// @return uint8_t decimal places 0, 1, 2, 3
///
uint8_t getDecimalPlaces(int32_t unit = 1);

///
/// @brief Rescale a value
///
/// @param x input value
/// @param in_min minimal input value
/// @param in_max maxinmum input value
/// @param out_min minimum output value
/// @param out_max mixmum output value
/// @return long output value
/// @note Formula is
/// @code {.cpp}
/// (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
/// @endcode
/// @deprecated Use hV_HAL_map() instead (6.0.7)
///
long scaleValue(long x, long in_min, long in_max, long out_min, long out_max);

/*
///
/// @brief Convert polar to rectangle coordinates
/// @param[in] centerX circle center, x coordinate
/// @param[in] centerY circle center, y coordinate
/// @param[in] angle angle, degrees 0..360°
/// @param[in] radius radius, pixels
/// @param[out] rectangularX x rectangular coordinate
/// @param[out] rectangularY y rectangular coordinate
/// @note This function uses floats.
///
void convertPolar2Rectangle(uint16_t centerX, uint16_t centerY, uint16_t angle, uint16_t radius, uint16_t & rectangularX, uint16_t & rectangularY);

/// @brief Convert rectangle to polar coordinates
/// @param[in] centerX circle center, x coordinate
/// @param[in] centerY circle center, y coordinate
/// @param[in] rectangularX x rectangular coordinate
/// @param[in] rectangularY y rectangular coordinate
/// @param[out] angle angle in degrees 0..360°
/// @param[out] radius radius in pixels
/// @note This function uses floats.
///
void convertRectangle2Polar(uint16_t centerX, uint16_t centerY, uint16_t rectangularX, uint16_t rectangularY, uint16_t & angle, uint16_t & radius);
*/

///
/// @brief Check value in range
/// @param value value to check
/// @param valueMin range minimum
/// @param valueMax range maximum
/// @return value within range
/// @note Assuming valueMin < valueMax
/// * if value < valueMin, return valueMin
/// * if value > valueMax, return valueMax
/// Otherwise, if valueMin > valueMax, valueMin and valueMax are swapped
/// @warning ISO C++ forbids declaration of 'swap' with no type, template <typename T> unless -fpermissive
///
int16_t checkRange(int16_t value, int16_t valueMin, int16_t valueMax);
uint16_t checkRange(uint16_t value, uint16_t valueMin, uint16_t valueMax);
int32_t checkRange(int32_t value, int32_t valueMin, int32_t valueMax);
uint32_t checkRange(uint32_t value, uint32_t valueMin, uint32_t valueMax);

// template<typename T>
// T checkRange(T value, T valueMin, T valueMax)
// {
//     T localMin = min(valueMin, valueMax);
//     T localMax = max(valueMin, valueMax);
//
//     return min(max(localMin, value), localMax);
// };

///
/// @brief Update min and max values
/// @param value value to consider
/// @param valueMin minimum value to update if value < valueMin
/// @param valueMax maximum value to update if value > valueMax
/// @note valueMin < valueMax is assumed
///
void setMinMax(uint16_t value, uint16_t & valueMin, uint16_t & valueMax);

///
/// @brief Round-up
/// @param value value to round-up
/// @param modulo modulo
/// @return rounded-up value / modulo
/// @note Examples
/// * roundUp(7, 8) = 1;
/// * roundUp(8, 8) = 1;
/// * roundUp(9, 8) = 2;
///
uint32_t roundUp(uint32_t value, uint16_t modulo);

/// @}

///
/// @name Miscellaneous functions
/// @brief Swap and miscellaneous functions
///
/// @{

///
/// @brief Swap number
/// @param a first number
/// @param b second number
/// @warning ISO C++ forbids declaration of 'swap' with no type, template <typename T> unless -fpermissive
///
void swap(int8_t & a, int8_t & b);
void swap(uint8_t & a, uint8_t & b);
void swap(int16_t & a, int16_t & b);
void swap(uint16_t & a, uint16_t & b);
void swap(int32_t & a, int32_t & b);
void swap(uint32_t & a, uint32_t & b);

// template <typename T>
// swap(T & a, T & b)
// {
//     T w = a;
//     a = b;
//     b = w;
// };

// ///
// /// @brief Swap number, uint16_t
// /// @param a first number
// /// @param b second number
// ///
// void swap(uint16_t & a, uint16_t & b);
//
// ///
// /// @brief Swap number, uint8_t
// /// @param a first number
// /// @param b second number
// ///
// void swap(uint8_t & a, int32_t & b);
//
// /// @brief Swap number, int16_t
// /// @param a first number
// /// @param b second number
// ///
// void swap(int32_t & a, int32_t & b);

///
/// @brief Print the current configuration
/// @details Steps 2 through 11
///
void configurationDebug(pins_t board);

/// @}

#endif // hV_UTILITIES_RELEASE
