///
/// @file hV_Screen_Virtual.h
/// @brief Class Generic LCD class library
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
/// * Evaluation edition: for professionals or organisations, no commercial usage
/// * Commercial edition: for professionals or organisations, for commercial usage
///

// SDK
#include "hV_HAL_Peripherals.h"

// Configuration
#include "hV_Configuration.h"

#ifndef hV_SCREEN_VIRTUAL_RELEASE
///
/// @brief Library release number
///
#define hV_SCREEN_VIRTUAL_RELEASE 613

#include "hV_Utilities.h"
#include "hV_Colours565.h"

///
/// @brief Pure virtual hV_screen class
/// @details This class provides all the functions for a screen
///
class hV_Screen_Virtual
{
  public:
    ///
    /// @brief Constructor
    ///
    hV_Screen_Virtual() = default;

    ///
    /// @brief Destructor
    ///
    ~hV_Screen_Virtual() = default;

    /// @name General
    /// @{

    ///
    /// @brief Initialisation
    /// @warning Definition for this method is compulsory.
    ///
    virtual void begin() = 0; // compulsory

    ///
    /// @brief Request information about the screen
    /// @return string with hardware version
    /// @warning Definition for this method is compulsory.
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
    /// @param colour 16-bit colour, default = black
    ///
    virtual void clear(uint16_t colour = myColours.black) = 0;

    ///
    /// @brief Send frame-buffer to display
    ///
    virtual void flush() = 0;

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
    virtual void setOrientation(uint8_t orientation) = 0;

    ///
    /// @brief Get orientation
    /// @return orientation orientation,
    /// * 0 = default
    /// * 1 = right rotated
    /// * 2 = reverse default
    /// * 3 = left rotated
    ///
    virtual uint8_t getOrientation() = 0;

    ///
    /// @brief Screen size, x-axis
    /// @return x-axis size of the screen, in pixels
    /// @note 240 means 240 pixels and thus 0..239 coordinates (decimal)
    ///
    virtual uint16_t screenSizeX() = 0;

    ///
    /// @brief Screen size, y-axis
    /// @return y-axis size of the screen, in pixels
    /// @note 240 means 240 pixels and thus 0..239 coordinates (decimal)
    ///
    virtual uint16_t screenSizeY() = 0;

    ///
    /// @brief Screen size, diagonal
    /// @return diagonal size of the screen, in inches x 10
    /// @note 320 means 3.20"
    ///
    virtual uint16_t screenDiagonal() = 0;

    ///
    /// @brief Screen colour depth
    /// @return colour depth, in bits
    /// @note Examples: BW=1, BWR=2, RGB565=16
    ///
    virtual uint8_t screenColourBits() = 0;
    /// @}

    /// @name Standard graphics
    /// @{

