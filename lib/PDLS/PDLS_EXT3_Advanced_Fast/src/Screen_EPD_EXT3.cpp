//
// Screen_EPD_EXT3.cpp
// Library C++ code
// ----------------------------------
//
// Project Pervasive Displays Library Suite
// Based on highView technology
//
// Created by Rei Vilo, 28 Jun 2016
//
// Copyright (c) Rei Vilo, 2010-2023
// Licence All rights reserved
//
// Release 508: Added eScreen_EPD_EXT3_437_0C xE2437CS0Cx
// Release 509: Added eScreen_EPD_EXT3_271_Fast xE2271PS09x
// Release 510: Improved consistency with PDLS_EXT3_Advanced_Fast_Large
// Release 510: Improved partial update for eScreen_EPD_EXT3_370_0C xE2370CS0Cx
// Release 511: Improved stability for external SPI SRAM
// Release 512: Added support for 266 Armor
// Release 521: Added friend class File_All
// Release 529: Added support for pixmap file
// Release 530: Added support for external SRAM
// Release 531: Added getBoardPins
// Release 540: Added global update option
// Release 542: Simulated global update
// Release 543: Improved support for eScreen_EPD_EXT3_266_0C xE2266CS0Cx
// Release 601: Added support for screens with embedded fast update
// Release 602: Improve functions structure
// Release 603: Improved numbering consistency
// Release 603: Simplified partial update
// Release 604: Improved optional external power management
// Release 604: Improved speed for screens with embedded fast update
// Release 607: Refactored peripherals
// Release 607: Improved screens names consistency
// Release 608: Added screen report
// Release 608: Added support for screens with wide temperature
// Release 608: Added support for fast update xE2417PS0Dx xE2417KS0Dx
// Release 609: Added temperature management
// Release 609: Deprecated partial update
// Release 610: Removed partial update
// Release 611: Added check on fast screens
// Release 613: Added x- and y-axis margins
//

// Library header
#include "Screen_EPD_EXT3.h"

#if (SRAM_MODE == USE_EXTERNAL_SPI)
Memory myBufferSRAM(MEMORY_SRAM, BUS_24_BITS);
#endif

///
/// @brief SPI debugging for screen
/// @details 1 = activated, otherwise 0; recommended = 0
///
#define DEBUG_SPI_SCREEN 0

//
// === COG section
//
/// @cond

//
// Use of delay in µs before and after SPI
// 0 = no, 1..500 = yes; recommended = 1
#define USE_DELAY_US 1

// Common settings
// 0x00, soft-reset, temperature, active temperature, PSR0, PSR1
uint8_t indexE5_data[] = {0x19}; // Temperature 0x19 = 25 °C
uint8_t indexE0_data[] = {0x02}; // Activate temperature
uint8_t index00_data[] = {0xff, 0x8f}; // PSR, constant
uint8_t index50a_data[] = {0x27}; // Only 154 213 266 and 370 screens, constant
uint8_t index50b_data[] = {0x07}; // Only 154 213 266 and 370 screens, constant
uint8_t index50c_data[] = {0x07}; // All screens, constant

void Screen_EPD_EXT3_Fast::COG_initial(uint8_t updateMode)
{
    // Work settings
    uint8_t indexE0_work[1]; // Activate temperature
    uint8_t indexE5_work[1]; // Temperature
    uint8_t index00_work[2]; // PSR

    indexE0_work[0] = indexE0_data[0];
    if ((_codeExtra & FEATURE_FAST) and (updateMode != UPDATE_GLOBAL)) // Specific settings for fast update
    {
        indexE5_work[0] = indexE5_data[0] | 0x40; // temperature | 0x40
        index00_work[0] = index00_data[0] | 0x10; // PSR0 | 0x10
        index00_work[1] = index00_data[1] | 0x02; // PSR1 | 0x02
    }
    else // Common settings
    {
        indexE5_work[0] = indexE5_data[0]; // Temperature
        index00_work[0] = index00_data[0]; // PSR0
        index00_work[1] = index00_data[1]; // PSR1
    } // _codeExtra updateMode

    // New algorithm
    uint8_t index00_reset[] = {0x0e};
    _sendIndexData(0x00, index00_reset, 1); // Soft-reset
    _waitBusy();

    _sendIndexData(0xe5, indexE5_work, 1); // Input Temperature: 25C
    _sendIndexData(0xe0, indexE0_work, 1); // Activate Temperature
    _sendIndexData(0x00, index00_work, 2); // PSR

    // Specific settings for fast update, all screens
    if ((_codeExtra & FEATURE_FAST) and (updateMode != UPDATE_GLOBAL))
    {
        uint8_t index50c_work[1]; // Vcom
        index50c_work[0] = index50c_data[0]; // 0x07
        _sendIndexData(0x50, index50c_work, 1); // Vcom and data interval setting
    }

    // Additional settings for fast update, 154 213 266 and 370 screens (_flag50)
    if ((_codeExtra & FEATURE_FAST) and (updateMode != UPDATE_GLOBAL) and _flag50)
    {
        uint8_t index50a_work[1]; // Vcom
        index50a_work[0] = index50a_data[0]; // 0x27
        _sendIndexData(0x50, index50a_work, 1); // Vcom and data interval setting
    }
}

