///
/// @file hV_GUI.h
/// @brief GUI with text, button, dialog box, menu, slider
///
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @author Rei Vilo
/// @date 20 Jul 2023
/// @version 612
///
/// @copyright (c) Rei Vilo, 2010-2023
/// @copyright All rights reserved
///
/// * Evaluation edition: for professionals or organisations, no commercial usage
/// * Commercial edition: for professionals or organisations, for commercial usage
///
/// @deprecated Rectangle coordinates are deprecated for GUI elements, use vector coordinates instead (6.0.5).
/// @n
/// See @ref Coordinate
/// * button::define() is deprecated, consider button::dDefine instead
/// * button::stringDefine() is deprecated, consider button::dStringDefine() instead.
/// * Label is deprecated, consider dLabel instead.
///
/// @warning Starting release 6.0.5,
/// * The element constructor requires a GUI as parameter;
/// * The first letter of the element class is capitalised.
/// @code {.cpp}
/// GUI myGUI(&myScreen);
/// Text myText(&myGUI);
/// @endcode

#ifndef hV_GUI_RELEASE
///
/// @brief Library release number
///
#define hV_GUI_RELEASE 612

// SDK
#include "hV_HAL_Peripherals.h"

// Configuration
#include "hV_Configuration.h"

// Other libraries
#include "Screen_EPD_EXT3.h"

#if (HAPTICS_MODE != USE_HAPTICS_NONE)
#include "hV_Haptics_DRV2605L.h"
#endif // HAPTICS_MODE

#if SCREEN_EPD_EXT3_RELEASE < 610
#error Required SCREEN_EPD_EXT3_RELEASE 610
#endif // SCREEN_EPD_EXT3_RELEASE

#if (SRAM_MODE != USE_INTERNAL_MCU)
#error SRAM_MODE should be USE_INTERNAL_MCU
#endif // SRAM_MODE

#if (TOUCH_MODE != USE_TOUCH_YES)
#warning TOUCH_MODE should be USE_TOUCH_YES
#endif // TOUCH_MODE

///
/// @name States
/// @{
#define stateDisabled false ///< Input mode disable
#define stateEnabled true ///< Input mode enable
/// @}

///
/// @name Modes for check()
/// @{
#define checkNormal 0 ///< Normal mode
#define checkDirect 1 ///< Direct mode
#define checkInstant 2 ///< Instant mode
#define checkSpecial 3 ///< Special mode
/// @}

///
/// @name Modes for check(), synonyms
/// @{
#define checkStepOne 2 ///< Instant mode, one step
#define checkStepOneAndHalf 1 ///< Direct mode, one and a half steps
#define checkStepTwo 3 ///< Special mode, two steps
#define checkStepThree 0 ///< Normal mode, three steps
/// @}

///
/// @name Styles
/// @{
#define styleNormal 0 ///< Normal style
#define styleRounded 9 ///< Rounded style
/// @}

///
/// @name Font size
/// @{
#define fontSizeAutomatic 0xff ///< Automatic
/// @}

///
/// @name Options for BarGraph, Cursor, PlusMinus, RadioButtons
/// @{
#define optionHorizontal false ///< Horizontal
#define optionVertical true ///< Vertical
/// @}

///
/// @name Options for Text and TextBox
/// @{
#define optionWithoutFrame false ///< No rectangle
#define optionWithFrame true ///< Text inside a rectangle
/// @}

///
/// @name Options for Button
/// @{
#define optionStandard false ///< Standard push button
#define optionFlipFlop true ///< Flip-flop button
/// @}

///
/// @name Options for horizontal and vertical position
/// @{
#define horizontalLeft -1 ///< Left
#define horizontalCentre 0 ///< Centre
#define horizontalCenter 0 ///< American English variant for horizontalCentre
#define horizontalRight 1 ///< Right
#define verticalTop 1 ///< Top
#define verticalMiddle 0 ///< Middle
#define verticalBottom -1 ///< Bottom

#define hL horizontalLeft ///< Short for horizontalLeft
#define hC horizontalCentre ///< Short for horizontalCentre
#define hR horizontalRight ///< Short for horizontalRight
#define vT verticalTop ///< Short for verticalTop
#define vM verticalMiddle ///< Short for verticalMiddle
#define vB verticalBottom ///< Short for verticalBottom
/// @}

///
/// @name Patterns for Table
/// @{
#define patternSquare 0x01 ///< Empty square
#define patternCircle 0x02 ///< Empty circle
#define patternTriangle 0x03 ///< Empty triangle
#define patternSquareSolid 0x11 ///< Solid square
#define patternCircleSolid 0x12 ///< Solid circle
#define patternTriangleSolid 0x13 ///< Solid triangle
/// @}

