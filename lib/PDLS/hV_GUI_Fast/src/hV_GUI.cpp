//
// hV_Haptics_GUI.cpp
// Library C++ code
// ----------------------------------
//
// Project Pervasive Displays Library Suite
// Based on highView technology
//
// Created by Rei Vilo, 14 Jun 2013
// Last updated by Rei Vilo, 25 Jan 2022
//
// Copyright (c) Rei Vilo, 2010-2023
// Licence All rights reserved
//
// See hV_Haptics_GUI.h and ReadMe.txt for references
//
// Release 507: Initial release
// Release 521: Added table and keyboard
// Release 523: Added rounded buttons
// Release 526: All elements based on area
// Release 530: Improve stability for table and keyboard
// Release 531: Improved performance
// Release 533: Added drawFormat()
// Release 540: Added checkDirect for button
// Release 540: Added customised labels for plus-minus element
// Release 541: Improved cursor
// Release 605: Improved elements link to GUI
// Release 605: Capitalised first letter of elements names
// Release 607: Improved delays consistency
// Release 607: Refactored peripherals
// Release 608: Shared common debouncing module
// Release 608: Added patterns for Table
// Release 609: Added check special mode
// Release 610: Removed partial update
// Release 610: Added continuity mode
// Release 612: Added Switch element
//

// Library header
#include "hV_GUI.h"
#include <stdarg.h>

#if (HAPTICS_MODE != USE_HAPTICS_NONE)
DRV2605L myDRV2605L(HAPTICS_MODE);
#endif // HAPTICS_MODE

#define DELAY_AFTER_RELEASE 4
#define DELAY_DEBOUNCE 8

// Continuity mode
// 0 or 1, recommended = 1
//
#define USE_CONTINUITY 1

GUI::GUI(Screen_EPD_EXT3_Fast * screen)
{
    _pScreen = screen;
}

void GUI::begin()
{
    _colourFront = myColours.black;
    _colourBack = myColours.white;
    _colourMiddle = myColours.grey;
    _delegate = true;
    _enable = true;
    _style = 0;

#if (HAPTICS_MODE != USE_HAPTICS_NONE)
    myDRV2605L.begin();
#endif // HAPTICS_MODE
}

void GUI::setColours(uint16_t frontColour, uint16_t backColour, uint16_t middleColour)
{
    _colourFront = frontColour;
    _colourBack = backColour;
    _colourMiddle = middleColour;
}

void GUI::setState(bool state)
{
    _enable = state;
}

void GUI::setStyle(uint8_t style)
{
    _style = style;
}

void GUI::delegate(bool delegate)
{
    _delegate = delegate;
}

void GUI::_beginContinuity()
{
    // _pScreen->_beginContinuity();
}

void GUI::_endContinuity()
{
    // _pScreen->_endContinuity();
}

// ---- Label
void GUI::dLabel(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
#if (STRING_MODE == USE_STRING_OBJECT)
                 String text0,
#elif (STRING_MODE == USE_CHAR_ARRAY)
                 const char * text0,
#endif // STRING_MODE
                 uint16_t textColour0, uint16_t backColour0,
                 int8_t horizontal0, int8_t vertical0, uint8_t size0, bool flush0)
{
    dRoundedLabel(x0, y0, dx, dy, 0, text0, textColour0, backColour0,
                  horizontal0, vertical0, size0, flush0);
}

void GUI::dRoundedLabel(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t radius,
#if (STRING_MODE == USE_STRING_OBJECT)
                        String text0,
#elif (STRING_MODE == USE_CHAR_ARRAY)
                        const char * text0,
#endif // STRING_MODE
                        uint16_t textColour0, uint16_t backColour0,
                        int8_t horizontal0, int8_t vertical0, uint8_t size0, bool flush0)
{
    uint8_t _size;
    uint16_t _xt, _yt;
#if (STRING_MODE == USE_STRING_OBJECT)
    String _text;
#elif (STRING_MODE == USE_CHAR_ARRAY)
    char _text[64];
#endif // STRING_MODE

    bool oldPenMode = _pScreen->getPenSolid();
    bool oldFontMode = _pScreen->getFontSolid();
    bool oldFont = _pScreen->getFont();

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
    uint16_t k = min((uint16_t)strlen(text0), (uint16_t)63);
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

    if (flush0 and _delegate)
    {
        _pScreen->flushFast();
    }

    _pScreen->setPenSolid(true);
    _pScreen->dRoundedRectangle(x0, y0, dx, dy, radius, backColour0);

    _pScreen->selectFont(_size);
    _pScreen->setFontSolid(false);
    _pScreen->gText(_xt, _yt, _text, textColour0);

    if (flush0 and _delegate)
    {
        _pScreen->flushFast();
    }

    _pScreen->setPenSolid(oldPenMode);
    _pScreen->setFontSolid(oldFontMode);
    _pScreen->selectFont(oldFont);
}

// ---- Utility
#if (STRING_MODE == USE_STRING_OBJECT)

Item setItem(uint16_t index, String text)
{
    Item anItem;
    anItem.index = index;
    memset(anItem.label, 0x00, sizeof(anItem));
    text.toCharArray(anItem.label, 15);
    return anItem;
}

#elif (STRING_MODE == USE_CHAR_ARRAY)

Item setItem(uint16_t index, const char * text)
{
    Item anItem;
    anItem.index = index;
    memset(anItem.label, 0x00, sizeof(anItem.label));
    strncpy(anItem.label, text, 15);
    return anItem;
}

#endif // STRING_MODE

// ---- Area
Area::Area(GUI * gui)
{
    _pGUI = gui;
}

void Area::dDefine(uint16_t x0, uint16_t y0,
                   uint16_t dx, uint16_t dy,
                   uint16_t index)
{
    _x0 = x0;
    _y0 = y0;
    _dx = dx;
    _dy = dy;
    _radius = _pGUI->_style;

    _index = index;
    _enable = _pGUI->_enable;
    _option = false;
}

bool Area::getState()
{
    return _enable;
}

void Area::setState(bool state)
{
    _enable = state;
}

void Area::setOption(bool option)
{
    _option = option;
}

void Area::setStyle(uint8_t style)
{
    _radius = style;
}

bool Area::check(uint8_t mode)
{
    if (!_enable)
    {
        return false;
    }
    // bool flag;

    uint16_t x, y, z, t;

    // down
    if (_pGUI->_pScreen->getTouch(x, y, z, t))
    {
        // pressed
        if ((x >= _x0) and (x < _x0 + _dx) and (y >= _y0) and (y < _y0 + _dy))
        {
#if (USE_CONTINUITY == 1)
            if (_pGUI->_delegate)
            {
                _pGUI->_beginContinuity();
            }
#endif // USE_CONTINUITY

            if (mode == checkInstant)
            {
                return true;
            }

            while (_pGUI->_pScreen->getTouch(x, y, z, t)) // Debounce
            {
                hV_HAL_delayMilliseconds(DELAY_DEBOUNCE);
            }

            // up
            if ((x >= _x0) and (x < _x0 + _dx) and (y >= _y0) and (y < _y0 + _dy))
            {
                return true;
            } // if up
#if (USE_CONTINUITY == 1)
            if (_pGUI->_delegate)
            {
                _pGUI->_endContinuity();
            }
#endif // USE_CONTINUITY

        } // if pressed
    } // if down
    return false;
}

uint16_t Area::getIndex()
{
    return _index;
}

// ---- Button
Button::Button(GUI * gui)
{
    _pGUI = gui;
}

void Button::dDefine(uint16_t x0, uint16_t y0,
                     uint16_t dx, uint16_t dy,
                     Item item0,
                     uint8_t size0)
{
    Area::dDefine(x0, y0, dx, dy, item0.index);
    _enable = _pGUI->_enable;

#if (STRING_MODE == USE_STRING_OBJECT)
    _text = item0.label;
#elif (STRING_MODE == USE_CHAR_ARRAY)
    memset(_text, 0x00, sizeof(_text));
    strncpy(_text, item0.label, 15); // last 16= \0
#endif // STRING_MODE

    _size = size0;
    _value = false;

    setOption(false);
}

#if (STRING_MODE == USE_STRING_OBJECT)

void Button::dStringDefine(uint16_t x0, uint16_t y0,
                           uint16_t dx, uint16_t dy,
                           String text0,
                           uint8_t size0)
{
    dDefine(x0, y0, dx, dy, setItem(0, text0), size0);
}

#elif (STRING_MODE == USE_CHAR_ARRAY)
void Button::dStringDefine(uint16_t x0, uint16_t y0,
                           uint16_t dx, uint16_t dy,
                           const char * text0,
                           uint8_t size0)
{
    dDefine(x0, y0, dx, dy, setItem(0, text0), size0);
}
#endif // STRING_MODE

void Button::draw(fsmGUI_e fsm)
{
    // Different options
    // enable = the button is active (true)
    // option = the button is flip-flop (true)
    // value = for flip-flop only
    // fsm = pressed and released, plus confirmed for flip-flop only

    uint16_t colourFront = _pGUI->_colourFront;
    uint16_t colourBack = _pGUI->_colourBack;

    if (_enable)
    {
        if (_option)
        {
            switch (fsm)
            {
                case fsmTouched:

                    colourFront = _pGUI->_colourFront;
                    colourBack = _pGUI->_colourMiddle;
                    break;

                case fsmConfirmed:

                    colourFront = (_value) ? _pGUI->_colourFront : _pGUI->_colourBack;
                    colourBack = (_value) ? _pGUI->_colourBack : _pGUI->_colourFront;
                    break;

                default:

                    colourFront = (_value) ? _pGUI->_colourBack : _pGUI->_colourFront;
                    colourBack = (_value) ? _pGUI->_colourFront : _pGUI->_colourBack;
                    break;
            }
        }
        else
        {
            colourFront = (fsm == fsmReleased) ? _pGUI->_colourFront : _pGUI->_colourBack;
            colourBack = (fsm == fsmReleased) ? _pGUI->_colourBack : _pGUI->_colourFront;
        }
    }
    else
    {
        switch (fsm)
        {
            case fsmTouched:

                colourFront = _pGUI->_colourFront;
                colourBack = _pGUI->_colourMiddle;
                break;

            default:

                colourFront = _pGUI->_colourMiddle;
                colourBack = _pGUI->_colourBack;
                break;
        }
    }

    _pGUI->dRoundedLabel(_x0, _y0, _dx, _dy, _radius, _text, colourFront, colourBack, 0, 0, _size, false);
    _pGUI->_pScreen->setPenSolid(false);
    if(fsm != fsmTouched)
    {
        _pGUI->_pScreen->dRoundedRectangle(_x0, _y0, _dx, _dy, _radius, colourFront);
    }
    
    if (_pGUI->_delegate)
    {
        _pGUI->_pScreen->flushFast();
    }
}

// void Button::setState(bool state)
// {
// _enable = state;
// }