void Screen_EPD_EXT3_Fast::COG_getUserData()
{
    uint16_t _codeSizeType = _eScreen_EPD_EXT3 & 0xffff;

    // Size cSize cType Driver
    switch (_codeSizeType)
    {
        case 0x150C: // 1.54” = 0xcf, 0x02
        case 0x210E: // 2.13” = 0xcf, 0x02
        case 0x260C: // 2.66” = 0xcf, 0x02

            index00_data[0] = 0xcf;
            index00_data[1] = 0x02;
            _flag50 = true;
            break;

        case 0x2709: // 2.71” = 0xcf, 0x8d

            index00_data[0] = 0xcf;
            index00_data[1] = 0x8d;
            _flag50 = false;
            break;

        case 0x2809: // 2.87” = 0xcf, 0x8d

            index00_data[0] = 0xcf;
            index00_data[1] = 0x8d;
            _flag50 = false;
            break;

        case 0x370C: // 3.70” = 0xcf, 0x0f

            index00_data[0] = 0xcf;
            index00_data[1] = 0x8f;
            _flag50 = true;
            break;

        case 0x410D:// 4.17” = 0x0f, 0x0e

            index00_data[0] = 0x0f;
            index00_data[1] = 0x0e;
            _flag50 = false;
            break;

        case 0x430C: // 4.37” = 0x0f, 0x0e

            index00_data[0] = 0x0f;
            index00_data[1] = 0x0e;
            _flag50 = true;
            break;

        case 0x580B:

            break;

        default:

            break;
    }
}

void Screen_EPD_EXT3_Fast::COG_sendImageDataGlobal()
{
#if (SRAM_MODE == USE_INTERNAL_MCU)

    uint8_t * nextBuffer = _newImage; // Also called blackBuffer
    uint8_t * previousBuffer = _newImage + _pageColourSize;

    _sendIndexData(0x10, nextBuffer, _frameSize); // First frame, blackBuffer
    _sendIndexFixed(0x13, 0x00, _frameSize); // Second frame, redBuffer
    memcpy(previousBuffer, nextBuffer, _frameSize); // Copy displayed next to previous

#elif (SRAM_MODE == USE_EXTERNAL_SPI)

    uint32_t nextBuffer = _newImage;
    uint32_t previousBuffer = _newImage + _pageColourSize;

    _sendIndexDataSRAM(0x10, nextBuffer, _frameSize); // First frame
    _sendIndexFixed(0x13, 0x00, _frameSize); // Second frame
    myBufferSRAM.copyMemory(previousBuffer, nextBuffer, _frameSize); // Copy displayed next to previous

#endif // SRAM_MODE
}

void Screen_EPD_EXT3_Fast::COG_sendImageDataFast()
{
#if (SRAM_MODE == USE_INTERNAL_MCU)

    uint8_t * nextBuffer = _newImage;
    uint8_t * previousBuffer = _newImage + _pageColourSize;

    _sendIndexData(0x10, previousBuffer, _frameSize); // Previous frame
    _sendIndexData(0x13, nextBuffer, _frameSize); // Next frame
    memcpy(previousBuffer, nextBuffer, _frameSize); // Copy displayed next to previous

#elif (SRAM_MODE == USE_EXTERNAL_SPI)

    uint32_t nextBuffer = _newImage;
    uint32_t previousBuffer = _newImage + _pageColourSize;

    _sendIndexDataSRAM(0x10, previousBuffer, _frameSize); // Second frame
    _sendIndexDataSRAM(0x13, nextBuffer, _frameSize); // First frame
    myBufferSRAM.copyMemory(previousBuffer, nextBuffer, _frameSize); // Copy displayed next to previous

#endif // SRAM_MODE
}

void Screen_EPD_EXT3_Fast::COG_update(uint8_t updateMode)
{
    // Specific settings for fast update, 154 213 266 and 370 screens (_flag50)
    if ((_codeExtra & FEATURE_FAST) and (updateMode != UPDATE_GLOBAL) and _flag50)
    {
        uint8_t index50b_work[1]; // Vcom
        index50b_work[0] = index50b_data[0]; // 0x07
        _sendIndexData(0x50, index50b_work, 1); // Vcom and data interval setting
    }

    _sendCommand8(0x04); // Power on
    hV_HAL_GPIO_set(_pin.panelCS); // CS# = 1
    _waitBusy();

    _sendCommand8(0x12); // Display Refresh
    hV_HAL_GPIO_set(_pin.panelCS); // CS# = 1
    _waitBusy();
}

void Screen_EPD_EXT3_Fast::COG_powerOff()
{
    _sendCommand8(0x02); // Turn off DC/DC
    hV_HAL_GPIO_set(_pin.panelCS); // CS# = 1
    _waitBusy();
}
/// @endcond
//
// === End of COG section
//

// Utilities

// Class
// Option 1
Screen_EPD_EXT3_Fast::Screen_EPD_EXT3_Fast(eScreen_EPD_EXT3_t eScreen_EPD_EXT3, pins_t board)
{
    _eScreen_EPD_EXT3 = eScreen_EPD_EXT3;
    _pin = board;
    _newImage = 0; // nullptr
}

void Screen_EPD_EXT3_Fast::begin()
{
    begin(_eScreen_EPD_EXT3);
}

// Option 2
#if (SRAM_MODE == USE_INTERNAL_MCU)

Screen_EPD_EXT3_Fast::Screen_EPD_EXT3_Fast(eScreen_EPD_EXT3_t eScreen_EPD_EXT3, pins_t board, uint8_t * frameBuffer)
{
    _pin = board;
    _newImage = (uint8_t *)frameBuffer;
    _eScreen_EPD_EXT3 = eScreen_EPD_EXT3;
}

#endif // SRAM_MODE

