//
// hV_Utilities.cpp
// Library C++ code
// ----------------------------------
//
// Project Pervasive Displays Library Suite
// Based on highView technology
//
// Created by Rei Vilo, 01 Jun 2013
//
// Copyright (c) Rei Vilo, 2010-2023
// Licence All rights reserved
//
// See hV_Utilities.h for references
//
// Release 521: Added getDecimalPlaces
// Release 605: Added scaleValue()
// Release 607: Refactored peripherals
// Release 610: Refactored peripherals
// Release 611: Refactored functions
//

// Library header
#include "hV_Utilities.h"
#include "stdarg.h"
#include "stdio.h"

char bufferIn[128];
char bufferOut[128];

// Code
// Utilities

#if (STRING_MODE == USE_STRING_OBJECT)
String formatString(const char * format, ...)
#elif (STRING_MODE == USE_CHAR_ARRAY)
char * formatString(const char * format, ...)
#endif // STRING_MODE
{
    memset(&bufferOut, 0x00, sizeof(bufferOut));
    va_list args;
    va_start(args, format);
    vsnprintf(bufferOut, 127, format, args);
    va_end(args);

#if (STRING_MODE == USE_STRING_OBJECT)
    return String(bufferOut);
#elif (STRING_MODE == USE_CHAR_ARRAY)
    return bufferOut;
#endif // STRING_MODE
}

#if (STRING_MODE == USE_STRING_OBJECT)
String trimString(String text)
#elif (STRING_MODE == USE_CHAR_ARRAY)
char * trimString(const char * text)
#endif // STRING_MODE
{
#if (STRING_MODE == USE_STRING_OBJECT)
    text.toCharArray(bufferIn, text.length() + 1);
#elif (STRING_MODE == USE_CHAR_ARRAY)
    strncpy(bufferIn, text, sizeof(bufferIn));
#endif // STRING_MODE

    memset(&bufferOut, 0x00, sizeof(bufferOut));

    bool flag = true;
    uint8_t index;
    uint8_t start, end;

    // Upwards from start
    index = 0;
    flag = true;
    while ((index < strlen(bufferIn)) and flag)
    {
        if ((bufferIn[index] != '\n') and (bufferIn[index] != '\r') and (bufferIn[index] != ' ') and (bufferIn[index] != '\t'))
        {
            flag = false;
            start = index;
        }
        index++;
    }

    // Downwards from end
    index = strlen(bufferIn);
    flag = true;
    while ((index > 0) and flag)
    {
        if ((bufferIn[index] != '\n') and (bufferIn[index] != '\r') and (bufferIn[index] != ' ') and (bufferIn[index] != '\t'))
        {
            flag = false;
            end = index - 1;
        }
        index--;
    }

    strncpy(bufferOut, bufferIn + start, end - start);

#if (STRING_MODE == USE_STRING_OBJECT)

    return (String)bufferOut;

#elif (STRING_MODE == USE_CHAR_ARRAY)

    return bufferOut;

#endif // STRING_MODE
}

int32_t cos32x100(int32_t degreesX100)
{
    int32_t i = 1;

    if (degreesX100 < 0)
    {
        i = -i;
        degreesX100 = -degreesX100;
    }

    degreesX100 %= 36000;

    if (degreesX100 > 9000)
    {
        if (degreesX100 < 18000)
        {
            i = -i;
            degreesX100 = 18000 - degreesX100;
        }
        else if (degreesX100 < 27000)
        {
            i = -i;
            degreesX100 = degreesX100 - 18000;
        }
        else
        {
            degreesX100 = 36000 - degreesX100;
        }
    }

    if (degreesX100 < 1000)
    {
        return i * scaleValue(degreesX100,    0, 1000, 100, 98);
    }
    else if (degreesX100 < 2000)
    {
        return i * scaleValue(degreesX100, 1000, 2000,  98, 93);
    }
    else if (degreesX100 < 3000)
    {
        return i * scaleValue(degreesX100, 2000, 3000,  93, 86);
    }
    else if (degreesX100 < 4000)
    {
        return i * scaleValue(degreesX100, 3000, 4000,  86, 76);
    }
    else if (degreesX100 < 5000)
    {
        return i * scaleValue(degreesX100, 4000, 5000,  76, 64);
    }
    else if (degreesX100 < 6000)
    {
        return i * scaleValue(degreesX100, 5000, 6000,  64, 50);
    }
    else if (degreesX100 < 7000)
    {
        return i * scaleValue(degreesX100, 6000, 7000,  50, 34);
    }
    else if (degreesX100 < 8000)
    {
        return i * scaleValue(degreesX100, 7000, 8000,  34, 17);
    }
    else
    {
        return i * scaleValue(degreesX100, 8000, 9000,  17,  0);
    }
}