bool Button::check(uint8_t mode)
{
    uint16_t x, y, z, t;
    bool flag = false;

    if (!_enable)
    {
        return false;
    }

    // down
    if (_pGUI->_pScreen->getTouch(x, y, z, t))
    {
        // pressed
        if ((x >= _x0) and (x < _x0 + _dx) and (y >= _y0) and (y < _y0 + _dy))
        {
#if (HAPTICS_MODE != USE_HAPTICS_NONE)
            myDRV2605L.playEffect(DRV2605L_SHARP_TICK_1_100);
#endif // HAPTICS_MODE

#if (USE_CONTINUITY == 1)
            if (_pGUI->_delegate)
            {
                _pGUI->_beginContinuity();
            }
#endif // USE_CONTINUITY

            // mode
            if (mode == checkInstant)
            {
                if (_option == optionFlipFlop)
                {
                    _value = (_value == false);
                    draw(fsmReleased);
                }
                flag = true;
            }
            else // checkNormal or checkDirect
            {
                draw(fsmTouched);

                uint8_t count = 0;
                if (_option == optionFlipFlop) // flip-flop
                {
                    if (mode == checkSpecial)
                    {
                        while (_pGUI->_pScreen->getTouch(x, y, z, t)) // Debounce
                        {
                            hV_HAL_delayMilliseconds(DELAY_DEBOUNCE);
                        }

                        count = 10;
                    }
                    else
                    {
                        while (_pGUI->_pScreen->getTouch(x, y, z, t)) // Debounce
                        {
                            hV_HAL_delayMilliseconds(DELAY_DEBOUNCE);
                            count += (count < 255) ? 1 : 0;
                            if (count == 5)
                            {
#if (HAPTICS_MODE != USE_HAPTICS_NONE)
                                myDRV2605L.playEffect(DRV2605L_STRONG_CLICK_100);
#endif // HAPTICS_MODE
                                draw(fsmConfirmed);
                            }
                        }
                    }

                    // Released
                    hV_HAL_delayMilliseconds(DELAY_AFTER_RELEASE);

                    if ((count > 5) and (x >= _x0) and (x < _x0 + _dx) and (y >= _y0) and (y < _y0 + _dy))
                    {
                        flag = true;
                        _value = (_value == false);
                    }
                    draw(fsmReleased); // final state
                }
                else // standard
                {
                    while (_pGUI->_pScreen->getTouch(x, y, z, t)) // Debounce
                    {
                        hV_HAL_delayMilliseconds(DELAY_DEBOUNCE);
                    }

                    // Released
                    if ((x >= _x0) and (x < _x0 + _dx) and (y >= _y0) and (y < _y0 + _dy))
                    {
                        flag = true;
                        if (_option)
                        {
                            _value = 1 - _value;
                        }
                    }
                    if ((mode == checkNormal) or (mode == checkSpecial) or (flag == false))
                    {
                        draw(fsmReleased);
                    }
                } // if flip-flop
                // draw(fsmReleased);
            } // if mode

#if (USE_CONTINUITY == 1)
            if (_pGUI->_delegate)
            {
                _pGUI->_endContinuity();
            }
#endif // USE_CONTINUITY

        } // if pressed
    } // if down

    return flag;
}

void Button::setValue(bool value)
{
    _value = value;
}

bool Button::getValue()
{
    return _value;
}

// ---- CheckBox
CheckBox::CheckBox(GUI * gui)
{
    _pGUI = gui;
}

void CheckBox::draw(fsmGUI_e fsm)
{
    // 4 states
    // enable = true, bool flag = true : pressed button
    // enable = true, bool flag = false : available button
    // enable = false, bool flag = true : statement button
    // enable = false, bool flag = false : unavailable button

    uint16_t colourFront = _pGUI->_colourFront;
    uint16_t colourBack = _pGUI->_colourBack;
    uint16_t colourCheck = _pGUI->_colourBack;
    uint16_t colourBox = _pGUI->_colourFront;

    if (_enable)
    {
        switch (fsm)
        {
            case fsmTouched:

                colourFront = _pGUI->_colourBack;
                colourBack = _pGUI->_colourFront;
                colourCheck = _pGUI->_colourMiddle;
                colourBox = colourFront;
                break;

            case fsmConfirmed:

                colourFront = _pGUI->_colourBack;
                colourBack = _pGUI->_colourFront;
                colourCheck = (_value) ? _pGUI->_colourBack : _pGUI->_colourFront;
                colourBox = (_value) ? _pGUI->_colourFront : _pGUI->_colourBack;
                break;

            default:

                colourFront = _pGUI->_colourFront;
                colourBack = _pGUI->_colourBack;
                colourCheck = (_value) ? _pGUI->_colourFront : _pGUI->_colourBack;
                colourBox = (_value) ? _pGUI->_colourBack : _pGUI->_colourFront;
                break;
        }
    }

    _pGUI->_pScreen->selectFont(_size);
    uint16_t _dz = _pGUI->_pScreen->characterSizeY();
    _dz = min(uint16_t(_dy - 16), _dz);
    uint16_t _z0 = (_dy - _dz) / 2;

    if (fsm == fsmConfirmed)
    {
        _pGUI->_pScreen->setPenSolid(true);
        _pGUI->_pScreen->dRoundedRectangle(_x0 + _z0, _y0 + _z0, _dz, _dz, _radius / 2, colourCheck);

        _pGUI->_pScreen->setPenSolid(false);
        _pGUI->_pScreen->dRoundedRectangle(_x0 + _z0 + 1, _y0 + _z0 + 1, _dz - 2, _dz - 2, _radius / 2, colourFront);
    }
    else
    {
        _pGUI->_pScreen->setPenSolid(true);
        _pGUI->_pScreen->dRoundedRectangle(_x0, _y0, _dx, _dy, _radius, colourBack);
        _pGUI->_pScreen->dRoundedRectangle(_x0 + _z0, _y0 + _z0, _dz, _dz, _radius / 2, colourCheck);

        _pGUI->dRoundedLabel(_x0 + _dy, _y0, _dx - _dy, _dy, _radius, _text, colourFront, colourBack, -1, 0, _size, false);

        _pGUI->_pScreen->setPenSolid(false);
        _pGUI->_pScreen->dRoundedRectangle(_x0 + 2, _y0 + 2, _dx - 4, _dy - 4, _radius, colourFront);
        _pGUI->_pScreen->dRoundedRectangle(_x0 + _z0 + 1, _y0 + _z0 + 1, _dz - 2, _dz - 2, _radius / 2, colourBox);
    }

    if (_pGUI->_delegate)
    {
        _pGUI->_pScreen->flushFast();
    }
}

bool CheckBox::check(uint8_t mode)
{
    uint16_t x, y, z, t;
    bool flag = false;

    if (!_enable)
    {
        return false;
    }

    // down
    if (_pGUI->_pScreen->getTouch(x, y, z, t))
    {
        // pressed
        if ((x >= _x0) and (x < _x0 + _dx) and (y >= _y0) and (y < _y0 + _dy))
        {
#if (HAPTICS_MODE != USE_HAPTICS_NONE)
            myDRV2605L.playEffect(DRV2605L_SHARP_TICK_1_100);
#endif // HAPTICS_MODE

#if (USE_CONTINUITY == 1)
            if (_pGUI->_delegate)
            {
                _pGUI->_beginContinuity();
            }
#endif // USE_CONTINUITY

            // mode
            if (mode == checkInstant)
            {
                _value = (_value == false);
                flag = true;
            }
            else
            {
                draw(fsmTouched);

                uint8_t count = 0;
                if (mode == checkSpecial)
                {
                    while (_pGUI->_pScreen->getTouch(x, y, z, t)) // Debounce
                    {
                        hV_HAL_delayMilliseconds(DELAY_DEBOUNCE);
                    }

                    count = 10;
                }
                else
                {
                    while (_pGUI->_pScreen->getTouch(x, y, z, t)) // Debounce
                    {
                        hV_HAL_delayMilliseconds(DELAY_DEBOUNCE);
                        count += (count < 255) ? 1 : 0;
                        if (count == 5)
                        {
#if (HAPTICS_MODE != USE_HAPTICS_NONE)
                            myDRV2605L.playEffect(DRV2605L_STRONG_CLICK_100);
#endif // HAPTICS_MODE
                            draw(fsmConfirmed);
                        }
                    }
                }
                // Released
                hV_HAL_delayMilliseconds(DELAY_AFTER_RELEASE);

                if ((count > 5) and (x >= _x0) and (x < _x0 + _dx) and (y >= _y0) and (y < _y0 + _dy))
                {
                    flag = true;
                    _value = (_value == false);
                }
            } // if mode

            draw(fsmReleased);

#if (USE_CONTINUITY == 1)
            if (_pGUI->_delegate)
            {
                _pGUI->_endContinuity();
            }
#endif // USE_CONTINUITY

        } // if pressed
    } // if down

    return flag;
}

// --- Plus-minus button
PlusMinus::PlusMinus(GUI * gui)
{
    _pGUI = gui;
}

void PlusMinus::dDefine(uint16_t x0, uint16_t y0,
                        uint16_t dx, uint16_t dy,
                        const char * minus, const char * plus)
{
    Area::dDefine(x0, y0, dx, dy, 0);
    _enable = _pGUI->_enable;

    memset(_labelMinus, 0x00, sizeof(_labelMinus));
    memset(_labelPlus, 0x00, sizeof(_labelPlus));
    strncpy(_labelMinus, minus, 4);
    strncpy(_labelPlus, plus, 4);

    _option = optionVertical;
    _dSlot = _dy / 2; // vertical
}

void PlusMinus::setOption(bool option)
{
    Area::setOption(option);

    if (_option == optionVertical) // vertical
    {
        _dSlot = _dy / 2;
    }
    else // horizontal
    {
        _dSlot = _dx / 2;
    }
}

