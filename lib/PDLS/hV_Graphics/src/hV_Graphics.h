///
/// @file hV_Graphics.h
/// @brief Graphics library for Pervasive Displays iTC monochrome and colour screens
///
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @author Rei Vilo
/// @date 20 Mar 2023
/// @version 607
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

#ifndef hV_GRAPHICS_RELEASE
///
/// @brief Library release number
///
#define hV_GRAPHICS_RELEASE 607

// Other libraries
#include "hV_Screen_Virtual.h"

///
/// @brief Options for font size
/// @{
#define fontSizeAutomatic 0xff ///< automatic
/// @}

///
/// @name Options for horizontal and vertical position
/// @{
#define horizontalLeft -1 ///< left
#define horizontalCentre 0 ///< centre
#define horizontalCenter 0 ///< American English variant for horizontalCentre
#define horizontalRight 1 ///< right
#define verticalTop 1 ///< top
#define verticalMiddle 0 ///< middle
#define verticalBottom -1 ///< bottom

#define hL horizontalLeft ///< short for horizontalLeft
#define hC horizontalCentre ///< short for horizontalCentre
#define hR horizontalRight ///< short for horizontalRight
#define vT verticalTop ///< short for verticalTop
#define vM verticalMiddle ///< short for verticalMiddle
#define vB verticalBottom ///< short for verticalBottom
/// @}

///
/// @class Graphics
/// @brief Graphics
/// @details Shared settings
///
class Graphics
{
    friend class mtgDial;
    friend class Clock;
    friend class Gauge;
    friend class gYaw;
    friend class gPitch;
    friend class gRoll;
    friend class mtgPanel;
    friend class Histogram;
    friend class Channels;
    friend class Multiple;

  public :
    ///
    /// @brief Constructor
    ///
    /// @param screen &screen to which the graphics elements belong
    ///
    Graphics(hV_Screen_Virtual * screen);

    Graphics();

    ///
    /// @brief Initialise the graphics
    ///
    void begin();

    ///
    /// @brief Set the colours palette
    ///
    /// @param frontColour 16-bit colours for text and rectangles
    /// @param backColour 16-bit colours for background
    /// @param middleColour 16-bit colours for intermediate
    ///
    void setColours(uint16_t frontColour = myColours.black,
                    uint16_t backColour = myColours.white,
                    uint16_t middleColour = myColours.grey);

    void setFonts(uint8_t fontLegend, uint8_t fontValue);

    ///
    /// @brief Label, vector coordinates
    ///
    /// @param x0 point coordinate, x-axis
    /// @param y0 point coordinate, y-axis
    /// @param dx length, x-axis
    /// @param dy height, y-axis
    /// @param text0 text for the label
    /// @param textColour0 16-bit colour for text, default = white, optional
    /// @param backColour0 16-bit colour for background, default = black, optional
    /// @param horizontal0 horizontal position with -1 = left, 0 = center, 1 = right, optional
    /// @param vertical0 vertical position with 1 = top, 0 = center, -1 = bottom, optional
    /// @param size0 size of the font, default = fontSizeAutomatic = automatic, optional
    /// @param flush0 not used, for compatibility only
    /// @note Horizontal and vertical margins of 3 + 3 pixels
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    void dLabel(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
#if (STRING_MODE == USE_STRING_OBJECT)
                String text0,
#elif (STRING_MODE == USE_CHAR_ARRAY)
                const char * text0,
#endif // STRING_MODE
                uint16_t textColour0, uint16_t backColour0,
                int8_t horizontal0 = horizontalCentre,
                int8_t vertical0 = verticalMiddle,
                uint8_t size0 = fontSizeAutomatic,
                bool flush0 = true);

  private:
    hV_Screen_Virtual * _pScreen;
    uint16_t _colourFront, _colourBack, _colourMiddle;
    uint8_t _fontLegend, _fontValue;
};

///
/// @brief Meta-class for graphics based on a rounded dial
/// @details mtgDial is a meta-class for graphics based on a rounded dial
///
class mtgDial
{
  public:
    ///
    /// @brief Constructor
    /// @param graphics &graphics to which the element belongs
    ///
    mtgDial(Graphics * graphics);

