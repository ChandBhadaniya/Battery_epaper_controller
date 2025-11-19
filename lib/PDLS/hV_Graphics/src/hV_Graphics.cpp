//
// hV_Graphics.cpp
// Library C++ code
// ----------------------------------
//
// Project Pervasive Displays Library Suite
// Based on highView technology
//
// Created by Rei Vilo, 31 May 2013
//
// Copyright (c) Rei Vilo, 2010-2023
// Licence All rights reserved
//
// See hV_Graphics.h and ReadMe.txt for references
//
// Release 520: Improved graphics and simplified syntax
// Release 521: Added Channels graphic
// Release 540: Added steps for Histogramm and Multiple elements
// Release 605: Improved elements link to GUI
// Release 605: Capitalised first letter of elements names
// Release 607: Refactored peripherals
//

// Library header
#include "hV_Graphics.h"

Graphics::Graphics(hV_Screen_Virtual * screen)
{
    _pScreen = screen;
};

void Graphics::begin()
{
    _colourFront = myColours.black;
    _colourBack = myColours.white;
    _colourMiddle = myColours.grey;
}

void Graphics::setColours(uint16_t frontColour, uint16_t backColour, uint16_t middleColour)
{
    _colourFront = frontColour;
    _colourBack = backColour;
    _colourMiddle = middleColour;
};

void Graphics::setFonts(uint8_t fontLegend, uint8_t fontValue)
{
    _fontLegend = fontLegend;
    _fontValue = fontValue;
}

void Graphics::dLabel(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
#if (STRING_MODE == USE_STRING_OBJECT)
                      String text0,
#elif (STRING_MODE == USE_CHAR_ARRAY)
                      const char * text0,
#endif // STRING_MODE
                      uint16_t textColour0, uint16_t backColour0,
                      int8_t horizontal0, int8_t vertical0,
                      uint8_t size0, bool flush0)
{
    uint8_t _size;
    uint16_t _xt, _yt;

#if (STRING_MODE == USE_STRING_OBJECT)

    String _text;

#elif (STRING_MODE == USE_CHAR_ARRAY)

    char _text[64];
    memset(_text, 0x00, sizeof(_text));

    uint16_t k = min((uint16_t)strlen(text0), (uint16_t)63);

#endif // STRING_MODE

    bool oldPenMode = _pScreen->getPenSolid();

    if (size0 == fontSizeAutomatic)
    {
        uint8_t index = _pScreen->fontMax();
        do
        {
            index--;
            _pScreen->selectFont(index);
        }
        while (((_pScreen->stringSizeX(text0) > (dx - 6)) or (_pScreen->characterSizeY() > (dy - 6))) and (index > 0));

        _size = index;
    }
    else
    {
        _size = size0;
    }
    _pScreen->selectFont(_size);

#if (STRING_MODE == USE_STRING_OBJECT)

    _text = text0;
    while (_pScreen->stringSizeX(_text) > (dx - 6))
    {
        _text = _text.substring(0, _text.length() - 1);
    }

#elif (STRING_MODE == USE_CHAR_ARRAY)

    memset(_text, 0x00, sizeof(_text));
    strncpy(_text, text0, k);
    while (_pScreen->stringSizeX(_text) > (dx - 6))
    {
        k--;
        memset(_text, 0x00, sizeof(_text));
        strncpy(_text, text0, k);
    }

#endif // STRING_MODE

    // horizontal
    switch (horizontal0)
    {
        case -1:

            _xt = x0;
            break;

        case 1:

            _xt = x0 + dx - _pScreen->stringSizeX(_text);
            break;

        default:

            _xt = x0 + (dx - (_pScreen->stringSizeX(_text))) / 2;
            break;
    }

    switch (vertical0)
    {
        case 1:

            _yt = y0;
            break;

        case -1:

            _yt = y0 + dy - _pScreen->characterSizeY();
            break;

        default:

            _yt = y0 + (dy - _pScreen->characterSizeY()) / 2;
            break;
    }

    _pScreen->setPenSolid(true);
    _pScreen->dRectangle(x0, y0, dx, dy, backColour0);
    hV_HAL_delayMilliseconds(10); // delay required by chipKIT

    _pScreen->selectFont(_size);
    _pScreen->setFontSolid(false);
    _pScreen->gText(_xt, _yt, _text, textColour0);

    _pScreen->setPenSolid(oldPenMode);
}

// Code
mtgDial::mtgDial(Graphics * graphics)
{
    _pGraphics = graphics;
}

void mtgDial::drawDial(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t valueColour)
{
    _x0 = x0;
    _y0 = y0;
    _radius = radius;

    _valueColour = valueColour;

    _pGraphics->_pScreen->setPenSolid(true);
    _pGraphics->_pScreen->circle(_x0, _y0, _radius, _pGraphics->_colourBack);
    _pGraphics->_pScreen->setPenSolid(false);
    _pGraphics->_pScreen->circle(_x0, _y0, _radius, _pGraphics->_colourFront);
}

Clock::Clock(Graphics * graphics)
{
    _pGraphics = graphics;
}

void Clock::dDefine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                    uint16_t hourColour, uint16_t minuteColour, uint16_t secondColour)
{
    uint16_t radius = min(dx, dy) / 2;
    define(x0 + dx / 2, y0 + dy / 2, radius - 2, hourColour, minuteColour, secondColour);
}

void Clock::define(uint16_t x0, uint16_t y0, uint16_t radius,
                   uint16_t hourColour, uint16_t minuteColour, uint16_t secondColour)
{
    radius -= 3;
    // void mtgDial::drawDial(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t valueColour)
    drawDial(x0, y0, radius - 1, _pGraphics->_colourMiddle);

    _hourColour = hourColour;
    _minuteColour = minuteColour;
    _secondColour = secondColour;

    _oldHourX = x0;
    _oldHourY = y0;
    _oldMinuteX = x0;
    _oldMinuteY = y0;
    _oldSecondX = x0;
    _oldSecondY = y0;

    int32_t aa, bb;
    _pGraphics->_pScreen->setPenSolid(true);
    for (int8_t i = 0; i < 12; i += 1)
    {
        aa = i * 3000;
        bb = 2; // All hours
        if ((i % 3) == 0)
        {
            bb = 4; // 3, 6, 9 and 12 hours
        }
        if (i == 3)
        {
            bb = 6; // 12 hours
        }

        _pGraphics->_pScreen->circle(_x0 - _radius * cos32x100(aa) / 100, _y0 - _radius * sin32x100(aa) / 100, bb, _pGraphics->_colourFront);
    }
}

void Clock::_needle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
    _pGraphics->_pScreen->line(x1, y1 - 1, x2, y2 - 1, colour);
    _pGraphics->_pScreen->line(x1, y1, x2, y2, colour);
    _pGraphics->_pScreen->line(x1, y1 + 1, x2, y2 + 1, colour);
}

#if (STRING_MODE == USE_STRING_OBJECT)