void PlusMinus::draw(int8_t value)
{
    uint16_t colourFront = _pGUI->_colourFront;
    uint16_t colourBack = _pGUI->_colourBack;

    // dLabel is solid by default
    switch (value)
    {
        case -1:

            if (_option == optionVertical) // vertical
            {
                _pGUI->_pScreen->dLine(_x0 + 1, _y0 +  _dSlot - 1, _dx - 2, 1, colourBack);

                _pGUI->dRoundedLabel(_x0, _y0 +  _dSlot, _dx, _dSlot, _radius, _labelMinus, colourBack, colourFront, 0, 0, fontSizeAutomatic, false);
                _pGUI->_pScreen->setPenSolid(false);
                _pGUI->_pScreen->dRoundedRectangle(_x0 + 1, _y0 +  _dSlot + 1, _dx - 2, _dy -  _dSlot - 2, _radius, colourBack);
            }
            else // horizontal
            {
                _pGUI->_pScreen->dLine(_x0 +  _dSlot + 1, _y0 + 1, 1, _dy - 2, colourBack);

                _pGUI->dRoundedLabel(_x0, _y0,  _dSlot + 1, _dy, _radius, _labelMinus, colourBack, colourFront, 0, 0, fontSizeAutomatic, false);
                _pGUI->_pScreen->setPenSolid(false);
                _pGUI->_pScreen->dRoundedRectangle(_x0 + 1, _y0 + 1,  _dSlot - 2 + 1, _dy - 2, _radius, colourBack);
            }
            break;

        case 1:

            if (_option == optionVertical) // vertical
            {
                _pGUI->_pScreen->dLine(_x0 + 1, _y0 +  _dSlot + 1, _dx - 2, 1, colourBack);

                _pGUI->dRoundedLabel(_x0, _y0, _dx,  _dSlot + 1, _radius, _labelPlus, colourBack, colourFront, 0, 0, fontSizeAutomatic, false);
                _pGUI->_pScreen->setPenSolid(false);
                _pGUI->_pScreen->dRoundedRectangle(_x0 + 1, _y0 + 1, _dx - 2,  _dSlot - 2 + 1, _radius, colourBack);
            }
            else // horizontal
            {
                _pGUI->_pScreen->dLine(_x0 +  _dSlot - 1, _y0 + 1, 1, _dy - 2, colourBack); // colourBack);

                _pGUI->dRoundedLabel(_x0 +  _dSlot, _y0,  _dSlot, _dy, _radius, _labelPlus, colourBack, colourFront, 0, 0, fontSizeAutomatic, false);
                _pGUI->_pScreen->setPenSolid(false);
                _pGUI->_pScreen->dRoundedRectangle(_x0 +  _dSlot + 1, _y0 + 1, _dx -  _dSlot - 2, _dy - 2, _radius, colourBack);
            }
            break;

        default:

            if (_option == optionVertical) // vertical
            {
                _pGUI->dRoundedLabel(_x0, _y0, _dx,  _dSlot + 1, _radius, _labelPlus, colourFront, colourBack, 0, 0, fontSizeAutomatic, false);
                _pGUI->dRoundedLabel(_x0, _y0 +  _dSlot, _dx,  _dSlot, _radius, _labelMinus, colourFront, colourBack, 0, 0, fontSizeAutomatic, false);

                _pGUI->_pScreen->dLine(_x0 + 1, _y0 +  _dSlot, _dx - 2, 1, _pGUI->_colourMiddle);
            }
            else // horizontal
            {
                _pGUI->dRoundedLabel(_x0, _y0,  _dSlot + 1, _dy, _radius, _labelMinus, colourFront, colourBack, 0, 0, fontSizeAutomatic, false);
                _pGUI->dRoundedLabel(_x0 +  _dSlot, _y0,  _dSlot, _dy, _radius, _labelPlus, colourFront, colourBack, 0, 0, fontSizeAutomatic, false);

                _pGUI->_pScreen->dLine(_x0 +  _dSlot, _y0 + 1, 1, _dy - 2, _pGUI->_colourMiddle);
            }

            // All
            _pGUI->_pScreen->setPenSolid(false);
            _pGUI->_pScreen->dRoundedRectangle(_x0 + 1, _y0 + 1, _dx - 2, _dy - 2, _radius, colourFront);
            break;
    }

    if (_pGUI->_delegate)
    {
        _pGUI->_pScreen->flushFast();
    }
}

bool PlusMinus::check(uint8_t mode)
{
    uint16_t x, y, z, t;
    _value = 0;

    if (!_enable)
    {
        return false;
    }

    // down
    if (_pGUI->_pScreen->getTouch(x, y, z, t))
    {
        // pressed
        if ((x >= _x0) and (x < _x0 + _dx) and (y >= _y0) and (y < _y0 + _dy))
        {
#if (HAPTICS_MODE != USE_HAPTICS_NONE)
            myDRV2605L.playEffect(DRV2605L_SHARP_TICK_1_100);
#endif // HAPTICS_MODE

#if (USE_CONTINUITY == 1)
            if (_pGUI->_delegate)
            {
                _pGUI->_beginContinuity();
            }
#endif // USE_CONTINUITY

            if (_option == optionVertical) // vertical
            {
                _value = (y - _y0 < _dSlot) ? 1 : -1;
            }
            else // horizontal
            {
                _value = (x - _x0 < _dSlot) ? -1 : 1;
            }

            if (mode == checkInstant)
            {
                return true;
            }

            draw(_value);

            while (_pGUI->_pScreen->getTouch(x, y, z, t)) // Debounce
            {
                hV_HAL_delayMilliseconds(DELAY_DEBOUNCE);
            }

            // up
            if ((x >= _x0) and (x < _x0 + _dx) and (y >= _y0) and (y < _y0 + _dy))
            {
                if (_option == optionVertical) // vertical
                {
                    _value = (y - _y0 < _dSlot) ? 1 : -1;
                }
                else // horizontal
                {
                    _value = (x - _x0 < _dSlot) ? -1 : 1;
                }

                draw(0);
                return true;
            } // if up

#if (USE_CONTINUITY == 1)
            if (_pGUI->_delegate)
            {
                _pGUI->_endContinuity();
            }
#endif // USE_CONTINUITY

        } // if pressed
    } // if down
    return false;
}

int8_t PlusMinus::getValue()
{
    int8_t _work = _value;
    _value = 0;
    return _work;
}

// ---- Radio-buttons
RadioButtons::RadioButtons(GUI * gui)
{
    _pGUI = gui;
}

#if (STRING_MODE == USE_STRING_OBJECT)
void RadioButtons::dStringDefine(uint16_t x0, uint16_t y0,
                                 uint16_t dx, uint16_t dy,
                                 String button1,
                                 String button2,
                                 String button3,
                                 uint8_t size)
#elif (STRING_MODE == USE_CHAR_ARRAY)
void RadioButtons::dStringDefine(uint16_t x0, uint16_t y0,
                                 uint16_t dx, uint16_t dy,
                                 const char * button1,
                                 const char * button2,
                                 const char * button3,
                                 uint8_t size)
#endif // STRING_MODE
{
    Area::dDefine(x0, y0, dx, dy, 0);
    _item[0] = setItem(0x0001, button1);
    _item[1] = setItem(0x0002, button2);
    _items = 2;
    if (button3 != "")
    {
        _item[2] = setItem(0x0003, button3);
        _items = 3;
    }

    _size = size;
    _value = 0;
    _oldValue = 0;
    _option = optionVertical;
    _dSlot = _dy / _items; // vertical
}

void RadioButtons::dDefine(uint16_t x0, uint16_t y0,
                           uint16_t dx, uint16_t dy,
                           Item item1,
                           Item item2,
                           Item item3,
                           uint8_t size)
{
    Area::dDefine(x0, y0, dx, dy, 0);
    _enable = _pGUI->_enable;

    _item[0] = item1;
    _item[1] = item2;
    _items = 2;
    if ((item3.label != "") and (item3.index != 0))
    {
        _item[2] = item3;
        _items = 3;
    }

    _size = size;
    _value = 0;
    _oldValue = 0;
    _option = optionVertical;
    _dSlot = _dy / _items; // vertical slot
}

void RadioButtons::setOption(bool option)
{
    Area::setOption(option);

    if (_option == optionVertical) // vertical slot
    {
        _dSlot = _dy / _items;
    }
    else // horizontal slot
    {
        _dSlot = _dx / _items;
    }
}

void RadioButtons::draw(fsmGUI_e fsm)
{
    uint16_t colourFront = _pGUI->_colourFront;
    uint16_t colourBack = _pGUI->_colourBack;

    _pGUI->_pScreen->setPenSolid(true);
    _pGUI->_pScreen->dRoundedRectangle(_x0, _y0, _dx, _dy, _radius, colourBack);

    uint16_t _vLx, _vLy, _vLdx, _vLdy; // Line
    uint16_t _vTx, _vTy, _vTdx, _vTdy; // Text
    uint16_t _vCx, _vCy, _vCradius; // Circle

    // All buttons
    _pGUI->_pScreen->selectFont(_size);
    _vCradius = _pGUI->_pScreen->characterSizeY() / 2; // radius

    for (uint8_t i = 0; i < _items; i++)
    {
        if (_option == optionHorizontal)
        {
            _dSlot = _dx / _items;
            _vLx = _x0 + i * _dSlot;
            _vLy = _y0;
            _vLdx = _dSlot;
            _vLdy = _dy;
        }
        else // vertical
        {
            _dSlot = _dy / _items;
            _vLx = _x0;
            _vLy = _y0 + i * _dSlot;
            _vLdx = _dx;
            _vLdy = _dSlot;
        }

        _vCx = _vLx + _vLdy / 2;
        _vCy = _vLy + _vLdy / 2;

        _vTx = _vLx + _vLdy;
        _vTy = _vLy;
        _vTdx = _vLdx - _vLdy;
        _vTdy = _vLdy;

        _pGUI->_pScreen->setPenSolid(true);
        _pGUI->_pScreen->dRoundedRectangle(_vLx, _vLy, _vLdx, _vLdy, _radius, colourBack);
        _pGUI->dRoundedLabel(_vTx, _vTy, _vTdx, _vTdy, _radius, _item[i].label, colourFront, colourBack, -1, 0, _size, false);

        _pGUI->_pScreen->setPenSolid(false);
        _pGUI->_pScreen->circle(_vCx, _vCy, _vCradius - 1, colourFront);

        if ((_value > 0) and (_value <= _items) and (fsm == fsmReleased) and (i + 1 == _value))
        {
            // Improved readability
            _pGUI->_pScreen->setPenSolid(true);
            _pGUI->_pScreen->circle(_vCx, _vCy, _vCradius, colourBack);
            _pGUI->_pScreen->circle(_vCx, _vCy, _vCradius - 3, colourFront);

            _pGUI->_pScreen->setPenSolid(false);
            _pGUI->_pScreen->circle(_vCx, _vCy, _vCradius, colourFront); // improved circles for 16-bit MCUs
        }
    }

    _pGUI->_pScreen->setPenSolid(false);
    _pGUI->_pScreen->dRoundedRectangle(_x0 + 1, _y0 + 1, _dx - 2, _dy - 2, _radius, colourFront);

    if ((_enable) and (fsm != fsmReleased))
    {
        // Previous choice
        if ((_oldValue != _value) and (_oldValue > 0) and (_oldValue <= _items))
        {
            if (_option == optionHorizontal)
            {
                _dSlot = _dx / _items;
                _vLx = _x0 + (_oldValue - 1) * _dSlot;
                _vLy = _y0;
                _vLdx = _dSlot;
                _vLdy = _dy;
            }
            else // vertical
            {
                _dSlot = _dy / _items;
                _vLx = _x0;
                _vLy = _y0 + (_oldValue - 1) * _dSlot;
                _vLdx = _dx;
                _vLdy = _dSlot;
            }

            _vCx = _vLx + _vLdy / 2;
            _vCy = _vLy + _vLdy / 2;

            _vTx = _vLx + _vLdy;
            _vTy = _vLy;
            _vTdx = _vLdx - _vLdy;
            _vTdy = _vLdy;

            _pGUI->_pScreen->setPenSolid(true);
            _pGUI->_pScreen->circle(_vCx, _vCy, _vCradius, (fsm == fsmTouched) ? _pGUI->_colourMiddle : _pGUI->_colourBack);

            _pGUI->_pScreen->setPenSolid(false);
            _pGUI->_pScreen->circle(_vCx, _vCy, _vCradius - 1, (fsm == fsmTouched) ? _pGUI->_colourBack : _pGUI->_colourFront);
        }

        // Current choice
        if ((_oldValue != _value) and (_value > 0) and (_value <= _items))
        {
            if (_option == optionHorizontal)
            {
                _vLx = _x0 + (_value - 1) * _dSlot;
                _vLy = _y0;
                _vLdx = _dSlot;
                _vLdy = _dy;
            }
            else // vertical
            {
                _vLx = _x0;
                _vLy = _y0 + (_value - 1) * _dSlot;
                _vLdx = _dx;
                _vLdy = _dSlot;
            }

            _vCx = _vLx + _vLdy / 2;
            _vCy = _vLy + _vLdy / 2;

            _vTx = _vLx + _vLdy;
            _vTy = _vLy;
            _vTdx = _vLdx - _vLdy;
            _vTdy = _vLdy;

            _pGUI->_pScreen->setPenSolid(true);
            _pGUI->_pScreen->dRoundedRectangle(_vLx, _vLy, _vLdx, _vLdy, _radius, colourFront);
            _pGUI->dRoundedLabel(_vTx, _vTy, _vTdx, _vTdy, _radius, _item[_value - 1].label, colourBack, colourFront, -1, 0, _size, false);

            _pGUI->_pScreen->setPenSolid(true);
            _pGUI->_pScreen->dRoundedRectangle(_vLx, _vLy, _vLdy, _vLdy, _radius, _pGUI->_colourFront);

            _pGUI->_pScreen->circle(_vCx, _vCy, _vCradius, (fsm == fsmTouched) ? _pGUI->_colourMiddle : _pGUI->_colourFront);

            _pGUI->_pScreen->setPenSolid(false);
            _pGUI->_pScreen->circle(_vCx, _vCy, _vCradius - 1, _pGUI->_colourBack);

            _pGUI->_pScreen->dRoundedRectangle(_vLx + 1, _vLy + 1, _vLdx - 2, _vLdy - 2, _radius, _pGUI->_colourBack);
        }
    }

    if (_pGUI->_delegate)
    {
        _pGUI->_pScreen->flushFast();
    }
}