void Screen_EPD_EXT3_Fast::begin(eScreen_EPD_EXT3_t eScreen_EPD_EXT3)
{
    _eScreen_EPD_EXT3 = eScreen_EPD_EXT3;
    _codeExtra = (_eScreen_EPD_EXT3 >> 16) & 0xff;
    _codeSize = (_eScreen_EPD_EXT3 >> 8) & 0xff;
    _codeType = _eScreen_EPD_EXT3 & 0xff;
    _screenColourBits = 2; // BWR and BWRY

    // Checks
    if (not(_codeExtra & FEATURE_FAST))
    {
        hV_HAL_Serial_crlf();
        hV_HAL_log(LEVEL_CRITICAL, "No embedded fast update");
        hV_HAL_exit(0x01);
    }

    switch (_codeSize)
    {
        case 0x15: // 1.54"

            _screenSizeV = 152; // vertical = wide size
            _screenSizeH = 152; // horizontal = small size
            _screenDiagonal = 154;
            break;

        case 0x21: // 2.13"

            _screenSizeV = 212; // vertical = wide size
            _screenSizeH = 104; // horizontal = small size
            _screenDiagonal = 213;
            break;

        case 0x26: // 2.66"

            _screenSizeV = 296; // vertical = wide size
            _screenSizeH = 152; // horizontal = small size
            _screenDiagonal = 266;
            break;

        case 0x27: // 2.71" and 2.71"-Touch

            _screenSizeV = 264; // vertical = wide size
            _screenSizeH = 176; // horizontal = small size
            _screenDiagonal = 271;
            break;

        case 0x28: // 2.87"

            _screenSizeV = 296; // vertical = wide size
            _screenSizeH = 128; // horizontal = small size
            _screenDiagonal = 287;
            break;

        case 0x37: // 3.70" and 3.70"-Touch

            _screenSizeV = 416; // vertical = wide size
            _screenSizeH = 240; // horizontal = small size
            _screenDiagonal = 370;
            break;

        case 0x41: // 4.17"

            _screenSizeV = 300; // vertical = wide size
            _screenSizeH = 400; // horizontal = small size
            _screenDiagonal = 417;
            break;

        case 0x43: // 4.37"

            _screenSizeV = 480; // vertical = wide size
            _screenSizeH = 176; // horizontal = small size
            _screenDiagonal = 437;
            break;

        case 0x56: // 5.65"

            _screenSizeV = 600; // v = wide size
            _screenSizeH = 448; // h = small size
            _screenDiagonal = 565;
            break;

        case 0x58: // 5.81"

            _screenSizeV = 720; // v = wide size
            _screenSizeH = 256; // h = small size
            _screenDiagonal = 581;
            break;

        case 0x74: // 7.40"

            _screenSizeV = 800; // v = wide size
            _screenSizeH = 480; // h = small size
            _screenDiagonal = 741;
            break;

        case 0x96: // 9.69"

            _screenSizeV = 672; // v = wide size
            _screenSizeH = 960; // Actually, 960 = 480 x 2, h = small size
            _screenDiagonal = 969;
            break;

        case 0xB9: // 11.98"

            _screenSizeV = 768; // v = wide size
            _screenSizeH = 960; // Actually, 960 = 480 x 2, h = small size
            _screenDiagonal = 1198;
            break;

        default:

            break;
    } // _codeSize
    
    hV_HAL_log(LEVEL_DEBUG, "_screenSizeV %d, _screenSizeH %d, _screenDiagonal %d", _screenSizeV, _screenSizeH, _screenDiagonal);

    _screenMarginV = 0;
    _screenMarginH = 0;

    // Check panelCSS for large screens
    if (((_codeSize == 0x96) or (_codeSize == 0xB9)) and (_pin.panelCSS == NOT_CONNECTED))
    {
        hV_HAL_Serial_crlf();
        hV_HAL_log(LEVEL_CRITICAL, "Required pin panelCSS is NOT_CONNECTED");
        hV_HAL_exit(0x01);
    }

    _bufferDepth = _screenColourBits; // 2 colours
    _bufferSizeV = _screenSizeV; // vertical = wide size
    _bufferSizeH = _screenSizeH / 8; // horizontal = small size 112 / 8;

    // Force conversion for two unit16_t multiplication into uint32_t.
    // Actually for 1 colour; BWR requires 2 pages.
    _pageColourSize = (uint32_t)_bufferSizeV * (uint32_t)_bufferSizeH;
    hV_HAL_log(LEVEL_DEBUG, "_bufferDepth %d, _pageColourSize %d, SRAM_MODE %d ", _bufferDepth, _pageColourSize, SRAM_MODE);

    // _frameSize = _pageColourSize, except for 9.69 and 11.98
    // 9.69 and 11.98 combine two half-screens, hence two frames with adjusted size
    switch (_codeSize)
    {
        case 0x96: // 9.69"
        case 0xB9: // 11.98"

            _frameSize = _pageColourSize / 2;
            hV_HAL_log(LEVEL_DEBUG, "_frameSize %d", _frameSize);
            break;

        default:

            _frameSize = _pageColourSize;
            hV_HAL_log(LEVEL_DEBUG, "_frameSize %d", _frameSize);
            break;
    } // _codeSize

#if (SRAM_MODE == USE_INTERNAL_MCU)

    hV_HAL_log(LEVEL_DEBUG, "Here 1");
    if (_newImage == 0)
    {
//        hV_HAL_log(LEVEL_DEBUG, "Here 2");
        static uint8_t * _newFrameBuffer;
//        hV_HAL_log(LEVEL_DEBUG, "Here 3");
        _newFrameBuffer = new uint8_t[_pageColourSize * _bufferDepth];
        
//        hV_HAL_log(LEVEL_DEBUG, "Here 4");
        _newImage = (uint8_t *) _newFrameBuffer;
       
    }

#endif // SRAM_MODE

#if (SRAM_MODE == USE_INTERNAL_MCU)

    // Check FRAM
    bool flagFRAM = true;
    uint8_t count = 8;

        hV_HAL_log(LEVEL_DEBUG, "Here 6");
    _newImage[1] = 0x00;
    while (flagFRAM)
    {
        _newImage[1] = 0xaa;
        hV_HAL_delayMilliseconds(100);
        count--;
        if ((_newImage[1] == 0xaa) or (count == 0))
        {
            flagFRAM = false;
        }
    }

    if (count == 0)
    {
        hV_HAL_Serial_crlf();
        hV_HAL_log(LEVEL_CRITICAL, "FRAM not initialised");
        hV_HAL_exit(1);
    }
    memset(_newImage, 0x00, _pageColourSize * _bufferDepth);

#endif // SRAM_MODE

    // Turn SPI on, initialise GPIOs and set GPIO levels
    // Reset panel and get tables
    resume();

    // _screenSizeH = _screenSizeH;
    // _screenSizeV = _screenSizeV;

    // Standard
    // hV_Screen_Buffer::begin();
#if (FONT_MODE == USE_FONT_FLASH)

    _f_begin(_pin.flashCS);

#else

    _f_begin();

#endif // FONT_MODE

#if (SRAM_MODE == USE_EXTERNAL_SPI)

    myBufferSRAM = Memory(MEMORY_SRAM, BUS_24_BITS);

    myBufferSRAM.begin(_pin.flashCSS);
    myBufferSRAM.reset(); // Just in case
    myBufferSRAM.test(); // Detect with or with dummy read
    _newImage = 0;
    myBufferSRAM.setMemory(_newImage, 0x00, _pageColourSize * _bufferDepth);

#endif // SRAM_MODE

    setOrientation(0);
    if (_f_fontMax() > 0)
    {
        _f_selectFont(0);
    }
    _f_fontSolid = false;

    _penSolid = false;
    _invert = false;
    _flagRead = true;
    _flagEnergy = false;
    _flagIntensity = false;

    // Report
    hV_HAL_log(LEVEL_DEBUG, "Screen %s %ix%i", WhoAmI(), screenSizeX(), screenSizeY());
    hV_HAL_log(LEVEL_DEBUG, "PDLS v%i", SCREEN_EPD_EXT3_RELEASE);

    //
    // === Touch section
    //
    _touchTrim = 0x00; // no touch
    _touchEvent = false;
    //
    // === End of Touch section
    //

    setTemperatureC(25); // 25 Celsius = 77 Fahrenheit
}