///
/// @class GUI
/// @brief GUI
/// @details Shared settings
///
class GUI
{
    friend class Area;
    friend class Button;
    friend class CheckBox;
    friend class Cursor;
    friend class Text;
    friend class PlusMinus;
    friend class RadioButtons;
    friend class Table;
    friend class Keyboard;
    friend class Switch;
    friend class TextBox;
    friend class BarGraph;

  public:
    ///
    /// @brief Constructor
    ///
    /// @param screen &screen to which the GUI elements belong
    ///
    GUI(Screen_EPD_EXT3_Fast * screen);

    ///
    /// @brief Initialise the GUI
    /// @note By default, the GUI element manages refresh with fast update. Use delegate(false) to change.
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

    ///
    /// @brief Define the default state
    ///
    /// @param state default = stateEnabled = enabled, stateDisabled = disabled
    ///
    void setState(bool state = stateEnabled);

    ///
    /// @brief Define the default style
    /// @param state default = 0 = styleNormal, otherwise = 5 = styleRounded
    ///
    void setStyle(uint8_t style = styleNormal);

    ///
    /// @brief Delegate the refresh policy
    ///
    /// @param delegate true = default = refresh managed by the GUI element with fast update
    /// @note If false, refresh managed by the caller
    /// * full update
    /// * fast update
    ///
    void delegate(bool delegate = true);

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
                bool flush0 = false);

    void dRoundedLabel(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t radius,
#if (STRING_MODE == USE_STRING_OBJECT)
                       String text0,
#elif (STRING_MODE == USE_CHAR_ARRAY)
                       const char * text0,
#endif // STRING_MODE
                       uint16_t textColour0, uint16_t backColour0,
                       int8_t horizontal0, int8_t vertical0,
                       uint8_t size0 = fontSizeAutomatic, bool flush0 = false);

  private:
    void _beginContinuity();
    void _endContinuity();
    Screen_EPD_EXT3_Fast * _pScreen;
    uint16_t _colourFront, _colourBack, _colourMiddle;
    bool _delegate;
    bool _enable;
    uint8_t _style;
};

///
/// @brief Item combines index and text for GUI elements
///
/// @code {.cpp}
/// item myMenuItems[] = {
///     { 0x0000, "Menu 0" } ,
///     { 0x1000, "Item 1" } ,
///     { 0x1100, "Item 11" } ,
///     ...
///     { 0x1f00, "Item 1f" } ,
///     { 0x2000, "Item 2" } ,
///     { 0x2100, "Item 21" } ,
///     { 0x2110, "Item 211" } ,
///     { 0x2121, "Item 2111" } ,
/// };
/// @endcode
/// @n Use setItem to convert a string into an item.
struct Item
{
    uint16_t index; ///< index 0x1000..0xffff, 0x0000 is reserved for menu root
    char label[16]; ///< text, up to 16 characters
};

const Item itemEmpty = {0};

///
/// @brief Finite state machine
/// @details Used for Button, CheckBox, Cursor, BarGraph
///
enum fsmGUI_e
{
    fsmReleased, ///< No touch
    fsmTouched, ///< First touch
    fsmConfirmed ///< Touch maintained
};

///
/// @brief Set item
/// @details Packs index and text into an item
/// @param index index
/// @param text text
/// @return item
///
#if (STRING_MODE == USE_STRING_OBJECT)

Item setItem(uint16_t index, String text);

#elif (STRING_MODE == USE_CHAR_ARRAY)

Item setItem(uint16_t index, const char * text);

#endif // STRING_MODE

///
/// @brief ARRAY_SIZE calculates the number of elements in an array
///
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#if (HAPTICS_MODE != USE_HAPTICS_NONE)
///
/// @brief Haptic controller
/// @details Three options
extern DRV2605L myDRV2605L;
#endif

///
/// @class Area
/// @brief Area GUI element
/// @details Area defines an active zone on the screen to manage touch
///
class Area
{
  public:
    ///
    /// @brief Constructor
    /// @param gui &gui to which the element belongs
    ///
    Area(GUI * gui);

    Area() = default;

    ///
    /// @brief Define area, vector coordinates
    /// @param x0 point coordinate, x-axis
    /// @param y0 point coordinate, y-axis
    /// @param dx length, x-axis
    /// @param dy height, y-axis
    /// @param index index, optional
    /// @note State set to value from GUI
    ///
    /// @n @b More: @ref Coordinate
    ///
    void dDefine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                 uint16_t index = 0x0000);

    ///
    /// @brief Return state of the area
    /// @return true if area enabled, false otherwise
    ///
    bool getState();

    ///
    /// @brief Define state of the area
    /// @param state default = stateEnabled = area enabled, stateDisabled = otherwise
    /// @note When created, state set to value from GUI
    ///
    void setState(bool state = stateEnabled);

    ///
    /// @brief Set option of the area
    /// @details Exact usage of the option is defined by the object.
    /// @param option default = true = option enabled, false = otherwise
    ///
    void setOption(bool option = true);

    ///
    /// @brief Define the default style
    /// @param state default = 0 = styleNormal, otherwise = 5 = styleRounded
    ///
    void setStyle(uint8_t style = styleNormal);

    ///
    /// @brief Check if area is pressed
    /// @param mode default = checkNormal = area waits for release, checkDirect = no final redraw if selected, checkInstant = element is activated by press only
    /// @return true if area pressed
    ///
    bool check(uint8_t mode = checkNormal);

    ///
    /// @brief Return index
    /// @return index of the area
    ///
    uint16_t getIndex();

  protected:
    /// @cond
    uint16_t _x0, _y0, _dx, _dy, _radius;
    bool _enable, _option;
    uint16_t _index;
    GUI * _pGUI;
    /// @endcond
};