    mtgDial() = default;

  protected:
    /// @cond
    Graphics * _pGraphics;
    void drawDial(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t valueColour);

    uint16_t _x0, _y0, _radius;
    uint16_t _valueColour;
    /// @endcond
};

///
/// @class Clock
/// @brief Clock graphics element
/// @details Clock displays an analog clock inside a rounded dial
///
class Clock : public mtgDial
{
  public:
    ///
    /// @brief Constructor
    /// @param graphics &graphics to which the element belongs
    ///
    Clock(Graphics * graphics);

    ///
    /// @brief Define the clock, vector coordinates
    /// @param screen &screen to which the graphic belongs
    /// @param x0 point coordinate, x-axis
    /// @param y0 point coordinate, y-axis
    /// @param dx length, x-axis
    /// @param dy height, y-axis
    /// @param hourColour 16-bit colour for hours, default = black, optional
    /// @param minuteColour 16-bit colour for minutes, default = black, optional
    /// @param secondColour 16-bit colour for seconds, default = black, optional
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    void dDefine(uint16_t x0, uint16_t y0,
                 uint16_t dx, uint16_t dy,
                 uint16_t hourColour = myColours.black,
                 uint16_t minuteColour = myColours.black,
                 uint16_t secondColour = myColours.black);

    ///
    /// @brief Define the clock, rectangle coordinates
    /// @param screen &screen to which the graphic belongs
    /// @param x0 center, point coordinate, x-axis
    /// @param y0 center, point coordinate, y-axis
    /// @param radius radius
    /// @param hourColour 16-bit colour for hours, default = black, optional
    /// @param minuteColour 16-bit colour for minutes, default = black, optional
    /// @param secondColour 16-bit colour for seconds, default = black, optional
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    void define(uint16_t x0, uint16_t y0, uint16_t radius,
                uint16_t hourColour = myColours.black,
                uint16_t minuteColour = myColours.black,
                uint16_t secondColour = myColours.black);

    ///
    /// @brief Update and draw the clock
    /// @param hour hours, 0..23
    /// @param minute minutes, 0.59
    /// @param second seconds, 0..49
    /// @param text text to be displayed
    ///

#if (STRING_MODE == USE_STRING_OBJECT)

    void draw(uint8_t hour, uint8_t minute, uint8_t second, uint8_t s100th = 0, String text = "");

#elif (STRING_MODE == USE_CHAR_ARRAY)

    void draw(uint8_t hour, uint8_t minute, uint8_t second, uint8_t s100th = 0, const char * text = "");

#endif // STRING_MODE

  private:
    void _needle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour);
    uint16_t _hourColour, _minuteColour, _secondColour;
    int32_t _oldHourX, _oldHourY, _oldMinuteX, _oldMinuteY, _oldSecondX, _oldSecondY, _oldK;
};

///
/// @class Gauge
/// @brief Gauge graphics element
/// @details Gauge displays a needle inside a rounded dial
///
class Gauge : public mtgDial
{
  public:
    ///
    /// @brief Constructor
    /// @param graphics &graphics to which the element belongs
    ///
    Gauge(Graphics * graphics);

    ///
    /// @brief Define the graphic, rectangle coordinates
    /// @param screen &screen to which the graphic belongs
    /// @param x0 center, point coordinate, x-axis
    /// @param y0 center, point coordinate, y-axis
    /// @param radius radius
    /// @param valueMin minimum value for measures
    /// @param valueMax maximum value for measures, already multiplied by unit
    /// @param unit multiplier for all values, default = 1, 10 or 100
    /// @param grid number of areas for measures, default = 8, 0 = no grid
    /// @param valueColour 16-bit colour for measures, default = black, optional
    ///
    /// @n @b More: @ref Coordinate, @ref Colour, @ref Value
    ///
    void define(uint16_t x0, uint16_t y0, uint16_t radius,
                int32_t valueMin, int32_t valueMax, int32_t unit = 1,
                uint16_t grid = 8,
                uint16_t valueColour = myColours.black);