void Clock::draw(uint8_t hour, uint8_t minute, uint8_t second, uint8_t s100th, String text)
{
    int32_t hourX, hourY, minuteX, minuteY, secondX, secondY;
    int32_t _newHour, _newMinute, _newSecond;
    bool flagH, flagM, flagS, flagT;

    _newHour = hour;
    _newMinute = minute;
    _newSecond = second;

    hourX = int32_t(_x0 + _radius * 6 / 10 * cos32x100((100 * _newHour + 100 * _newMinute / 60 + 900) * 360 / 12) / 100);
    hourY = int32_t(_y0 + _radius * 6 / 10 * sin32x100((100 * _newHour + 100 * _newMinute / 60 + 900) * 360 / 12) / 100);
    minuteX = int32_t(_x0 + _radius * 8 / 10 * cos32x100((100 * _newMinute + 100 * _newSecond / 60 + 4500) * 360 / 60) / 100);
    minuteY = int32_t(_y0 + _radius * 8 / 10 * sin32x100((100 * _newMinute + 100 * _newSecond / 60 + 4500) * 360 / 60) / 100);
    secondX = int32_t(_x0 + _radius * 8 / 10 * cos32x100((100 * _newSecond + s100th + 4500) * 360 / 60) / 100);
    secondY = int32_t(_y0 + _radius * 8 / 10 * sin32x100((100 * _newSecond + s100th + 4500) * 360 / 60) / 100);

    flagH = (hourX != _oldHourX) || (hourY != _oldHourY);
    flagM = (minuteX != _oldMinuteX) || (minuteY != _oldMinuteY);
    flagS = (secondX != _oldSecondX) || (secondY != _oldSecondY);
    flagT = (text != "");

    // old time with _colourBack
    if (flagS)
    {
        _pGraphics->_pScreen->line(_x0, _y0, _oldSecondX, _oldSecondY, _colourBack);
    }
    if (flagM)
    {
        _needle(_x0, _y0, _oldMinuteX, _oldMinuteY, _colourBack);
    }
    if (flagH)
    {
        _needle(_x0 - 1, _y0, _oldHourX - 1, _oldHourY, _colourBack);
        _needle(_x0, _y0, _oldHourX, _oldHourY, _colourBack);
        _needle(_x0 + 1, _y0, _oldHourX + 1, _oldHourY, _colourBack);
    }

    if ((flagH || flagM || flagS) && flagT)
    {
        _pGraphics->_pScreen->selectFont(_fontValue);
        _pGraphics->_pScreen->setFontSolid(true);

        String _s = text.substring(0, min(text.length(), 16));
        // int32_t k = _s.length() * _pGraphics->_pScreen->fontSizeX() / 2;
        int32_t k = _pGraphics->_pScreen->stringSizeX(text);

        if (k != _oldK)
        {
            // Label
            _pGraphics->_pScreen->setPenSolid(true);
            _pGraphics->_pScreen->dRectangle(_x0 - _oldK / 2 - 2, _y0 + _radius / 2 - 2, _oldK + 4, _pGraphics->_pScreen->characterSizeY() + 3, _colourBack);

            _oldK = k;
        }

        _pGraphics->_pScreen->setPenSolid(false);
        _pGraphics->_pScreen->dRectangle(_x0 - k / 2 - 2, _y0 + _radius / 2 - 2, k + 4, _pGraphics->_pScreen->characterSizeY() + 3, _colourFront);
        _pGraphics->_pScreen->gText(_x0 - k / 2, _y0 + _radius / 2, _s, _colourFront, _colourBack);
    }

    if (flagH || flagM || flagS || flagT)
    {
        // new time with _hourColour, _minuteColour, _secondColour
        _needle(_x0 - 1, _y0, hourX - 1, hourY, _hourColour);
        _needle(_x0, _y0, hourX, hourY, _hourColour);
        _needle(_x0 + 1, _y0, hourX + 1, hourY, _hourColour);

        _needle(_x0, _y0, minuteX, minuteY, _minuteColour);

        _pGraphics->_pScreen->line(_x0, _y0, secondX, secondY, _secondColour);

        _pGraphics->_pScreen->setPenSolid(true);
        _pGraphics->_pScreen->circle(_x0, _y0, 4, _colourFront);
    }

    _oldHourX = hourX;
    _oldHourY = hourY;
    _oldMinuteX = minuteX;
    _oldMinuteY = minuteY;
    _oldSecondX = secondX;
    _oldSecondY = secondY;
}

#elif (STRING_MODE == USE_CHAR_ARRAY)

void Clock::draw(uint8_t hour, uint8_t minute, uint8_t second, uint8_t s100th, const char * text)
{
    int32_t hourX, hourY, minuteX, minuteY, secondX, secondY;
    int32_t _newHour, _newMinute, _newSecond;
    bool flagH, flagM, flagS, flagT;

    _newHour = hour;
    _newMinute = minute;
    _newSecond = second;

    hourX = int32_t(_x0 + _radius * 6 / 10 * cos32x100((100 * _newHour + 100 * _newMinute / 60 + 900) * 360 / 12) / 100);
    hourY = int32_t(_y0 + _radius * 6 / 10 * sin32x100((100 * _newHour + 100 * _newMinute / 60 + 900) * 360 / 12) / 100);
    minuteX = int32_t(_x0 + _radius * 8 / 10 * cos32x100((100 * _newMinute + 100 * _newSecond / 60 + 4500) * 360 / 60) / 100);
    minuteY = int32_t(_y0 + _radius * 8 / 10 * sin32x100((100 * _newMinute + 100 * _newSecond / 60 + 4500) * 360 / 60) / 100);
    secondX = int32_t(_x0 + _radius * 8 / 10 * cos32x100((100 * _newSecond + s100th + 4500) * 360 / 60) / 100);
    secondY = int32_t(_y0 + _radius * 8 / 10 * sin32x100((100 * _newSecond + s100th + 4500) * 360 / 60) / 100);

    flagH = (hourX != _oldHourX) || (hourY != _oldHourY);
    flagM = (minuteX != _oldMinuteX) || (minuteY != _oldMinuteY);
    flagS = (secondX != _oldSecondX) || (secondY != _oldSecondY);
    flagT = (strlen(text) > 0); // non empty

    // old time with _colourBack
    if (flagS)
    {
        _pGraphics->_pScreen->line(_x0, _y0, _oldSecondX, _oldSecondY, _pGraphics->_colourBack);
    }
    if (flagM)
    {
        _needle(_x0, _y0, _oldMinuteX, _oldMinuteY, _pGraphics->_colourBack);
    }
    if (flagH)
    {
        _needle(_x0 - 1, _y0, _oldHourX - 1, _oldHourY, _pGraphics->_colourBack);
        _needle(_x0, _y0, _oldHourX, _oldHourY, _pGraphics->_colourBack);
        _needle(_x0 + 1, _y0, _oldHourX + 1, _oldHourY, _pGraphics->_colourBack);
    }

    if ((flagH || flagM || flagS) && flagT)
    {
        _pGraphics->_pScreen->selectFont(_pGraphics->_fontValue);
        _pGraphics->_pScreen->setFontSolid(true);

        // String _s = text.substring(0, min(text.length(), 16));
        // int32_t k = _s.length() * _pGraphics->_pScreen->fontSizeX() / 2;
        int32_t k = _pGraphics->_pScreen->stringSizeX(text);

        if (k != _oldK)
        {
            // Label
            _pGraphics->_pScreen->setPenSolid(true);
            _pGraphics->_pScreen->dRectangle(_x0 - _oldK / 2 - 2, _y0 + _radius / 2 - 2, _oldK + 4, _pGraphics->_pScreen->characterSizeY() + 3, _pGraphics->_colourBack);

            _oldK = k;
        }

        _pGraphics->_pScreen->setPenSolid(false);
        _pGraphics->_pScreen->dRectangle(_x0 - k / 2 - 2, _y0 + _radius / 2 - 2, k + 4, _pGraphics->_pScreen->characterSizeY() + 3, _pGraphics->_colourFront);
        _pGraphics->_pScreen->gText(_x0 - k / 2, _y0 + _radius / 2, text, _pGraphics->_colourFront, _pGraphics->_colourBack);
    }

    if (flagH || flagM || flagS || flagT)
    {
        // new time with _hourColour, _minuteColour, _secondColour
        _needle(_x0 - 1, _y0, hourX - 1, hourY, _hourColour);
        _needle(_x0, _y0, hourX, hourY, _hourColour);
        _needle(_x0 + 1, _y0, hourX + 1, hourY, _hourColour);

        _needle(_x0, _y0, minuteX, minuteY, _minuteColour);

        _pGraphics->_pScreen->line(_x0, _y0, secondX, secondY, _secondColour);

        _pGraphics->_pScreen->setPenSolid(true);
        _pGraphics->_pScreen->circle(_x0, _y0, 4, _pGraphics->_colourFront);
    }

    _oldHourX = hourX;
    _oldHourY = hourY;
    _oldMinuteX = minuteX;
    _oldMinuteY = minuteY;
    _oldSecondX = secondX;
    _oldSecondY = secondY;
}

#endif // STRING_MODE

Gauge::Gauge(Graphics * graphics)
{
    _pGraphics = graphics;
}

void Gauge::dDefine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                    int32_t valueMin, int32_t valueMax, int32_t unit,
                    uint16_t grid,
                    uint16_t valueColour)
{
    uint16_t radius = min(dx, dy) / 2;
    define(x0 + dx / 2, y0 + dy / 2, radius - 2,
           valueMin, valueMax, unit,
           grid,
           valueColour);
}

void Gauge::define(uint16_t x0, uint16_t y0, uint16_t radius,
                   int32_t valueMin, int32_t valueMax, int32_t unit,
                   uint16_t grid,
                   uint16_t valueColour)
{
    drawDial(x0, y0, radius, valueColour);

    _oldZ = 45000;
    _oldK = 0;

    _valueMin = valueMin;
    _valueMax = valueMax;
    _unit = unit;
    _formatUnit = getDecimalPlaces(unit);
    _grid = grid;

    _n = 0; // number of values shown

    int32_t aa;
    _pGraphics->_pScreen->setPenSolid(true);
    if (_grid > 0)
    {
        for (int32_t i = 0; i <= _grid; i += 1)
        {
            aa = 31500 + i * 27000 / _grid;
            _pGraphics->_pScreen->circle(_x0 - (_radius - 2) * cos32x100(aa) / 100, _y0 - (_radius - 2) * sin32x100(aa) / 100, 2, _pGraphics->_colourMiddle);
        }
    }
    _pGraphics->_pScreen->setPenSolid(false);
}