int32_t sin32x100(int32_t degreesX100)
{
    return cos32x100(degreesX100 + 27000);
}

uint8_t getDecimalPlaces(int32_t unit)
{
    uint8_t _decimals = 0;
    while (unit > 1)
    {
        _decimals += 1;
        unit /= 10;
    }
    return (_decimals);
}

/*
    void convertPolar2Rectangle(uint16_t centerX, uint16_t centerY, uint16_t angle, uint16_t radius, uint16_t & rectangularX, uint16_t & rectangularY)
    {
    rectangularX = (uint16_t)(centerX + radius * sin32x100(angle * 100) / 100);
    rectangularY = (uint16_t)(centerY - radius * cos32x100(angle * 100) / 100);
    }

    void convertRectangle2Polar(uint16_t centerX, uint16_t centerY, uint16_t rectangularX, uint16_t rectangularY, uint16_t & angle, uint16_t & radius)
    {
    float fX = (float)rectangularX - centerX;
    float fY = (float)rectangularY - centerY;
    float fZ = sqrt(fX * fX + fY * fY);
    fX /= fZ;
    fY /= fZ;

    if ((fY == 0) and (fX > 0))
    {
        angle = 90;
    }
    else if ((fY == 0) and (fX < 0))
    {
        angle = 270;
    }
    else
    {
        float fAngle = -atan(fX / fY);
        fAngle *= 360 / 2 / PI;

        if (fY > 0)
        {
            fAngle += 180;
        }
        if ((fX < 0) and (fY < 0))
        {
            fAngle += 360;
        }
        angle = (uint16_t)fAngle;
    }
    }
*/

#if (STRING_MODE == USE_STRING_OBJECT)
String utf2iso(String s)
#elif (STRING_MODE == USE_CHAR_ARRAY)
char * utf2iso(const char * s)
#endif // STRING_MODE
{
    uint8_t c;

#if (STRING_MODE == USE_STRING_OBJECT)
    s.toCharArray(bufferIn, s.length() + 1);
    // strcpy(bufferIn, s.c_str());
#elif (STRING_MODE == USE_CHAR_ARRAY)
    strncpy(bufferIn, s, sizeof(bufferIn));
#endif // STRING_MODE

    memset(&bufferOut, 0x00, sizeof(bufferOut));

    for (uint8_t i = 0; i < strlen(bufferIn); i++)
    {
        c = (uint8_t)bufferIn[i];

        if (c < 0x80)
        {
            bufferOut[strlen(bufferOut)] = c;
        }
        else if (c == 0xc3)
        {
            bufferOut[strlen(bufferOut)] = (bufferIn[++i] + 64);
        }
        else if (c == 0xc2)
        {
            bufferOut[strlen(bufferOut)] = bufferIn[++i];
        }
        else if (c == 0xe2)
        {
            if ((bufferIn[i + 1] == 0x82) && (bufferIn[i + 2] == 0xac))
            {
                bufferOut[strlen(bufferOut)] = 0x80;
                i += 2;
            }
        }
    }
#if (STRING_MODE == USE_STRING_OBJECT)

    return String(bufferOut);

#elif (STRING_MODE == USE_CHAR_ARRAY)

    return bufferOut;

#endif // STRING_MODE
}