///
/// @class Button
/// @brief Button GUI element
/// @details Button is an area with a label
///
class Button : public Area
{
    friend class CheckBox;

  public:
    ///
    /// @brief Constructor
    /// @param gui &gui to which the element belongs
    ///
    Button(GUI * gui);

    Button() = default;

    ///
    /// @brief Define item-based button, vector coordinates
    /// @param x0 point coordinate, x-axis
    /// @param y0 point coordinate, y-axis
    /// @param dx length, x-axis
    /// @param dy height, y-axis
    /// @param item0 item (including index and text)
    /// @param size0 size for text, default = fontSizeAutomatic = automatic
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    void dDefine(uint16_t x0, uint16_t y0,
                 uint16_t dx, uint16_t dy,
                 Item item0,
                 uint8_t size0 = fontSizeAutomatic);

    //     ///
    //     /// @brief Define item-based button, vector coordinates
    //     /// @param x0 point coordinate, x-axis
    //     /// @param y0 point coordinate, y-axis
    //     /// @param dx length, x-axis
    //     /// @param dy height, y-axis
    //     /// @param text0 text
    //     /// @param size0 size for text, default = fontSizeAutomatic = automatic
    //     ///
    //     /// @n @b More: @ref Coordinate, @ref Colour
    //     ///
    // #if (STRING_MODE == USE_STRING_OBJECT)
    //     void dStringDefine(uint16_t x0, uint16_t y0,
    //                        uint16_t dx, uint16_t dy,
    //                        String text0,
    //                        uint8_t size0 = fontSizeAutomatic);
    // #elif (STRING_MODE == USE_CHAR_ARRAY)
    //     void dStringDefine(uint16_t x0, uint16_t y0,
    //                        uint16_t dx, uint16_t dy,
    //                        const char * text0,
    //                        uint8_t size0 = fontSizeAutomatic);
    // #endif // STRING_MODE

    ///
    /// @brief Define item-based button, vector coordinates
    /// @param screen &screen to which the button belongs
    /// @param x0 point coordinate, x-axis
    /// @param y0 point coordinate, y-axis
    /// @param dx length, x-axis
    /// @param dy height, y-axis
    /// @param text0 text
    /// @param size0 size for text, default = fontSizeAutomatic = automatic
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
#if (STRING_MODE == USE_STRING_OBJECT)

    void dStringDefine(uint16_t x0, uint16_t y0,
                       uint16_t dx, uint16_t dy,
                       String text0,
                       uint8_t size0 = fontSizeAutomatic);

#elif (STRING_MODE == USE_CHAR_ARRAY)

    void dStringDefine(uint16_t x0, uint16_t y0,
                       uint16_t dx, uint16_t dy,
                       const char * text0 = "",
                       uint8_t size0 = fontSizeAutomatic);

#endif // STRING_MODE

    // ///
    // /// @brief Return state of the button
    // /// @return true if button enabled, false otherwise
    // ///
    // bool state();

    ///
    /// @brief Draw the button
    /// @param fsm state
    /// @note 4 states
    /// @n * if button enabled,
    /// @n fsm = fsmReleased, fsmTouched, fsmConfirmed
    /// @n * if button disabled,
    /// @n flag = true: statement checkbox, flag = false: unavailable checkbox
    ///
    void draw(fsmGUI_e fsm = fsmReleased);

    ///
    /// @brief Check button is pressed
    /// @details With button animation
    /// @param mode default = checkNormal = area waits for release, checkInstant = element is activated by press only
    /// @return true if button pressed
    ///
    bool check(uint8_t mode = checkNormal);

    ///
    /// @brief Set current value of the flip-flop button
    /// @param value value true = default, false
    /// @note Only if setOption = optionFlipFlop for flip-flop button
    ///
    void setValue(bool value = true);

    ///
    /// @brief Return current value of the flip-flop button
    /// @return current value true = checked or false = unchecked
    /// @note Only if setOption = optionFlipFlop for flip-flop button
    ///
    bool getValue();

  private:
    /// @cond
#if (STRING_MODE == USE_STRING_OBJECT)