void Screen_EPD_EXT3_Fast::suspend()
{
    hV_HAL_SPI_end();
    // hV_HAL_Wire_end();

    hV_HAL_GPIO_clear(_pin.panelReset); // ClrGPIOBit(PANEL_RESET_PIN >> 4, PANEL_RESET_PIN & 0xf);
    hV_HAL_GPIO_clear(_pin.panelCS); // CS# = 1
    hV_HAL_GPIO_clear(_pin.panelDC); // DC = LOW

    if (_pin.flashCS != NOT_CONNECTED) // generic
    {
        hV_HAL_GPIO_clear(_pin.flashCS);
    }
    if (_pin.flashCSS != NOT_CONNECTED) // generic
    {
        hV_HAL_GPIO_clear(_pin.flashCSS);
    }

    if (_pin.panelCSS != NOT_CONNECTED) // generic
    {
        hV_HAL_GPIO_clear(_pin.panelCSS);
    }
    if (_pin.cardCS != NOT_CONNECTED) // generic
    {
        hV_HAL_GPIO_clear(_pin.cardCS);
    }

    // Optional power circuit
    if (_pin.panelPower != NOT_CONNECTED) // generic
    {
        hV_HAL_GPIO_clear(_pin.panelPower);
    }
}

void Screen_EPD_EXT3_Fast::resume()
{
    // Initialise GPIO expander
    hV_HAL_GPIO_begin();

    // Optional power circuit
    if (_pin.panelPower != NOT_CONNECTED) // generic
    {
        hV_HAL_GPIO_define(_pin.panelPower, OUTPUT);
        hV_HAL_GPIO_set(_pin.panelPower);
    }

    // Configure GPIOs
    hV_HAL_GPIO_define(_pin.panelBusy, INPUT);

    hV_HAL_GPIO_define(_pin.panelDC, OUTPUT);
    hV_HAL_GPIO_set(_pin.panelDC);

    hV_HAL_GPIO_define(_pin.panelReset, OUTPUT);
    hV_HAL_GPIO_set(_pin.panelReset);

    hV_HAL_GPIO_define(_pin.panelCS, OUTPUT);
    hV_HAL_GPIO_set(_pin.panelCS); // CS# = 1

    if (_pin.panelCSS != NOT_CONNECTED) // generic
    {
        hV_HAL_GPIO_define(_pin.panelCSS, OUTPUT);
        hV_HAL_GPIO_set(_pin.panelCSS);
    }

    // External SPI memory
    if (_pin.flashCS != NOT_CONNECTED) // generic
    {
        hV_HAL_GPIO_define(_pin.flashCS, OUTPUT);
        hV_HAL_GPIO_set(_pin.flashCS);
    }
    if (_pin.flashCSS != NOT_CONNECTED) // generic
    {
        hV_HAL_GPIO_define(_pin.flashCSS, OUTPUT);
        hV_HAL_GPIO_set(_pin.flashCSS);
    }

    // External SD card
    if (_pin.cardCS != NOT_CONNECTED) // generic
    {
        hV_HAL_GPIO_define(_pin.cardCS, OUTPUT);
        hV_HAL_GPIO_set(_pin.cardCS);
    }
    if (_pin.cardDetect != NOT_CONNECTED) // generic
    {
        hV_HAL_GPIO_define(_pin.cardCS, INPUT);
    }

    // Reset
    switch (_codeSize)
    {
        case 0x56: // 5.65"
        case 0x58: // 5.81"
        case 0x74: // 7.40"

            _reset(200, 20, 200, 50, 5); // medium
            break;

        case 0x96: // 9.69"
        case 0xB9: // 11.98"

            _reset(200, 20, 200, 200, 5); // large
            break;

        default:

            _reset(5, 5, 10, 5, 5); // small
            break;
    } // _codeSize

    // Check type and get tables
    COG_getUserData(); // nothing sent to panel

    // Start SPI and Wire
    // hV_HAL_SPI_configure();
    // hV_HAL_SPI_begin(); // Standard 8 MHz
    // hV_HAL_Wire_begin();
}

