//
// hV_Font_Header.cpp
// C++ code
// ----------------------------------
//
// Details Description of the file or library
// Project iTC_BWR_Flash
//
// Created by Rei Vilo, 10 Nov 2018
//
// Copyright (c) Rei Vilo, 2018-2020
// Licence All rights reserved
//
// See hV_Font_Header.h and ReadMe.txt for references
//

// Configuration
#include "hV_Configuration.h"

// The Arduino IDE doesn't allow to select the libraries, hence this condition.
#if (FONT_MODE == USE_FONT_HEADER)

#include "hV_Utilities.h"
#include "hV_Font_Header.h"

// Code
// Font functions
//hV_Font_Header::hV_Font_Header()
void hV_Font_Header::_f_begin()
{
    _f_fontSize = 0;
    _f_fontNumber = 0;
    _f_fontSolid = true;
    _f_fontSpaceX = 0;

    // Default font= 12
#if (FONT_STYLE == FONT_MONO)

    addFont(Font_DejaVuMono12);

#elif (FONT_STYLE == FONT_SANS)

    //#if (MAX_FONT_SIZE > 0)
//    _f_addFont(Font_DejaVuSans12);

#elif (FONT_STYLE == FONT_SERIF)

    _f_addFont(Font_DejaVuSerif12);

#else
#error Incorrect FONT_STYLE
#endif // FONT_STYLE

    // Take first font
    _f_selectFont(0);
}

uint8_t hV_Font_Header::_f_addFont(font_s fontName)
{
    if (_f_fontNumber < MAX_FONT_SIZE)
    {
        _f_fontTable[_f_fontNumber] = fontName;
        _f_fontNumber++;
        return _f_fontNumber;
    }
    else
    {
        return 0;
    }
}

void hV_Font_Header::_f_setFontSolid(bool flag)
{
    _f_fontSolid = flag;
}

void hV_Font_Header::_f_selectFont(uint8_t size)
{
    _f_fontSize = min(size, (uint8_t)(_f_fontNumber - 1));
    _f_font = _f_fontTable[_f_fontSize];

    // _f_fontSpaceX = 0;//roundUp(_f_font.maxWidth, 16);

    if ((_f_font.kind & 0x40) == 0x40) // Monospaced font
    {
        _f_fontSpaceX = 0;
    }

    // _f_font.maxWidth has been checked for all the fonts
    // _f_font.maxWidth = 0;
    // for (uint8_t i = 0; i < _f_font.number; i++)
    // {
    // _f_font.maxWidth = max(_f_font.maxWidth, _f_font.width[i].pixel);
    // }
}

uint8_t hV_Font_Header::_f_fontMax()
{
    return _f_fontNumber;
}

void hV_Font_Header::_f_setFontSpaceX(uint8_t number)
{
    _f_fontSpaceX = number;
}

void hV_Font_Header::_f_setFontSpaceY(uint8_t number)
{
    _f_fontSpaceY = number;
}

uint8_t hV_Font_Header::_f_getCharacter(uint8_t character, uint16_t index)
{
    // if (character >= _f_font.first)
    // {
    //#if defined(ENERGIA)
    // return _f_font.table[(character - _f_font.first) * _f_font.bytes + index + 1];
    //#else
    // return pgm_read_byte(&_font.table[(character - _f_font.first) * _f_font.bytes + index + 1]);
    //#endif
    // }
    // else
    // {
    uint8_t result = 0;

    if ((character >= _f_font.first) and (character < _f_font.first + _f_font.number))
    {
        uint32_t address = _f_font.width[character - _f_font.first].index;
        result = _f_font.table[address + index];
    }
    return result;
    // }
}

uint16_t hV_Font_Header::_f_characterSizeX(uint8_t character)
{
    uint16_t result = 0;
    // if ((_f_font.variable) and (character >= _f_font.first) and (character < _f_font.first + _f_font.number))
    // {
    //#if defined(ENERGIA)
    // return _f_font.table[(character - _f_font.first) * _f_font.bytes + 0] + _f_fontSpaceX;
    //#else
    // return pgm_read_byte(&_font.table[(character - _f_font.first) * _f_font.bytes + 0]) + _f_fontSpaceX;
    //#endif
    // }
    // else
    // {
    // return _f_font.width + _f_fontSpaceX;
    // }
    if (character >= _f_font.first)
    {
        result = _f_font.width[character - _f_font.first].pixel + _f_fontSpaceX;
    }

    return result;
}

uint16_t hV_Font_Header::_f_characterSizeY()
{
    return _f_font.height;
}

#if (STRING_MODE == USE_STRING_OBJECT)
uint16_t hV_Font_Header::_f_stringSizeX(String text)
{
    uint16_t textWidth = 0;

    if ((_f_font.kind & FONT_MONO) == FONT_MONO)
    {
        textWidth = (_f_font.maxWidth + _f_fontSpaceX) * text.length();
    }
    else
    {
        for (uint8_t k = 0; k < text.length(); k++)
        {
            textWidth += _f_characterSizeX(text.charAt(k));
        }
    }

    return textWidth;
}
#elif (STRING_MODE == USE_CHAR_ARRAY)
uint16_t hV_Font_Header::_f_stringSizeX(const char * text)
{
    uint16_t textWidth = 0;

    if ((_f_font.kind & FONT_MONO) == FONT_MONO)
    {
        textWidth = (_f_font.maxWidth + _f_fontSpaceX) * strlen(text);
    }
    else
    {
        for (uint8_t k = 0; k < strlen(text); k++)
        {
            textWidth += _f_characterSizeX(text[k]);
        }
    }

    return textWidth;
}
#endif // STRING_MODE

#if (STRING_MODE == USE_STRING_OBJECT)
uint8_t hV_Font_Header::_f_stringLengthToFitX(String text, uint16_t pixels)
{
    uint8_t index = 0;
    uint16_t textWidth = 0;

    // Monospaced font
    if ((_f_font.kind & FONT_MONO) == FONT_MONO)
    {
        index = pixels / _f_font.maxWidth - 1;
        if (index > text.length())
        {
            index = text.length();
        }
    }
    else
    {
        for (uint8_t k = 0; k < text.length(); k++)
        {
            textWidth += _f_characterSizeX(text.charAt(k));
            if (textWidth <= pixels)
            {
                index = k + 1;
            }
            else
            {
                exit;
            }
        }
    }
    return index;
}
#elif (STRING_MODE == USE_CHAR_ARRAY)
uint8_t hV_Font_Header::_f_stringLengthToFitX(const char * text, uint16_t pixels)
{
    uint8_t index = 0;
    uint16_t textWidth = 0;

    // Monospaced font
    if ((_f_font.kind & FONT_MONO) == FONT_MONO)
    {
        index = pixels / _f_font.maxWidth - 1;
        if (index > strlen(text))
        {
            index = strlen(text);
        }
    }
    else
    {
        for (uint8_t k = 0; k < strlen(text); k++)
        {
            textWidth += _f_characterSizeX(text[k]);
            if (textWidth <= pixels)
            {
                index = k + 1;
            }
            else
            {
                return index;
            }
        }
    }
    return index;
}
#endif // STRING_MODE

/*
    uint32_t hV_Font_Header::roundUp(uint32_t value, uint16_t modulo)
    {
    uint32_t result = value / modulo;
    if ((value % modulo) > 0)
    {
        result++;
    }
    return result;
    }
*/
uint8_t hV_Font_Header::_f_getFontKind()
{
    return _f_font.kind;
}
uint8_t hV_Font_Header::_f_getFontMaxWidth()
{
    return _f_font.maxWidth;
}

#endif // USE_FONT_HEADER