    String _text;

#elif (STRING_MODE == USE_CHAR_ARRAY)

    char _text[16];

#endif // STRING_MODE

    uint8_t _size;
    bool _value;
    /// @endcond
};

///
/// @class CheckBox
/// @brief Check-box GUI element
/// @details CheckBox is a flip-flop button
///
class CheckBox : public Button
{
  public:
    ///
    /// @brief Constructor
    /// @param gui &gui to which the element belongs
    ///
    CheckBox(GUI * gui);

    ///
    /// @brief Draw the button
    /// @param fsm state
    /// @note 4 states
    /// @n * if button enabled,
    /// @n fsm = fsmReleased, fsmTouched, fsmConfirmed
    /// @n * if button disabled,
    /// @n flag = true: statement checkbox, flag = false: unavailable checkbox
    ///
    void draw(fsmGUI_e fsm = fsmReleased);

    ///
    /// @brief Check button is pressed
    /// @details With button animation
    /// @param mode default = checkNormal = area waits for release, checkInstant = element is activated by press only
    /// @return true if button pressed
    ///
    bool check(uint8_t mode = checkNormal);
};

///
/// @class Switch
/// @brief Switch GUI element
/// @details Switch is a two-state button
///
class Switch : public Area
{
  public:
    ///
    /// @brief Constructor
    /// @param gui &gui to which the element belongs
    ///
    Switch(GUI * gui);

    ///
    /// @brief Define item-based switch, vector coordinates
    /// @param x0 point coordinate, x-axis
    /// @param y0 point coordinate, y-axis
    /// @param dx length, x-axis
    /// @param dy height, y-axis
    /// @param itemLeftBottom left or bottom item (including index and text)
    /// @param itemRightTop right or top item (including index and text)
    /// @param size0 size for text, default = fontSizeAutomatic = automatic
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    void dDefine(uint16_t x0, uint16_t y0,
                 uint16_t dx, uint16_t dy,
                 Item itemLeftBottom,
                 Item itemRightTop,
                 uint8_t size0 = fontSizeAutomatic);

    ///
    /// @brief Define switch, vector coordinates
    /// @param x0 point coordinate, x-axis
    /// @param y0 point coordinate, y-axis
    /// @param dx length, x-axis
    /// @param dy height, y-axis
    /// @param textLeftBottom text for left or bottom
    /// @param textRightTop text for right or top
    /// @param size size of the font, default = fontSizeAutomatic = automatic, optional
    ///
#if (STRING_MODE == USE_STRING_OBJECT)

    void dStringDefine(uint16_t x0, uint16_t y0,
                       uint16_t dx, uint16_t dy,
                       String textLeftBottom,
                       String textRightTop,
                       uint8_t size = fontSizeAutomatic);

#elif (STRING_MODE == USE_CHAR_ARRAY)

    void dStringDefine(uint16_t x0, uint16_t y0,
                       uint16_t dx, uint16_t dy,
                       const char * textLeftBottom,
                       const char * textRightTop,
                       uint8_t size = fontSizeAutomatic);

#endif // STRING_MODE
    ///
    /// @brief Draw the switch
    /// @param fsm state
    /// @note 4 states
    /// @n * if switch enabled,
    /// @n fsm = fsmReleased, fsmTouched, fsmConfirmed
    /// @n * if switch disabled,
    /// @n flag = true: statement switch, flag = false: unavailable switch
    ///
    void draw(fsmGUI_e fsm = fsmReleased);

    ///
    /// @brief Check switch is pressed
    /// @details With switch animation
    /// @param mode default = checkNormal = area waits for release, checkInstant = element is activated by press only
    /// @return true if switch pressed
    ///
    bool check(uint8_t mode = checkNormal);

    ///
    /// @brief Return current value
    /// @return current position, horizontalLeft = verticalBottom = -1 or horizontalRight = verticalTop = +1
    ///
    int8_t getValue();

    ///
    /// @brief Return index of the switch
    /// @return index from items defined with dDefine
    ///
    uint16_t getIndex();

  private:
    /// @cond
    uint16_t _index1;

#if (STRING_MODE == USE_STRING_OBJECT)

    String _textLeft;
    String _textRight;

#elif (STRING_MODE == USE_CHAR_ARRAY)

    char _textLeft[16];
    char _textRight[16];

#endif // STRING_MODE

    uint8_t _size;
    int8_t _value; /// -1 or +1
    /// @endcond
};

///
/// @class Cursor
/// @brief Cursor GUI element
/// @details Cursor is an area with a cursor
///
class Cursor : public Area
{
  public:
    ///
    /// @brief Constructor
    /// @param gui &gui to which the element belongs
    ///
    Cursor(GUI * gui);