bool RadioButtons::check(uint8_t mode)
{
    uint16_t x, y, z, t;
    bool flag = false;
    _oldValue = _value;

    if (!_enable)
    {
        return false;
    }

    // down
    if (_pGUI->_pScreen->getTouch(x, y, z, t))
    {
        // pressed
        if ((x >= _x0) and (x < _x0 + _dx) and (y >= _y0) and (y < _y0 + _dy))
        {
            // New value
            if (_option == optionVertical) // vertical
            {
                _value = (y - _y0) / _dSlot + 1;
            }
            else
            {
                _value = (x - _x0) / _dSlot + 1;
            }

#if (HAPTICS_MODE != USE_HAPTICS_NONE)
            // if ((mode == checkInstant) or (_value != _oldValue))
            if (_value != _oldValue)
            {
                myDRV2605L.playEffect(DRV2605L_SHARP_TICK_1_100);
            }
#endif // HAPTICS_MODE

#if (USE_CONTINUITY == 1)
            if (_pGUI->_delegate)
            {
                _pGUI->_beginContinuity();
            }
#endif // USE_CONTINUITY

            // mode
            if ((mode == checkInstant) or (_value == _oldValue))
            {
                flag = true;
            }
            else if (_value != _oldValue)
            {
                draw(fsmTouched);

                uint8_t count = 0;
                if (mode == checkSpecial)
                {
                    while (_pGUI->_pScreen->getTouch(x, y, z, t)) // Debounce
                    {
                        hV_HAL_delayMilliseconds(DELAY_DEBOUNCE);
                    }

                    count = 10;
                }
                else
                {
                    while (_pGUI->_pScreen->getTouch(x, y, z, t)) // Debounce
                    {
                        hV_HAL_delayMilliseconds(DELAY_DEBOUNCE);
                        count += (count < 255) ? 1 : 0;
                        if (count == 5)
                        {
#if (HAPTICS_MODE != USE_HAPTICS_NONE)
                            myDRV2605L.playEffect(DRV2605L_STRONG_CLICK_100);
#endif // HAPTICS_MODE

                            draw(fsmConfirmed);
                        }
                    }
                }
                // Released
                hV_HAL_delayMilliseconds(DELAY_AFTER_RELEASE);

                if ((count > 5) and (x >= _x0) and (x < _x0 + _dx) and (y >= _y0) and (y < _y0 + _dy))
                {
                    flag = true;
                    if (_option == optionVertical) // vertical
                    {
                        _value = (y - _y0) / _dSlot + 1;
                    }
                    else
                    {
                        _value = (x - _x0) / _dSlot + 1;
                    }
                }
                else
                {
                    _value = _oldValue;
                }
            } // if mode

            draw(fsmReleased); // ensure clean exit

#if (USE_CONTINUITY == 1)
            if (_pGUI->_delegate)
            {
                _pGUI->_endContinuity();
            }
#endif // USE_CONTINUITY

        } // if pressed
    } // if down

    return flag;
}

void RadioButtons::setValue(uint8_t value)
{
    _value = value;
}

uint8_t RadioButtons::getValue()
{
    return _value;
}

uint16_t RadioButtons::getIndex()
{
    if (_value == 0)
    {
        return 0;
    }
    else
    {
        return _item[_value - 1].index;
    }
}

// ---- Switch
Switch::Switch(GUI * gui)
{
    _pGUI = gui;
}

void Switch::dDefine(uint16_t x0, uint16_t y0,
                     uint16_t dx, uint16_t dy,
                     Item itemLeftBottom,
                     Item itemRightTop,
                     uint8_t size0)
{
    Area::dDefine(x0, y0, dx, dy, itemLeftBottom.index);
    _index1 = itemLeftBottom.index;
    _enable = _pGUI->_enable;

#if (STRING_MODE == USE_STRING_OBJECT)
    _textLeft = item0.label;
    _textRight = item1.label;
#elif (STRING_MODE == USE_CHAR_ARRAY)
    memset(_textLeft, 0x00, sizeof(_textLeft));
    strncpy(_textLeft, itemLeftBottom.label, 15); // last 16= \0
    memset(_textRight, 0x00, sizeof(_textRight));
    strncpy(_textRight, itemRightTop.label, 15); // last 16= \0
#endif // STRING_MODE

    _size = size0;
    _value = false;

    setOption(optionHorizontal);
}

#if (STRING_MODE == USE_STRING_OBJECT)
void Switch::dStringDefine(uint16_t x0, uint16_t y0,
                           uint16_t dx, uint16_t dy,
                           String textLeftBottom,
                           String textRightTop,
                           uint8_t size)
#elif (STRING_MODE == USE_CHAR_ARRAY)
void Switch::dStringDefine(uint16_t x0, uint16_t y0,
                           uint16_t dx, uint16_t dy,
                           const char * textLeftBottom,
                           const char * textRightTop,
                           uint8_t size)
#endif // STRING_MODE
{
    dDefine(x0, y0, dx, dy, setItem(0x0001, textLeftBottom), setItem(0x0002, textRightTop), size);
}

// #if (STRING_MODE == USE_STRING_OBJECT)
//
// void Switch::dStringDefine(uint16_t x0, uint16_t y0,
//                            uint16_t dx, uint16_t dy,
//                            String text0,
//                            uint8_t size0)
// {
//     dDefine(x0, y0, dx, dy, setItem(0, text0), size0);
// }
//
// #elif (STRING_MODE == USE_CHAR_ARRAY)
// void Switch::dStringDefine(uint16_t x0, uint16_t y0,
//                            uint16_t dx, uint16_t dy,
//                            const char * text0,
//                            uint8_t size0)
// {
//     dDefine(x0, y0, dx, dy, setItem(0, text0), size0);
// }
// #endif // STRING_MODE

void Switch::draw(fsmGUI_e fsm)
{
    // 4 states
    // enable = true, bool flag = true : pressed button
    // enable = true, bool flag = false : available button
    // enable = false, bool flag = true : statement button
    // enable = false, bool flag = false : unavailable button

    // Define colours
    uint16_t colourFront = _pGUI->_colourFront;
    uint16_t colourBack = _pGUI->_colourBack;
    uint16_t colourCheck = _pGUI->_colourBack;
    uint16_t colourLeftBottom = (_value == +1) ? _pGUI->_colourBack : _pGUI->_colourFront;
    uint16_t colourRightTop = (_value == +1) ? _pGUI->_colourFront : _pGUI->_colourBack;

    if (_enable)
    {
        switch (fsm)
        {
            case fsmTouched:
            case fsmConfirmed:

                colourCheck = _pGUI->_colourMiddle;
                colourLeftBottom = (_value) ? colourCheck : _pGUI->_colourFront;
                colourRightTop = (_value) ? _pGUI->_colourFront : colourCheck;
                break;

            default:

                break;
        }
    }

    _pGUI->_pScreen->selectFont(_size);
    uint16_t _dz = _pGUI->_pScreen->characterSizeY();
    _dz = min(uint16_t(_dy - 16), _dz);

    // Draw element
    if (_option == optionHorizontal)
    {
        uint16_t _z0 = _y0 + (_dy - _dz) / 2; // active square

        uint16_t _t0 = _x0 + _dx / 2; // middle, left at _t0 - _dz, right at _t0 + _dz
        uint16_t _dt = _dx / 2 - _dz; // _dx = _dt + _dz * 2 + _dt

        if (fsm == fsmConfirmed)
        {
            _pGUI->_pScreen->setPenSolid(true);
            _pGUI->_pScreen->dRoundedRectangle(_t0 - _dz, _z0, _dz * 2, _dz, _radius / 2, _pGUI->_colourMiddle); // 2

            // left at _t0 - _dz
            _pGUI->_pScreen->dRoundedRectangle(_t0 - _dz + 2, _z0 + 2, _dz - 4, _dz - 4, _radius / 2, colourRightTop); // 2
            // right at _t0 + _dz
            _pGUI->_pScreen->dRoundedRectangle(_t0 + 2, _z0 + 2, _dz - 4, _dz - 4, _radius / 2, colourLeftBottom); // 2

            _pGUI->_pScreen->setPenSolid(false);
            _pGUI->_pScreen->dRoundedRectangle(_t0 - _dz, _z0, _dz * 2, _dz, _radius / 2, colourFront); // 2
        }
        else
        {
            _pGUI->_pScreen->setPenSolid(true);
            _pGUI->_pScreen->dRoundedRectangle(_x0, _y0, _dx, _dy, _radius, colourBack); // 1
            _pGUI->_pScreen->dRoundedRectangle(_t0 - _dz, _z0, _dz * 2, _dz, _radius / 2, colourCheck); // 2
            // left at _t0 - _dz
            _pGUI->_pScreen->dRoundedRectangle(_t0 - _dz + 2, _z0 + 2, _dz - 4, _dz - 4, _radius / 2, colourLeftBottom); // 2
            // right at _t0 + _dz
            _pGUI->_pScreen->dRoundedRectangle(_t0 + 2, _z0 + 2, _dz - 4, _dz - 4, _radius / 2, colourRightTop); // 2

            _pGUI->dRoundedLabel(_x0, _y0, _dt, _dy, _radius, _textLeft, colourFront, colourBack, 0, 0, _size, false);
            _pGUI->dRoundedLabel(_x0 + _dx - _dt, _y0, _dt, _dy, _radius, _textRight, colourFront, colourBack, 0, 0, _size, false);

            _pGUI->_pScreen->setPenSolid(false);
            _pGUI->_pScreen->dRoundedRectangle(_x0 + 2, _y0 + 2, _dx - 4, _dy - 4, _radius, colourFront); // 3
            _pGUI->_pScreen->dRoundedRectangle(_t0 - _dz, _z0, _dz * 2, _dz, _radius / 2, colourFront); // 4
        }
    }
    else // optionVertical
    {
        uint16_t _z0 = _x0 + (_dx - _dz) / 2; // active square

        uint16_t _t0 = _y0 + _dy / 2; // middle, top at _t0 - _dz, bottom at _t0 + _dz
        uint16_t _dt = _dy / 2 - _dz; // _dy = _dt + _dz * 2 + _dt

        if (fsm == fsmConfirmed)
        {
            _pGUI->_pScreen->setPenSolid(true);
            _pGUI->_pScreen->dRoundedRectangle(_z0, _t0 - _dz, _dz, _dz * 2, _radius / 2, _pGUI->_colourMiddle); // 2

            // _pGUI->_pScreen->setPenSolid(false);
            // top at _t0 - _dz
            _pGUI->_pScreen->dRoundedRectangle(_z0 + 2, _t0 - _dz + 2, _dz - 4, _dz - 4, _radius / 2, colourRightTop); // 2
            // bottom at _t0 + _dz
            _pGUI->_pScreen->dRoundedRectangle(_z0 + 2, _t0 + 2, _dz - 4, _dz - 4, _radius / 2, colourLeftBottom); // 2

            _pGUI->_pScreen->setPenSolid(false);
            _pGUI->_pScreen->dRoundedRectangle(_z0, _t0 - _dz, _dz, _dz * 2, _radius / 2, colourFront); // 2
        }
        else
        {
            _pGUI->_pScreen->setPenSolid(true);
            _pGUI->_pScreen->dRoundedRectangle(_x0, _y0, _dx, _dy, _radius, colourBack); // 1
            _pGUI->_pScreen->dRoundedRectangle(_z0, _t0 - _dz, _dz, _dz * 2, _radius / 2, colourCheck); // 2
            // left at _t0 - _dz
            _pGUI->_pScreen->dRoundedRectangle(_z0 + 2, _t0 - _dz + 2, _dz - 4, _dz - 4, _radius / 2, colourLeftBottom); // 2
            // right at _t0 + _dz
            _pGUI->_pScreen->dRoundedRectangle(_z0 + 2, _t0 + 2, _dz - 4, _dz - 4, _radius / 2, colourRightTop); // 2

            _pGUI->dRoundedLabel(_x0, _y0, _dx, _dt, _radius, _textLeft, colourFront, colourBack, 0, 0, _size, false);
            _pGUI->dRoundedLabel(_x0, _y0 + _dy - _dt, _dx, _dt, _radius, _textRight, colourFront, colourBack, 0, 0, _size, false);

            _pGUI->_pScreen->setPenSolid(false);
            _pGUI->_pScreen->dRoundedRectangle(_x0 + 2, _y0 + 2, _dx - 4, _dy - 4, _radius, colourFront); // 3
            _pGUI->_pScreen->dRoundedRectangle(_z0, _t0 - _dz, _dz, _dz * 2, _radius / 2, colourFront); // 4
        }
    }

    if (_pGUI->_delegate)
    {
        _pGUI->_pScreen->flushFast();
    }
}