    ///
    /// @brief Define the graphic, vector coordinates
    /// @param screen &screen to which the graphic belongs
    /// @param x0 point coordinate, x-axis
    /// @param y0 point coordinate, y-axis
    /// @param dx length, x-axis
    /// @param dy height, y-axis
    /// @param valueMin minimum value for measures, already multiplied by unit
    /// @param valueMax maximum value for measures, already multiplied by unit
    /// @param unit multiplier for all values, default = 1, 10 or 100
    /// @param grid number of areas for measures, default = 8, 0 = no grid
    /// @param valueColour 16-bit colour for measures, default = black, optional
    ///
    /// @n @b More: @ref Coordinate, @ref Colour, @ref Value
    ///
    void dDefine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                 int32_t valueMin, int32_t valueMax, int32_t unit = 1,
                 uint16_t grid = 8,
                 uint16_t valueColour = myColours.black);

    ///
    /// @brief Update the graphic
    /// @param value new measure, already multiplied by unit
    /// @param stringValue label, default = "", optional
    /// @note If value=1234 with unit=100, actual value is 1234/100=12.34
    ///
    /// @n @b More: @ref Value
    ///
#if (STRING_MODE == USE_STRING_OBJECT)

    void draw(int32_t value, String stringValue = "");

#elif (STRING_MODE == USE_CHAR_ARRAY)

    void draw(int32_t value, const char * text = "");

#endif // STRING_MODE

  private:
    void needle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour);

    uint16_t _dx, _dy;
    int32_t _valueMin, _valueMax, _unit;
    int32_t _oldZ, _minZ, _maxZ, _oldK;
    uint16_t _n, _grid;
    uint8_t _formatUnit;
};

///
/// @class gYaw
/// @brief Yaw graphics element
/// @details Yaw is a specific display for yaw angle
///
class gYaw : public mtgDial
{
  public:
    ///
    /// @brief Constructor
    /// @param graphics &graphics to which the element belongs
    ///
    gYaw(Graphics * graphics);

    ///
    /// @brief Define the graphic, vector coordinates
    /// @param screen &screen to which the graphic belongs
    /// @param x0 point coordinate, x-axis
    /// @param y0 point coordinate, y-axis
    /// @param dx length, x-axis
    /// @param dy height, y-axis
    /// @param unit multiplier for all values, default = 1, 10 or 100
    /// @param valueColour 16-bit colour for measures, default = black, optional
    ///
    /// @n @b More: @ref Coordinate, @ref Colour, @ref Value
    ///
    void dDefine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                 int32_t unit = 1, uint16_t valueColour = myColours.black);

    ///
    /// @brief Define the graphic, rectangle coordinates
    /// @param screen &screen to which the graphic belongs
    /// @param x0 center, point coordinate, x-axis
    /// @param y0 center, point coordinate, y-axis
    /// @param radius radius
    /// @param unit multiplier for all values, default = 1, 10 or 100
    /// @param valueColour 16-bit colour for measures, default = black, optional
    ///
    /// @n @b More: @ref Coordinate, @ref Colour, @ref Value
    ///
    void define(uint16_t x0, uint16_t y0, uint16_t radius,
                int32_t unit = 1, uint16_t valueColour = myColours.black);

    ///
    /// @brief Update the graphic
    /// @param valueDegrees yaw angle in degrees, already multiplied by unit
    /// @note If valueDegrees=9000 with unit=100, actual angle is 9000 / 100 = 90.00
    ///
    /// @n @b More: @ref Value
    ///
    void draw(int32_t valueDegrees);

  private:
    int32_t _oldDegrees, _unit;
    uint8_t _formatUnit;
};

///
/// @class gPitch
/// @brief Pitch graphics element
/// @details Pitch is a specific display for pitch angle
///
class gPitch : public mtgDial
{
  public:
    ///
    /// @brief Constructor
    /// @param graphics &graphics to which the element belongs
    ///
    gPitch(Graphics * graphics);