    ///
    /// @brief Define cursor, vector coordinates
    /// @param x0 point coordinate, x-axis
    /// @param y0 point coordinate, y-axis
    /// @param dx length, x-axis
    /// @param dy height, y-axis
    /// @param minValue minimum value, int32
    /// @param maxValue maximum value, int32
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    void dDefine(uint16_t x0, uint16_t y0,
                 uint16_t dx, uint16_t dy,
                 int32_t minValue, int32_t maxValue);

    ///
    /// @brief Set orientation of the cursor
    /// @details The cursor is initialised with false = horizontal. Use setOption to change the orientation.
    /// @param flag default = optionVertical = vertical, optionHorizontal = horizontal
    ///
    void setOption(bool option = optionVertical);

    ///
    /// @brief Draw the cursor
    /// @param flag default = false = no overriding
    /// @note 4 states
    /// @n * if cursor enabled,
    /// @n fsm = fsmReleased, fsmTouched, fsmConfirmed
    /// @n * if cursor disabled,
    /// @n flag = true: statement cursor, flag = false: unavailable cursor
    ///
    void draw(fsmGUI_e fsm = fsmReleased);

    ///
    /// @brief Check cursor is pressed
    /// @details With cursor animation
    /// @param mode default = checkNormal = area waits for release, checkInstant = element is activated by press only
    /// @return true if button pressed
    ///
    bool check(uint8_t mode = checkNormal);

    ///
    /// @brief Set current value of the cursor
    /// @param value value between minValue and maxValue
    ///
    void setValue(int32_t value);

    ///
    /// @brief Return current value
    /// @return current value between minValue and maxValue
    ///
    int32_t getValue();

  private:
    /// @cond
    ///
    /// @brief Play haptic feed-back
    /// @param z0 origin, x0 or y0
    /// @param dz x-lenght or y-height
    /// @param z actual coordinate
    /// @note All the coordinates are screen- and touch-related.
    ///
    void playHaptic(uint16_t zMin, uint16_t zMax, uint16_t z);

    int32_t _minValue, _maxValue;
    uint16_t _gMin, _gMax, _gPosition, _gOldPosition, _gLine;
    /// @endcond
};

///
/// @class TextBox
/// @brief TextBox GUI element
/// @details TextBox is an area for text with word-wrap and auto-scrolling
/// @n Special commands are accepted:
/// * \n = new line
/// * \r = for clear
///
class TextBox : public Area
{
  public:
    ///
    /// @brief Constructor
    /// @param gui &gui to which the element belongs
    ///
    TextBox(GUI * gui);

    ///
    /// @brief Define a text box, vector coordinates

    /// @param x0 point coordinate, x-axis
    /// @param y0 point coordinate, y-axis
    /// @param dx length, x-axis
    /// @param dy height, y-axis
    /// @param size size of the font, default = 0
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    void dDefine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                 uint8_t size = 0);

    ///
    /// @brief Draw a frame around the text box option of the area
    /// @param option default = optionWithoutFrame no frame, true = with frame
    ///
    void setOption(bool option = optionWithoutFrame);

    ///
    /// @brief Clear and draw the text box
    ///
    void clear();

    ///
    /// @brief Scroll the text on line up
    ///
    void scroll();

    ///
    /// @brief Display text on the text box
    /// @param text string to be printed
    ///
#if (STRING_MODE == USE_STRING_OBJECT)

    void draw(String text);

#elif (STRING_MODE == USE_CHAR_ARRAY)

    void draw(const char * text);

#endif

    ///
    /// @brief Format and display the values on the text box
    /// @param format format with standard codes
    /// @param ... list of values
    ///
    void drawFormat(const char * format, ...);

  private:
    /// @cond
    uint8_t _size, _ty; // , _tx;
    int32_t _maxY, _maxX, _indexLine, _indexColumn;
    uint16_t _z0; // origin
    /// @endcond
};

///
/// @class Text
/// @brief Text GUI element
/// @details Text displays a text with format options
///
class Text : public Area
{
  public:
    ///
    /// @brief Constructor
    /// @param gui &gui to which the element belongs
    ///
    Text(GUI * gui);

    ///
    /// @brief Define a text box, vector coordinates
    /// @param x0 point coordinate, x-axis
    /// @param y0 point coordinate, y-axis
    /// @param dx length, x-axis
    /// @param dy height, y-axis
    /// @param horizontal0 horizontal position with 0 = center, 1 = left, 2 = right, optional
    /// @param vertical0 vertical position with 0 = center, 1 = top, 2 = bottom, optional
    /// @param size size of the font, default = fontSizeAutomatic = automatic, optional
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    void dDefine(uint16_t x0, uint16_t y0,
                 uint16_t dx, uint16_t dy,
                 uint8_t horizontal0 = 0,
                 uint8_t vertical0 = 0,
                 uint8_t size = fontSizeAutomatic);