void Gauge::needle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
    _pGraphics->_pScreen->line(x1, y1 - 1, x2, y2 - 1, colour);
    _pGraphics->_pScreen->line(x1, y1, x2, y2, colour);
    _pGraphics->_pScreen->line(x1, y1 + 1, x2, y2 + 1, colour);
}

#if (STRING_MODE == USE_STRING_OBJECT)

void Gauge::draw(int32_t value, String text)
{
    // int32_t z = 31500+27000*(value-_valueMin)/(_valueMax-_valueMin);
    String _s;
    int32_t z = hV_HAL_map(value, _valueMin, _valueMax, 31500, 58500);
    if (z < 31500)
    {
        z = 31500;
    }
    if (z > 58500)
    {
        z = 58500;
    }
    int32_t k = min(_radius * 8 / 10, _radius - 8 - 3);
    int32_t l = 0;
    uint16_t vX = _x0 - k * cos32x100(z) / 100;
    uint16_t vY = _y0 - k * sin32x100(z) / 100;
    uint16_t _vX = _x0 - k * cos32x100(_oldZ) / 100;
    uint16_t _vY = _y0 - k * sin32x100(_oldZ) / 100;

    _oldZ = z;
    if ((vX == _vX) && (vY == _vY))
    {
        return;
    }

    needle(_x0 - 1, _y0, _vX - 1, _vY, _colourBack);
    needle(_x0, _y0, _vX, _vY, _colourBack);
    needle(_x0 + 1, _y0, _vX + 1, _vY, _colourBack);

    needle(_x0 - 1, _y0, vX - 1, vY, _valueColour);
    needle(_x0, _y0, vX, vY, _valueColour);
    needle(_x0 + 1, _y0, vX + 1, vY, _valueColour);

    // min and max values
    _pGraphics->_pScreen->selectFont(_fontLegend);
    _pGraphics->_pScreen->setFontSolid(true);
    String legend;
    legend = formatString(" %+i.%0*i ", _valueMin / _unit, _formatUnit, abs(_valueMin) % _unit);
    _pGraphics->_pScreen->gText(_x0 - _radius, _y0 + _radius - _pGraphics->_pScreen->characterSizeY(), legend, _colourFront, _colourBack);
    legend = formatString(" %+i.%0*i ", _valueMin / _unit, _formatUnit, abs(_valueMin) % _unit);
    _pGraphics->_pScreen->gText(_x0 + _radius - _pGraphics->_pScreen->stringSizeX(legend), _y0 + _radius - _pGraphics->_pScreen->characterSizeY(), legend, _colourFront, _colourBack);

    // string display
    if (text != "")
    {
        _pGraphics->_pScreen->setPenSolid(false);
        _pGraphics->_pScreen->setFontSolid(true);
        _pGraphics->_pScreen->selectFont(_fontValue);
        _pGraphics->_pScreen->setFontSolid(true);

        _s = text.substring(0, min(text.length(), 8));
        k = _pGraphics->_pScreen->stringSizeX(_s);
        l = _radius * 9 / 10 - 2 * _pGraphics->_pScreen->characterSizeY();

        if (k != _oldK)
        {
            // Label
            _pGraphics->_pScreen->setPenSolid(true);
            _pGraphics->_pScreen->dRectangle(_x0 - _radius / 2, _y0 + l - 2, _radius, _pGraphics->_pScreen->characterSizeY() + 3, _colourBack);

            _pGraphics->_pScreen->setPenSolid(false);
            _pGraphics->_pScreen->dRectangle(_x0 - _radius / 2, _y0 + l - 2, _radius, _pGraphics->_pScreen->characterSizeY() + 3, _colourMiddle);

            _oldK = k;
        }

        _pGraphics->_pScreen->gText(_x0 - k / 2, _y0 + l, _s, _colourFront, _colourBack); // , 2, 2);

    } // string display

    _pGraphics->_pScreen->setPenSolid(true);
    _pGraphics->_pScreen->circle(_x0, _y0, 3, _valueColour);
}

#elif (STRING_MODE == USE_CHAR_ARRAY)

void Gauge::draw(int32_t value, const char * text)
{
    char aText[17] = {0};
    strncpy(aText, text, 8);

    // int32_t z = 31500+27000*(value-_valueMin)/(_valueMax-_valueMin);
    // String _s;
    int32_t z = hV_HAL_map(value, _valueMin, _valueMax, 31500, 58500);
    if (z < 31500)
    {
        z = 31500;
    }
    if (z > 58500)
    {
        z = 58500;
    }
    int32_t k = min(_radius * 8 / 10, _radius - 8 - 3);
    int32_t l = 0;
    uint16_t vX = _x0 - k * cos32x100(z) / 100;
    uint16_t vY = _y0 - k * sin32x100(z) / 100;
    uint16_t _vX = _x0 - k * cos32x100(_oldZ) / 100;
    uint16_t _vY = _y0 - k * sin32x100(_oldZ) / 100;

    _oldZ = z;
    if ((vX == _vX) && (vY == _vY))
    {
        return;
    }

    needle(_x0 - 1, _y0, _vX - 1, _vY, _pGraphics->_colourBack);
    needle(_x0, _y0, _vX, _vY, _pGraphics->_colourBack);
    needle(_x0 + 1, _y0, _vX + 1, _vY, _pGraphics->_colourBack);

    needle(_x0 - 1, _y0, vX - 1, vY, _valueColour);
    needle(_x0, _y0, vX, vY, _valueColour);
    needle(_x0 + 1, _y0, vX + 1, vY, _valueColour);

    // min and max values
    _pGraphics->_pScreen->selectFont(_pGraphics->_fontLegend);
    _pGraphics->_pScreen->setFontSolid(true);
    char legend[48] = {0};
    strcpy(legend, formatString(" %+i.%0*i ", _valueMin / _unit, _formatUnit, abs(_valueMin) % _unit));
    _pGraphics->_pScreen->gText(_x0 - _radius, _y0 + _radius - _pGraphics->_pScreen->characterSizeY(), legend, _pGraphics->_colourFront, _pGraphics->_colourBack);
    strcpy(legend, formatString(" %+i.%0*i ", _valueMax / _unit, _formatUnit, abs(_valueMax) % _unit));
    _pGraphics->_pScreen->gText(_x0 + _radius - _pGraphics->_pScreen->stringSizeX(legend), _y0 + _radius - _pGraphics->_pScreen->characterSizeY(), legend, _pGraphics->_colourFront, _pGraphics->_colourBack);

    // string display
    if (strlen(aText) != 0)
    {
        _pGraphics->_pScreen->setPenSolid(false);
        _pGraphics->_pScreen->setFontSolid(true);
        _pGraphics->_pScreen->selectFont(_pGraphics->_fontValue);
        _pGraphics->_pScreen->setFontSolid(true);

        k = _pGraphics->_pScreen->stringSizeX(aText);
        l = _radius * 9 / 10 - 2 * _pGraphics->_pScreen->characterSizeY();

        if (k != _oldK)
        {
            // Label
            _pGraphics->_pScreen->setPenSolid(true);
            _pGraphics->_pScreen->dRectangle(_x0 - _radius / 2, _y0 + l - 2, _radius, _pGraphics->_pScreen->characterSizeY() + 3, _pGraphics->_colourBack);

            _pGraphics->_pScreen->setPenSolid(false);
            _pGraphics->_pScreen->dRectangle(_x0 - _radius / 2, _y0 + l - 2, _radius, _pGraphics->_pScreen->characterSizeY() + 3, _pGraphics->_colourMiddle);

            _oldK = k;
        }

        _pGraphics->_pScreen->gText(_x0 - k / 2, _y0 + l, aText, _pGraphics->_colourFront, _pGraphics->_colourBack); // , 2, 2);

    } // string display

    _pGraphics->_pScreen->setPenSolid(true);
    _pGraphics->_pScreen->circle(_x0, _y0, 3, _valueColour);
}

#endif // STRING_MODE

gYaw::gYaw(Graphics * graphics)
{
    _pGraphics = graphics;
}

void gYaw::dDefine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                   int32_t unit, uint16_t valueColour)
{
    uint16_t radius = min(dx, dy) / 2;
    define(x0 - radius, y0 - radius, radius - 2, valueColour);
}

