///
/// @file hV_Serial_P4.h
/// @brief Save screen content to P4 and header files
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

#ifndef hV_SERIAL_P4_RELEASE
///
/// @brief Release
///
#define hV_SERIAL_P4_RELEASE 611

// #if (SRAM_MODE != USE_INTERNAL_MCU)
// #error hV_Serial_P4 requires SRAM_MODE = USE_INTERNAL_MCU
// #endif // SRAM_MODE

// Other libraries
#include "Screen_EPD_EXT3.h"
#include "hV_Serial_System.h"

#if defined(WITH_FAST_FRIENDS)

///
/// @brief Screen printed as pixmap header file on Serial console
/// @note For monochrome screens only, 1 bit per pixel
///
class Serial_P4: public Serial_System
{
  public:
    ///
    /// @brief Constructor object
    /// @param screen &screen to link Serial_P4 to
    ///
    Serial_P4(Screen_EPD_EXT3_Fast * screen);

    ///
    /// @brief Read an image from a header file
    /// @details Framebuffer updated, flush required
    /// @note P4 compiled as header file at build-time
    ///
    /// @param imageP4 PBM P4 pixmap image
    ///
    void readScreen(const uint8_t * imageP4);

    // #if (STORAGE_MODE & USE_SERIAL_CONSOLE)

    ///
    /// @brief Send screen as pixmap header file to serial console
    /// @param name file name, only letters and numbers
    ///
#if (STRING_MODE == USE_STRING_OBJECT)

    void saveScreen(String name);

#elif (STRING_MODE == USE_CHAR_ARRAY)

    void saveScreen(const char * name);

#endif // STRING_MODE

    // #endif // STORAGE_MODE

    /// @}

    /// @cond
  private:
    uint8_t _pinCS, _pinDetect;
    Screen_EPD_EXT3_Fast * _pScreen;
    /// @endcond
};

#endif // WITH_FAST_FRIENDS

#endif // hV_SERIAL_P4_RELEASE