    ///
    /// @brief Draw a frame around the text
    /// @details The text is initialised with false = no frame. Use setOption to draw a frame.
    /// @param option default = optionWithFrame = within a frame, optionWithoutFrame = no frame
    ///
    void setOption(bool option = optionWithFrame);

    ///
    /// @brief Display the text
    /// @param text text to be displayed
    ///
#if (STRING_MODE == USE_STRING_OBJECT)

    void draw(String text);

#elif (STRING_MODE == USE_CHAR_ARRAY)

    void draw(const char * text);

#endif // STRING_MODE

    ///
    /// @brief Format and display the values
    /// @param format format with standard codes
    /// @param ... list of values
    ///
    void drawFormat(const char * format, ...);

  private:
    /// @cond
    uint8_t _horizontal, _vertical;
    uint8_t _fontSize;

#if (STRING_MODE == USE_STRING_OBJECT)

    String _text;

#elif (STRING_MODE == USE_CHAR_ARRAY)

    char _text[32];

#endif // STRING_MODE

    /// @endcond
};

///
/// @class BarGraph
/// @brief Bar-graph GUI element
///
class BarGraph : public Area
{
  public:
    ///
    /// @brief Constructor
    /// @param gui &gui to which the element belongs
    ///
    BarGraph(GUI * gui);

    ///
    /// @brief Define bar-graph, vector coordinates
    /// @param x0 point coordinate, x-axis
    /// @param y0 point coordinate, y-axis
    /// @param dx length, x-axis
    /// @param dy height, y-axis
    /// @param minValue minimum value, int32
    /// @param maxValue maximum value, int32
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    void dDefine(uint16_t x0, uint16_t y0,
                 uint16_t dx, uint16_t dy,
                 int32_t minValue, int32_t maxValue);

    ///
    /// @brief Set orientation of the bar-graph
    /// @details The bar-graph is initialised with false = horizontal. Use setOption to change the orientation.
    /// @param option default = optionVertical = vertical, optionHorizontal = horizontal
    ///
    void setOption(bool option = optionVertical);

    ///
    /// @brief Update the bargraph with value
    /// @param value value to display
    ///
    void draw(int32_t value);

  private:
    /// @cond
    int32_t _minValue, _maxValue, _value;
    int16_t _oldSection;
    /// @endcond
};

/// @class PlusMinus
/// @brief Plus-minus button GUI element
/// @details PlusMinus displays a combined plus-minus button
///
class PlusMinus : public Area
{
  public:
    PlusMinus(GUI * gui);

    ///
    /// @brief Define plus-minus button, vector coordinates
    /// @param x0 point coordinate, x-axis
    /// @param y0 point coordinate, y-axis
    /// @param dx length, x-axis
    /// @param dy height, y-axis
    /// @param minus bottom or left label, default = "-"
    /// @param plus top or right label, default = "+"
    ///
    void dDefine(uint16_t x0, uint16_t y0,
                 uint16_t dx, uint16_t dy,
                 const char * minus = "-", const char * plus = "+");

    ///
    /// @brief Set orientation of the plus-minus button
    /// @param flag default = optionVertical = vertical, optionHorizontal = horizontal
    ///
    void setOption(bool option = optionVertical);

    ///
    /// @brief Draw the button
    /// @param value default = 0 = no overriding, -1 = minus, 1 = plus
    ///
    void draw(int8_t value = 0);

    ///
    /// @brief Check plus-minus button is pressed
    /// @details With button animation
    /// @param mode default = checkNormal = area waits for release, checkInstant = element is activated by press only
    /// @return true if button pressed
    ///
    bool check(uint8_t mode = checkNormal);

    ///
    /// @brief Return current value of the plus-minus button
    /// @return current value minus = -1, nil = 0, plus = +1
    ///
    int8_t getValue();

  private:
    uint16_t _dSlot; /// middle, horizontal or vertical axis
    int8_t _value; /// -1, 0 or +1
    char _labelMinus[5], _labelPlus[5];
};

///
/// @class RadioButtons
/// @brief Radio-buttons GUI element
/// @details RadioButtons contains 2 or 3 mutually exclusive options.
///
class RadioButtons : public Area
{
  public:
    ///
    /// @brief Constructor
    /// @param gui &gui to which the element belongs
    ///
    RadioButtons(GUI * gui);

    ///
    /// @brief Define radio buttons, vector coordinates
    /// @param x0 point coordinate, x-axis
    /// @param y0 point coordinate, y-axis
    /// @param dx length, x-axis
    /// @param dy height, y-axis
    /// @param item1 item for first button
    /// @param item2 item for second button
    /// @param item3 item for third button, optional
    /// @param size size of the font, default = fontSizeAutomatic = automatic, optional
    /// @note button1 and button2 are compulsory, button3 optional.
    /// @note Buttons are displayed from top to down or from left to right.
    ///
    void dDefine(uint16_t x0, uint16_t y0,
                 uint16_t dx, uint16_t dy,
                 Item item1,
                 Item item2,
                 Item item3 = itemEmpty,
                 uint8_t size = fontSizeAutomatic);