    ///
    /// @brief Define the graphic, vector coordinates
    /// @param screen &screen to which the graphic belongs
    /// @param x0 point coordinate, x-axis
    /// @param y0 point coordinate, y-axis
    /// @param dx length, x-axis
    /// @param dy height, y-axis
    /// @param unit multiplier for all values, default = 1, 10 or 100
    /// @param valueColour 16-bit colour for measures, default = black, optional
    ///
    /// @n @b More: @ref Coordinate, @ref Colour, @ref Value
    ///
    void dDefine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                 int32_t unit = 1, uint16_t valueColour = myColours.black);

    ///
    /// @brief Define the graphic, rectangle coordinates
    /// @param screen &screen to which the graphic belongs
    /// @param x0 center, point coordinate, x-axis
    /// @param y0 center, point coordinate, y-axis
    /// @param radius radius
    /// @param unit multiplier for all values, default = 1, 10 or 100
    /// @param valueColour 16-bit colour for measures, default = black, optional
    ///
    /// @n @b More: @ref Coordinate, @ref Colour, @ref Value
    ///
    void define(uint16_t x0, uint16_t y0, uint16_t radius,
                int32_t unit = 1, uint16_t valueColour = myColours.black);

    ///
    /// @brief Update the graphic
    /// @param valueDegrees pitch angle in degrees, already multiplied by unit
    /// @note If valueDegrees=9000 with unit=100, actual angle is 9000 / 100 = 90.00
    ///
    /// @n @b More: @ref Value
    ///
    void draw(int32_t valueDegrees);

  private:
    int32_t _oldDegrees, _unit;
    uint8_t _formatUnit;
};

///
/// @class gRoll
/// @brief Roll graphics element
/// @details Roll is a specific display for roll angle
///
class gRoll : public mtgDial
{
  public:
    ///
    /// @brief Constructor
    /// @param graphics &graphics to which the element belongs
    ///
    gRoll(Graphics * graphics);

    ///
    /// @brief Define the graphic, vector coordinates
    /// @param screen &screen to which the graphic belongs
    /// @param x0 point coordinate, x-axis
    /// @param y0 point coordinate, y-axis
    /// @param dx length, x-axis
    /// @param dy height, y-axis
    /// @param unit multiplier for all values, default = 1, 10 or 100
    /// @param valueColour 16-bit colour for measures, default = black, optional
    ///
    /// @n @b More: @ref Coordinate, @ref Colour, @ref Value
    ///
    void dDefine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                 int32_t unit = 1, uint16_t valueColour = myColours.black);

    ///
    /// @brief Define the graphic, rectangle coordinates
    /// @param screen &screen to which the graphic belongs
    /// @param x0 center, point coordinate, x-axis
    /// @param y0 center, point coordinate, y-axis
    /// @param radius radius
    /// @param unit multiplier for all values, default = 1, 10 or 100
    /// @param valueColour 16-bit colour for measures, default = black, optional
    ///
    /// @n @b More: @ref Coordinate, @ref Colour, @ref Value
    ///
    void define(uint16_t x0, uint16_t y0, uint16_t radius,
                int32_t unit = 1, uint16_t valueColour = myColours.black);

    ///
    /// @brief Update the graphic
    /// @param valueDegrees roll angle in degrees, already multiplied by unit
    /// @note If valueDegrees=9000 with unit=100, actual angle is 9000 / 100 = 90.00
    ///
    /// @n @b More: @ref Value
    ///
    void draw(int32_t valueDegrees);

  private:
    int32_t _oldDegrees, _unit;
    uint8_t _formatUnit;
};

///
/// @brief Meta-Class for graphics based on a rectangular panel
/// @details mtgPanel is a meta-class for graphics based on a rectangular panel
///
class mtgPanel
{
  public:
    ///
    /// @brief Constructor
    /// @param graphics &graphics to which the element belongs
    ///
    mtgPanel(Graphics * graphics);

    mtgPanel() = default;

  protected:
    /// @cond
    Graphics * _pGraphics;
    void drawPanel(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                   uint16_t marksX, uint16_t sectionsY, bool continous);
    uint16_t drawContinue();

