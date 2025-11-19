///
/// @file hV_Screen_Buffer.h
/// @brief Class Generic LCD with buffer class library
///
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @author Rei Vilo
/// @date 21 Aug 2023
/// @version 613
///
/// @copyright (c) Rei Vilo, 2010-2023
/// @copyright All rights reserved
///
/// * Basic edition: for hobbyists and for basic usage
/// @n Creative Commons Attribution-ShareAlike 4.0 International (CC BY-SA 4.0)
///
/// * Evaluation edition: for professionals or organisations, no commercial usage
/// @n All rights reserved
///
/// * Commercial edition: for professionals or organisations, commercial usage
/// @n All rights reserved
///

// SDK
#include "hV_HAL_Peripherals.h"

// Configuration
#include "hV_Configuration.h"

#ifndef hV_SCREEN_BUFFER_RELEASE
///
/// @brief Library release number
///
#define hV_SCREEN_BUFFER_RELEASE 613

// Other libraries
#if (USE_hV_SCREEN == USE_hV_SCREEN_VIRTUAL)
#include "hV_Screen_Virtual.h"

#if (hV_SCREEN_VIRTUAL_RELEASE < 613)
#error Required hV_SCREEN_VIRTUAL_RELEASE 613
#endif // hV_SCREEN_VIRTUAL_RELEASE

#else // USE_hV_SCREEN
#include "hV_Colours565.h"
#include "hV_Utilities.h"

#endif // USE_hV_SCREEN

#include "hV_Font.h"

#if (FONT_MODE == USE_FONT_TERMINAL)
#include "hV_Font_Terminal.h"

#elif (FONT_MODE == USE_FONT_HEADER)
#include "hV_Font_Header.h"

#elif (FONT_MODE == USE_FONT_FLASH)
#include "hV_Font_Flash.h"

#else
#error FONT_MODE not defined
#endif // FONT_MODE

#if (USE_hV_SCREEN == USE_hV_SCREEN_VIRTUAL)
#if (hV_SCREEN_VIRTUAL_RELEASE < 306)
#error Required hV_SCREEN_VIRTUAL_RELEASE 306
#endif // hV_SCREEN_VIRTUAL_RELEASE
#endif // USE_hV_SCREEN

///
/// @brief Generic class for buffered LCD
///
#if (USE_hV_SCREEN == USE_hV_SCREEN_VIRTUAL)
#warning USE_hV_SCREEN == USE_hV_SCREEN_VIRTUAL

#if (FONT_MODE == USE_FONT_TERMINAL)
#warning FONT_MODE == USE_FONT_TERMINAL
class hV_Screen_Buffer : public hV_Screen_Virtual, protected hV_Font_Terminal

#elif (FONT_MODE == USE_FONT_HEADER)
#warning FONT_MODE == USE_FONT_HEADER
class hV_Screen_Buffer : public hV_Screen_Virtual, protected hV_Font_Header

#elif (FONT_MODE == USE_FONT_FLASH)
#warning FONT_MODE == USE_FONT_FLASH
class hV_Screen_Buffer : public hV_Screen_Virtual, protected hV_Font_Flash

#else // FONT_MODE
#error FONT_MODE not defined
#endif // FONT_MODE

// Variant without hV_Screen_Virtual
#elif (USE_hV_SCREEN == USE_hV_SCREEN_SELF)
#warning USE_hV_SCREEN == USE_hV_SCREEN_SELF

#if (FONT_MODE == USE_FONT_TERMINAL)
#warning FONT_MODE == USE_FONT_TERMINAL
class hV_Screen_Buffer : protected hV_Font_Terminal

#elif (FONT_MODE == USE_FONT_HEADER)
#warning FONT_MODE == USE_FONT_HEADER
class hV_Screen_Buffer : protected hV_Font_Header

#elif (FONT_MODE == USE_FONT_FLASH)
#warning FONT_MODE == USE_FONT_FLASH
class hV_Screen_Buffer : protected hV_Font_Flash