void gYaw::define(uint16_t x0, uint16_t y0, uint16_t radius,
                  int32_t unit, uint16_t valueColour)
{
    drawDial(x0, y0, radius, valueColour);

    _oldDegrees = 0;
    _unit = unit;
    _formatUnit = 1;
    _formatUnit = getDecimalPlaces(unit);
}

void gYaw::draw(int32_t valueDegrees)
{
    int32_t value, c, s;
    uint16_t x1, x2, x3, x4, y1, y2, y3, y4;

    valueDegrees *= 100 / _unit;

    if (valueDegrees != _oldDegrees)
    {
        // old value with _colourBack
        //  radians = (-_oldDegrees / 180.0 * PI + PI);
        //  c = cos(radians) * 0.6 * _radius;
        //  s = sin(radians) * 0.6 * _radius;
        value = -_oldDegrees + 18000;
        c = cos32x100(value) * 6 / 10 * _radius / 100;
        s = sin32x100(value) * 6 / 10 * _radius / 100;

        x1 = _x0 - c * 5 / 10;
        y1 = _y0 + s * 5 / 10;
        x2 = _x0 + c * 5 / 10;
        y2 = _y0 - s * 5 / 10;
        x3 = _x0 - c * 5 / 10 - s;
        y3 = _y0 + s * 5 / 10 - c;
        x4 = _x0 + c * 5 / 10 - s;
        y4 = _y0 - s * 5 / 10 - c;

        _pGraphics->_pScreen->line(x1, y1, x3, y3, _pGraphics->_colourBack);
        _pGraphics->_pScreen->line(x3, y3, x4, y4, _pGraphics->_colourBack);
        _pGraphics->_pScreen->line(x2, y2, x4, y4, _pGraphics->_colourBack);

        x3 = _x0 - c;
        y3 = _y0 + s;
        x4 = _x0 + c;
        y4 = _y0 - s;
        _pGraphics->_pScreen->line(x1, y1, x3, y3, _pGraphics->_colourBack);
        _pGraphics->_pScreen->line(x2, y2, x4, y4, _pGraphics->_colourBack);

        x1 = _x0 + s;
        y1 = _y0 + c;
        _pGraphics->_pScreen->line(x1, y1, x3, y3, _pGraphics->_colourBack);
        _pGraphics->_pScreen->line(x1, y1, x4, y4, _pGraphics->_colourBack);

        // new value with _pGraphics->_colourFront
        //  radians = (-degrees / 180.0 * PI + PI);
        //  c = cos(radians) * 0.6 * _radius;
        //  s = sin(radians) * 0.6 * _radius;
        value = -valueDegrees + 18000;
        c = cos32x100(value) * 6 / 10 * _radius / 100;
        s = sin32x100(value) * 6 / 10 * _radius / 100;

        x1 = _x0 - c * 5 / 10;
        y1 = _y0 + s * 5 / 10;
        x2 = _x0 + c * 5 / 10;
        y2 = _y0 - s * 5 / 10;
        x3 = _x0 - c * 5 / 10 - s;
        y3 = _y0 + s * 5 / 10 - c;
        x4 = _x0 + c * 5 / 10 - s;
        y4 = _y0 - s * 5 / 10 - c;

        _pGraphics->_pScreen->line(x1, y1, x3, y3, _valueColour);
        _pGraphics->_pScreen->line(x3, y3, x4, y4, _valueColour);
        _pGraphics->_pScreen->line(x2, y2, x4, y4, _valueColour);

        x3 = _x0 - c;
        y3 = _y0 + s;
        x4 = _x0 + c;
        y4 = _y0 - s;
        _pGraphics->_pScreen->line(x1, y1, x3, y3, _valueColour);
        _pGraphics->_pScreen->line(x2, y2, x4, y4, _valueColour);

        x1 = _x0 + s;
        y1 = _y0 + c;
        _pGraphics->_pScreen->line(x1, y1, x3, y3, _valueColour);
        _pGraphics->_pScreen->line(x1, y1, x4, y4, _valueColour);

        _oldDegrees = valueDegrees;
    }

    _pGraphics->_pScreen->setPenSolid(true);
    _pGraphics->_pScreen->circle(_x0, _y0, 2, _pGraphics->_colourFront);
}

gPitch::gPitch(Graphics * graphics)
{
    _pGraphics = graphics;
}

void gPitch::dDefine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                     int32_t unit, uint16_t valueColour)
{
    uint16_t radius = min(dx, dy) / 2;
    define(x0 - radius, y0 - radius, radius - 2, unit, valueColour);
}

void gPitch::define(uint16_t x0, uint16_t y0, uint16_t radius,
                    int32_t unit, uint16_t valueColour)
{
    drawDial(x0, y0, radius, valueColour);

    //  _pScreen = screen;
    //
    //  _x0 = x0;
    //  _y0 = y0;
    //  _radius = radius;
    //  _pGraphics->_colourBack = backColour;
    //  _pGraphics->_colourFront = frontColour;
    //  _pGraphics->_colourMiddle = gridColour;
    //  _valueColour = valueColour;

    //  _pGraphics->_pScreen->setPenSolid(true);
    //  _pGraphics->_pScreen->circle(_x0, _y0, _radius, _pGraphics->_colourBack);
    //  _pGraphics->_pScreen->setPenSolid(false);
    //  _pGraphics->_pScreen->circle(_x0, _y0, _radius, _pGraphics->_colourMiddle);

    _oldDegrees = 0;
    _unit = unit;
    _formatUnit = getDecimalPlaces(unit);
}

void gPitch::draw(int32_t valueDegrees)
{
    int32_t value, c, s; // radians,
    uint16_t x1, x2, y1, y2;

    valueDegrees *= 100 / _unit;

    if (valueDegrees != _oldDegrees)
    {
        // old value with _pGraphics->_colourBack
        value = _oldDegrees;

        if (value > 18000)
        {
            value -= 36000;
        }
        if (value < -18000)
        {
            value += 36000;
        }
        //  radians = (value / 180.0 * PI);
        c = cos32x100(value) * 6 / 10 * _radius / 100;
        s = sin32x100(value) * 6 / 10 * _radius / 100;

        x1 = _x0 + c;
        y1 = _y0 - s;
        x2 = _x0 - c;

        if (x1 > x2)
        {
            y2 = x2;
            x2 = x1;
            x1 = y2;
        }
        _pGraphics->_pScreen->line(x1 - _radius / 10, y1, x2 + _radius / 10, y1, _pGraphics->_colourBack);

        x1 = _x0 + c * 7 / 10;
        x2 = _x0 - c * 7 / 10;

        if (x1 > x2)
        {
            y2 = x2;
            x2 = x1;
            x1 = y2;
        }

        x1 -= _radius / 20;
        x2 += _radius / 20;

        if ((valueDegrees >= -90) && (value <= 90))
        {
            y1 -= _radius / 6;
        }
        else
        {
            y1 += _radius / 6;
        }
        _pGraphics->_pScreen->line(x1, y1, x2, y1, _pGraphics->_colourBack);

        // new value with _pGraphics->_colourFront
        value = valueDegrees;
        if (value > 18000)
        {
            value -= 36000;
        }
        if (value < -18000)
        {
            value += 36000;
        }
        // radians = (value / 180.0 * PI);
        c = cos32x100(value) * 6 / 10 * _radius / 100;
        s = sin32x100(value) * 6 / 10 * _radius / 100;

        x1 = _x0 + c;
        y1 = _y0 - s;
        x2 = _x0 - c;

        if (x1 > x2)
        {
            y2 = x2;
            x2 = x1;
            x1 = y2;
        }
        _pGraphics->_pScreen->line(x1 - _radius / 10, y1, x2 + _radius / 10, y1, _valueColour);

        x1 = _x0 + c * 7 / 10;
        x2 = _x0 - c * 7 / 10;

        if (x1 > x2)
        {
            y2 = x2;
            x2 = x1;
            x1 = y2;
        }

        x1 -= _radius / 20;
        x2 += _radius / 20;

        if ((valueDegrees >= -90) && (value <= 90))
        {
            y1 -= _radius / 6;
        }
        else
        {
            y1 += _radius / 6;
        }
        _pGraphics->_pScreen->line(x1, y1, x2, y1, _valueColour);

        _pGraphics->_pScreen->setPenSolid(true);
        _pGraphics->_pScreen->circle(_x0, _y0, 2, _pGraphics->_colourFront);

        _oldDegrees = valueDegrees;
    }
}

gRoll::gRoll(Graphics * graphics)
{
    _pGraphics = graphics;
}

void gRoll::dDefine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                    int32_t unit, uint16_t valueColour)
{
    uint16_t radius = min(dx, dy) / 2;
    define(x0 - radius, y0 - radius, radius - 2, unit, valueColour);
}

