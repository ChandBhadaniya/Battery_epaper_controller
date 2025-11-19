///
/// @file hV_Serial_Image.h
/// @brief Print screen content as Image to serial
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

#ifndef hV_SERIAL_IMAGE_RELEASE
///
/// @brief Release
///
#define hV_SERIAL_IMAGE_RELEASE 61

// #if (SRAM_MODE != USE_INTERNAL_MCU)
// #error hV_Serial_Image requires SRAM_MODE = USE_INTERNAL_MCU
// #endif // SRAM_MODE

// Other libraries
#include "Screen_EPD_EXT3.h"
#include "hV_Serial_System.h"
#include "hV_Image.h" // image structure

#if defined(WITH_FAST_FRIENDS)

///
/// @brief Screen printed as image header file on Serial console
/// @note For monochrome screens only, 1 bit per pixel
///
class Serial_Image: public Serial_System
{
  public:
    ///
    /// @brief Constructor object
    /// @param screen &screen to link Serial_Image to
    ///
    Serial_Image(Screen_EPD_EXT3_Fast * screen);

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
    /// @brief Send screen as image header file to serial console
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
    /// @note Image compiled as header file at build-time
    ///
    /// @param image image_s image
    ///
    void readScreen(image_s image);

    /// @}

    /// @cond
  private:
    uint8_t _pinCS, _pinDetect;
    Screen_EPD_EXT3_Fast * _pScreen;
    /// @endcond
};

#endif // WITH_FAST_FRIENDS

#endif // hV_SERIAL_IMAGE_RELEASE