#else // FONT_MODE
#error FONT_MODE not defined
#endif // FONT_MODE

#else // USE_hV_SCREEN
#error USE_hV_SCREEN not defined
#endif // USE_hV_SCREEN
{
  public:
    ///
    /// @brief Constructor
    ///
    hV_Screen_Buffer();

    /// @name General
    /// @{

    ///
    /// @brief Initialisation
    ///
    virtual void begin();

    ///
    /// @brief Request information about the screen
    /// @return string with hardware version
    ///
#if (STRING_MODE == USE_STRING_OBJECT)

    virtual String WhoAmI() = 0; // compulsory

#elif (STRING_MODE == USE_CHAR_ARRAY)

    virtual char * WhoAmI() = 0; // compulsory

#else
#error STRING_MODE
#endif

    ///
    /// @brief Clear the screen
    /// @param colour 16-bit colour, default = white
    ///
    virtual void clear(uint16_t colour = myColours.black);

    ///
    /// @brief Send frame-buffer to display
    ///
    virtual void flush() = 0; // compulsory

    ///
    /// @brief Set orientation
    /// @param orientation orientation,
    /// * 0 = default
    /// * 1 = right rotated
    /// * 2 = reverse default
    /// * 3 = left rotated
    /// * ORIENTATION_PORTRAIT = 6 = check portrait
    /// * ORIENTATION_LANDSCAPE = 7 = check landscape
    ///
    virtual void setOrientation(uint8_t orientation);

    ///
    /// @brief Get orientation
    /// @return orientation orientation:
    /// * 0 = portrait
    /// * 1 = right rotated landscape
    /// * 2 = reverse portrait
    /// * 3 = left rotated landscape
    ///
    uint8_t getOrientation();

    ///
    /// @brief Screen size, x-axis of logical screen
    /// @return x-axis size of the screen, in pixels
    /// @note 240 means 240 pixels and thus 0..239 coordinates (decimal)
    ///
    virtual uint16_t screenSizeX();

    ///
    /// @brief Screen size, y-axis of logical screen
    /// @return y-axis size of the screen, in pixels
    /// @note 240 means 240 pixels and thus 0..239 coordinates (decimal)
    ///
    virtual uint16_t screenSizeY();

    ///
    /// @brief Screen size, diagonal
    /// @return diagonal size of the screen, in inches x 100
    /// @note 320 means 3.20"
    ///
    virtual uint16_t screenDiagonal();
    ///

    ///
    /// @brief Screen colour depth
    /// @return colour depth, in bits
    /// @note Examples: BW=1, BWR=2, RGB565=16
    ///
    /// @n @b More: @ref Coordinate
    ///
    virtual uint8_t screenColourBits();

    // ///
    // /// @brief Set margin for physical screen
    // /// @param marginH margin in pixels, horizontal
    // /// @param marginV margin in pixels, vertical
    // ///
    // virtual void setMarginHV(uint16_t marginH = 0, uint16_t marginV = 0);

    ///
    /// @brief Set margin for logical screen
    /// @param marginX margin in pixels, x coordinates
    /// @param marginY margin in pixels, y coordinates
    ///
    virtual void setMarginXY(uint16_t marginX = 0, uint16_t marginY = 0);

    ///
    /// @brief Margin size, x-axis of logical screen
    /// @return x-axis size of the screen, in pixels
    ///
    virtual uint16_t screenMarginX();

    ///
    /// @brief Magin size, y-axis of logical screen
    /// @return y-axis size of the screen, in pixels
    /// @note 240 means 240 pixels and thus 0..239 coordinates (decimal)
    ///
    virtual uint16_t screenMarginY();

    // ///
    // /// @brief Get margin, physical screen
    // /// @return uint16_t margin in pixels, vertical
    // ///
    // virtual uint16_t getMarginV();
    //
    // ///
    // /// @brief Get margin, physical screen
    // /// @return uint16_t margin in pixels, horizontal
    // ///
    // virtual uint16_t getMarginH();

    /// @}

    /// @name Graphics
    ///
    /// @{

    ///
    /// @brief Draw circle
    /// @param x0 center, point coordinate, x-axis
    /// @param y0 center, point coordinate, y-axis
    /// @param radius radius
    /// @param colour 16-bit colour
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    virtual void circle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t colour);

    ///
    /// @brief Draw arc
    /// @param x0 center, point coordinate, x-axis
    /// @param y0 center, point coordinate, y-axis
    /// @param radius radius
    /// @param start starting angle, in degrees
    /// @param end ending angle, in degrees
    /// @param colour 16-bit colour
    /// @note if ending angle < starting angle, then starting angle..360 and 0..starting angle arcs are drawn
    ///
    virtual void arc(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t start, uint16_t end, uint16_t colour);

    ///
    /// @brief Draw level in a circle
    /// @param x0 center, point coordinate, x-axis
    /// @param y0 center, point coordinate, y-axis
    /// @param radius radius
    /// @param start starting angle, in degrees
    /// @param end ending angle, in degrees
    /// @param colour 16-bit colour
    /// @note 0 <= starting angle < ending angle <= 180
    ///
    virtual void level(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t start, uint16_t end, uint16_t colour);

    ///
    /// @brief Draw ring
    /// @param x0 center, point coordinate, x-axis
    /// @param y0 center, point coordinate, y-axis
    /// @param inner inner radius
    /// @param outer outer radius
    /// @param colour 16-bit colour
    /// @note inner radius < outer radius
    ///
    virtual void ring(uint16_t x0, uint16_t y0, uint16_t inner, uint16_t outer, uint16_t colour);

    ///
    /// @brief Draw line, rectangle coordinates
    /// @param x1 top left coordinate, x-axis
    /// @param y1 top left coordinate, y-axis
    /// @param x2 bottom right coordinate, x-axis
    /// @param y2 bottom right coordinate, y-axis
    /// @param colour 16-bit colour
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    virtual void line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour);

    ///
    /// @brief Draw line, vector coordinates
    /// @param x0 point coordinate, x-axis
    /// @param y0 point coordinate, y-axis
    /// @param dx length, x-axis
    /// @param dy height, y-axis
    /// @param colour 16-bit colour
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    virtual void dLine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t colour);

    ///
    /// @brief Set pen opaque
    /// @param flag default = true = opaque = solid, false = wire frame
    ///
    virtual void setPenSolid(bool flag = true);

    ///
    /// @brief Get pen mode
    /// @return true = solid or opaque pen, false = transparent pen
    ///
    virtual bool getPenSolid();

    ///
    /// @brief Draw triangle, rectangle coordinates
    /// @param x1 first point coordinate, x-axis
    /// @param y1 first point coordinate, y-axis
    /// @param x2 second point coordinate, x-axis
    /// @param y2 second point coordinate, y-axis
    /// @param x3 third point coordinate, x-axis
    /// @param y3 third point coordinate, y-axis
    /// @param colour 16-bit colour
    /// @note If two points are equal, draw a line
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    virtual void triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t colour);

    ///
    /// @brief Draw rectangle, rectangle coordinates
    /// @param x1 top left coordinate, x-axis
    /// @param y1 top left coordinate, y-axis
    /// @param x2 bottom right coordinate, x-axis
    /// @param y2 bottom right coordinate, y-axis
    /// @param colour 16-bit colour
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    virtual void rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour);

    ///
    /// @brief Draw rectangle, vector coordinates
    /// @param x0 point coordinate, x-axis
    /// @param y0 point coordinate, y-axis
    /// @param dx length, x-axis
    /// @param dy height, y-axis
    /// @param colour 16-bit colour
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    virtual void dRectangle(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t colour);

    ///
    /// @brief Draw rectangle, rectangle coordinates
    /// @param x1 top left coordinate, x-axis
    /// @param y1 top left coordinate, y-axis
    /// @param x2 bottom right coordinate, x-axis
    /// @param y2 bottom right coordinate, y-axis
    /// @param radius radius for rounded corners, pixels
    /// @param colour 16-bit colour
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    virtual void roundedRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t radius, uint16_t colour);

    ///
    /// @brief Draw rounded rectangle, vector coordinates
    /// @param x0 point coordinate, x-axis
    /// @param y0 point coordinate, y-axis
    /// @param dx length, x-axis
    /// @param dy height, y-axis
    /// @param radius radius for rounded corners, pixels
    /// @param colour 16-bit colour
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    virtual void dRoundedRectangle(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t radius, uint16_t colour);

    ///
    /// @brief Draw pixel
    /// @param x1 point coordinate, x-axis
    /// @param y1 point coordinate, y-axis
    /// @param colour 16-bit colour
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    virtual void point(uint16_t x1, uint16_t y1, uint16_t colour);

    ///
    /// @brief Read pixel colour
    /// @param x1 point coordinate, x-axis
    /// @param y1 point coordinate, y-axis
    /// @return 16-bit colour, bits 15-11 red, bits 10-5 green, bits 4-0 blue
    ///
    //    uint16_t readPixel(uint16_t x1, uint16_t y1);

    /// @}

    /// @name Fast area features
    /// @details Specific to each controller
    /// @{

    ///
    /// @brief Is area available?
    /// @return true if area available, false otherwise
    ///
    /// @n @b More: @ref Area
    ///
    virtual bool isArea();

    ///
    /// @brief Open an area for fast sequential writing, vector coordinates
    /// @param x0 source top left coordinate, x-axis
    /// @param y0 source top left coordinate, y-axis
    /// @param dx target to be pasted, x-axis
    /// @param dy target to be pasted, y-axis
    ///
    /// @n @b More: @ref Area, @ref Coordinate
    ///
    virtual void openArea(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy) { };

    ///
    /// @brief Feed the area with colour
    /// @details Maximum dx*dy pixels
    /// @warning No check!
    /// @param colour 16-bit colour
    /// @note Pixels are displayed from left to right, top to bottom
    ///
    /// @n @b More: @ref Area, @ref Colour
    ///
    virtual void feedArea(uint16_t colour) { };

    ///
    /// @brief Close the area previously opened
    ///
    /// @n @b More: @ref Area
    ///
    virtual void closeArea() { };

    /// @}

    /// @name Text
    /// @{

    ///
    /// @brief Select font number
    /// @param font default = 0, 0..fontMax()-1
    ///
    virtual void selectFont(uint8_t font);

    ///
    /// @brief Get font number
    /// @return Number of the font, 0..fontMax()-1
    ///
    virtual uint8_t getFont();

    ///
    /// @brief Add a font
    /// @param fontName name of the font
    /// @return number of the font, 0 otherwise
    /// @note Previously selectFont()
    /// @n @b More: @ref Fonts
    ///
    virtual uint8_t addFont(font_s fontName);

    ///
    /// @brief Set transparent or opaque text
    /// @param flag default = 1 = opaque = solid, false = transparent
    ///
    virtual void setFontSolid(bool flag = true);

    ///
    /// @brief Get text mode
    /// @return true = solid or opaque text, false = transparent text
    ///
    virtual bool getFontSolid();

    ///
    /// @brief Set additional spaces between two characters, horizontal axis
    /// @param number of spaces default = 1 pixel
    ///
    virtual void setFontSpaceX(uint8_t number = 1);

    ///
    /// @brief Set additional spaces between two characters, vertical axis
    /// @param number of spaces default = 1 pixel
    ///
    virtual void setFontSpaceY(uint8_t number = 1);

    ///
    /// @brief Character size, x-axis
    /// @param character character to evaluate, default = 0 = font general size
    /// @return horizontal size of the font for the specified character, in pixels
    /// @note With setSpaceX included
    /// @note In variable font, each character has a specific size.
    /// The general size is indicative.
    /// @note Previously fontSizeX()
    /// @n @b More: @ref Fonts
    ///
    virtual uint16_t characterSizeX(uint8_t character = 0x00);

    ///
    /// @brief Character size, y-axis
    /// @return vertical size for current font, in pixels
    /// @note Previously fontSizeY()
    /// @n @b More: @ref Fonts
    ///
    virtual uint16_t characterSizeY();

    ///
    /// @brief String size, x-axis
    /// @param text string to evaluate
    /// @return horizontal size of the string for current font, in pixels
    /// @n @b More: @ref Fonts
    ///