void gRoll::define(uint16_t x0, uint16_t y0, uint16_t radius,
                   int32_t unit, uint16_t valueColour)
{
    drawDial(x0, y0, radius, valueColour);

    _oldDegrees = 0;
    _unit = unit;
    _formatUnit = getDecimalPlaces(unit);
}

void gRoll::draw(int32_t valueDegrees)
{
    int32_t value, c, s; // radians,
    // uint16_t x1, x2, y1, y2;

    valueDegrees *= 100 / _unit;

    if (valueDegrees != _oldDegrees)
    {
        // old value with _pGraphics->_colourBack
        // radians = (-_oldDegrees / 180.0 * PI + PI);
        value = -_oldDegrees + 18000;
        c = cos32x100(value) * 6 / 10 * _radius / 100;
        s = sin32x100(value) * 6 / 10 * _radius / 100;

        // x1 = _x0 - c;
        // y1 = _y0 + s;
        // x2 = _x0 + c;
        // y2 = _y0 - s;
        // _pGraphics->_pScreen->line(x1, y1, x2, y2, _pGraphics->_colourBack);

        _pGraphics->_pScreen->line(_x0 - c, _y0 + s, _x0 + c, _y0 - s, _pGraphics->_colourBack);

        // x1 = _x0 - c * 7/10 + s * 3/10;
        // y1 = _y0 + s * 7/10 + c * 3/10;
        // x2 = _x0 + c * 7/10 + s * 3/10;
        // y2 = _y0 - s * 7/10 + c * 3/10;
        // _pGraphics->_pScreen->line(x1, y1, x2, y2, _valueColour);

        _pGraphics->_pScreen->line(_x0 - c * 7 / 10 + s * 3 / 10, _y0 + s * 7 / 10 + c * 3 / 10,
                                   _x0 + c * 7 / 10 + s * 3 / 10, _y0 - s * 7 / 10 + c * 3 / 10,
                                   _pGraphics->_colourBack);

        // new value with _pGraphics->_colourFront
        // radians = (-degrees / 180.0 * PI + PI);
        // c = cos(radians) * 0.6 * _radius;
        // s = sin(radians) * 0.6 * _radius;
        value = -valueDegrees + 18000;
        c = cos32x100(value) * 6 / 10 * _radius / 100;
        s = sin32x100(value) * 6 / 10 * _radius / 100;

        _pGraphics->_pScreen->line(_x0 - c, _y0 + s, _x0 + c, _y0 - s, _valueColour);

        _pGraphics->_pScreen->line(_x0 - c * 7 / 10 + s * 3 / 10, _y0 + s * 7 / 10 + c * 3 / 10,
                                   _x0 + c * 7 / 10 + s * 3 / 10, _y0 - s * 7 / 10 + c * 3 / 10, _valueColour);

        _pGraphics->_pScreen->setPenSolid(true);
        _pGraphics->_pScreen->circle(_x0, _y0, 2, _pGraphics->_colourFront);

        _oldDegrees = valueDegrees;
    }
}

mtgPanel::mtgPanel(Graphics * graphics)
{
    _pGraphics = graphics;

    _dz = 1;
    _notFirst = false;
    _n = 0; // number of values shown
}

void mtgPanel::drawPanel(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                         uint16_t marksX, uint16_t sectionsY, bool continous)
{
    _x0 = x0;
    _y0 = y0;
    _dx = dx;
    _dy = dy;

    _continous = (continous && (_pGraphics->_pScreen->isReadable()));
    _sectionsY = sectionsY;
    _marksX = marksX;
    _gridC = 0;
    _n = 0; // number of values shown

    _pGraphics->_pScreen->setPenSolid(true);
    _pGraphics->_pScreen->dRectangle(_x0, _y0, _dx, _dy, _pGraphics->_colourBack);

    _pGraphics->_pScreen->setPenSolid(false);
    _pGraphics->_pScreen->dRectangle(_x0, _y0, _dx, _dy, _pGraphics->_colourMiddle);

    if (_sectionsY > 0)
    {
        for (uint16_t i = 1; i < _sectionsY; i += 1)
        {
            _pGraphics->_pScreen->dLine(_x0 + 1, _y0 + _dy * i / _sectionsY, _dx - 2, 1, _pGraphics->_colourMiddle);
        }
    }
}

uint16_t mtgPanel::drawContinue()
{
    uint16_t delta = 0;

    if (_dz > 1)  // step > 1
    {
        if (_continous) // continuity management
        {
            if (_n > _dx - 3 - _dz) // right-most side
            {
                _pGraphics->_pScreen->copyPasteArea(_x0 + 1 + _dz, _y0, _x0 + 1, _y0, _dx - 2 - _dz, _dy);

                delta = _dz;

                _pGraphics->_pScreen->setPenSolid(true);
                _pGraphics->_pScreen->rectangle(_x0 + _n - 1, _y0 + 1, _x0 + _dx - 2, _y0 + _dy - 2, _pGraphics->_colourBack);
                _pGraphics->_pScreen->setPenSolid(false);

                if (_sectionsY > 0) // horizontal grid
                {
                    for (uint16_t i = 1; i < _sectionsY; i += 1)
                    {
                        _pGraphics->_pScreen->dLine(_x0 + 1, _y0 + _dy * i / _sectionsY, _dx - 2, 1, _pGraphics->_colourMiddle);
                    }
                    _pGraphics->_pScreen->dRectangle(_x0, _y0, _dx, _dy, _pGraphics->_colourMiddle);
                }
            }
            else
            {
                _n += _dz;
            } // right-most side

            if (_marksX > 0) // marksX
            {
                for (uint16_t k = 0; k < _dz; k += 1)
                {
                    if (_gridC == 0)
                    {
                        _pGraphics->_pScreen->dLine(_x0 + _n - _dz + k + 1, _y0 + 1, 1, _dy - 3, _pGraphics->_colourMiddle);
                    }
                    _gridC += 1;
                    _gridC %= _marksX;
                }
            } // marksX
        }
        else // no continuity
        {
            if (_n > _dx - 3 - _dz) // right
            {
                _pGraphics->_pScreen->dLine(_x0 + _n + 2, _y0 + 1, 1, _dy - 3, _pGraphics->_colourBack);
                _notFirst = false;
                _n = 1;
            }
            else
            {
                _pGraphics->_pScreen->setPenSolid(true);
                _pGraphics->_pScreen->dRectangle(_x0 + _n + 1, _y0 + 1, _dz, _dy - 3, _pGraphics->_colourBack);
                _pGraphics->_pScreen->dLine(_x0 + _n + 2, _y0 + 1, 1, _dy - 3, _pGraphics->_colourBack);
                _pGraphics->_pScreen->setPenSolid(false);

                _n += _dz;
            } // right

            _pGraphics->_pScreen->dLine(_x0 + _n + 1, _y0 + 1, 1, _dy - 3, _pGraphics->_colourBack);
            _pGraphics->_pScreen->dLine(_x0 + _n + 2, _y0 + 1, 1, _dy - 3, _pGraphics->_colourFront);
            _pGraphics->_pScreen->dLine(_x0 + _n + 3, _y0 + 1, 1, _dy - 3, _pGraphics->_colourBack);

            if (_marksX > 0) // marksX
            {
                for (uint16_t k = 0; k < _dz; k += 1)
                {
                    if (_gridC == 0)
                    {
                        _pGraphics->_pScreen->dLine(_x0 + _lastN + k + 1, _y0 + 1, 1, _dy - 3, _pGraphics->_colourMiddle);
                    }
                    _gridC += 1;
                    _gridC %= _marksX;
                }
            } // marksX

            if (_sectionsY > 0) // sectionsY
            {
                for (uint16_t i = 1; i < _sectionsY; i += 1)
                {
                    _pGraphics->_pScreen->dLine(_x0 + _lastN + 1, _y0 + _dy * i / _sectionsY, _dz, 1, _pGraphics->_colourMiddle);
                }
            } // sectionsY
        } // continuity management
    }
    else // step = 1
    {
        if (_continous) // continuity management
        {
            if (_n > _dx - 5) // right-most side
            {
                _pGraphics->_pScreen->copyPasteArea(_x0 + 2, _y0, _x0 + 1, _y0, _dx - 3, _dy); // move
                _pGraphics->_pScreen->dLine(_x0 + _n + 1, _y0 + 1, 1, _dy - 3, _pGraphics->_colourBack); // new

                if (_sectionsY > 0) // grid
                {
                    _pGraphics->_pScreen->setPenSolid(false);
                    for (uint16_t i = 1; i < _sectionsY; i += 1)
                    {
                        _pGraphics->_pScreen->dLine(_x0 + 1, _y0 + _dy * i / _sectionsY, _dx - 2, 1, _pGraphics->_colourMiddle);
                    }
                    _pGraphics->_pScreen->dRectangle(_x0, _y0, _dx, _dy, _pGraphics->_colourMiddle);
                }
            }
            else
            {
                _n += 1;
            } // right-most side

            if (_marksX > 0) // marksX
            {
                if (_gridC == 0)
                {
                    _pGraphics->_pScreen->dLine(_x0 + _n + 1, _y0 + 1, 1, _dy - 3, _pGraphics->_colourMiddle);
                }
                _gridC += 1;
                _gridC %= _marksX;
            } // marksX
        }
        else // non-continuous
        {
            _pGraphics->_pScreen->dLine(_x0 + _n + 2, _y0 + 1, 1, _dy - 3, _pGraphics->_colourBack);

            if (_n > _dx - 5) // right
            {
                _notFirst = false;
                _n = 1;
                _pGraphics->_pScreen->dLine(_x0 + _dx - 1, _y0 + 1,  1, _dy - 3, _pGraphics->_colourMiddle);
            }
            else
            {
                _n += 1;
            } // right

            _pGraphics->_pScreen->dLine(_x0 + _n + 2, _y0 + 1, 1, _dy - 3, _pGraphics->_colourFront); // _pGraphics->_colourMiddle
            _pGraphics->_pScreen->dLine(_x0 + _n + 3, _y0 + 1, 1, _dy - 3, _pGraphics->_colourBack);

            if (_marksX > 0) // marksX
            {
                if (_gridC == 0)
                {
                    _pGraphics->_pScreen->dLine(_x0 + _n + 1, _y0 + 1, 1, _dy - 3, _pGraphics->_colourMiddle);
                }
                _gridC += 1;
                _gridC %= _marksX;
            } // marksX
        } // continuity management

        if (_sectionsY > 0) // sectionsY
        {
            for (uint16_t i = 1; i < _sectionsY; i += 1)
            {
                _pGraphics->_pScreen->point(_x0 + _n, _y0 + _dy * i / _sectionsY, _pGraphics->_colourMiddle);
            }
        }

    }

    return delta;
}