    uint16_t _x0, _y0, _dx, _dy, _dz;
    uint16_t _n;
    uint16_t _lastN, _lastY;
    bool _notFirst;
    bool _continous;
    uint16_t _sectionsY, _marksX, _gridC;
    /// @endcond
};

///
/// @class Histogram
/// @brief Histogram graphics element
/// @details Hitogram displays a time-based histogram
///
class Histogram : public mtgPanel
{
  public:
    ///
    /// @brief Constructor
    /// @param graphics &graphics to which the element belongs
    ///
    Histogram(Graphics * graphics);

    ///
    /// @brief Define the graphic, vector coordinates
    /// @param screen &screen to which the graphic belongs
    /// @param x0 point coordinate, x-axis
    /// @param y0 point coordinate, y-axis
    /// @param dx length, x-axis
    /// @param dy height, y-axis
    /// @param valueMin minimum value for measures, already multiplied by unit
    /// @param valueMax maximum value for measures, already multiplied by unit
    /// @param unit multiplier for all values, default = 1, 10 or 100
    /// @param marksX grid on the x or time axis every specified number measures, default = 0 = no grid
    /// @param sectionsY number of sections on the y axis, default = 4, 0 = no grid
    /// @param continous true = keep previous measures, default = false = over-write
    /// @param valueColour 16-bit colour for measures, default = black, optional
    ///
    /// @n @b More: @ref Coordinate, @ref Colour, @ref Value
    ///
    void dDefine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                 int32_t valueMin, int32_t valueMax, int32_t unit = 1,
                 uint16_t marksX = 0, uint16_t sectionsY = 4, bool continous = false,
                 uint16_t valueColour = myColours.black);

    ///
    /// @brief Define the graphic, rectangle coordinates
    /// @param screen &screen to which the graphic belongs
    /// @param x1 top left coordinate, x-axis
    /// @param y1 top left coordinate, y-axis
    /// @param x2 bottom right coordinate, x-axis
    /// @param y2 bottom right coordinate, y-axis
    /// @param valueMin minimum value for measures, already multiplied by unit
    /// @param valueMax maximum value for measures, already multiplied by unit
    /// @param unit multiplier for all values, default = 1, 10 or 100
    /// @param marksX grid on the x or time axis every specified number measures, default = 0 = no grid
    /// @param sectionsY number of sections on the y axis, default = 4, 0 = no grid
    /// @param continous true = keep previous measures, default = false = over-write
    /// @param valueColour 16-bit colour for measures, default = black, optional
    ///
    /// @n @b More: @ref Coordinate, @ref Colour, @ref Value
    ///
    void define(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                int32_t valueMin, int32_t valueMax, int32_t unit = 1,
                uint16_t marksX = 0, uint16_t sectionsY = 4, bool continous = false,
                uint16_t valueColour = myColours.black);

    ///
    /// @brief Update the graphic
    /// @param value new measure, already multiplied by unit
    /// @note If value = 1234 with unit = 100, actual value is 1234/100 = 12.34
    ///
    /// @n @b More: @ref Value
    ///
    void draw(int32_t value);

    ///
    /// @brief Set the x-axis step
    ///
    /// @param step step between two valuesn, x-axis, default = 1
    ///
    void setStep(uint16_t step = 1);

  private:
    int32_t _valueMin, _valueMax, _unit;
    uint16_t _min, _max;
    uint16_t _valueColour;
    uint8_t _formatUnit;
};

///
/// @class Channels
/// @brief Channels graphics element
/// @details Channels displays a multiple channels graph
///
class Channels : public mtgPanel
{
  public:
    ///
    /// @brief Constructor
    /// @param graphics &graphics to which the element belongs
    ///
    Channels(Graphics * graphics);