void Screen_EPD_EXT3_Fast::_reset(uint32_t ms1, uint32_t ms2, uint32_t ms3, uint32_t ms4, uint32_t ms5)
{
    hV_HAL_delayMilliseconds(ms1); // delay_ms 5ms
    hV_HAL_GPIO_set(_pin.panelReset); // RES# = 1
    hV_HAL_delayMilliseconds(ms2); // delay_ms 5ms
    hV_HAL_GPIO_clear(_pin.panelReset);
    hV_HAL_delayMilliseconds(ms3);
    hV_HAL_GPIO_set(_pin.panelReset);
    hV_HAL_delayMilliseconds(ms4);
    hV_HAL_GPIO_set(_pin.panelCS); // CS# = 1
    hV_HAL_delayMilliseconds(ms5);
}

#if (STRING_MODE == USE_STRING_OBJECT)

String Screen_EPD_EXT3_Fast::WhoAmI()

#elif (STRING_MODE == USE_CHAR_ARRAY)

char * Screen_EPD_EXT3_Fast::WhoAmI()

#endif // STRING_MODE
{
    char cSRAM = '.';

#if (SRAM_MODE == USE_INTERNAL_MCU)

    cSRAM = 'i';

#elif (SRAM_MODE == USE_EXTERNAL_SPI)

    cSRAM = 'e';

#endif // SRAM_MODE

    char cFont = '?';

#if (FONT_MODE == USE_FONT_HEADER)

    cFont = 'H';

#elif (FONT_MODE == USE_FONT_FLASH)

    cFont = 'F';

#elif (FONT_MODE == USE_FONT_TERMINAL)

    cFont = 'T';

#endif // FONT_MODE

    char cExtra[8] = {0x00};
    if (_codeExtra > 0)
    {
        strcpy(cExtra, "-");
        if (_codeExtra & FEATURE_FAST)
        {
            strcat(cExtra, "F");
        }
        if (_codeExtra & FEATURE_TOUCH)
        {
            strcat(cExtra, "T");
        }
        if (_codeExtra & FEATURE_OTHER)
        {
            strcat(cExtra, "b");
        }
        if (_codeExtra & FEATURE_WIDE_TEMPERATURE)
        {
            strcat(cExtra, "W");
        }
        if (_codeExtra & FEATURE_RED)
        {
            strcat(cExtra, "BWR");
        }
    }

    return formatString("iTC %i.%i\"%s -%c%c", _screenDiagonal / 100, _screenDiagonal % 100, cExtra, cSRAM, cFont);
}

void Screen_EPD_EXT3_Fast::flush()
{
    flushMode(UPDATE_GLOBAL);
}

void Screen_EPD_EXT3_Fast::_flushGlobal()
{
    // Initialise
    COG_initial(UPDATE_GLOBAL);

    // Send image data
    COG_sendImageDataGlobal();

    // Update
    COG_update(UPDATE_GLOBAL);
    COG_powerOff();
}

void Screen_EPD_EXT3_Fast::flushFast()
{
    flushMode(UPDATE_FAST);
}

void Screen_EPD_EXT3_Fast::_flushFast()
{
    // Configure
    COG_initial(UPDATE_FAST);

    // Send image data
    COG_sendImageDataFast();

    // Update
    COG_update(UPDATE_FAST);
    COG_powerOff();
}

void Screen_EPD_EXT3_Fast::clear(uint16_t colour)

#if (SRAM_MODE == USE_INTERNAL_MCU)

{
    if (colour == myColours.grey)
    {
        for (uint16_t i = 0; i < _bufferSizeV; i++)
        {
            uint16_t pattern = (i % 2) ? 0b10101010 : 0b01010101;
            for (uint16_t j = 0; j < _bufferSizeH; j++)
            {
                _newImage[i * _bufferSizeH + j] = pattern;
            }
        }
    }
    else if ((colour == myColours.white) xor _invert)
    {
        // physical black 00
        memset(_newImage, 0x00, _pageColourSize);
    }
    else
    {
        // physical white 10
        memset(_newImage, 0xff, _pageColourSize);
    }
}

#elif (SRAM_MODE == USE_EXTERNAL_SPI)

{
    if (colour == myColours.grey)
    {
        for (uint16_t i = 0; i < _bufferSizeV; i++)
        {
            uint8_t pattern = (i % 2) ? 0b10101010 : 0b01010101;
            for (uint16_t j = 0; j < _bufferSizeH; j++)
            {
                myBufferSRAM.set(i * _bufferSizeH + j, pattern);
            }
        }
    }
    else if ((colour == myColours.white) xor _invert)
    {
        // physical black 00
        myBufferSRAM.setMemory(_newImage, 0x00, _pageColourSize);
    }
    else
    {
        // physical white 10
        myBufferSRAM.setMemory(_newImage, 0xff, _pageColourSize);
    }
}
#endif // SRAM_MODE