    ///
    /// @brief Define radio buttons, vector coordinates
    /// @param x0 point coordinate, x-axis
    /// @param y0 point coordinate, y-axis
    /// @param dx length, x-axis
    /// @param dy height, y-axis
    /// @param button1 text for first button
    /// @param button2 text for second button
    /// @param button3 text for third button, optional
    /// @param size size of the font, default = fontSizeAutomatic = automatic, optional
    /// @note button1 and button2 are compulsory, button3 optional.
    /// @note Buttons are displayed from top to down or from left to right.
    ///
#if (STRING_MODE == USE_STRING_OBJECT)

    void dStringDefine(uint16_t x0, uint16_t y0,
                       uint16_t dx, uint16_t dy,
                       String button1,
                       String button2,
                       String button3 = "\0",
                       uint8_t size = fontSizeAutomatic);

#elif (STRING_MODE == USE_CHAR_ARRAY)

    void dStringDefine(uint16_t x0, uint16_t y0,
                       uint16_t dx, uint16_t dy,
                       const char * button1,
                       const char * button2,
                       const char * button3 = "\0",
                       uint8_t size = fontSizeAutomatic);

#endif // STRING_MODE

    ///
    /// @brief Set orientation of the radio-buttons
    /// @details The radio-buttons are initialised with true = vertical. Use setOption to change the orientation.
    /// @param option default = optionVertical = vertical, optionHorizontal = horizontal
    ///
    void setOption(bool option = optionVertical);

    ///
    /// @brief Draw the radio-buttons
    /// @details With button animation
    /// @param fsm state
    /// @note 4 states
    /// @n * if button enabled,
    /// @n fsm = fsmReleased, fsmTouched, fsmConfirmed
    /// @n * if button disabled,
    /// @n flag = true: statement checkbox, flag = false: unavailable checkbox
    /// @note If value = 0, display all
    ///
    void draw(fsmGUI_e fsm = fsmReleased);

    ///
    /// @brief Check radio-button is pressed
    /// @details With button animation
    /// @param mode default = checkNormal = area waits for release, checkInstant = element is activated by press only
    /// @return true if button pressed
    ///
    bool check(uint8_t mode = checkNormal);

    ///
    /// @brief Set selected radio-button
    /// @param value selected radio-button, default = 0 = none
    ///
    void setValue(uint8_t value = 0);

    ///
    /// @brief Return selected radio-button
    /// @return current value 1, 2, 3, from top to down or from left to right, 0 = none
    ///
    uint8_t getValue();

    ///
    /// @brief Return index of the selected radio-button
    /// @return index from items defined with dDefine, 0 = none
    ///
    uint16_t getIndex();

  private:
    /// @cond
    Item _item[3]; ///< array of items, 2 or 3
    uint8_t _size; ///< font
    uint8_t _items; ///< number of items
    uint16_t _dSlot; ///< vertical or horizontal slot size
    uint8_t _oldValue, _value; ///< 0, 1, 2 and optionally 3
    /// @endcond
};

///
/// @class Table
/// @brief Table GUI element
/// @details Table displays columns and rows of cells.
///
class Table: public Area
{
  public:
    ///
    /// @brief Constructor
    /// @param gui &gui to which the element belongs
    ///
    Table(GUI * gui);

    /// @brief Define the table, vector coordinates
    /// @param x0 point coordinate, x-axis
    /// @param y0 point coordinate, y-axis
    /// @param dx length, x-axis
    /// @param dy height, y-axis
    /// @param columns number of columns, default = 4
    /// @param lines number of rows, default = 3
    ///
    /// @note The touch on table is disabled at initialisation.
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    void dDefine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                 uint8_t columns = 4, uint8_t lines = 3);

    /// @brief Define the table, rectangle coordinates
    /// @param x1 top left coordinate, x-axis
    /// @param y1 top left coordinate, y-axis
    /// @param x2 bottom right coordinate, x-axis
    /// @param y2 bottom right coordinate, y-axis
    /// @param columns number of columns, default = 4
    /// @param lines number of rows, default = 3
    ///
    /// @note The touch on table is disabled at initialisation.
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    void define(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                uint8_t columns = 4, uint8_t lines = 3);

    /// @brief Draw the initial empty table
    ///
    void drawInitial();

    /// @brief Update the table
    /// @param indexColumn cell to update, column number = 0..columns - 1
    /// @param indexLine cell to update, row number = 0..lines - 1
    /// @param colour colour for the cell default = myColours.black
    /// @param pattern on the cell, default = patternSquare
    ///
    void draw(uint8_t indexColumn, uint8_t indexLine, uint16_t colour = myColours.black, uint8_t pattern = patternSquareSolid);

    ///
    /// @brief Define state of the table
    /// @param state default = stateEnabled = table enabled, stateDisabled = otherwise
    /// @note When created, state set to false
    ///
    void setState(bool state = stateEnabled);

    ///
    /// @brief Check if table is pressed
    /// @param mode default = checkNormal = area waits for release, checkInstant = element is activated by press only
    /// @return true if button pressed
    ///
    bool check(uint8_t mode = checkNormal);

    ///
    /// @brief Return column and row indexes of the cell touched
    /// @param indexColumn cell touched, column index = 0..columns - 1
    /// @param indexLine cell touched, row index = 0..lines - 1
    ///
    /// @code {.cpp}
    /// uint8_t indexColumn, indexLine;
    /// if myTable.check()
    /// {
    ///   myTable.getIndex(&indexColumn, &indexLine);
    /// }
    /// @endcode
    ///
    void getIndex(uint8_t & indexColumn, uint8_t & indexLine);

  private:
    // uint16_t _x0, _y0, _dx, _dy, _radius;
    uint8_t _lines, _columns;
    // bool _enable;
    uint8_t _indexColumn, _indexLine;
};