    ///
    /// @brief Draw circle
    /// @param x0 center, point coordinate, x-axis
    /// @param y0 center, point coordinate, y-axis
    /// @param radius radius
    /// @param colour 16-bit colour
    ///
    virtual void circle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t colour) = 0;

    ///
    /// @brief Draw line, rectangle coordinates
    /// @param x1 top left coordinate, x-axis
    /// @param y1 top left coordinate, y-axis
    /// @param x2 bottom right coordinate, x-axis
    /// @param y2 bottom right coordinate, y-axis
    /// @param colour 16-bit colour
    ///
    virtual void line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour) = 0;

    ///
    /// @brief Set pen opaque
    /// @param flag default = true = opaque = solid, false = wire frame
    ///
    virtual void setPenSolid(bool flag = true) = 0;

    ///
    /// @brief Get pen mode
    /// @return true = solid or opaque pen, false = transparent pen
    ///
    virtual bool getPenSolid() = 0;

    ///
    /// @brief Draw triangle, rectangle coordinates
    /// @param x1 first point coordinate, x-axis
    /// @param y1 first point coordinate, y-axis
    /// @param x2 second point coordinate, x-axis
    /// @param y2 second point coordinate, y-axis
    /// @param x3 third point coordinate, x-axis
    /// @param y3 third point coordinate, y-axis
    /// @param colour 16-bit colour
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    virtual void triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t colour) = 0;

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
    virtual void rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour) = 0;

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
    virtual void roundedRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t radius, uint16_t colour) = 0;

    ///
    /// @brief Draw pixel
    /// @param x1 point coordinate, x-axis
    /// @param y1 point coordinate, y-axis
    /// @param colour 16-bit colour
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    virtual void point(uint16_t x1, uint16_t y1, uint16_t colour) = 0;

    /// @}

    /// @name Advanced graphics
    /// @{

    ///
    /// @brief Draw arc
    /// @param x0 center, point coordinate, x-axis
    /// @param y0 center, point coordinate, y-axis
    /// @param radius radius
    /// @param start starting angle, in degrees
    /// @param end ending angle, in degrees
    /// @param colour 16-bit colour
    /// @note If ending angle < starting angle, then starting angle..360 and 0..starting angle arcs are drawn
    ///
    virtual void arc(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t start, uint16_t end, uint16_t colour) = 0;

    ///
    /// @brief Draw level in a circle
    /// @param x0 center, point coordinate, x-axis
    /// @param y0 center, point coordinate, y-axis
    /// @param radius radius
    /// @param start starting angle, in degrees
    /// @param end ending angle, in degrees
    /// @param colour 16-bit colour
    /// @note Angles: 0 <= starting angle < ending angle <= 180
    ///
    virtual void level(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t start, uint16_t end, uint16_t colour) = 0;

    ///
    /// @brief Draw ring
    /// @param x0 center, point coordinate, x-axis
    /// @param y0 center, point coordinate, y-axis
    /// @param inner inner radius
    /// @param outer outer radius
    /// @param colour 16-bit colour
    /// @note inner radius < outer radius
    ///
    virtual void ring(uint16_t x0, uint16_t y0, uint16_t inner, uint16_t outer, uint16_t colour) = 0;

    /// @}

    /// @name Vector coordinates
    /// @{

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
    virtual void dLine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t colour) = 0;

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
    virtual void dRectangle(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t colour) = 0;

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
    virtual void dRoundedRectangle(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t radius, uint16_t colour) = 0;

    /// @}

    /// @name Text
    /// @{

    ///
    /// @brief Set transparent or opaque text
    /// @param flag default = 1 = opaque = solid, false = transparent
    /// @warning Definition for this method is compulsory.
    ///
    virtual void setFontSolid(bool flag = true) = 0;

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
    virtual uint16_t characterSizeX(uint8_t character = 0x00) = 0;

    ///
    /// @brief Character size, y-axis
    /// @return vertical size for current font, in pixels
    /// @note Previously fontSizeY()
    /// @n @b More: @ref Fonts
    ///
    virtual uint16_t characterSizeY() = 0;

    ///
    /// @brief String size, x-axis
    /// @param text string to evaluate
    /// @return horizontal size of the string for current font, in pixels
    /// @n @b More: @ref Fonts
    ///
#if (STRING_MODE == USE_STRING_OBJECT)

    virtual uint16_t stringSizeX(String text) = 0;

#elif (STRING_MODE == USE_CHAR_ARRAY)

    virtual uint16_t stringSizeX(const char * text) = 0;

#endif

    ///
    /// @brief Number of characters to fit a size, x-axis
    /// @param text string to evaluate
    /// @param pixels number of pixels to fit in
    /// @return number of characters to be displayed inside the pixels
    /// @n @b More: @ref Fonts
    ///
#if (STRING_MODE == USE_STRING_OBJECT)

    virtual uint8_t stringLengthToFitX(String text, uint16_t pixels) = 0;