int16_t checkRange(int16_t value, int16_t valueMin, int16_t valueMax)
{
    int16_t localMin = min(valueMin, valueMax);
    int16_t localMax = max(valueMin, valueMax);

    return min(max(localMin, value), localMax);
}
uint16_t checkRange(uint16_t value, uint16_t valueMin, uint16_t valueMax)
{
    uint16_t localMin = min(valueMin, valueMax);
    uint16_t localMax = max(valueMin, valueMax);

    return min(max(localMin, value), localMax);
}
int32_t checkRange(int32_t value, int32_t valueMin, int32_t valueMax)
{
    int32_t localMin = min(valueMin, valueMax);
    int32_t localMax = max(valueMin, valueMax);

    return min(max(localMin, value), localMax);
}
uint32_t checkRange(uint32_t value, uint32_t valueMin, uint32_t valueMax)
{
    uint32_t localMin = min(valueMin, valueMax);
    uint32_t localMax = max(valueMin, valueMax);

    return min(max(localMin, value), localMax);
}

void setMinMax(uint16_t value, uint16_t & valueMin, uint16_t & valueMax)
{
    if (value < valueMin)
    {
        valueMin = value;
    }
    if (value > valueMax)
    {
        valueMax = value;
    }
}

uint32_t roundUp(uint32_t value, uint16_t modulo)
{
    uint32_t result = value / modulo;
    if ((value % modulo) > 0)
    {
        result++;
    }
    return result;
}

// Utilities
// template<typename T>
void swap(int8_t & a, int8_t & b)
{
    int8_t w = a;
    a = b;
    b = w;
}
void swap(uint8_t & a, uint8_t & b)
{
    uint8_t w = a;
    a = b;
    b = w;
}
void swap(int16_t & a, int16_t & b)
{
    int16_t w = a;
    a = b;
    b = w;
}
void swap(uint16_t & a, uint16_t & b)
{
    uint16_t w = a;
    a = b;
    b = w;
}
void swap(int32_t & a, int32_t & b)
{
    int32_t w = a;
    a = b;
    b = w;
}
void swap(uint32_t & a, uint32_t & b)
{
    uint32_t w = a;
    a = b;
    b = w;
}

void printPin(const char * label, uint8_t pin)
{
    hV_HAL_Serial_printf(". %s= ", label);
    if (pin == NOT_CONNECTED)
    {
        hV_HAL_Serial_printf("not connected");
    }
    else if (pin < 0x80)
    {
        hV_HAL_Serial_printf("%i", pin);
    }
    else
    {
        hV_HAL_Serial_printf("%i expanded", pin & 0x7f);
    }
    hV_HAL_Serial_crlf();
}