///
/// @class Keyboard
/// @brief Keyboard GUI element
/// @details Keyboard displays columns and rows of keys.
///
class Keyboard: public Area
{
  public:
    ///
    /// @brief Constructor
    /// @param gui &gui to which the element belongs
    ///
    Keyboard(GUI * gui);

    /// @brief Define the keyboard, vector coordinates
    /// @param x0 point coordinate, x-axis
    /// @param y0 point coordinate, y-axis
    /// @param dx length, x-axis
    /// @param dy height, y-axis
    /// @param keys array of items for keys, one item per key
    /// @param columns number of columns, default = 4
    /// @param lines number of rows, default = 3
    /// @param size0 size for text, default = fontSizeAutomatic = automatic
    ///
    /// @note Keys are arranged per line.
    /// @code {.cpp}
    ///   Line 1: Column 1, Column 2, ... Column<columns>
    ///   Line 2: Column 1, Column 2, ... Column<columns>
    ///   ...
    ///   Line <lines>: Column 1, Column 2, ... Column<columns>
    /// @endcode
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    void dDefine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                 Item * keys,
                 uint8_t columns = 4, uint8_t lines = 3,
                 uint8_t size0 = fontSizeAutomatic);

    /// @brief Define the keyboard, rectangle coordinates
    /// @param x1 top left coordinate, x-axis
    /// @param y1 top left coordinate, y-axis
    /// @param x2 bottom right coordinate, x-axis
    /// @param y2 bottom right coordinate, y-axis
    /// @param keys array of items for keys
    /// @param columns number of columns, default = 4
    /// @param lines number of rows, default = 3
    /// @param size0 size for text, default = fontSizeAutomatic = automatic
    ///
    /// @n @b More: @ref Coordinate, @ref Colour
    ///
    void define(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                Item * keys,
                uint8_t columns = 4, uint8_t lines = 3,
                uint8_t size0 = fontSizeAutomatic);

    ///
    /// @brief Draw frames around keys
    /// @details The keyboard is initialised with false = no boxes. Use setOption to draw frames.
    /// @param option default = optionWithFrame = within boxes, false = no frames
    ///
    void setOption(bool option = optionWithFrame);

    /// @brief Draw the initial keyboard
    ///
    void drawInitial();

    /// @brief Update the keyboard
    /// @param indexColumn cell to update, column number = 0..columns - 1
    /// @param indexLine cell to update, row number = 0..lines - 1
    /// @param fsm state
    /// @note 4 states
    /// @n * if button enabled,
    /// @n fsm = fsmReleased, fsmTouched, fsmConfirmed
    /// @n * if button disabled,
    /// @n flag = true: statement checkbox, flag = false: unavailable checkbox
    ///
    void draw(uint8_t indexColumn, uint8_t indexLine, fsmGUI_e fsm = fsmReleased);

    ///
    /// @brief Define state of the area
    /// @param state default = stateEnabled = area enabled, stateDisabled = otherwise
    /// @note When created, state set to false
    ///
    void setState(bool state = stateEnabled);

    ///
    /// @brief Check if area is pressed
    /// @param mode default = checkNormal = area waits for release, checkInstant = element is activated by press only
    /// @return true if button pressed
    ///
    bool check(uint8_t mode = checkNormal);

    ///
    /// @brief Return index of the touched key
    /// @return index from items defined with dDefine, 0 = none
    ///
    uint16_t getIndex();

  private:
    // uint16_t _x0, _y0, _dx, _dy, _radius;
    uint8_t _lines, _columns;
    uint8_t _size; ///< font
    Item * _keys;
    // bool _enable, _option;
    // uint16_t _index;
    uint8_t _indexColumn, _indexLine;
};

#endif // hV_GUI_RELEASE