bool Switch::check(uint8_t mode)
{
    uint16_t x, y, z, t;
    bool flag = false;

    if (!_enable)
    {
        return false;
    }

    // down
    if (_pGUI->_pScreen->getTouch(x, y, z, t))
    {
        // pressed
        if ((x >= _x0) and (x < _x0 + _dx) and (y >= _y0) and (y < _y0 + _dy))
        {
#if (HAPTICS_MODE != USE_HAPTICS_NONE)
            myDRV2605L.playEffect(DRV2605L_SHARP_TICK_1_100);
#endif // HAPTICS_MODE

#if (USE_CONTINUITY == 1)
            if (_pGUI->_delegate)
            {
                _pGUI->_beginContinuity();
            }
#endif // USE_CONTINUITY

            // mode
            if (mode == checkInstant)
            {
                if (_option == optionHorizontal)
                {
                    _value = ((x - _x0) > (_dx / 2)) ? +1 : -1;
                }
                else // optionVertical
                {
                    hV_HAL_Debug_println("check optionVertical");
                    _value = ((y - _y0) > (_dy / 2)) ? +1 : -1;
                }

                flag = true;
            }
            else
            {
                draw(fsmTouched);

                uint8_t count = 0;
                if (mode == checkSpecial)
                {
                    while (_pGUI->_pScreen->getTouch(x, y, z, t)) // Debounce
                    {
                        hV_HAL_delayMilliseconds(DELAY_DEBOUNCE);
                    }

                    count = 10;
                }
                else
                {
                    while (_pGUI->_pScreen->getTouch(x, y, z, t)) // Debounce
                    {
                        hV_HAL_delayMilliseconds(DELAY_DEBOUNCE);
                        count += (count < 255) ? 1 : 0;
                        if (count == 5)
                        {
#if (HAPTICS_MODE != USE_HAPTICS_NONE)
                            myDRV2605L.playEffect(DRV2605L_STRONG_CLICK_100);
#endif // HAPTICS_MODE
                            draw(fsmConfirmed);
                        }
                    }
                }
                // Released
                hV_HAL_delayMilliseconds(DELAY_AFTER_RELEASE);

                if ((count > 5) and (x >= _x0) and (x < _x0 + _dx) and (y >= _y0) and (y < _y0 + _dy))
                {
                    flag = true;
                    if (_option == optionHorizontal)
                    {
                        _value = ((x - _x0) > (_dx / 2)) ? +1 : -1;
                    }
                    else // optionVertical
                    {
                        _value = ((y - _y0) > (_dy / 2)) ? +1 : -1;
                    }
                }
            } // if mode

            draw(fsmReleased);

#if (USE_CONTINUITY == 1)
            if (_pGUI->_delegate)
            {
                _pGUI->_endContinuity();
            }
#endif // USE_CONTINUITY

        } // if pressed
    } // if down

    return flag;
}

int8_t Switch::getValue()
{
    // false = left, right = true
    return _value;
}

uint16_t Switch::getIndex()
{
    return (_value == 1) ? _index1 : _index;
}

// ---- Cursor
Cursor::Cursor(GUI * gui)
{
    _pGUI = gui;
}

void Cursor::dDefine(uint16_t x0, uint16_t y0,
                     uint16_t dx, uint16_t dy,
                     int32_t minValue, int32_t maxValue)
{
    Area::dDefine(x0, y0, dx, dy, 0);

    _minValue = minValue;
    _maxValue = maxValue;
    _gPosition = x0;

    setOption(false);
    _enable = _pGUI->_enable;
}

void Cursor::setOption(bool option)
{
    Area::setOption(option);

    if (_option == optionVertical) // vertical
    {
        _gMin = _y0 + 8;
        _gMax = _y0 + _dy - 8;
        _gLine = _x0 + _dx / 2;
    }
    else // horizontal
    {
        _gMin = _x0 + 8;
        _gMax = _x0 + _dx - 8;
        _gLine = _y0 + _dy / 2;
    }

    if (_gMin > _gMax)
    {
        swap(_gMin, _gMax);
    }
    _gPosition = _gMin;
}

void Cursor::draw(fsmGUI_e fsm)
{
    // 4 states
    // enable = true, bool flag = true : pressed button
    // enable = true, bool flag = false : available button
    // enable = false, bool flag = true : statement button
    // enable = false, bool flag = false : unavailable button

    uint16_t colourFront = _pGUI->_colourFront;
    uint16_t colourBack = _pGUI->_colourBack;

    if (_enable)
    {
        switch (fsm)
        {
            case fsmTouched:
            case fsmConfirmed:

                colourBack = _pGUI->_colourFront;
                colourFront = _pGUI->_colourBack;
                break;

            default:

                break;
        }

        if ((fsm == fsmConfirmed) and (_gOldPosition != _gPosition))
        {
            _pGUI->_pScreen->setPenSolid(true);
            if (_option == optionVertical) // vertical
            {
                _pGUI->_pScreen->dRoundedRectangle(_x0 + 4, _gOldPosition - 3, _dx - 8, 6, _radius, colourBack);
                _pGUI->_pScreen->dRoundedRectangle(_gLine, _gMin, 2, _gMax - _gMin, _radius, colourFront);
                _pGUI->_pScreen->dRoundedRectangle(_x0 + 4, _gPosition - 3, _dx - 8, 6, _radius, colourFront);
            }
            else // horizontal
            {
                _pGUI->_pScreen->dRoundedRectangle(_gOldPosition - 3, _y0 + 4, 6, _dy - 8, _radius, colourBack);
                _pGUI->_pScreen->dRoundedRectangle(_gMin, _gLine, _gMax - _gMin, 2, _radius, colourFront);
                _pGUI->_pScreen->dRoundedRectangle(_gPosition - 3, _y0 + 4, 6, _dy - 8, _radius, colourFront);
            }
        }
        else
        {
            _pGUI->_pScreen->setPenSolid(true);
            _pGUI->_pScreen->dRoundedRectangle(_x0, _y0, _dx, _dy, _radius, colourBack);
            _pGUI->_pScreen->setPenSolid(false);
            _pGUI->_pScreen->dRoundedRectangle(_x0 + 2, _y0 + 2, _dx - 4, _dy - 4, _radius, colourFront);

            _pGUI->_pScreen->setPenSolid(true);
            if (_option == optionVertical) // vertical
            {
                _pGUI->_pScreen->dRoundedRectangle(_gLine, _gMin, 2, _gMax - _gMin, _radius, colourFront);
                _pGUI->_pScreen->dRoundedRectangle(_x0 + 4, _gPosition - 3, _dx - 8, 6, _radius, colourFront);
            }
            else // horizontal
            {
                _pGUI->_pScreen->dRoundedRectangle(_gMin, _gLine, _gMax - _gMin, 2, _radius, colourFront);
                _pGUI->_pScreen->dRoundedRectangle(_gPosition - 3, _y0 + 4, 6, _dy - 8, _radius, colourFront);
            }
        }
        if (_pGUI->_delegate)
        {
            _pGUI->_pScreen->flushFast();
        }
    }
}

void Cursor::playHaptic(uint16_t zMin, uint16_t zMax, uint16_t z)
{
    if ((z < zMin) or (z > zMax))
    {
        return;
    }
    // zMin < z < zMax
    z -= zMin; // zMix turned into 0
    zMax -= zMin; // zMax turned into dz

#if (HAPTICS_MODE != USE_HAPTICS_NONE)
    if (z == 0)
    {
        myDRV2605L.playEffect(DRV2605L_SHORT_DOUBLE_SHARP_TICK_1_100);
    }
    else if (z == zMax)
    {
        myDRV2605L.playEffect(DRV2605L_SHORT_DOUBLE_CLICK_STRONG_1_100);
    }
    else if (5 * z > 4 * zMax)
    {
        myDRV2605L.playEffect(DRV2605L_BUZZ_1_100);
    }
    else if (5 * z > 3 * zMax)
    {
        myDRV2605L.playEffect(DRV2605L_BUZZ_2_80);
    }
    else if (5 * z > 2 * zMax)
    {
        myDRV2605L.playEffect(DRV2605L_BUZZ_3_60);
    }
    else if (5 * z > 1 * zMax)
    {
        myDRV2605L.playEffect(DRV2605L_BUZZ_4_40);
    }
    else
    {
        myDRV2605L.playEffect(DRV2605L_BUZZ_5_20);
    }
#endif // HAPTICS_MODE
}