void configurationDebug(pins_t board)
{
    const char * stringFONT_MODE[] = {"USE_FONT_TERMINAL", "USE_FONT_HEADER", "USE_FONT_FLASH", "USE_FONT_INTERNAL"};
    const char * stringSRAM_MODE[] = {"EMPTY", "USE_INTERNAL_MCU", "USE_EXTERNAL_SPI"};
    const char * stringUSE_hV_SCREEN[] = {"EMPTY", "USE_hV_SCREEN_VIRTUAL", "USE_hV_SCREEN_SELF"};
    const char * stringTOUCH_MODE[] = {"USE_TOUCH_NONE", "USE_TOUCH_YES"};
    const char * stringHAPTICS_MODE[] = {"USE_HAPTICS_NONE", "USE_DRV2605L_ERM", "USE_DRV2605L_LRA"};
    const char * stringEXPANDER_MODE[] = {"USE_EXPANDER_NONE", "USE_I2C_PCF8574", "USE_I2C_PCA9536", "USE_I2C_TCA6408"};
    const char * stringSTRING_MODE[] = {"EMPTY", "USE_STRING_OBJECT", "USE_CHAR_ARRAY"};
    const char * stringSTORAGE_MODE[] = {"USE_NONE", "USE_SD_CARD", "USE_SPI_FLASH", "USE_SERIAL_CONSOLE", "USE_LINUX_FILES"};

    hV_HAL_Serial_crlf();
    hV_HAL_Serial_printf("hV . Configuration from 'hV_Configuration.h'");
    hV_HAL_Serial_crlf();

    hV_HAL_Serial_crlf(); // Section

    hV_HAL_Serial_printf("2- Board GPIOs");
    hV_HAL_Serial_crlf();
    printPin("Panel Busy", board.panelBusy);
    printPin("Panel DC", board.panelDC);
    printPin("Panel /Reset", board.panelReset);
    printPin("Panel /CS", board.panelCS);
    printPin("Panel /CSS", board.panelCS);
    printPin("Flash /CS", board.flashCS);
    printPin("Flash /CSS", board.flashCSS);
    printPin("SD-card /CS", board.cardCS);
    printPin("SD-card Detect", board.cardDetect);
    printPin("Touch /Reset", board.touchReset);
    printPin("Touch !Int", board.touchInt);

    hV_HAL_Serial_crlf(); // Section

    hV_HAL_Serial_printf("3- Font mode");
    hV_HAL_Serial_crlf();
    hV_HAL_Serial_printf(". FONT_MODE= %s", stringFONT_MODE[FONT_MODE]);
    hV_HAL_Serial_crlf();

    hV_HAL_Serial_crlf(); // Section

    hV_HAL_Serial_printf("4- Maximum number of fonts");
    hV_HAL_Serial_crlf();
    hV_HAL_Serial_printf(". MAX_FONT_SIZE= %i", MAX_FONT_SIZE);
    hV_HAL_Serial_crlf();

    hV_HAL_Serial_crlf(); // Section

    hV_HAL_Serial_printf("5- SRAM mode");
    hV_HAL_Serial_crlf();
    hV_HAL_Serial_printf(". SRAM_MODE= %s", stringSRAM_MODE[SRAM_MODE]);
    hV_HAL_Serial_crlf();

    hV_HAL_Serial_crlf(); // Section

    hV_HAL_Serial_printf("6. Use virtual");
    hV_HAL_Serial_crlf();
    hV_HAL_Serial_printf(". USE_hV_SCREEN= %s", stringUSE_hV_SCREEN[USE_hV_SCREEN]);
    hV_HAL_Serial_crlf();

    hV_HAL_Serial_crlf(); // Section

    hV_HAL_Serial_printf("7. Touch mode");
    hV_HAL_Serial_crlf();
    hV_HAL_Serial_printf(". TOUCH_MODE= %s", stringTOUCH_MODE[TOUCH_MODE]);
    hV_HAL_Serial_crlf();

    hV_HAL_Serial_crlf(); // Section

    hV_HAL_Serial_printf("8. Haptic mode");
    hV_HAL_Serial_crlf();
    hV_HAL_Serial_printf(". HAPTICS_MODE= %s", stringHAPTICS_MODE[HAPTICS_MODE]);
    hV_HAL_Serial_crlf();

    hV_HAL_Serial_crlf(); // Section

    hV_HAL_Serial_printf("9. Expander mode");
    hV_HAL_Serial_crlf();
    hV_HAL_Serial_printf(". EXPANDER_MODE= %s", stringEXPANDER_MODE[EXPANDER_MODE]);
    hV_HAL_Serial_crlf();

    hV_HAL_Serial_crlf(); // Section

    hV_HAL_Serial_printf("10. String mode");
    hV_HAL_Serial_crlf();
    hV_HAL_Serial_printf(". STRING_MODE= %s", stringSTRING_MODE[STRING_MODE]);
    hV_HAL_Serial_crlf();

    hV_HAL_Serial_crlf(); // Section

    hV_HAL_Serial_printf("11. Storage mode");
    hV_HAL_Serial_crlf();
    hV_HAL_Serial_printf(". STORAGE_MODE= ");
    if (STORAGE_MODE == USE_NONE)
    {
        hV_HAL_Serial_printf("%s ", stringSTORAGE_MODE[0]);
    }
    if (STORAGE_MODE & USE_SD_CARD)
    {
        hV_HAL_Serial_printf("%s ", stringSTORAGE_MODE[1]);
    }
    if (STORAGE_MODE & USE_SPI_FLASH)
    {
        hV_HAL_Serial_printf("%s ", stringSTORAGE_MODE[2]);
    }
    if (STORAGE_MODE & USE_SERIAL_CONSOLE)
    {
        hV_HAL_Serial_printf("%s ", stringSTORAGE_MODE[3]);
    }
    if (STORAGE_MODE & USE_LINUX_FILES)
    {
        hV_HAL_Serial_printf("%s ", stringSTORAGE_MODE[4]);
    }
    hV_HAL_Serial_crlf();

    hV_HAL_Serial_printf("===");
    hV_HAL_Serial_crlf();
}

long scaleValue(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
