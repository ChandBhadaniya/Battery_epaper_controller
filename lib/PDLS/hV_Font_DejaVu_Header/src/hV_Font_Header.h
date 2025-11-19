///
/// @file hV_Font_Header.h
/// @brief Font structure, header mode
///
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @author Rei Vilo
/// @date 29 Dec 2021
/// @version 525
///
/// @copyright (c) Rei Vilo, 2010-2022
/// @copyright All rights reserved
///
/// * Advanced edition: for professionals or organisations, no commercial usage
/// * Commercial edition: for professionals or organisations, for commercial usage
///

// SDK
#include "hV_HAL_Peripherals.h"

// Configuration
#include "hV_Configuration.h"

// The Arduino IDE doesn't allow to select the libraries, hence this condition.
#if (FONT_MODE == USE_FONT_HEADER)

#ifndef hV_FONT_HEADER_RELEASE
///
/// @brief hV_Font release number
///
#define hV_FONT_HEADER_RELEASE 525

#include "hV_Font.h"

/*
    ///
    /// @brief Biggest font size
    /// @details 32-bit MCUs allow arrays larger than 16384
    ///
    #if defined(ENERGIA_ARCH_CC13XX) || defined(ENERGIA_ARCH_CC3200) || defined(ENERGIA_ARCH_TIVAC) || defined(ENERGIA_ARCH_MSP432) || defined(ARDUINO_ARCH_SAMD) || defined(ENERGIA_ARCH_MSP432R) || defined(ENERGIA_ARCH_CC13X2) || defined(ARDUINO_ARCH_PIDUINO)
    #define hV_FONT_LIMIT 65536
    #else
    #define hV_FONT_LIMIT 16384
    #endif
*/

///
/// @brief Font styles
/// @{
#define FONT_MONO 0x40 ///< Mono-spaced
#define FONT_SANS 0x20 ///< Sans serif
#define FONT_SERIF 0x10 ///< Serif

#define FONT_STYLE FONT_SANS ///< Selected style
/// @}

#include "Font_InterLight78.h"
#include "Font_InterRegular55.h"
#include "Font_InterMedium36.h"
#include "Font_InterMedium24.h"
#include "Font_PXSansRegular16.h"

#define _A "\xC4"//Ä 
#define _a "\xE4"//ä
#define _U "\xDC"//Ü
#define _u "\xFC"//ü
#define _O "\xD6"//Ö
#define _o "\xF6"//ö
#define _Z "\x8E"//Ž
#define _z "\x9E"//ž
#define _S "\x8A"//Š
#define _s "\x9A"//	š
#define _01 "\xD5"//Õ
#define _02 "\xF5"//õ

///
/// @brief Class for font as header file
///
class hV_Font_Header
{
    // public:
    // ///
    // /// @brief Constructor
    // ///
    // hV_Font_Header();

    /// @cond
  protected:
    ///
    /// @brief Initialisation
    ///
    void _f_begin();

    ///
    /// @brief Select font
    /// @param font default = 0 = small, 1 = large
    /// @warning Definition for this method is compulsory.
    /// @note Previously _setFontSize()
    ///
    void _f_selectFont(uint8_t font);

    ///
    /// @brief Use a font
    /// @param fontName name of the font
    /// @return number of the font, 0 otherwise
    /// @warning Definition for this method is compulsory.
    /// @note Previously setFontSize()
    /// @n @b More: @ref Fonts
    ///
    uint8_t _f_addFont(font_s fontName);

    ///
    /// @brief Set transparent or opaque text
    /// @param flag default = 1 = opaque = solid, false = transparent
    /// @warning Definition for this method is compulsory.
    ///
    void _f_setFontSolid(bool flag = true);

    ///
    /// @brief Set additional pixels between two characters, horizontal axis
    /// @param number of spaces default = 1 pixel
    /// @warning Definition for this method is compulsory.
    ///
    void _f_setFontSpaceX(uint8_t number = 1);

    ///
    /// @brief Set additional pixels between two characters, vertical axis
    /// @param number of spaces default = 1 pixel
    /// @warning Definition for this method is compulsory.
    ///
    void _f_setFontSpaceY(uint8_t number = 1);

    ///
    /// @brief Character size, x-axis
    /// @param character character to evaluate, default = 0 = font general size
    /// @return horizontal size of character for current font, in pixels
    /// @note With setSpaceX included
    /// @note In variable font, each character has a specific size. The general size is indicative.
    /// @note Previously fontSizeX()
    /// @n @b More: @ref Fonts
    ///
    uint16_t _f_characterSizeX(uint8_t character = 0x00);

    ///
    /// @brief Character size, y-axis
    /// @return vertical size for current font, in pixels
    /// @note Previously fontSizeY()
    /// @n @b More: @ref Fonts
    ///
    uint16_t _f_characterSizeY();

    ///
    /// @brief String size, x-axis
    /// @param text string to evaluate
    /// @return horizontal size of the string for current font, in pixels
    /// @n @b More: @ref Fonts
    ///
#if (STRING_MODE == USE_STRING_OBJECT)
    uint16_t _f_stringSizeX(String text);
#elif (STRING_MODE == USE_CHAR_ARRAY)
    uint16_t _f_stringSizeX(const char * text);
#endif // STRING_MODE

    ///
    /// @brief Number of characters to fit a size, x-axis
    /// @param text string to evaluate
    /// @param pixels number of pixels to fit in
    /// @return number of characters to be displayed inside the pixels
    /// @n @b More: @ref Fonts
    ///
#if (STRING_MODE == USE_STRING_OBJECT)
    uint8_t _f_stringLengthToFitX(String text, uint16_t pixels);
#elif (STRING_MODE == USE_CHAR_ARRAY)
    uint8_t _f_stringLengthToFitX(const char * text, uint16_t pixels);
#endif // STRING_MODE

    ///
    /// @brief Number of fonts
    /// @return number of fonts available
    /// @note First font is numbered 0, second 1, ...
    /// @n The latest font is numbered fontMax()-1
    ///
    uint8_t _f_fontMax();

    ///
    /// @brief Get selected font details
    /// @return font description
    ///
    uint8_t _f_getFontKind();

    ///
    /// @brief Get selected font details
    /// @return maximum width in pixels from *width array
    ///
    uint8_t _f_getFontMaxWidth();

    ///
    /// @brief Get definition for line of character
    /// @param character character 32~255
    /// @param index column index
    /// @return definition for line of character
    ///
    uint8_t _f_getCharacter(uint8_t character, uint16_t index);

    ///
    /// @name Variables for font management
    /// @{
    ///
    font_s _f_font; ///< font
    uint8_t _f_fontNumber; ///< number of fonts available, 0.._fontNumber-1
    uint8_t _f_fontSize; ///< actual font selected
    uint8_t _f_fontSpaceX; ///< pixels between two characters, horizontal axis
    uint8_t _f_fontSpaceY; ///< pixels between two characters, vertical axis
    bool _f_fontSolid; ///< opaque print

    font_s _f_fontTable[MAX_FONT_SIZE];
    /// @}
};
/// @endcond

#endif // hV_FONT_HEADER_RELEASE

#endif // USE_FONT_HEADER