bool Cursor::check(uint8_t mode)
{
    uint16_t x, y, z, t;
    bool flag = false;

    if (!_enable)
    {
        return false;
    }

    // down
    if (_pGUI->_pScreen->getTouch(x, y, z, t))
    {
        // pressed
        if ((x >= _x0) and (x < _x0 + _dx) and (y >= _y0) and (y < _y0 + _dy))
        {
            // Better without initial effect
            // #if (HAPTICS_MODE != USE_HAPTICS_NONE)
            // myDRV2605L.playEffect(DRV2605L_SHARP_TICK_1_100);
            // #endif
            _gOldPosition = _gPosition;

#if (USE_CONTINUITY == 1)
            if (_pGUI->_delegate)
            {
                _pGUI->_beginContinuity();
            }
#endif // USE_CONTINUITY

            // mode
            if (mode == checkInstant)
            {
                if (_option == optionVertical) // vertical
                {
                    if (y != _gOldPosition)
                    {
                        y = checkRange((uint16_t)(y), (uint16_t)(_gMin), (uint16_t)(_gMax));
                        _gPosition = y;
                        flag = true;
                    }
                }
                else // horizontal
                {
                    if (x != _gOldPosition)
                    {
                        x = checkRange((uint16_t)(x), (uint16_t)(_gMin), (uint16_t)(_gMax));
                        _gPosition = x;
                        flag = true;
                    }
                }
            }
            else
            {
                draw(fsmTouched);

                while (_pGUI->_pScreen->getTouch(x, y, z, t)) // Debounce
                {
                    if (_option == optionVertical) // vertical
                    {
                        y = checkRange((uint16_t)(y), (uint16_t)(_gMin), (uint16_t)(_gMax));
                    }
                    else // horizontal
                    {
                        x = checkRange((uint16_t)(x), (uint16_t)(_gMin), (uint16_t)(_gMax));
                    }

                    if ((x >= _x0) and (x < _x0 + _dx) and (y >= _y0) and (y < _y0 + _dy))
                    {
                        flag = true;
                        if (_option == optionVertical) // vertical
                        {
                            if (y != _gPosition)
                            {
                                // playHaptic(min(_gMin, _gMax), max(_gMin, _gMax), y);
                                playHaptic(_gMin, _gMax, y);
                                _gPosition = y;
                            }
                        }
                        else // horizontal
                        {
                            if (x != _gPosition)
                            {
                                // playHaptic(min(_gMin, _gMax), max(_gMin, _gMax), x);
                                playHaptic(_gMin, _gMax, x);
                                _gPosition = x;
                            }
                        }
                    }
                    else // out of range
                    {
                        _gPosition = _gOldPosition;
                    }

                    if (_gPosition != _gOldPosition)
                    {
                        draw(fsmConfirmed);
                        _gOldPosition = _gPosition;
                    }
                    hV_HAL_delayMilliseconds(DELAY_DEBOUNCE);
                }

                // up
                if (z == TOUCH_EVENT_RELEASE)
                {
                    if ((x >= _x0) and (x < _x0 + _dx) and (y >= _y0) and (y < _y0 + _dy))
                    {
                        flag = true;

                        if (_option == optionVertical) // vertical
                        {
                            if (y != _gOldPosition)
                            {
                                _gPosition = y;
                                flag = true;
                            }
                        }
                        else // horizontal
                        {
                            if (x != _gOldPosition)
                            {
                                _gPosition = x;
                                flag = true;
                            }
                        }
                    }
                    else
                    {
                        _gPosition = _gOldPosition;
                    }
                } // if up
            } // if mode
            draw(fsmReleased);

#if (USE_CONTINUITY == 1)
            if (_pGUI->_delegate)
            {
                _pGUI->_endContinuity();
            }
#endif // USE_CONTINUITY

        } // if pressed
    } // if down

    return flag;
}

void Cursor::setValue(int32_t value)
{
    value = checkRange((int32_t)(value), (int32_t)(_minValue), (int32_t)(_maxValue));

    _gPosition = hV_HAL_map(value * 10, _minValue * 10, _maxValue * 10, _gMin * 10, _gMax * 10);
    _gPosition = (_gPosition + 5) / 10;
}

int32_t Cursor::getValue()
{
    int32_t result = hV_HAL_map(_gPosition * 10, _gMin * 10, _gMax * 10, _minValue * 10, _maxValue * 10);
    return (result + 5) / 10;
}

// ---- TextBox
TextBox::TextBox(GUI * gui)
{
    _pGUI = gui;
}

void TextBox::dDefine(
    uint16_t x0, uint16_t y0,
    uint16_t dx, uint16_t dy,
    uint8_t size)
{
    Area::dDefine(x0, y0, dx, dy, 0);
    _enable = false;

    _size = size;
    _pGUI->_pScreen->selectFont(size);
    // _tx = _pGUI->_pScreen->characterSizeX('M') + 0;
    _ty = _pGUI->_pScreen->characterSizeY(); // +1;
    _maxX = dx; // _tx - 0;
    _maxY = dy - _ty; // _ty - 1;

    setOption(false);
}

void TextBox::setOption(bool option)
{
    Area::setOption(option);

    _z0 = option ? 4 : 0;
    _indexLine = _z0;
    _indexColumn = _z0;
}

void TextBox::clear()
{
    _pGUI->_pScreen->setPenSolid(true);
    _pGUI->_pScreen->dRoundedRectangle(_x0, _y0, _dx, _dy, _radius, _pGUI->_colourBack);

    if (_option)
    {
        _pGUI->_pScreen->setPenSolid(false);
        _pGUI->_pScreen->dRoundedRectangle(_x0, _y0, _dx, _dy, _radius, _pGUI->_colourFront);
    }

    _indexLine = _z0;
    _indexColumn = _z0;

    if (_pGUI->_delegate)
    {
        _pGUI->_pScreen->flushFast();
    }
}

void TextBox::scroll()
{
    uint16_t _d2z = 2 * _z0;

    if (_indexLine + _ty > _maxY - _d2z)
    {
        if (_pGUI->_delegate)
        {
            _pGUI->_pScreen->flushFast();
        }

        _pGUI->_pScreen->copyPasteArea(_x0 + _z0, _y0 + _ty + _z0,
                                       _x0 + _z0, _y0 + _z0,
                                       _dx - _d2z, _dy - _ty - _z0);

        _pGUI->_pScreen->setPenSolid(true);
        _pGUI->_pScreen->rectangle(_x0, _y0 + _dy - _ty - 1, _x0 + _dx - 1, _y0 + _dy - 1, _pGUI->_colourBack);

        if (_option)
        {
            _pGUI->_pScreen->setPenSolid(false);
            _pGUI->_pScreen->dRoundedRectangle(_x0, _y0, _dx, _dy, _radius, _pGUI->_colourFront);
        }

        if (_pGUI->_delegate)
        {
            _pGUI->_pScreen->flushFast();
        }
    }
    else
    {
        _indexLine += _ty;
    }
    _indexColumn = _z0;
}

#if (STRING_MODE == USE_STRING_OBJECT)

void TextBox::draw(String text)
{
    // Get current font
    uint8_t oldSize = _pGUI->_pScreen->getFont();
    // Use defined font for text box
    _pGUI->_pScreen->selectFont(_size);

    int8_t _index, _indexSpace, _indexClear, _indexNewLine;
    String _workString;
    uint8_t _workX;

    _pGUI->_pScreen->selectFont(_size);
    _pGUI->_pScreen->setFontSolid(true);
    //_pScreen->setBackGroundColour(_colourBack);

    uint16_t _d2z = 2 * _z0;

    while (text.length())
    {
        _index = text.length();
        _indexClear = text.indexOf(0x0d);
        if (_indexClear < 0)
        {
            _indexSpace = text.indexOf(' ');
            _indexNewLine = text.indexOf(0x0a);
            if (_indexSpace < 0)
            {
                _indexSpace = text.length();
            }
            if (_indexNewLine < 0)
            {
                _indexNewLine = text.length();
            }
            _index = min(_indexSpace, _indexNewLine);

            _workString = text.substring(0, _index);
            _workX = _pGUI->_pScreen->stringSizeX(_workString);

            if (_indexColumn + _workX > _maxX - _d2z)
            {
                bool oldDelegateGUI = _pGUI->_delegate;
                _pGUI->_delegate = false;
                scroll();
                _pGUI->_delegate = oldDelegateGUI;
            }

            _pGUI->_pScreen->gText(_x0 + _indexColumn, _y0 + _indexLine, _workString, _pGUI->_colourFront, _pGUI->_colourBack);
            _indexColumn += _workX;

            _workX = _pGUI->_pScreen->characterSizeX(' ');
            if (_indexColumn + _workX > _maxX - _d2z)
            {
                // No space to start the line
            }
            else
            {
                _pGUI->_pScreen->gText(_x0 + _indexColumn, _y0 + _indexLine, " ", _pGUI->_colourFront, _pGUI->_colourBack);
                _indexColumn += _workX;
            }
        }
        else
        {
            clear();
        }
        text = text.substring(_index + 1);
    }
    _pGUI->_pScreen->selectFont(oldSize);

    if (_option)
    {
        _pGUI->_pScreen->setPenSolid(false);
        _pGUI->_pScreen->dRoundedRectangle(_x0, _y0, _dx, _dy, _radius, _pGUI->_colourFront);
    }
    if (_pGUI->_delegate)
    {
        _pGUI->_pScreen->flushFast();
    }
}

#elif (STRING_MODE == USE_CHAR_ARRAY)

void TextBox::draw(const char * text)
{
    // Get current font
    uint8_t oldSize = _pGUI->_pScreen->getFont();
    // Use defined font for text box
    _pGUI->_pScreen->selectFont(_size);

    char workString[64];
    uint16_t _workX;
    uint16_t indexFrom = 0;

    _pGUI->_pScreen->selectFont(_size);
    _pGUI->_pScreen->setFontSolid(true);
    //_pScreen->setBackGroundColour(_colourBack);

    uint16_t _d2z = 2 * _z0;

    for (uint16_t index = 0; index < strlen(text) + 1; index++)
    {
        char current = text[index];

        if (current == 0x0d) // \c = clear
        {
            clear();
            indexFrom = index + 1;
        }
        else if ((current == 0x20) || (current == 0x0a) || (index == strlen(text))) // space, new line, end of text
        {
            memset(workString, 0x00, sizeof(workString));
            strncpy(workString, text + indexFrom, index - indexFrom);
            indexFrom = index + 1;

            _workX = _pGUI->_pScreen->stringSizeX(workString);

            if (_indexColumn + _workX > _maxX - _d2z)
            {
                scroll();
            }

            _pGUI->_pScreen->gText(_x0 + _indexColumn, _y0 + _indexLine, workString, _pGUI->_colourFront, _pGUI->_colourBack);
            _indexColumn += _workX;

            if (current == 0x20) // space
            {
                _workX = _pGUI->_pScreen->characterSizeX(' ');
                if ((_indexColumn + _workX > _maxX - _d2z) || (index == strlen(text)))
                {
                    // No space to start the line
                    // No added space at end of text
                }
                else
                {
                    _pGUI->_pScreen->gText(_x0 + _indexColumn, _y0 + _indexLine, " ", _pGUI->_colourFront, _pGUI->_colourBack);
                    _indexColumn += _workX;
                }
            }
            else if (current == 0x0a) // \n = new line
            {
                scroll();
            }
        }
    }

    _pGUI->_pScreen->selectFont(oldSize);

    if (_option == optionWithFrame)
    {
        _pGUI->_pScreen->setPenSolid(false);
        _pGUI->_pScreen->dRoundedRectangle(_x0, _y0, _dx, _dy, _radius, _pGUI->_colourFront);
    }
    if (_pGUI->_delegate)
    {
        _pGUI->_pScreen->flushFast();
    }
}
#endif // STRING_MODE