#if (STRING_MODE == USE_STRING_OBJECT)
    virtual uint16_t stringSizeX(String text);
#elif (STRING_MODE == USE_CHAR_ARRAY)
    virtual uint16_t stringSizeX(const char * text);
#endif

    ///
    /// @brief Number of characters to fit a size, x-axis
    /// @param text string to evaluate
    /// @param pixels number of pixels to fit in
    /// @return number of characters to be displayed inside the pixels
    /// @n @b More: @ref Fonts
    ///
#if (STRING_MODE == USE_STRING_OBJECT)
    virtual uint8_t stringLengthToFitX(String text, uint16_t pixels);
#elif (STRING_MODE == USE_CHAR_ARRAY)
    virtual uint8_t stringLengthToFitX(const char * text, uint16_t pixels);
#endif

    ///
    /// @brief Number of fonts
    /// @return number of fonts available
    /// @note First font is numbered 0, second 1, ...
    /// @n The latest font is numbered fontMax()-1
    ///
    virtual uint8_t fontMax();

    ///
    /// @brief Draw ASCII Text (pixel coordinates) with selection of size
    /// @param x0 point coordinate, x-axis
    /// @param y0 point coordinate, y-axis
    /// @param text text string
    /// @param textColour 16-bit colour, default = white
    /// @param backColour 16-bit colour, default = black
    /// @note Previously gText() with ix and iy
    ///
    /// @n @b More: @ref Colour, @ref Fonts, @ref Coordinate
    ///
    virtual void gText(uint16_t x0, uint16_t y0,
#if (STRING_MODE == USE_STRING_OBJECT)
                       String text,
#elif (STRING_MODE == USE_CHAR_ARRAY)
                       const char * text,
#endif // STRING_MODE
                       uint16_t textColour = myColours.black,
                       uint16_t backColour = myColours.white);
    /// @}

    /// @name Energy features
    /// @{

    ///
    /// @brief Does the screen provide energy features?
    /// @return true is energy featured, false otherwise
    ///
    /// @n @b More: @ref Energy
    ///
    bool isEnergy();

    ///
    /// @brief Is screen backlight variable?
    /// @return true is backlight variable, false otherwise
    ///
    /// @n @b More: @ref Screen
    ///
    bool isIntensity();

    ///
    /// @brief  Turn display backlight on or off
    /// @param state true = HIGH = on, false = LOW = off
    /// @note Feature specific to the SSD1963
    ///
    void setBacklight(bool state);

    ///
    /// @brief  Set backlight intensity
    /// @param intensity 0..255
    /// @note Feature specific to the SSD1963 and 4D Systems Diablo16
    ///
    void setIntensity(uint8_t intensity);

    ///
    /// @brief Get display backlight intensity
    /// @return intensity 0..255
    /// @note Feature specific to the SSD1963 and 4D Systems Diablo16
    ///
    uint8_t getIntensity();

    ///
    /// @brief Set delay for power saving
    /// @param seconds idle period in seconds before power saving, default = 6 s
    /// @note Set seconds = 0 to disable
    /// @note Feature specific to the SSD1963 and the Screen_EPD_FRAM
    ///
    void setEnergy(uint32_t seconds = 6);

    ///
    /// @brief Update power saving
    /// @param state true = update or start new period or false = check period
    /// @note Feature specific to the SSD1963
    ///
    void updateEnergy(bool state);

    /// @}

    /// @name Advanced features
    /// @{

    ///
    /// @brief Is screen readable?
    /// @return true is screen readable, false otherwise
    ///
    /// @n @b More: @ref Screen
    ///
    bool isReadable();

    ///
    /// @brief Does the screen feature an external storage?
    /// @return true is storage available, false otherwise
    ///
    /// @n @b More: @ref Storage
    ///
    bool isStorage();

    ///
    /// @brief Read pixel colour
    /// @param x1 point coordinate, x-axis
    /// @param y1 point coordinate, y-axis
    /// @return 16-bit colour, bits 15-11 red, bits 10-5 green, bits 4-0 blue
    /// @note This feature requires a readable screen.
    ///
    /// @n @b More: @ref Coordinate, @ref Colour, @ref Screen
    ///
    virtual uint16_t readPixel(uint16_t x1, uint16_t y1) {
        return 0;
    };

    ///
    /// @brief Copy a source area to a target area
    /// @param x1 source top left coordinate, x-axis
    /// @param y1 source top left coordinate, y-axis
    /// @param x2 target top left coordinate, x-axis
    /// @param y2 target top left coordinate, y-axis
    /// @param dx width to be copied, x-axis
    /// @param dy height to be copied, y-axis
    /// @note This feature requires a readable screen.
    /// @warning The function does not manage the overlapping of the source and target areas.
    /// If such a case, use copyArea() pasteArea() instead.
    ///
    /// @n @b More: @ref Coordinate, @ref Screen, @ref Storage
    ///
    virtual void copyPasteArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t dx, uint16_t dy) { };

    ///
    /// @brief Copy an area to an external support
    /// @param x0 source top left coordinate, x-axis
    /// @param y0 source top left coordinate, y-axis
    /// @param dx width to be copied, x-axis
    /// @param dy height to be copied, y-axis
    /// @param address identifier, as SRAM address or file number
    /// @note This feature requires a readable screen and a storage.
    ///
    /// @n @b More: @ref Coordinate, @ref Screen, @ref Storage
    ///
    virtual void copyArea(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t & address) { };

    ///
    /// @brief Paste an area from an external support
    /// @param x0 source top left coordinate, x-axis
    /// @param y0 source top left coordinate, y-axis
    /// @param dx target to be pasted, x-axis
    /// @param dy target to be pasted, y-axis
    /// @param address identifier, as SRAM address or file number
    /// @param option false = default = original colours, true = halved colours
    /// @note This feature requires a readable screen and a storage.
    ///
    /// @n @b More: @ref Coordinate, @ref Screen, @ref Storage
    ///
    virtual void pasteArea(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t & address, bool option = false) { };

    /// @}

    /// @name Touch
    /// @note Those functions are optional, and thus defined as empty and not compulsory.
    /// @{

    ///
    /// @brief Is touch available?
    /// @return true is touch available, false otherwise
    ///
    bool isTouch();

    ///
    /// @brief Are touch events available?
    /// @return true if touch events available, false otherwise
    ///
    bool isTouchEvent();

    ///
    /// @brief Poll touch
    /// @param[out]    x x coordinate
    /// @param[out]    y y coordinate
    /// @param[out]    z z coordinate=pressure
    /// @param[out]    t event if available, otherwise 0
    /// @return true if pressed
    ///
    /// @n @b More: @ref Coordinate, @ref Touch
    ///
    bool getTouch(uint16_t & x, uint16_t & y, uint16_t & z, uint16_t & t);

    ///
    /// @brief Clear touch
    ///
    void clearTouch();

    ///
    /// @brief Check touch interrupt
    ///
    /// @return true interrupt raised, false otherwise
    ///
    bool getTouchInterrupt();

    ///
    /// @brief Calibrate the touch
    ///
    void calibrateTouch() { };

    /// @}

    ///
    /// @brief Get the details of the selected font
    /// @return font_s structure
    ///
    font_s getFontDetails();
    void showImage(uint16_t x, uint16_t y, uint16_t width, uint16_t length, uint8_t *data, uint16_t colour);

  protected:
    /// @cond
    ///
    // * Virtual =0 compulsory functions
    // Orientation
    ///
    /// @brief Set orientation
    /// @param orientation same as setOrientation()
    ///
    virtual void _setOrientation(uint8_t orientation) = 0; // compulsory

    ///
    /// @brief Orient coordinates and check within screen
    /// @param[out] x1 x coordinate
    /// @param[out] y1 y coordinate
    /// @return false = success, true = error
    ///
    virtual bool _orientCoordinates(uint16_t & x1, uint16_t & y1) = 0; // compulsory

    // Position
    void _setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void _closeWindow();
    virtual void _setPoint(uint16_t x1, uint16_t y1, uint16_t colour) = 0; // compulsory
    void _fastFill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour);

    // required by ring() to avoid aliasing
    bool _flagX3;
    void _pointX3(int16_t x0, int16_t y0, int16_t x, int16_t y, uint16_t colour);

    // Write and Read
    void _writeData88(uint8_t dataHigh8, uint8_t dataLow8);

    // Touch
    virtual void _getRawTouch(uint16_t & x0, uint16_t & y0, uint16_t & z0, uint16_t & t0); // compulsory
    virtual bool _getInterruptTouch(); // compulsory

    // Other functions
    // required by calibrateTouch
    void _displayTarget(uint16_t x0, uint16_t y0, uint16_t colour);

    // required by triangle()
    ///
    /// @brief Triangle utility
    /// @param x1 first point coordinate, x-axis
    /// @param y1 first point coordinate, y-axis
    /// @param x2 second point coordinate, x-axis
    /// @param y2 second point coordinate, y-axis
    /// @param x3 third point coordinate, x-axis
    /// @param y3 third point coordinate, y-axis
    /// @param colour 16-bit colour
    ///
    void _triangleArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t colour);
    // required by arc()
    bool _inValue(int16_t value, int16_t valueLow, int16_t valueHigh);
    bool _inSector(int16_t valueStart, int16_t valueEnd,
                   int16_t sectorLow, int16_t sectorHigh,
                   int16_t criteriaStart, int16_t criteriaEnd,
                   int16_t criteriaLow, int16_t criteriaHigh,
                   int16_t criteria);
    bool _inCycle(int16_t value, int16_t valueLow, int16_t valueHigh);

    // required by gText()
    ///
    /// @brief Get definition for line of character
    /// @param character character 32~255
    /// @param index column index
    /// @return definition for line of character
    ///
    uint8_t _getCharacter(uint8_t character, uint8_t index);

    // Energy
    uint32_t _msEnergy;
    uint32_t _chronoEnergy;
    bool _stateEnergy;

    // required by setBacklight() and setBacklightPWM, not implemented for hV_Screen_Buffer
    bool _flagIntensity;
    void _setIntensity(uint8_t intensity); // not implemented for hV_Screen_Buffer
    void _setBacklight(bool flag); // not implemented for hV_Screen_Buffer

    // Variables provided by hV_Screen_Virtual
#if (USE_hV_SCREEN == USE_hV_SCREEN_SELF)
    bool _penSolid, _flagRead, _flagStorage, _flagEnergy;
    uint16_t _screenSizeH, _screenSizeV, _screenDiagonal, _screenMarginH, _screenMarginV;
    uint8_t _orientation, _intensity;
    uint16_t _screenColourBits;
    bool _touchEvent;
    uint8_t _touchTrim, _fsmArea;
    uint16_t _touchXmin, _touchXmax, _touchYmin, _touchYmax;
#endif
    /// @endcond
};

#endif // hV_SCREEN_BUFFER_RELEASE