    ///
    /// @brief Define the graphic, vector coordinates
    /// @param x0 point coordinate, x-axis
    /// @param y0 point coordinate, y-axis
    /// @param dx length, x-axis
    /// @param dy height, y-axis
    /// @param valueMin minimum value for measures, already multiplied by unit
    /// @param valueMax maximum value for measures, already multiplied by unit
    /// @param unit multiplier for all values, default = 1, 10 or 100
    /// @param channels number of channels, default = 4
    /// @param sectionsY number of sections on the y axis, default = 4, 0 = no grid
    /// @param valueColour 16-bit colour for measures, default = black, optional
    ///
    /// @n @b More: @ref Coordinate, @ref Colour, @ref Value
    ///
    void dDefine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                 int32_t valueMin, int32_t valueMax, int32_t unit = 1,
                 uint8_t channels = 4, uint8_t sectionsY = 4,
                 uint16_t valueColour = myColours.black);

    ///
    /// @brief Define the graphic, rectangle coordinates
    /// @param x1 top left coordinate, x-axis
    /// @param y1 top left coordinate, y-axis
    /// @param x2 bottom right coordinate, x-axis
    /// @param y2 bottom right coordinate, y-axis
    /// @param valueMin minimum value for measures, already multiplied by unit
    /// @param valueMax maximum value for measures, already multiplied by unit
    /// @param unit multiplier for all values, default = 1, 10 or 100
    /// @param channels number of channels, default = 4
    /// @param sectionsY number of sections on the y axis, default = 4, 0 = no grid
    /// @param valueColour 16-bit colour for measures, default = black, optional
    ///
    /// @n @b More: @ref Coordinate, @ref Colour, @ref Value
    ///
    void define(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                int32_t valueMin, int32_t valueMax, int32_t unit = 1,
                uint8_t channels = 4, uint8_t sectionsY = 4,
                uint16_t valueColour = myColours.black);

    ///
    /// @brief Update the graphic
    /// @param channel channel to update, 0..channels - 1
    /// @param value new measure, already multiplied by unit
    /// @note If value = 1234 with unit = 100, actual value is 1234/100 = 12.34
    ///
    /// @n @b More: @ref Value
    ///
    void draw(uint8_t channel, int32_t value);

  private:
    int32_t _valueMin, _valueMax, _unit;
    uint16_t _valueColour;
    uint8_t _formatUnit;
};

///
/// @class Multiple
/// @brief Multiple graphics element
/// @details Multiple display a graphic with up to 4 series
///
class Multiple : public mtgPanel
{
  public:
    ///
    /// @brief Constructor
    /// @param graphics &graphics to which the element belongs
    ///
    Multiple(Graphics * graphics);

    ///
    /// @brief Define the graphic, vector coordinates
    /// @param screen &screen to which the graphic belongs
    /// @param x0 point coordinate, x-axis
    /// @param y0 point coordinate, y-axis
    /// @param dx length, x-axis
    /// @param dy height, y-axis
    ///
    /// @param serie1 serie 1: name of the serie
    /// @param valueMin1 serie 1: minimum value for measures, already multiplied by unit
    /// @param valueMax1 serie 1: maximum value for measures, already multiplied by unit
    /// @param unit1 serie 1: multiplier for values, default = 1, 10 or 100
    /// @param valueColour1 serie 1: 16-bit colour for measures, default = black, optional
    ///
    /// @param serie2 serie 2: name of the serie, optional
    /// @param valueMin2 serie 2: minimum value for measures, already multiplied by unit, optional
    /// @param valueMax2 serie 2: maximum value for measures, already multiplied by unit, optional
    /// @param unit2 serie 2: multiplier for values, default = 1, 10 or 100
    /// @param valueColour2 serie 2: 16-bit colour for measures, default = black, optional
    ///
    /// @param serie3 serie 3: name of the serie, optional
    /// @param valueMin3 serie 3: minimum value for measures, already multiplied by unit, optional
    /// @param valueMax3 serie 3: maximum value for measures, already multiplied by unit, optional
    /// @param unit3 serie 3: multiplier for values, default = 1, 10 or 100
    /// @param valueColour3 serie 3: 16-bit colour for measures, default = black, optional
    ///
    /// @param serie4 serie 4: name of the serie, optional
    /// @param valueMin4 serie 4: minimum value for measures, already multiplied by unit, optional
    /// @param valueMax4 serie 4: maximum value for measures, already multiplied by unit, optional
    /// @param unit4 serie 4: multiplier for values, default = 1, 10 or 100
    /// @param valueColour4 serie 4: 16-bit colour for measures, default = black, optional
    ///
    /// @param marksX grid on the x or time axis every specified number measures, default = 0 = no grid
    /// @param sectionsY number of sections on the y axis, default = 4, 0 = no grid
    /// @param continous true = keep previous measures, default = false = over-write
    ///
    /// @note This graphic does not feature memory.
    /// @note For series 2, 3 and 4, minimum and maximum values can't be both = 0.
    ///
    /// @n @b More: @ref Coordinate, @ref Colour, @ref Value
    ///
#if (STRING_MODE == USE_STRING_OBJECT)