void Screen_EPD_EXT3_Fast::regenerate(uint8_t mode)
{
    if (mode == UPDATE_GLOBAL)
    {
        clear();
        flush();
    }
    else
    {
        clear(myColours.black);
        flushFast();
        hV_HAL_delayMilliseconds(100);

        clear(myColours.white);
        flushFast();
        hV_HAL_delayMilliseconds(100);
    }
}


uint8_t Screen_EPD_EXT3_Fast::ScreenBufferUpdated()
{
    uint8_t * nextBuffer = _newImage;
    uint8_t * previousBuffer = _newImage + _pageColourSize;
    if(memcmp(previousBuffer, nextBuffer, _frameSize) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void Screen_EPD_EXT3_Fast::invert(bool flag)
{
    _invert = flag;
}

void Screen_EPD_EXT3_Fast::_setPoint(uint16_t x1, uint16_t y1, uint16_t colour)
{
    // Orient and check coordinates are within screen
    // _orientCoordinates() returns false = success, true = error
    if (_orientCoordinates(x1, y1))
    {
        return;
    }

    uint32_t z1 = _getZ(x1, y1);

    // Convert combined colours into basic colours
    bool flagOdd = ((x1 + y1) % 2 == 0);

    if (colour == myColours.grey)
    {
        if (flagOdd)
        {
            colour = myColours.black; // black
        }
        else
        {
            colour = myColours.white; // white
        }
    }

    // Basic colours
#if (SRAM_MODE == USE_INTERNAL_MCU)

    if ((colour == myColours.white) xor _invert)
    {
        // physical black 00
        bitClear(_newImage[z1], 7 - (y1 % 8));
    }
    else if ((colour == myColours.black) xor _invert)
    {
        // physical white 10
        bitSet(_newImage[z1], 7 - (y1 % 8));
    }

#elif (SRAM_MODE == USE_EXTERNAL_SPI)

    if ((colour == myColours.white) xor _invert)
    {
        // physical black 00
        myBufferSRAM.clearBit(_newImage + z1, 7 - (y1 % 8));
    }
    else if ((colour == myColours.black) xor _invert)
    {
        // physical white 10
        myBufferSRAM.setBit(_newImage + z1, 7 - (y1 % 8));
    }

#endif // SRAM_MODE
}

void Screen_EPD_EXT3_Fast::_setOrientation(uint8_t orientation)
{
    _orientation = orientation % 4;
}

bool Screen_EPD_EXT3_Fast::_orientCoordinates(uint16_t & x, uint16_t & y)
{
    bool flagError = true; // false = success, true = error
    // Origin + _screenMarginXY, logical screen
    x += screenMarginX();
    y += screenMarginY();
    // Final - _screenMarginHV, physical screen
    uint16_t _screenSizeVmargin = _screenSizeV - _screenMarginV;
    uint16_t _screenSizeHmargin = _screenSizeH - _screenMarginH;

    switch (_orientation)
    {
        case 3: // checked, previously 1

            if ((x < _screenSizeVmargin) and (y < _screenSizeHmargin))
            {
                x = _screenSizeV - 1 - x;
                flagError = false;
            }
            break;

        case 2: // checked

            if ((x < _screenSizeHmargin) and (y < _screenSizeVmargin))
            {
                x = _screenSizeH - 1 - x;
                y = _screenSizeV - 1 - y;
                swap(x, y);
                flagError = false;
            }
            break;

        case 1: // checked, previously 3

            if ((x < _screenSizeVmargin) and (y < _screenSizeHmargin))
            {
                y = _screenSizeH - 1 - y;
                flagError = false;
            }
            break;

        default: // checked

            if ((x < _screenSizeHmargin) and (y < _screenSizeVmargin))
            {
                swap(x, y);
                flagError = false;
            }
            break;
    }

    return flagError;
}

uint32_t Screen_EPD_EXT3_Fast::_getZ(uint16_t x1, uint16_t y1)
{
    uint32_t z1 = 0;
    // According to 11.98 inch Spectra Application Note
    // at http:// www.pervasivedisplays.com/LiteratureRetrieve.aspx?ID=245146

    z1 = (uint32_t)x1 * _bufferSizeH + (y1 >> 3);

    return z1;
}

uint16_t Screen_EPD_EXT3_Fast::_getPoint(uint16_t x1, uint16_t y1)
{
    // Orient and check coordinates are within screen
    // _orientCoordinates() returns false = success, true = error
    if (_orientCoordinates(x1, y1))
    {
        return 0;
    }

    uint16_t _result = 0;
    uint8_t _value = 0;

    uint32_t z1 = _getZ(x1, y1);
    uint8_t b1 = 7 - (y1 % 8);

#if (SRAM_MODE == USE_INTERNAL_MCU)

    _value = bitRead(_newImage[z1], b1);

#elif (SRAM_MODE == USE_EXTERNAL_SPI)

    _value = myBufferSRAM.readBit(_newImage + z1, b1);

#endif // SRAM_MODE

    _value <<= 4;
    _value &= 0b11110000;

    // red = 0-1, black = 1-0, white 0-0
    switch (_value)
    {
        case 0x10:

            _result = myColours.black;
            break;

        default:

            _result = myColours.white;
            break;
    }

    return _result;
}

void Screen_EPD_EXT3_Fast::point(uint16_t x1, uint16_t y1, uint16_t colour)
{
    _setPoint(x1, y1, colour);
}

void Screen_EPD_EXT3_Fast::copyPasteArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t dx, uint16_t dy)
{
    uint16_t buffer;

    for (uint16_t y = 0; y < dy; y++)
    {
        for (uint16_t x = 0; x < dx; x++)
        {
            buffer = _getPoint(x1 + x, y1 + y);
            _setPoint(x2 + x, y2 + y, buffer);
        }
    }
}

uint16_t Screen_EPD_EXT3_Fast::readPixel(uint16_t x1, uint16_t y1)
{
    return _getPoint(x1, y1);
}

// Utilities
void Screen_EPD_EXT3_Fast::_sendCommand8(uint8_t command)
{
#if (DEBUG_SPI_SCREEN > 0)

    hV_HAL_log(LEVEL_DEBUG, "%24s [%02x] ", "_sendCommand8", command);

#endif // DEBUG_SPI_SCREEN

    hV_HAL_GPIO_clear(_pin.panelDC);
    hV_HAL_GPIO_clear(_pin.panelCS);

    hV_HAL_SPI_transfer(command);

    hV_HAL_GPIO_set(_pin.panelCS);
}

void Screen_EPD_EXT3_Fast::_waitBusy()
{
    // LOW = busy, HIGH = ready
    hV_HAL_GPIO_waitFor(_pin.panelBusy, HIGH);
}

void Screen_EPD_EXT3_Fast::_sendIndexFixed(uint8_t index, uint8_t data, uint32_t size)
{
#if (DEBUG_SPI_SCREEN > 0)

    hV_HAL_log(LEVEL_DEBUG, "%24s [%02x] %02x", "_sendIndexFixed", index, data);

#endif // DEBUG_SPI_SCREEN

    hV_HAL_GPIO_clear(_pin.panelDC); // DC Low = Command
    hV_HAL_GPIO_clear(_pin.panelCS); // CS Low = Select

    hV_HAL_delayMicroseconds(50);
    hV_HAL_SPI_transfer(index);
    hV_HAL_delayMicroseconds(50);

    hV_HAL_GPIO_set(_pin.panelCS); // CS High = Unselect
    hV_HAL_GPIO_set(_pin.panelDC); // DC High = Data
    hV_HAL_GPIO_clear(_pin.panelCS); // CS Low = Select

    hV_HAL_delayMicroseconds(50);
    for (uint32_t i = 0; i < size; i++)
    {
        hV_HAL_SPI_transfer(data); // _sendIndexFixed
    }
    hV_HAL_delayMicroseconds(50);

    hV_HAL_GPIO_set(_pin.panelCS); // CS High = Unselect
}

void Screen_EPD_EXT3_Fast::_sendIndexData(uint8_t index, const uint8_t * data, uint32_t size)
{
#if (DEBUG_SPI_SCREEN > 0)

    // hV_HAL_Serial_printf("hV - %24s [%02x] ", "_sendIndexData", index);
    hV_HAL_log(LEVEL_DEBUG | WITH_NO_CRLF, "%24s [%02x] ", "_sendIndexData", index);
    for (uint32_t i = 0; i < min(size, (uint32_t)8); i++)
    {
        hV_HAL_Serial_printf("%02x ", data[i]);
    }
    if (size >= 8)
    {
        hV_HAL_Serial_printf("...");
    }
    hV_HAL_Serial_crlf();

#endif // DEBUG_SPI_SCREEN

    hV_HAL_GPIO_clear(_pin.panelDC); // DC Low = Command
    hV_HAL_GPIO_clear(_pin.panelCS); // CS Low = Select

#if (USE_DELAY_US > 0)
    hV_HAL_delayMicroseconds(USE_DELAY_US);
#endif // USE_DELAY_US
    hV_HAL_SPI_transfer(index);
#if (USE_DELAY_US > 0)
    hV_HAL_delayMicroseconds(USE_DELAY_US);
#endif // USE_DELAY_US

    // hV_HAL_GPIO_set(_pin.panelCS); // CS High = Unselect
    hV_HAL_GPIO_set(_pin.panelDC); // DC High = Data
    // hV_HAL_GPIO_clear(_pin.panelCS); // CS Low = Select

#if (USE_DELAY_US > 0)
    hV_HAL_delayMicroseconds(USE_DELAY_US);
#endif // USE_DELAY_US
    for (uint32_t i = 0; i < size; i++)
    {
        hV_HAL_SPI_transfer(data[i]);
    }
#if (USE_DELAY_US > 0)
    hV_HAL_delayMicroseconds(USE_DELAY_US);
#endif // USE_DELAY_US

    hV_HAL_GPIO_set(_pin.panelCS); // CS High = Unselect
}

#if (SRAM_MODE == USE_EXTERNAL_SPI)

void Screen_EPD_EXT3_Fast::_sendIndexDataSRAM(uint8_t index, uint32_t data, uint32_t size)
{
#if (DEBUG_SPI_SCREEN > 0)

    hV_HAL_log(LEVEL_DEBUG, "%24s [%02x] ...", "_sendIndexDataSRAM", index);

#endif // DEBUG_SPI_SCREEN

    const uint16_t segment = 256;
    uint8_t buffer[256];
    uint16_t bytes = 0;

    // Send command
    hV_HAL_GPIO_clear(_pin.panelDC); // DC Low = Command
    hV_HAL_GPIO_clear(_pin.panelCS); // CS Low = Select

#if (USE_DELAY_US > 0)
    hV_HAL_delayMicroseconds(USE_DELAY_US);
#endif // USE_DELAY_US
    hV_HAL_SPI_transfer(index);
#if (USE_DELAY_US > 0)
    hV_HAL_delayMicroseconds(USE_DELAY_US);
#endif // USE_DELAY_US

    hV_HAL_GPIO_set(_pin.panelDC); // DC High = Data
    hV_HAL_GPIO_set(_pin.panelCS); // CS High = Unselect

    // Send data
    for (uint32_t indexBuffer = 0; indexBuffer < size; indexBuffer += 256)
    {
        // Read from SRAM
        bytes = segment;
        if (indexBuffer + segment > size)
        {
            bytes = size - indexBuffer;
        }
        myBufferSRAM.read(&buffer, data + indexBuffer, bytes);

        // Write to panel
        hV_HAL_GPIO_clear(_pin.panelCS); // CS Low = Select

#if (USE_DELAY_US > 0)
        hV_HAL_delayMicroseconds(USE_DELAY_US);
#endif // USE_DELAY_US
        for (uint32_t i = 0; i < bytes; i++)
        {
            hV_HAL_SPI_transfer(buffer[i]);
        }
#if (USE_DELAY_US > 0)
        hV_HAL_delayMicroseconds(USE_DELAY_US);
#endif // USE_DELAY_US

        hV_HAL_GPIO_set(_pin.panelCS); // CS High = Unselect
    }
}

#endif // USE_EXTERNAL_SPI

//
// === Touch section
//

//
// === End of Touch section
//

//
// === Miscellaneous section
//
pins_t Screen_EPD_EXT3_Fast::getBoardPins()
{
    return _pin;
}
//
// === End of Miscellaneous section
//

//
// === Temperature section
//
void Screen_EPD_EXT3_Fast::setTemperatureC(int8_t temperatureC)
{
    _temperature = temperatureC;

    uint8_t _temperature2;
    if (_temperature < 0)
    {
        _temperature2 = -_temperature;
        _temperature2 = (uint8_t)(~_temperature2) + 1; // 2's complement
    }
    else
    {
        _temperature2 = _temperature;
    }
    indexE5_data[0] = _temperature2;
}

void Screen_EPD_EXT3_Fast::setTemperatureF(int16_t temperatureF)
{
    int8_t temperatureC = ((temperatureF - 32) * 5) / 9; // C = (F - 32) * 5 / 9
    setTemperatureC(temperatureC);
}

uint8_t Screen_EPD_EXT3_Fast::checkTemperatureMode(uint8_t updateMode)
{
    // #define FEATURE_FAST 0x01 ///< With embedded fast update
    // #define FEATURE_TOUCH 0x02 ///< With capacitive touch panel
    // #define FEATURE_OTHER 0x04 ///< With other feature
    // #define FEATURE_WIDE_TEMPERATURE 0x08 ///< With wide operating temperature
    // #define FEATURE_RED 0x10 ///< With red colour
    // #define FEATURE_RED_YELLOW 0x20 ///< With red and yellow colours

    switch (_codeExtra & 0x19)
    {
        case FEATURE_FAST: // PS series

            // Fast 	PS 	Embedded fast update 	FU: +15 to +30 °C 	GU: 0 to +50 °C
            if (updateMode == UPDATE_FAST) // Fast update
            {
                if ((_temperature < 15) or (_temperature > 30))
                {
                    updateMode = UPDATE_GLOBAL;
                }
            }
            if (updateMode == UPDATE_GLOBAL) // Global update
            {
                if ((_temperature < 0) or (_temperature > 50))
                {
                    updateMode = UPDATE_NONE;
                }
            }
            break;

        case (FEATURE_FAST | FEATURE_WIDE_TEMPERATURE): // KS series

            // Wide 	KS 	Wide temperature and embedded fast update 	FU: 0 to +50 °C 	GU: -15 to +60 °C
            if (updateMode == UPDATE_FAST) // Fast update
            {
                if ((_temperature < 0) or (_temperature > 50))
                {
                    updateMode = UPDATE_GLOBAL;
                }
            }
            if (updateMode == UPDATE_GLOBAL) // Global update
            {
                if ((_temperature < -15) or (_temperature > 60))
                {
                    updateMode = UPDATE_NONE;
                }
            }
            break;

        case FEATURE_WIDE_TEMPERATURE: // HS series

            // Freezer 	HS 	Global update below 0 °C 	FU: - 	GU: -25 to +30 °C
            updateMode = UPDATE_GLOBAL;
            if ((_temperature < -25) or (_temperature > 30))
            {
                updateMode = UPDATE_NONE;
            }
            break;

        // case FEATURE_RED: // JS series
        case FEATURE_BWR: // JS series
        // case FEATURE_RED_YELLOW: // QS series
        case FEATURE_BWRY: // QS series

            // BWR  JS 	Red colour 	FU: - 	GU: 0 to +40 °C
            // BWRY  QS 	Red and yellow colours 	FU: - 	GU: 0 to +40 °C
            if ((_temperature < 0) or (_temperature > 40))
            {
                updateMode = UPDATE_NONE;
            }
            break;

        default: // CS series

            // Normal 	CS 	Global update above 0 °C 	FU: - 	GU: 0 to +50 °C
            updateMode = UPDATE_GLOBAL;
            if ((_temperature < 0) or (_temperature > 50))
            {
                updateMode = UPDATE_NONE;
            }
            break;
    }

    return updateMode;
}

uint8_t Screen_EPD_EXT3_Fast::flushMode(uint8_t updateMode)
{
    updateMode = checkTemperatureMode(updateMode);

    switch (updateMode)
    {
        case UPDATE_FAST:

            _flushFast();
            break;

        case UPDATE_GLOBAL:

            _flushGlobal();
            break;

        default:

            hV_HAL_log(LEVEL_ERROR, "UPDATE_NONE invoked");
            break;
    }

    return updateMode;
}
//
// === End of Temperature section
//

