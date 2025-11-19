///
/// @file hV_Serial_Image16.h
/// @brief Save screen content to BMP and header files on SD card
///
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @author Rei Vilo
/// @date 21 Jul 2023
/// @version 612
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

#ifndef hV_SERIAL_IMAGE16_RELEASE
///
/// @brief Release
///
#define hV_SERIAL_IMAGE16_RELEASE 612

// #if (SRAM_MODE != USE_INTERNAL_MCU)
// #error hV_Serial_Image16 requires SRAM_MODE = USE_INTERNAL_MCU
// #endif // SRAM_MODE

// Other libraries
// #include "Screen_EPD_EXT3.h"
#include "hV_Screen_Virtual.h"
#include "hV_Serial_System.h"
#include "hV_Image16.h" // Image structure

///
/// @brief Screen printed as 16-bit image header file on Serial console
/// @note For all screens, monochrome and colour, 16 bits per pixel
/// @note Colours are RGB 5-6-5 coded
///
class Serial_Image16: public Serial_System
{
  public:
    ///
    /// @brief Constructor object
    /// @param screen &screen to link Serial_Image16 to
    ///
    // Serial_Image16(Screen_EPD_EXT3_Fast * screen);
    Serial_Image16(hV_Screen_Virtual * screen);

    // #if (STORAGE_MODE & USE_SERIAL_CONSOLE)

    ///
    /// @brief Send screen as header file to serial console
    /// @param name file name, only letters and numbers
    ///
#if (STRING_MODE == USE_STRING_OBJECT)

    void saveScreen(String name);

#elif (STRING_MODE == USE_CHAR_ARRAY)

    void saveScreen(const char * name);

#endif // STRING_MODE

    ///
    /// @brief Send screen as 16-bit image header file to serial console
    /// @param name file name, only letters and numbers
    /// @param x0 top left coordinate, x-axis
    /// @param y0 top left coordinate, y-axis
    /// @param dx width to be copied, x-axis
    /// @param dy height to be copied, y-axis
    ///
#if (STRING_MODE == USE_STRING_OBJECT)

    void saveWindow(String name, uint16_t x0, uint16_t  y0, uint16_t dx, uint16_t dy);

#elif (STRING_MODE == USE_CHAR_ARRAY)

    void saveWindow(const char * name, uint16_t x0, uint16_t  y0, uint16_t dx, uint16_t dy);

#endif // STRING_MODE

    // #endif // STORAGE_MODE

    ///
    /// @brief Read an image from a header file
    /// @details Frame-buffer updated, flush required
    /// @note Image16 compiled as header file at build-time
    ///
    /// @param image image16_s image
    ///
    void readScreen(image16_s image);

    /// @}

    /// @cond
  private:
    uint8_t _pinCS, _pinDetect;
    // Screen_EPD_EXT3_Fast * _pScreen;
    hV_Screen_Virtual * _pScreen;
    /// @endcond
};

#endif // hV_SERIAL_IMAGE16_RELEASE