    void dDefine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                 String serie1, int32_t valueMin1, int32_t valueMax1, int32_t unit1 = 1, uint16_t valueColour1 = myColours.black,
                 String serie2 = "", int32_t valueMin2 = 0, int32_t valueMax2 = 0, int32_t unit2 = 1, uint16_t valueColour2 = myColours.black,
                 String serie3 = "", int32_t valueMin3 = 0, int32_t valueMax3 = 0, int32_t unit3 = 1, uint16_t valueColour3 = myColours.black,
                 String serie4 = "", int32_t valueMin4 = 0, int32_t valueMax4 = 0, int32_t unit4 = 1, uint16_t valueColour4 = myColours.black,
                 uint16_t marksX = 0, uint16_t sectionsY = 4, bool continous = false);

#elif (STRING_MODE == USE_CHAR_ARRAY)

    void dDefine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                 char serie1[16], int32_t valueMin1, int32_t valueMax1, int32_t unit1 = 1, uint16_t valueColour1 = myColours.black,
                 char serie2[16] = {0}, int32_t valueMin2 = 0, int32_t valueMax2 = 0, int32_t unit2 = 1, uint16_t valueColour2 = myColours.black,
                 char serie3[16] = {0}, int32_t valueMin3 = 0, int32_t valueMax3 = 0, int32_t unit3 = 1, uint16_t valueColour3 = myColours.black,
                 char serie4[16] = {0}, int32_t valueMin4 = 0, int32_t valueMax4 = 0, int32_t unit4 = 1, uint16_t valueColour4 = myColours.black,
                 uint16_t marksX = 0, uint16_t sectionsY = 4, bool continous = false);

#endif // STRING_MODE

    ///
    /// @brief Define the graphic, rectangle coordinates
    /// @param screen &screen to which the graphic belongs
    /// @param x1 top left coordinate, x-axis
    /// @param y1 top left coordinate, y-axis
    /// @param x2 bottom right coordinate, x-axis
    /// @param y2 bottom right coordinate, y-axis
    ///
    /// @param serie1 serie 1: name of the serie
    /// @param valueMin1 serie 1: minimum value for measures, already multiplied by unit
    /// @param valueMax1 serie 1: maximum value for measures, already multiplied by unit
    /// @param unit1 serie 1: multiplier for values, default = 1, 10 or 100
    /// @param valueColour1 serie 1: 16-bit colour for measures, default = black, optional
    ///
    /// @param serie2 serie 2: name of the serie, optional
    /// @param valueMin2 serie 2: minimum value for measures, already multiplied by unit, optional
    /// @param valueMax2 serie 2: maximum value for measures, already multiplied by unit, optional
    /// @param unit2 serie 2: multiplier for values, default = 1, 10 or 100
    /// @param valueColour2 serie 2: 16-bit colour for measures, default = black, optional
    ///
    /// @param serie3 serie 3: name of the serie, optional
    /// @param valueMin3 serie 3: minimum value for measures, already multiplied by unit, optional
    /// @param valueMax3 serie 3: maximum value for measures, already multiplied by unit, optional
    /// @param unit3 serie 3: multiplier for values, default = 1, 10 or 100
    /// @param valueColour3 serie 3: 16-bit colour for measures, default = black, optional
    ///
    /// @param serie4 serie 4: name of the serie, optional
    /// @param valueMin4 serie 4: minimum value for measures, already multiplied by unit, optional
    /// @param valueMax4 serie 4: maximum value for measures, already multiplied by unit, optional
    /// @param unit4 serie 4: multiplier for values, default = 1, 10 or 100
    /// @param valueColour4 serie 4: 16-bit colour for measures, default = black, optional
    ///
    /// @param marksX grid on the x or time axis every specified number measures, default = 0 = no grid
    /// @param sectionsY number of sections on the y axis, default = 4, 0 = no grid
    /// @param continous true = keep previous measures, default = false = over-write
    ///
    /// @note For series 2, 3 and 4, minimum and maximum values can't be both = 0.
    ///
    /// @n @b More: @ref Coordinate, @ref Colour, @ref Value
    ///