Histogram::Histogram(Graphics * graphics)
{
    _pGraphics = graphics;
}

void Histogram::define(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                       int32_t valueMin, int32_t valueMax, int32_t unit,
                       uint16_t marksX, uint16_t sectionsY, bool continous,
                       uint16_t valueColour)
{
    dDefine(x1, y1, x2 - x1, y2 - y1,
            valueMin, valueMax,
            unit,
            marksX, sectionsY, continous,
            valueColour);
}

void Histogram::dDefine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                        int32_t valueMin, int32_t valueMax, int32_t unit,
                        uint16_t marksX, uint16_t sectionsY, bool continous,
                        uint16_t valueColour)
{
    drawPanel(x0, y0, dx, dy,
              marksX, sectionsY, continous);

    _valueColour = valueColour;

    _valueMin = valueMin;
    _valueMax = valueMax;
    _unit = unit;
    _formatUnit = getDecimalPlaces(unit);

    _dz = 1;
    _notFirst = false;
    _n = 0; // number of values shown
}

void Histogram::draw(int32_t value)
{
    // Manage continuity
    uint16_t delta = drawContinue(); // continuity management

    // Calculate and display new point
    _pGraphics->_pScreen->setPenSolid(true);

    uint16_t _y = _y0 + 1 + _dy - 2 - (uint16_t)((value - _valueMin) * (_dy - 2) / (_valueMax - _valueMin));
    _y = checkRange((uint16_t)(_y), (uint16_t)(_y0 + 2), (uint16_t)(_y0 + _dy - 4));

    if (_notFirst)
    {
        _pGraphics->_pScreen->line(_x0 + _lastN - delta, _lastY, _x0 + _n, _y, _valueColour);
    }
    _pGraphics->_pScreen->dRectangle(_x0 + _n - 1, _y - 1, 3, 3, _valueColour);
    _lastY = _y;

    _notFirst = true;
    _lastN = _n;

    // Display legends and units
    _pGraphics->_pScreen->selectFont(_pGraphics->_fontLegend);
    _pGraphics->_pScreen->setFontSolid(true);

#if (STRING_MODE == USE_STRING_OBJECT)

    String textMin = formatString(" %+i.%0*i ", _valueMin / _unit, _formatUnit, abs(_valueMin) % _unit);
    String textMax = formatString(" %+i.%0*i ", _valueMax / _unit, _formatUnit, abs(_valueMax) % _unit);
    uint16_t z = max(_pGraphics->_pScreen->stringSizeX(textMin), _pGraphics->_pScreen->stringSizeX(textMax));

    if ((_n < z) or (_sectionsY > 0))
    {
        _pGraphics->_pScreen->gText(_x0, _y0, textMax, _pGraphics->_colourBack, _valueColour);
        _pGraphics->_pScreen->gText(_x0, _y0 + _dy - _pGraphics->_pScreen->characterSizeY(), textMin, _pGraphics->_colourBack, _valueColour);
    }
    // _pGraphics->_pScreen->gText(_x0, _y0 , i32toa(_valueMax, _unit, 1, 0), _pGraphics->_colourBack);
    // _pGraphics->_pScreen->gText(_x0, _y0 + _dy - _pGraphics->_pScreen->characterSizeY(), i32toa(_valueMin, _unit, 1, 0), _pGraphics->_colourBack);

#elif (STRING_MODE == USE_CHAR_ARRAY)

    char textMin[24] = {0};
    strcpy(textMin, formatString(" %+i.%0*i ", _valueMin / _unit, _formatUnit, abs(_valueMin) % _unit));
    char textMax[24] = {0};
    strcpy(textMax, formatString(" %+i.%0*i ", _valueMax / _unit, _formatUnit, abs(_valueMax) % _unit));
    uint16_t z = max(_pGraphics->_pScreen->stringSizeX(textMin), _pGraphics->_pScreen->stringSizeX(textMax));

    if ((_n < z) or (_sectionsY > 0))
    {
        _pGraphics->_pScreen->gText(_x0, _y0, textMax, _pGraphics->_colourBack, _valueColour);
        _pGraphics->_pScreen->gText(_x0, _y0 + _dy - _pGraphics->_pScreen->characterSizeY(), textMin, _pGraphics->_colourBack, _valueColour);
    }
    // _pGraphics->_pScreen->gText(_x0, _y0 , formatString("%+i", _valueMax * _unit), _pGraphics->_colourBack);
    // _pGraphics->_pScreen->gText(_x0, _y0 + _dy - _pGraphics->_pScreen->characterSizeY(), formatString("%+i", _valueMin * _unit), _pGraphics->_colourBack);

#endif // STRING_MODE

    _pGraphics->_pScreen->setPenSolid(false);
} // end Histogram

void Histogram::setStep(uint16_t step)
{
    _dz = 1;
    if (step > 0)
    {
        _dz = step;
    }
}

Channels::Channels(Graphics * graphics)
{
    _pGraphics = graphics;
}

void Channels::define(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                      int32_t valueMin, int32_t valueMax, int32_t unit,
                      uint8_t channels, uint8_t sectionsY,
                      uint16_t valueColour)
{
    dDefine(x1, y1, x2 - x1, y2 - y1,
            valueMin, valueMax, unit,
            channels, sectionsY,
            valueColour);
}

void Channels::dDefine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                       int32_t valueMin, int32_t valueMax, int32_t unit,
                       uint8_t channels, uint8_t sectionsY,
                       uint16_t valueColour)
{
    drawPanel(x0, y0, dx, dy,
              channels, sectionsY, false);

    /*
        _pGraphics->_pScreen->setPenSolid(true);
        _pGraphics->_pScreen->dRectangle(_x0, _y0, _dx, _dy, _pGraphics->_colourBack);

        _pGraphics->_pScreen->setPenSolid(false);
        _pGraphics->_pScreen->dRectangle(_x0, _y0, _dx, _dy, _pGraphics->_colourMiddle);

        if (_marksX > 0)
        {
            for (uint16_t i = 1; i < _marksX; i += 1)
            {
                _pGraphics->_pScreen->dLine(_x0 + _dx * i / _marksX + 1, _y0 + 1, 1, _dy - 2, _pGraphics->_colourMiddle);
            }
        }
        if (_sectionsY > 0)
        {
            for (uint16_t i = 1; i < _sectionsY; i += 1)
            {
                _pGraphics->_pScreen->dLine(_x0 + 1, _y0 + _dy * i / _sectionsY, _dx - 2, 1, _pGraphics->_colourMiddle);
            }
        }
    */
    _valueColour = valueColour;
    _valueMin = valueMin;
    _valueMax = valueMax;
    _unit = unit;
    _formatUnit = getDecimalPlaces(unit);
}