void TextBox::drawFormat(const char * format, ...)
{
    char work[128] = { 0x00 };

    va_list args;
    va_start(args, format);
    vsnprintf(work, 127, format, args);
    va_end(args);

    draw(work);
}

// --- text
Text::Text(GUI * gui)
{
    _pGUI = gui;
}

void Text::setOption(bool option)
{
    Area::setOption(option);
}

void Text::dDefine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                   uint8_t horizontal0, uint8_t vertical0,
                   uint8_t size)
{
    Area::dDefine(x0, y0, dx, dy);
    _enable = false;

    _horizontal = horizontal0;
    _vertical = vertical0;
    _fontSize = size;

#if (STRING_MODE == USE_STRING_OBJECT)

    _text = "";

#elif (STRING_MODE == USE_CHAR_ARRAY)

    memset(_text, 0x00, sizeof(_text));

#endif // STRING_MODE
}

#if (STRING_MODE == USE_STRING_OBJECT)

void Text::draw(String text)
{
    if (text != _text)
    {
        _text = text;
        dRoundedLabel(Area::_x0 + 1, Area::_y0 + 1, Area::_dx - 2, Area::_dy - 2, _radius,
                      _text,
                      _pGUI->_colourFront, _pGUI->_colourBack,
                      _horizontal, _vertical,
                      _fontSize, false);

        _pGUI->_pScreen->setPenSolid(false);
        _pGUI->_pScreen->dRoundedRectangle(Area::_x0, Area::_y0, Area::_dx, Area::_dy, _radius, (Area::_option == optionWithFrame) ? _pGUI->_colourFront : _pGUI->_colourBack);

        if (_pGUI->_delegate)
        {
            _pGUI->_pScreen->flushFast();
        }
    }
}

#elif (STRING_MODE == USE_CHAR_ARRAY)

void Text::draw(const char * text)
{
    if (strcmp(text, _text) != 0)
    {
        memset(_text, 0x00, sizeof(_text));
        strncpy(_text, text, 31);
        _pGUI->dRoundedLabel(_x0 + 1, _y0 + 1, _dx - 2, _dy - 2, _radius,
                             _text,
                             _pGUI->_colourFront, _pGUI->_colourBack,
                             _horizontal, _vertical,
                             _fontSize, false);

        _pGUI->_pScreen->setPenSolid(false);
        _pGUI->_pScreen->dRoundedRectangle(_x0, _y0, _dx, _dy, _radius, (_option == optionWithFrame) ? _pGUI->_colourFront : _pGUI->_colourBack);

        if (_pGUI->_delegate)
        {
            _pGUI->_pScreen->flushFast();
        }
    }
}

#endif // STRING_MODE

void Text::drawFormat(const char * format, ...)
{
    char work[128] = { 0x00 };

    va_list args;
    va_start(args, format);
    vsnprintf(work, 127, format, args);
    va_end(args);

    draw(work);
}

// --- BarGraph
BarGraph::BarGraph(GUI * gui)
{
    _pGUI = gui;
}

void BarGraph::dDefine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                       int32_t minValue, int32_t maxValue)
{
    Area::dDefine(x0, y0, dx, dy);
    _enable = false;

    _minValue = minValue;
    _maxValue = maxValue;

    if (_minValue > _maxValue)
    {
        swap(_minValue, _maxValue);
    }

    _value = minValue - 1;
    _oldSection = -1;
}

void BarGraph::setOption(bool option)
{
    Area::setOption(option);
}

void BarGraph::draw(int32_t value)
{
    value = checkRange((int32_t)(value), (int32_t)(_minValue), (int32_t)(_maxValue));

    if (value == _value)
    {
        return;
    }

    if (_oldSection == -1)
    {
        _pGUI->_pScreen->setPenSolid(true);
        _pGUI->_pScreen->dRoundedRectangle(_x0, _y0, _dx, _dy, _radius, _pGUI->_colourBack);
        _pGUI->_pScreen->setPenSolid(false);
        // _pGUI->_pScreen->dRoundedRectangle(_x0, _y0, _dx, _dy, _radius, _pGUI->_colourFront);
        _pGUI->_pScreen->dRoundedRectangle(_x0, _y0, _dx, _dy, _radius, _pGUI->_colourMiddle);
        _oldSection = 0;
    }

    int16_t section;
    if (_option == optionVertical) // vertical
    {
        section = hV_HAL_map(value, _minValue, _maxValue, 0, _dy - 5);
    }
    else // horizontal
    {
        section = hV_HAL_map(value, _minValue, _maxValue, 0, _dx - 5);
    }

    if (section != _oldSection)
    {
        _pGUI->_pScreen->setPenSolid(true);
        if (_option == optionVertical) // vertical
        {
            _pGUI->_pScreen->dRoundedRectangle(_x0 + 2, _y0 + 2, _dx - 4, section, _radius, _pGUI->_colourMiddle);
            _pGUI->_pScreen->dRoundedRectangle(_x0 + 2, _y0 + 2 + section, _dx - 4, _dy - 4 - section + 1, _radius, _pGUI->_colourBack);
            _pGUI->_pScreen->dLine(_x0 + 2, _y0 + 2 + section, _dx - 4, 1, _pGUI->_colourFront);
        }
        else // horizontal
        {
            if (section > _oldSection)
            {
                for (uint16_t k = _oldSection; k < section; k++)
                {
                    _pGUI->_pScreen->dLine(_x0 + 2 + k, _y0 + 2, 1, _dy - 4, _pGUI->_colourMiddle);
                }
            }
            else
            {
                for (uint16_t k = _oldSection; k > section; k--)
                {
                    _pGUI->_pScreen->dLine(_x0 + 2 + k, _y0 + 2, 1, _dy - 4, _pGUI->_colourBack);
                }
            }
            _pGUI->_pScreen->dLine(_x0 + 2 + section, _y0 + 2, 1, _dy - 4, _pGUI->_colourFront);
        }
        _value = value;
    }

    if (_pGUI->_delegate)
    {
        _pGUI->_pScreen->flushFast();
    }

    _oldSection = section;
}

// ---- Table
Table::Table(GUI * gui)
{
    _pGUI = gui;
}

void Table::dDefine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                    uint8_t columns, uint8_t lines)
{
    _x0 = x0;
    _y0 = y0;
    _columns = columns;
    _lines = lines;
    _dx = dx;
    _dy = dy;
    _radius = _pGUI->_style;

    _enable = false;
}

void Table::drawInitial()
{
    _pGUI->_pScreen->setPenSolid(true);
    _pGUI->_pScreen->dRoundedRectangle(_x0, _y0, _dx, _dy, _radius, _pGUI->_colourBack);

    // Internal lines
    for (uint8_t i = 1; i < _columns; i++)
    {
        _pGUI->_pScreen->dLine(_x0 + _dx * i / _columns, _y0, 1, _dy, _pGUI->_colourMiddle);
    }
    for (uint8_t j = 1; j < _lines; j++)
    {
        _pGUI->_pScreen->dLine(_x0, _y0 + _dy * j / _lines, _dx, 1, _pGUI->_colourMiddle);
    }
    // External lines
    _pGUI->_pScreen->setPenSolid(false);
    _pGUI->_pScreen->dRoundedRectangle(_x0, _y0, _dx, _dy, _radius, _pGUI->_colourFront);

    if (_pGUI->_delegate)
    {
        _pGUI->_pScreen->flushFast();
    }
}

void Table::draw(uint8_t indexColumn, uint8_t indexLine, uint16_t colour, uint8_t pattern)
{
    if ((indexColumn < _columns) and (indexLine < _lines))
    {
        _pGUI->_pScreen->setPenSolid(true);
        // _pGUI->_pScreen->dRoundedRectangle(_x0 + _dx * indexColumn / _columns + 3, _y0 + _dy * indexLine / _lines + 3, _dx / _columns - 6, _dy / _lines - 6, _radius, colour);

        // uint16_t _dt = min(_dx / _columns, _dy / _lines) / 8; // thickness
        uint16_t _dt = 8; // thickness
        uint16_t _dz = min(_dx / _columns, _dy / _lines) / 2 - _dt;
        switch (pattern)
        {
            case patternSquareSolid:
                _pGUI->_pScreen->dRoundedRectangle(_x0 + _dx * indexColumn / _columns + _dt, _y0 + _dy * indexLine / _lines + _dt, _dx / _columns - _dt * 2, _dy / _lines - _dt * 2, _radius, colour);
                break;

            case patternTriangleSolid:
                _pGUI->_pScreen->triangle(_x0 + _dx * indexColumn / _columns + _dt,
                                          _y0 + _dy * indexLine / _lines + _dt + _dy / _lines - _dt * 2,
                                          _x0 + _dx * indexColumn / _columns + _dt + _dx / _columns - _dt * 2,
                                          _y0 + _dy * indexLine / _lines + _dt + _dy / _lines - _dt * 2,
                                          _x0 + _dx * indexColumn / _columns + _dt + _dx / _columns / 2 - _dt,
                                          _y0 + _dy * indexLine / _lines + _dt,
                                          colour);
                break;

            case patternCircleSolid:
                _pGUI->_pScreen->circle(_x0 + _dx * indexColumn / _columns + _dx / _columns / 2, _y0 + _dy * indexLine / _lines + _dy / _lines / 2, _dz, colour);
                _pGUI->_pScreen->circle(_x0 + _dx * indexColumn / _columns + _dx / _columns / 2, _y0 + _dy * indexLine / _lines + _dy / _lines / 2, _dz, colour);
                break;

            case patternSquare:
                _pGUI->_pScreen->dRoundedRectangle(_x0 + _dx * indexColumn / _columns + _dt, _y0 + _dy * indexLine / _lines + _dt, _dx / _columns - _dt * 2, _dy / _lines - _dt * 2, _radius, colour);
                _pGUI->_pScreen->dRoundedRectangle(_x0 + _dx * indexColumn / _columns + _dt * 2, _y0 + _dy * indexLine / _lines + _dt * 2, _dx / _columns - _dt * 4, _dy / _lines - _dt * 4, _radius, _pGUI->_colourBack);
                break;

            case patternTriangle:
                _pGUI->_pScreen->triangle(_x0 + _dx * indexColumn / _columns + _dt,
                                          _y0 + _dy * indexLine / _lines + _dt + _dy / _lines - _dt * 2,
                                          _x0 + _dx * indexColumn / _columns + _dt + _dx / _columns - _dt * 2,
                                          _y0 + _dy * indexLine / _lines + _dt + _dy / _lines - _dt * 2,
                                          _x0 + _dx * indexColumn / _columns + _dt + _dx / _columns / 2 - _dt,
                                          _y0 + _dy * indexLine / _lines + _dt,
                                          colour);
                _pGUI->_pScreen->triangle(_x0 + _dx * indexColumn / _columns + _dt * 3,
                                          _y0 + _dy * indexLine / _lines + _dt * 2 + _dy / _lines - _dt * 4,
                                          _x0 + _dx * indexColumn / _columns + _dt * 3 + _dx / _columns - _dt * 6,
                                          _y0 + _dy * indexLine / _lines + _dt * 2 + _dy / _lines - _dt * 4,
                                          _x0 + _dx * indexColumn / _columns + _dt * 2 + _dx / _columns / 2 - _dt * 2,
                                          _y0 + _dy * indexLine / _lines + _dt + _dt * 5 / 2,
                                          _pGUI->_colourBack);
                break;

            case patternCircle:
                _pGUI->_pScreen->circle(_x0 + _dx * indexColumn / _columns + _dx / _columns / 2, _y0 + _dy * indexLine / _lines + _dy / _lines / 2, _dz, colour);
                _pGUI->_pScreen->circle(_x0 + _dx * indexColumn / _columns + _dx / _columns / 2, _y0 + _dy * indexLine / _lines + _dy / _lines / 2, _dz - _dt, _pGUI->_colourBack);
                break;

            default:
                break;
        }
        // _pGUI->_pScreen->rectangle(_x0 + _dx * indexColumn / _lines + 3, _y0 + _dy * indexLine / _columns + 3, _x0 + _dx * (indexColumn + 1) / _lines - 3, _y0 + _dy * (indexLine + 1) / _columns - 3, colour);
        // _pGUI->_pScreen->rectangle(_x0 + _dx * indexColumn / _lines + 3, _y0 + _dy * indexLine / _columns + 3, _x0 + _dx * (indexColumn + 1) / _lines - 3, _y0 + _dy * (indexLine + 1) / _columns - 3, colour);

        if (_pGUI->_delegate)
        {
            _pGUI->_pScreen->flushFast();
        }
    }
}