#if (STRING_MODE == USE_STRING_OBJECT)

    void define(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                String serie1, int32_t valueMin1, int32_t valueMax1, int32_t unit1 = 1, uint16_t valueColour1 = myColours.black,
                String serie2 = "", int32_t valueMin2 = 0, int32_t valueMax2 = 0, int32_t unit2 = 1, uint16_t valueColour2 = myColours.black,
                String serie3 = "", int32_t valueMin3 = 0, int32_t valueMax3 = 0, int32_t unit3 = 1, uint16_t valueColour3 = myColours.black,
                String serie4 = "", int32_t valueMin4 = 0, int32_t valueMax4 = 0, int32_t unit4 = 1, uint16_t valueColour4 = myColours.black,
                uint16_t marksX = 0, uint16_t sectionsY = 4, bool continous = false);

#elif (STRING_MODE == USE_CHAR_ARRAY)

    void define(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                char serie1[16], int32_t valueMin1, int32_t valueMax1, int32_t unit1 = 1, uint16_t valueColour1 = myColours.black,
                char serie2[16] = {0}, int32_t valueMin2 = 0, int32_t valueMax2 = 0, int32_t unit2 = 1, uint16_t valueColour2 = myColours.black,
                char serie3[16] = {0}, int32_t valueMin3 = 0, int32_t valueMax3 = 0, int32_t unit3 = 1, uint16_t valueColour3 = myColours.black,
                char serie4[16] = {0}, int32_t valueMin4 = 0, int32_t valueMax4 = 0, int32_t unit4 = 1, uint16_t valueColour4 = myColours.black,
                uint16_t marksX = 0, uint16_t sectionsY = 4, bool continous = false);

#endif // STRING_MODE

    ///
    /// @brief Update the graphic
    /// @param value1 serie 1: new measure, already multiplied by unit1
    /// @param value2 serie 2: new measure, already multiplied by unit2
    /// @param value3 serie 3: new measure, already multiplied by unit3
    /// @param value4 serie 4: new measure, already multiplied by unit4
    /// @note If value=1234 with unit=100, actual value is 1234/100=12.34
    ///
    /// @n @b More: @ref Value
    ///
    void draw(int32_t value1, int32_t value2 = 0, int32_t value3 = 0, int32_t value4 = 0);

    ///
    /// @brief Set the x-axis step
    ///
    /// @param step step between two valuesn, x-axis, default = 1
    ///
    void setStep(uint16_t step = 1);

  private:
#if (STRING_MODE == USE_STRING_OBJECT)

    String _name1, _name2, _name3, _name4;

#elif (STRING_MODE == USE_CHAR_ARRAY)

    char _name1[16], _name2[16], _name3[16], _name4[16];

#endif

    int32_t _valueMin1, _valueMin2, _valueMin3, _valueMin4, _valueMax1, _valueMax2, _valueMax3, _valueMax4;
    int32_t _unit1, _unit2, _unit3, _unit4;
    uint16_t _lastY1, _lastY2, _lastY3, _lastY4;
    uint16_t _valueColour1, _valueColour2, _valueColour3, _valueColour4;
    uint8_t _formatUnit1, _formatUnit2, _formatUnit3, _formatUnit4;
};

#endif // hV_GRAPHICS_RELEASE