void Channels::draw(uint8_t channel, int32_t value)
{
    if (channel < _marksX)
    {
        uint16_t dz = _dx / _marksX;
        uint16_t z;
        int16_t y = (int16_t)(((value - _valueMin) * (_dy - 2)) / (_valueMax - _valueMin));
        y = checkRange((int16_t)y, (int16_t)0, (int16_t)(_dy - 2));

        // value
        _pGraphics->_pScreen->setPenSolid(true);
        _pGraphics->_pScreen->dRectangle(_x0 + _dx * channel / _marksX + 1, _y0 + 1, dz - 2, _dy - 2, _pGraphics->_colourBack);
        _pGraphics->_pScreen->dRectangle(_x0 + _dx * channel / _marksX + 1, _y0 + _dy - 1 - y, dz - 2, y, _valueColour);

        // sections
        if (_sectionsY > 0)
        {
            for (uint16_t i = 1; i < _sectionsY; i += 1)
            {
                _pGraphics->_pScreen->dLine(_x0 + 1, _y0 + _dy * i / _sectionsY, _dx - 2, 1, _pGraphics->_colourMiddle);
            }
        }
        // min and max
        _pGraphics->_pScreen->selectFont(_pGraphics->_fontLegend);

#if (STRING_MODE == USE_STRING_OBJECT)

        String textMin = formatString(" %+i.%0*i ", _valueMin / _unit, _formatUnit, abs(_valueMin) % _unit);
        String textMax = formatString(" %+i.%0*i ", _valueMax / _unit, _formatUnit, abs(_valueMax) % _unit);

#elif (STRING_MODE == USE_CHAR_ARRAY)

        char textMin[24] = {0};
        strcpy(textMin, formatString(" %+i.%0*i ", _valueMin / _unit, _formatUnit, abs(_valueMin) % _unit));
        char textMax[24] = {0};
        strcpy(textMax, formatString(" %+i.%0*i ", _valueMax / _unit, _formatUnit, abs(_valueMax) % _unit));

#endif // STRING_MODE

        z = max(_pGraphics->_pScreen->stringSizeX(textMin), _pGraphics->_pScreen->stringSizeX(textMax));

        if (_dx * channel / _marksX < z)
        {
            _pGraphics->_pScreen->setFontSolid();
            _pGraphics->_pScreen->gText(_x0, _y0, textMax, _pGraphics->_colourBack, _valueColour);
            _pGraphics->_pScreen->gText(_x0, _y0 + _dy - _pGraphics->_pScreen->characterSizeY(), textMin, _pGraphics->_colourBack, _valueColour);
        }
    }
    _pGraphics->_pScreen->setPenSolid(false);
} // end Channels

Multiple::Multiple(Graphics * graphics)
{
    _pGraphics = graphics;
}

#if (STRING_MODE == USE_STRING_OBJECT)

void Multiple::define(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                      String serie1, int32_t valueMin1, int32_t valueMax1, int32_t unit1, uint16_t valueColour1,
                      String serie2, int32_t valueMin2, int32_t valueMax2, int32_t unit2, uint16_t valueColour2,
                      String serie3, int32_t valueMin3, int32_t valueMax3, int32_t unit3, uint16_t valueColour3,
                      String serie4, int32_t valueMin4, int32_t valueMax4, int32_t unit4, uint16_t valueColour4,
                      uint16_t marksX, uint16_t sectionsY, bool continous)
{
    dDefine(x1, y1, x2 - x1, y2 - y1,
            serie1, valueMin1, valueMax1, unit1, valueColour1,
            serie2, valueMin2, valueMax2, unit2, valueColour2,
            serie3, valueMin3, valueMax3, unit3, valueColour3,
            serie4, valueMin4, valueMax4, unit4, valueColour4,
            marksX, sectionsY, continous);
}

void Multiple::dDefine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                       String serie1, int32_t valueMin1, int32_t valueMax1, int32_t unit1, uint16_t valueColour1,
                       String serie2, int32_t valueMin2, int32_t valueMax2, int32_t unit2, uint16_t valueColour2,
                       String serie3, int32_t valueMin3, int32_t valueMax3, int32_t unit3, uint16_t valueColour3,
                       String serie4, int32_t valueMin4, int32_t valueMax4, int32_t unit4, uint16_t valueColour4,
                       uint16_t marksX, uint16_t sectionsY, bool continous)
{
    drawPanel(x0, y0, dx, dy,
              marksX, sectionsY, continous,
              backColour, frontColour, gridColour);

    _pGraphics->_pScreen->selectFont(_pGraphics->_fontLegend);
    uint8_t n = min(6, _dx / _pGraphics->_pScreen->characterSizeX() / 4);

    _name1 = " " + serie1.substring(0, n) + " ";
    _name2 = " " + serie2.substring(0, n) + " ";
    _name3 = " " + serie3.substring(0, n) + " ";
    _name4 = " " + serie4.substring(0, n) + " ";

    _valueMin1 = valueMin1;
    _valueMin2 = valueMin2;
    _valueMin3 = valueMin3;
    _valueMin4 = valueMin4;

    _valueMax1 = valueMax1;
    _valueMax2 = valueMax2;
    _valueMax3 = valueMax3;
    _valueMax4 = valueMax4;

    _unit1 = unit1;
    _unit2 = unit2;
    _unit3 = unit3;
    _unit4 = unit4;

    _formatUnit1 = getDecimalPlaces(unit1);
    _formatUnit2 = getDecimalPlaces(unit2);
    _formatUnit3 = getDecimalPlaces(unit3);
    _formatUnit4 = getDecimalPlaces(unit4);

    _valueColour1 = valueColour1;
    _valueColour2 = valueColour2;
    _valueColour3 = valueColour3;
    _valueColour4 = valueColour4;

    _dz = 1;
    _notFirst = false;
    _n = 0; // number of values shown
}

#elif (STRING_MODE == USE_CHAR_ARRAY)

void Multiple::define(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                      char serie1[16], int32_t valueMin1, int32_t valueMax1, int32_t unit1, uint16_t valueColour1,
                      char serie2[16], int32_t valueMin2, int32_t valueMax2, int32_t unit2, uint16_t valueColour2,
                      char serie3[16], int32_t valueMin3, int32_t valueMax3, int32_t unit3, uint16_t valueColour3,
                      char serie4[16], int32_t valueMin4, int32_t valueMax4, int32_t unit4, uint16_t valueColour4,
                      uint16_t marksX, uint16_t sectionsY, bool continous)
{
    dDefine(x1, y1, x2 - x1, y2 - y1,
            serie1, valueMin1, valueMax1, unit1, valueColour1,
            serie2, valueMin2, valueMax2, unit2, valueColour2,
            serie3, valueMin3, valueMax3, unit3, valueColour3,
            serie4, valueMin4, valueMax4, unit4, valueColour4,
            marksX, sectionsY, continous);
}

void Multiple::dDefine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                       char serie1[16], int32_t valueMin1, int32_t valueMax1, int32_t unit1, uint16_t valueColour1,
                       char serie2[16], int32_t valueMin2, int32_t valueMax2, int32_t unit2, uint16_t valueColour2,
                       char serie3[16], int32_t valueMin3, int32_t valueMax3, int32_t unit3, uint16_t valueColour3,
                       char serie4[16], int32_t valueMin4, int32_t valueMax4, int32_t unit4, uint16_t valueColour4,
                       uint16_t marksX, uint16_t sectionsY, bool continous)
{
    drawPanel(x0, y0, dx, dy, marksX, sectionsY, continous);

    _pGraphics->_pScreen->selectFont(_pGraphics->_fontLegend);
    uint8_t n = min(6, _dx / _pGraphics->_pScreen->characterSizeX('M') / 4);

    strcpy(_name1, " ");
    strncat(_name1, serie1, n);
    strcat(_name1, " ");
    strcpy(_name2, " ");
    strncat(_name2, serie2, n);
    strcat(_name2, " ");
    strcpy(_name3, " ");
    strncat(_name3, serie3, n);
    strcat(_name3, " ");
    strcpy(_name4, " ");
    strncat(_name4, serie4, n);
    strcat(_name4, " ");

    _valueMin1 = valueMin1;
    _valueMin2 = valueMin2;
    _valueMin3 = valueMin3;
    _valueMin4 = valueMin4;

    _valueMax1 = valueMax1;
    _valueMax2 = valueMax2;
    _valueMax3 = valueMax3;
    _valueMax4 = valueMax4;

    _unit1 = unit1;
    _unit2 = unit2;
    _unit3 = unit3;
    _unit4 = unit4;

    _formatUnit1 = getDecimalPlaces(_unit1);
    _formatUnit2 = getDecimalPlaces(_unit2);
    _formatUnit3 = getDecimalPlaces(_unit3);
    _formatUnit4 = getDecimalPlaces(_unit4);

    _valueColour1 = valueColour1;
    _valueColour2 = valueColour2;
    _valueColour3 = valueColour3;
    _valueColour4 = valueColour4;

    _dz = 1;
    _notFirst = false;
    _n = 0; // number of values shown
}