#elif (STRING_MODE == USE_CHAR_ARRAY)

    virtual uint8_t stringLengthToFitX(const char * text, uint16_t pixels) = 0;

#endif

    ///
    /// @brief Draw ASCII Text (pixel coordinates) with selection of size
    /// @param x0 point coordinate, x-axis
    /// @param y0 point coordinate, y-axis
    /// @param text text string
    /// @param textColour 16-bit colour, default = white
    /// @param backColour 16-bit colour, default = black
    /// @note Previously gText() with ix and iy
    ///
    /// @n @b More: @ref Colour, Fonts
    ///
#if (STRING_MODE == USE_STRING_OBJECT)

    virtual void gText(uint16_t x0, uint16_t y0,
                       String text,
                       uint16_t textColour = myColours.white,
                       uint16_t backColour = myColours.black) = 0;

#elif (STRING_MODE == USE_CHAR_ARRAY)

    virtual void gText(uint16_t x0, uint16_t y0,
                       const char * text,
                       uint16_t textColour = myColours.white,
                       uint16_t backColour = myColours.black) = 0;

#endif

    ///
    /// @brief Select font
    /// @param font 0..fontMax()-1, default = 0
    /// @warning Definition for this method is compulsory.
    /// @note Previously setFontSize()
    ///
    virtual void selectFont(uint8_t font) = 0; // compulsory

    ///
    /// @brief Get font number
    /// @return Number of the font, 0..fontMax()-1
    /// @warning Definition for this method is compulsory.
    ///
    virtual uint8_t getFont() = 0; // compulsory

    ///
    /// @brief Number of fonts
    /// @return number of fonts available
    /// @note First font is numbered 0, second 1, ...
    /// @n The latest font is numbered fontMax()-1
    ///
    virtual uint8_t fontMax() = 0; // compulsory

    /// @}

    ///
    /// @brief Set margins for logical screen
    /// @param marginX x-axis margin in pixels
    /// @param marginY y-axis margin in pixels
    ///
    virtual void setMarginXY(uint16_t marginX = 0, uint16_t marginY = 0) = 0; // compulsory

    ///
    /// @brief Margin size, x-axis
    /// @return x-axis size of the screen, in pixels
    ///
    virtual uint16_t screenMarginX() = 0; // compulsory

    ///
    /// @brief Margin size, y-axis
    /// @return y-axis size of the screen, in pixels
    ///
    virtual uint16_t screenMarginY() = 0; // compulsory

    /// @name Energy features
    /// @{

    ///
    /// @brief Does the screen provide energy features?
    /// @return true is energy featured, false otherwise
    ///
    /// @n @b More: @ref Energy
    ///
    virtual bool isEnergy() = 0;

    ///
    /// @brief Is screen backlight variable?
    /// @return true is backlight variable, false otherwise
    ///
    /// @n @b More: @ref Screen
    ///
    virtual bool isIntensity() = 0;

    ///
    /// @brief Turn display backlight on or off
    /// @param state true = HIGH = on, false = LOW = off
    /// @note Feature specific to the SSD1963
    ///
    virtual void setBacklight(bool state) = 0;

    ///
    /// @brief Set display backlight intensity
    /// @param intensity 0..255
    /// @note Feature specific to the SSD1963 and 4D Systems Diablo16
    ///
    virtual void setIntensity(uint8_t intensity) = 0;

    ///
    /// @brief Get display backlight intensity
    /// @return intensity 0..255
    /// @note Feature specific to the SSD1963 and 4D Systems Diablo16
    ///
    virtual uint8_t getIntensity() = 0;

    ///
    /// @brief Set delay for power saving
    /// @param seconds idle period in seconds before power saving, default = 3 s
    /// @note Set seconds = 0 to disable
    /// @note Feature specific to the SSD1963 and the Screen_EPD_FRAM
    ///
    virtual void setEnergy(uint32_t seconds = 3) = 0;

    ///
    /// @brief Update power saving
    /// @param state true = update or start new period or false = check period
    /// @note Feature specific to the SSD1963
    ///
    virtual void updateEnergy(bool state) = 0;

    /// @}

    /// @name Advanced features
    /// @{

    ///
    /// @brief Is screen readable?
    /// @return true is screen readable, false otherwise
    ///
    /// @n @b More: @ref Screen
    ///
    virtual bool isReadable() = 0;

    ///
    /// @brief Does the screen feature an external storage?
    /// @return true is storage available, false otherwise
    ///
    /// @n @b More: @ref Storage
    ///
    virtual bool isStorage() = 0;

    ///
    /// @brief Read pixel colour
    /// @param x1 point coordinate, x-axis
    /// @param y1 point coordinate, y-axis
    /// @return 16-bit colour, bits 15-11 red, bits 10-5 green, bits 4-0 blue
    /// @note This feature requires a readable screen.
    ///
    /// @n @b More: @ref Coordinate, @ref Colour, @ref Screen
    ///
    virtual uint16_t readPixel(uint16_t x1, uint16_t y1) = 0;

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
    /// @n @b More: @ref Coordinate, @ref Screen
    ///
    virtual void copyPasteArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t dx, uint16_t dy) = 0;

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
    virtual void copyArea(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t & address) = 0;

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
    virtual void pasteArea(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint32_t & address, bool option = false) = 0;

    /// @}

    /// @name Fast area features
    /// @details Specific to each controller
    /// @{

    ///
    /// @brief Is area available?
    /// @return if area available, false otherwise
    ///
    virtual bool isArea() = 0;

    ///
    /// @brief Open an area for fast sequential writing
    /// @param x0 source top left coordinate, x-axis
    /// @param y0 source top left coordinate, y-axis
    /// @param dx target to be pasted, x-axis
    /// @param dy target to be pasted, y-axis
    ///
    /// @n @b More: @ref Coordinate
    ///
    virtual void openArea(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy) = 0;

    ///
    /// @brief Feed the area with colour
    /// @details Maximum dx*dy pixels, no check
    /// @param colour 16-bit colour
    /// @note Pixels are displayed from left to right, top to bottom
    ///
    /// @n @b More: @ref Colour
    ///
    virtual void feedArea(uint16_t colour) = 0;

    ///
    /// @brief Close the area previously opened
    ///
    virtual void closeArea() = 0;

    /// @}

    /// @name Touch
    /// @{

    ///
    /// @brief Is touch available?
    /// @return true is touch available, false otherwise
    ///
    virtual bool isTouch() = 0;

    ///
    /// @brief Are touch events available?
    /// @return true if touch events available, false otherwise
    ///
    virtual bool isTouchEvent() = 0;

    ///
    /// @brief Poll touch
    /// @param[out] x x coordinate
    /// @param[out] y y coordinate
    /// @param[out] z z coordinate=pressure
    /// @param[out] t event if available, otherwise 0
    /// @return true if pressed
    ///
    /// @n @b More: @ref Coordinate, @ref Touch
    ///
    virtual bool getTouch(uint16_t & x, uint16_t & y, uint16_t & z, uint16_t & t) = 0;

    ///
    /// @brief Clear touch
    ///
    virtual void clearTouch() = 0;

    ///
    /// @brief Calibrate the touch
    ///
    virtual void calibrateTouch() = 0;

    /// @}

  protected:
    /// @cond
    uint8_t _orientation, _intensity;
    bool _penSolid, _flagRead, _flagStorage, _flagEnergy;
    uint16_t _screenSizeH, _screenSizeV, _screenDiagonal, _screenColourBits, _screenMarginH, _screenMarginV;
    uint8_t _touchTrim, _fsmArea;
    bool _touchEvent;
    uint16_t _touchXmin, _touchXmax, _touchYmin, _touchYmax;
    /// @endcond
};

#endif