void Table::setState(bool state)
{
    _enable = state;
}

bool Table::check(uint8_t mode)
{
    if (!_enable)
    {
        return false;
    }

    uint16_t x, y, z, t;

    // down
    if (_pGUI->_pScreen->getTouch(x, y, z, t))
    {
        // pressed
        if ((x >= _x0) && (x < _x0 + _dx) && (y >= _y0) && (y < _y0 + _dy))
        {
#if (HAPTICS_MODE != USE_HAPTICS_NONE)

            myDRV2605L.playEffect(DRV2605L_SHARP_TICK_1_100);

#endif // HAPTICS_MODE

#if (USE_CONTINUITY == 1)
            if (_pGUI->_delegate)
            {
                _pGUI->_beginContinuity();
            }
#endif // USE_CONTINUITY

            _indexColumn = (x - _x0) * _columns / _dx;
            _indexLine = (y - _y0) * _lines / _dy;
            _indexColumn -= (_indexColumn == _columns) ? 1 : 0;
            _indexLine -= (_indexLine == _lines) ? 1 : 0;

            if (mode == checkInstant)
            {
                return true;
            }

            if (_pGUI->_delegate)
            {
                _pGUI->_pScreen->setPenSolid(false);
                _pGUI->_pScreen->dRoundedRectangle(_x0 + _dx * _indexColumn / _columns + 1, _y0 + _dy * _indexLine / _lines + 1, _dx / _columns - 2, _dy / _lines - 2, _radius, myColours.black);
                _pGUI->_pScreen->dRoundedRectangle(_x0 + _dx * _indexColumn / _columns + 2, _y0 + _dy * _indexLine / _lines + 2, _dx / _columns - 4, _dy / _lines - 4, _radius, myColours.black);
                _pGUI->_pScreen->flushFast();
            }

            while (t != TOUCH_EVENT_RELEASE) // Debounce
            {
                hV_HAL_delayMilliseconds(DELAY_DEBOUNCE);
                _pGUI->_pScreen->getTouch(x, y, z, t);
            }

            if (_pGUI->_delegate)
            {
                _pGUI->_pScreen->setPenSolid(false);
                _pGUI->_pScreen->dRoundedRectangle(_x0 + _dx * _indexColumn / _columns + 1, _y0 + _dy * _indexLine / _lines + 1, _dx / _columns - 2, _dy / _lines - 2, _radius, myColours.white);
                _pGUI->_pScreen->dRoundedRectangle(_x0 + _dx * _indexColumn / _columns + 2, _y0 + _dy * _indexLine / _lines + 2, _dx / _columns - 4, _dy / _lines - 4, _radius, myColours.white);
                _pGUI->_pScreen->flushFast();
            }

            // up
            if ((x >= _x0) && (x < _x0 + _dx) && (y >= _y0) && (y < _y0 + _dy))
            {
                return true;
            } // if up
#if (USE_CONTINUITY == 1)
            if (_pGUI->_delegate)
            {
                _pGUI->_endContinuity();
            }
#endif // USE_CONTINUITY

        } // if pressed
    } // if down
    return false;
}

void Table::getIndex(uint8_t & indexColumn, uint8_t & indexLine)
{
    indexColumn = _indexColumn;
    indexLine = _indexLine;
}

Keyboard::Keyboard(GUI * gui)
{
    _pGUI = gui;
}

void Keyboard::dDefine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy,
                       Item * keys, uint8_t columns, uint8_t lines, uint8_t size0)
{
    _x0 = x0;
    _y0 = y0;
    _dx = dx;
    _dy = dy;
    _radius = _pGUI->_style;

    _columns = columns;
    _lines = lines;
    _keys = keys;
    _size = size0;

    // Set selectFont

    _enable = _pGUI->_enable;
}

void Keyboard::setOption(bool option)
{
    _option = option;
}

void Keyboard::drawInitial()
{
    _pGUI->_pScreen->setPenSolid(true);
    _pGUI->_pScreen->dRoundedRectangle(_x0, _y0, _dx, _dy, _radius, _pGUI->_colourBack);

    // Optional internal lines
    if (_option == true)
    {
        for (uint8_t i = 1; i < _columns; i++) // columns
        {
            _pGUI->_pScreen->dLine(_x0 + _dx * i / _columns, _y0, 1, _dy, _pGUI->_colourMiddle);
        }
        for (uint8_t j = 1; j < _lines; j++) // lines
        {
            _pGUI->_pScreen->dLine(_x0, _y0 + _dy * j / _lines, _dx, 1, _pGUI->_colourMiddle);
        }
    }

    // External lines
    _pGUI->_pScreen->setPenSolid(false);
    _pGUI->_pScreen->dRoundedRectangle(_x0, _y0, _dx, _dy, _radius, _pGUI->_colourFront);

    for (uint8_t i = 0; i < _columns; i++) // column
    {
        for (uint8_t j = 0; j < _lines; j++) // line
        {
            _pGUI->dRoundedLabel(_x0 + _dx * i / _columns + 3, _y0 + _dy * j / _lines + 3,
                                 _dx / _columns - 6, _dy / _lines - 6, _radius,
                                 _keys[i + _columns * j].label,
                                 _pGUI->_colourFront, _pGUI->_colourBack,
                                 0, 0, _size, false);
        }
    }

    if (_pGUI->_delegate)
    {
        _pGUI->_pScreen->flushFast();
    }
}

void Keyboard::draw(uint8_t indexColumn, uint8_t indexLine, fsmGUI_e fsm)
{
    if ((indexColumn < _columns) and (indexLine < _lines))
    {
        // _pGUI->_pScreen->setPenSolid(true);
        // _pGUI->_pScreen->dRoundedRectangle(_x0 + _dx * indexColumn / _columns + 3, _y0 + _dy * indexLine / _lines + 3, _dx / _columns - 6, _dy / _lines - 6, colour);

        uint16_t colourFront = _pGUI->_colourFront;
        uint16_t colourBack = _pGUI->_colourBack;

        switch (fsm)
        {
            case fsmTouched:
            case fsmConfirmed:

                colourFront = _pGUI->_colourBack;
                colourBack = _pGUI->_colourFront;
                break;

            default:

                colourFront = _pGUI->_colourFront;
                colourBack = _pGUI->_colourBack;
                break;
        }

        _pGUI->dRoundedLabel(_x0 + _dx * indexColumn / _columns + 3, _y0 + _dy * indexLine / _lines + 3,
                             _dx / _columns - 6, _dy / _lines - 6, _radius,
                             _keys[indexColumn + _columns * indexLine].label,
                             colourFront, colourBack, 0, 0, _size, false);
        // _pGUI->_pScreen->rectangle(_x0 + _dx * indexColumn / _lines + 3, _y0 + _dy * indexLine / _columns + 3, _x0 + _dx * (indexColumn + 1) / _lines - 3, _y0 + _dy * (indexLine + 1) / _columns - 3, colour);

        if (_pGUI->_delegate)
        {
            _pGUI->_pScreen->flushFast();
        }
    }
}

void Keyboard::setState(bool state)
{
    _enable = state;
}

bool Keyboard::check(uint8_t mode)
{
    if (!_enable)
    {
        return false;
    }

    uint16_t x, y, z, t;

    // down
    if (_pGUI->_pScreen->getTouch(x, y, z, t))
    {
        // pressed
        if ((x >= _x0) && (x < _x0 + _dx) && (y >= _y0) && (y < _y0 + _dy))
        {
#if (HAPTICS_MODE != USE_HAPTICS_NONE)

            myDRV2605L.playEffect(DRV2605L_SHARP_TICK_1_100);

#endif // HAPTICS_MODE

#if (USE_CONTINUITY == 1)
            if (_pGUI->_delegate)
            {
                _pGUI->_beginContinuity();
            }
#endif // USE_CONTINUITY

            _indexColumn = ((x - _x0) * _columns) / _dx;
            _indexLine = ((y - _y0) * _lines) / _dy;
            // _indexColumn -= (_indexColumn == _columns) ? 1 : 0;
            // _indexLine -= (_indexLine == _lines) ? 1 : 0;

            if (mode == checkInstant)
            {
                _index = _keys[_indexColumn + _columns * _indexLine].index;
                return true;
            }

            if (_pGUI->_delegate)
            {
                draw(_indexColumn, _indexLine, fsmTouched);
            }

            while (t != TOUCH_EVENT_RELEASE) // Debounce
            {
                hV_HAL_delayMilliseconds(DELAY_DEBOUNCE);
                _pGUI->_pScreen->getTouch(x, y, z, t);
            }

            if (_pGUI->_delegate)
            {
                draw(_indexColumn, _indexLine, fsmReleased);
                _index = _keys[_indexColumn + _columns * _indexLine].index;
            }

            // up
            if ((x >= _x0) && (x < _x0 + _dx) && (y >= _y0) && (y < _y0 + _dy))
            {
                return true;
            } // if up
#if (USE_CONTINUITY == 1)
            if (_pGUI->_delegate)
            {
                _pGUI->_endContinuity();
            }
#endif // USE_CONTINUITY

        } // if pressed
    } // if down
    return false;
}

uint16_t Keyboard::getIndex()
{
    return _index;
}