#endif // STRING_MODE

void Multiple::draw(int32_t value1, int32_t value2, int32_t value3, int32_t value4)
{
    // Manage continuity
    uint16_t delta = drawContinue(); // continuity management

    bool flag2 = ((_valueMax2 != 0) or (_valueMin2 != 0));
    bool flag3 = ((_valueMax3 != 0) or (_valueMin3 != 0));
    bool flag4 = ((_valueMax4 != 0) or (_valueMin4 != 0));

    // Calculate and display new points
    _pGraphics->_pScreen->setPenSolid(true);

    uint16_t _y1 = _y0 + 1 + _dy - 2 - (uint16_t)((value1 - _valueMin1) * (_dy - 2) / (_valueMax1 - _valueMin1));
    _y1 = checkRange((uint16_t)(_y1), (uint16_t)(_y0 + 2), (uint16_t)(_y0 + _dy - 4));

    if (_notFirst)
    {
        _pGraphics->_pScreen->line(_x0 + _lastN - delta, _lastY1, _x0 + _n, _y1, _valueColour1);
    }
    _pGraphics->_pScreen->dRectangle(_x0 + _n - 1, _y1 - 1, 3, 3, _valueColour1);
    _lastY1 = _y1;

    if (flag2)
    {
        uint16_t _y2 = _y0 + 1 + _dy - 2 - (uint16_t)((value2 - _valueMin2) * (_dy - 2) / (_valueMax2 - _valueMin2));
        _y2 = checkRange((uint16_t)(_y2), (uint16_t)(_y0 + 2), (uint16_t)(_y0 + _dy - 4));

        if (_notFirst)
        {
            _pGraphics->_pScreen->line(_x0 + _lastN - delta, _lastY2, _x0 + _n, _y2, _valueColour2);
        }
        _pGraphics->_pScreen->dRectangle(_x0 + _n - 1, _y2 - 1, 3, 3, _valueColour2);
        _lastY2 = _y2;
    }

    if (flag3)
    {
        uint16_t _y3 = _y0 + 1 + _dy - 2 - (uint16_t)((value3 - _valueMin3) * (_dy - 2) / (_valueMax3 - _valueMin3));
        _y3 = checkRange((uint16_t)(_y3), (uint16_t)(_y0 + 2), (uint16_t)(_y0 + _dy - 4));

        if (_notFirst)
        {
            _pGraphics->_pScreen->line(_x0 + _lastN - delta, _lastY3, _x0 + _n, _y3, _valueColour3);
        }
        _pGraphics->_pScreen->dRectangle(_x0 + _n - 1, _y3 - 1, 3, 3, _valueColour3);
        _lastY3 = _y3;
    }

    if (flag4)
    {
        uint16_t _y4 = _y0 + 1 + _dy - 2 - (uint16_t)((value4 - _valueMin4) * (_dy - 2) / (_valueMax4 - _valueMin4));
        _y4 = checkRange((uint16_t)(_y4), (uint16_t)(_y0 + 2), (uint16_t)(_y0 + _dy - 4));

        if (_notFirst)
        {
            _pGraphics->_pScreen->line(_x0 + _lastN - delta, _lastY4, _x0 + _n, _y4, _valueColour4);
        }
        _pGraphics->_pScreen->dRectangle(_x0 + _n - 1, _y4 - 1, 3, 3, _valueColour4);
        _lastY4 = _y4;
    }

    _notFirst = true;
    _lastN = _n;

    // Display legends and units
    _pGraphics->_pScreen->setPenSolid(true);
    _pGraphics->_pScreen->selectFont(_pGraphics->_fontLegend);
    _pGraphics->_pScreen->setFontSolid(true);

    uint16_t _zMax = _x0;
    uint16_t _zMin = _x0;
    uint16_t _zName = _x0;

    char legend[48] = {0};

    strcpy(legend, formatString(" %+i.%0*i ", _valueMax1 / _unit1, _formatUnit1, abs(_valueMax1) % _unit1));
    _pGraphics->_pScreen->gText(_zMax, _y0, legend, _pGraphics->_colourBack, _valueColour1);
    _zMax += _pGraphics->_pScreen->stringSizeX(legend);

    strcpy(legend, formatString(" %+i.%0*i ", _valueMin1 / _unit1, _formatUnit1, abs(_valueMin1) % _unit1));
    _pGraphics->_pScreen->gText(_zMin, _y0 + _dy - _pGraphics->_pScreen->characterSizeY(), legend, _pGraphics->_colourBack, _valueColour1);
    _zMin += _pGraphics->_pScreen->stringSizeX(legend);

    _pGraphics->_pScreen->gText(_zName, _y0  + _pGraphics->_pScreen->characterSizeY(), _name1, _pGraphics->_colourBack, _valueColour1);
    _zName += _pGraphics->_pScreen->stringSizeX(_name1);

    if (flag2)
    {
        strcpy(legend, formatString(" %+i.%0*i ", _valueMax2 / _unit2, _formatUnit2, abs(_valueMax2) % _unit2));
        _pGraphics->_pScreen->gText(_zMax, _y0, legend, _pGraphics->_colourBack, _valueColour2);
        _zMax += _pGraphics->_pScreen->stringSizeX(legend);

        strcpy(legend, formatString(" %+i.%0*i ", _valueMin2 / _unit2, _formatUnit2, abs(_valueMin2) % _unit2));
        _pGraphics->_pScreen->gText(_zMin, _y0 + _dy - _pGraphics->_pScreen->characterSizeY(), legend, _pGraphics->_colourBack, _valueColour2);
        _zMin += _pGraphics->_pScreen->stringSizeX(legend);

        _pGraphics->_pScreen->gText(_zName, _y0  + _pGraphics->_pScreen->characterSizeY(), _name2, _pGraphics->_colourBack, _valueColour2);
        _zName += _pGraphics->_pScreen->stringSizeX(_name2);
    }

    if (flag3)
    {
        strcpy(legend, formatString(" %+i.%0*i ", _valueMax3 / _unit3, _formatUnit3, abs(_valueMax3) % _unit3));
        _pGraphics->_pScreen->gText(_zMax, _y0, legend, _pGraphics->_colourBack, _valueColour3);
        _zMax += _pGraphics->_pScreen->stringSizeX(legend);

        strcpy(legend, formatString(" %+i.%0*i ", _valueMin3 / _unit3, _formatUnit3, abs(_valueMin3) % _unit3));
        _pGraphics->_pScreen->gText(_zMin, _y0 + _dy - _pGraphics->_pScreen->characterSizeY(), legend, _pGraphics->_colourBack, _valueColour3);
        _zMin += _pGraphics->_pScreen->stringSizeX(legend);

        _pGraphics->_pScreen->gText(_zName, _y0  + _pGraphics->_pScreen->characterSizeY(), _name3, _pGraphics->_colourBack, _valueColour3);
        _zName += _pGraphics->_pScreen->stringSizeX(_name3);
    }

    if (flag4)
    {
        strcpy(legend, formatString(" %+i.%0*i ", _valueMax4 / _unit4, _formatUnit4, abs(_valueMax4) % _unit4));
        _pGraphics->_pScreen->gText(_zMax, _y0, legend, _pGraphics->_colourBack, _valueColour4);
        _zMax += _pGraphics->_pScreen->stringSizeX(legend);

        strcpy(legend, formatString(" %+i.%0*i ", _valueMin4 / _unit4, _formatUnit4, abs(_valueMin4) % _unit4));
        _pGraphics->_pScreen->gText(_zMin, _y0 + _dy - _pGraphics->_pScreen->characterSizeY(), legend, _pGraphics->_colourBack, _valueColour4);
        _zMin += _pGraphics->_pScreen->stringSizeX(legend);

        _pGraphics->_pScreen->gText(_zName, _y0  + _pGraphics->_pScreen->characterSizeY(), _name4, _pGraphics->_colourBack, _valueColour4);
        _zName += _pGraphics->_pScreen->stringSizeX(_name4);
    }
} // end Multiple

void Multiple::setStep(uint16_t step)
{
    _dz = 1;
    if (step > 0)
    {
        _dz = step;
    }
}

