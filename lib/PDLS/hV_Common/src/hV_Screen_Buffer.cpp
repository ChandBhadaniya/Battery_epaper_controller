//
// hV_Screen_Buffer.cpp
// Class library C++ code
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
// See hV_Screen_Buffer.h for references
//
// Release 520: Added use of hV_HAL_Peripherals
// Release 523: Fixed rounded rectangles
// Release 526: Improved touch management
// Release 530: Added support for new 3.70"-Touch
// Release 607: Refactored peripherals
// Release 613: Added x- and y-axis margins
//

// Library header
#include "hV_Screen_Buffer.h"
//#include "QuickDebug.h"

// Code
hV_Screen_Buffer::hV_Screen_Buffer()
{
    _f_fontSize = 0;
    _f_fontNumber = 0;
    _f_fontSolid = true;
    _penSolid = false;
    _flagRead = false;
    _flagStorage = false;
    _flagEnergy = false;
    _touchTrim = 0;
    _flagX3 = false;
    _f_fontSpaceX = 1;
}

void hV_Screen_Buffer::begin()
{
#if (FONT_MODE == USE_FONT_FLASH)
    _f_begin(NOT_CONNECTED); // hV_Font_...
#else
    _f_begin();
#endif
}

void hV_Screen_Buffer::clear(uint16_t colour)
{
    uint8_t oldOrientation = _orientation;
    bool oldPenSolid = _penSolid;
    setOrientation(0);
    setPenSolid();
    rectangle(0, 0, screenSizeX() - 1, screenSizeY() - 1, colour);
    setOrientation(oldOrientation);
    setPenSolid(oldPenSolid);
}

void hV_Screen_Buffer::flush()
{
    ;
}

void hV_Screen_Buffer::setOrientation(uint8_t orientation)
{
    switch (orientation)
    {
        case 6:

            _orientation = 0;
            _setOrientation(_orientation);
            if (screenSizeX() > screenSizeY())
            {
                _orientation = 1;
                _setOrientation(_orientation);
            }
            break;

        case 7:

            _orientation = 0;
            _setOrientation(_orientation);
            if (screenSizeX() < screenSizeY())
            {
                _orientation = 1;
                _setOrientation(_orientation);
            }
            break;

        default:

            _orientation = orientation % 4;
            _setOrientation(_orientation);
            break;
    }
}

uint8_t hV_Screen_Buffer::getOrientation()
{
    return _orientation;
}

uint16_t hV_Screen_Buffer::screenSizeX()
{
    switch (_orientation)
    {
        case 1:
        case 3:

            return _screenSizeV - _screenMarginV * 2; // _maxX
            break;

        // case 0:
        // case 2:
        default:

            return _screenSizeH - _screenMarginH * 2; // _maxX
            break;
    }
    return 0;
}

uint16_t hV_Screen_Buffer::screenSizeY()
{
    switch (_orientation)
    {
        case 1:
        case 3:

            return _screenSizeH - _screenMarginH * 2; // _maxY
            break;

        // case 0:
        // case 2:
        default:

            return _screenSizeV - _screenMarginV * 2; // _maxY
            break;
    }
    return 0;
}

uint16_t hV_Screen_Buffer::screenDiagonal()
{
    return _screenDiagonal;
}

uint8_t hV_Screen_Buffer::screenColourBits()
{
    return _screenColourBits;
}

void hV_Screen_Buffer::_pointX3(int16_t x0, int16_t y0, int16_t x, int16_t y, uint16_t colour)
{
    point(x0 + x, y0 + y, colour);
    if (x > 0)
    {
        point(x0 + x - 1, y0 + y, colour);
    }
    if (x < 0)
    {
        point(x0 + x + 1, y0 + y, colour);
    }
    if (y > 0)
    {
        point(x0 + x, y0 + y - 1, colour);
    }
    if (y < 0)
    {
        point(x0 + x, y0 + y + 1, colour);
    }
}

void hV_Screen_Buffer::circle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t colour)
{
    int16_t f = 1 - radius;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * radius;
    int16_t x = 0;
    int16_t y = radius;

    if (_penSolid == false)
    {
        point(x0, y0 + radius, colour);
        point(x0, y0 - radius, colour);
        point(x0 + radius, y0, colour);
        point(x0 - radius, y0, colour);

        if (_flagX3)
        {
            while (x < y)
            {
                if (f >= 0)
                {
                    y--;
                    ddF_y += 2;
                    f += ddF_y;
                }

                x++;
                ddF_x += 2;
                f += ddF_x;

                _pointX3(x0, y0, +x, +y, colour);
                _pointX3(x0, y0, -x, +y, colour);
                _pointX3(x0, y0, +x, -y, colour);
                _pointX3(x0, y0, -x, -y, colour);
                _pointX3(x0, y0, +y, +x, colour);
                _pointX3(x0, y0, -y, +x, colour);
                _pointX3(x0, y0, +y, -x, colour);
                _pointX3(x0, y0, -y, -x, colour);
            }
        }
        else
        {
            while (x < y)
            {
                if (f >= 0)
                {
                    y--;
                    ddF_y += 2;
                    f += ddF_y;
                }

                x++;
                ddF_x += 2;
                f += ddF_x;

                point(x0 + x, y0 + y, colour);
                point(x0 - x, y0 + y, colour);
                point(x0 + x, y0 - y, colour);
                point(x0 - x, y0 - y, colour);
                point(x0 + y, y0 + x, colour);
                point(x0 - y, y0 + x, colour);
                point(x0 + y, y0 - x, colour);
                point(x0 - y, y0 - x, colour);
            }
        }
    }
    else
    {
        while (x < y)
        {
            if (f >= 0)
            {
                y--;
                ddF_y += 2;
                f += ddF_y;
            }

            x++;
            ddF_x += 2;
            f += ddF_x;

            line(x0 + x, y0 + y, x0 - x, y0 + y, colour); // bottom
            line(x0 + x, y0 - y, x0 - x, y0 - y, colour); // top
            line(x0 + y, y0 - x, x0 + y, y0 + x, colour); // right
            line(x0 - y, y0 - x, x0 - y, y0 + x, colour); // left
        }

        setPenSolid(true);
        rectangle(x0 - x, y0 - y, x0 + x, y0 + y, colour);
    }
}

void hV_Screen_Buffer::ring(uint16_t x0, uint16_t y0, uint16_t inner, uint16_t outer, uint16_t colour)
{
    // Normalisation
    if (inner > outer)
    {
        swap(inner, outer);
    }

    _flagX3 = true;
    if (_penSolid)
    {
        bool oldPenSolid = _penSolid;
        _penSolid = false;
        for (uint16_t radius = inner; radius < outer + 1; radius++)
        {
            circle(x0, y0, radius, colour);
        }
        _penSolid = oldPenSolid;
    }
    else
    {
        circle(x0, y0, inner, colour);
        circle(x0, y0, outer, colour);
    }
    _flagX3 = false;
}

void hV_Screen_Buffer::dLine(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t colour)
{
    line(x0, y0, x0 + dx - 1, y0 + dy - 1, colour);
}

void hV_Screen_Buffer::line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
    if ((x1 == x2) and (y1 == y2))
    {
        _setPoint(x1, y1, colour);
    }
    else if (x1 == x2)
    {
        if (y1 > y2)
        {
            swap(y1, y2);
        }
        for (uint16_t y = y1; y <= y2; y++)
        {
            _setPoint(x1, y, colour);
        }
    }
    else if (y1 == y2)
    {
        if (x1 > x2)
        {
            swap(x1, x2);
        }
        for (uint16_t x = x1; x <= x2; x++)
        {
            _setPoint(x, y1, colour);
        }
    }
    else
    {
        int16_t wx1 = (int16_t)x1;
        int16_t wx2 = (int16_t)x2;
        int16_t wy1 = (int16_t)y1;
        int16_t wy2 = (int16_t)y2;

        bool flag = abs(wy2 - wy1) > abs(wx2 - wx1);
        if (flag)
        {
            swap(wx1, wy1);
            swap(wx2, wy2);
        }

        if (wx1 > wx2)
        {
            swap(wx1, wx2);
            swap(wy1, wy2);
        }

        int16_t dx = wx2 - wx1;
        int16_t dy = abs(wy2 - wy1);
        int16_t err = dx / 2;
        int16_t ystep;

        if (wy1 < wy2)
        {
            ystep = 1;
        }
        else
        {
            ystep = -1;
        }

        for (; wx1 <= wx2; wx1++)
        {
            if (flag)
            {
                _setPoint(wy1, wx1, colour);
            }
            else
            {
                _setPoint(wx1, wy1, colour);
            }

            err -= dy;
            if (err < 0)
            {
                wy1 += ystep;
                err += dx;
            }
        }
    }
}

void hV_Screen_Buffer::setPenSolid(bool flag)
{
    _penSolid = flag;
}

void hV_Screen_Buffer::point(uint16_t x1, uint16_t y1, uint16_t colour)
{
    _setPoint(x1, y1, colour);
}

void hV_Screen_Buffer::rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t colour)
{
    if (_penSolid == false)
    {
        line(x1, y1, x1, y2, colour);
        line(x1, y1, x2, y1, colour);
        line(x1, y2, x2, y2, colour);
        line(x2, y1, x2, y2, colour);
    }
    else
    {
        if (x1 > x2)
        {
            swap(x1, x2);
        }
        if (y1 > y2)
        {
            swap(y1, y2);
        }
        for (uint16_t x = x1; x <= x2; x++)
        {
            for (uint16_t y = y1; y <= y2; y++)
            {
                _setPoint(x, y, colour);
            }
        }
    }
}

void hV_Screen_Buffer::roundedRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t radius, uint16_t colour)
{
    if (radius == 0)
    {
        rectangle(x1, y1, x2, y2, colour);
    }
    else
    {
        if (x1 > x2)
        {
            swap(x1, x2);
        }
        if (y1 > y2)
        {
            swap(y1, y2);
        }

        uint16_t z = min(x2 - x1, y2 - y1);
        if (3 * radius > z) // self-adjustment
        {
            radius = z / 3;
        }

        if (_penSolid == false)
        {
            line(x1, y1 + radius, x1, y2 - radius, colour);
            line(x1 + radius, y1, x2 - radius, y1, colour);
            line(x1 + radius, y2, x2 - radius, y2, colour);
            line(x2, y1 + radius, x2, y2 - radius, colour);

            int16_t f = 1 - radius;
            int16_t ddF_x = 1;
            int16_t ddF_y = -2 * radius;
            int16_t x = 0;
            int16_t y = radius;

            while (x < y)
            {
                if (f >= 0)
                {
                    y--;
                    ddF_y += 2;
                    f += ddF_y;
                }
                x++;
                ddF_x += 2;
                f += ddF_x;
                point(x2 - radius + x, y1 + radius - y, colour);
                point(x2 - radius + y, y1 + radius - x, colour);
                point(x1 + radius - y, y2 - radius + x, colour);
                point(x1 + radius - x, y2 - radius + y, colour);
                point(x1 + radius - y, y1 + radius - x, colour);
                point(x1 + radius - x, y1 + radius - y, colour);
                point(x2 - radius + x, y2 - radius + y, colour);
                point(x2 - radius + y, y2 - radius + x, colour);
            }
        }
        else
        {
            for (uint16_t x = x1 + radius; x <= x2 - radius; x++)
            {
                for (uint16_t y = y1; y <= y2; y++)
                {
                    _setPoint(x, y, colour);
                }
            }

            int16_t f = 1 - radius;
            int16_t ddF_x = 1;
            int16_t ddF_y = -2 * radius;
            int16_t x = 0;
            int16_t y = radius;

            while (x < y)
            {
                if (f >= 0)
                {
                    y--;
                    ddF_y += 2;
                    f += ddF_y;
                }
                x++;
                ddF_x += 2;
                f += ddF_x;

                line(x2 - radius + x, y1 + radius - y, x2 - radius + x, y2 - radius + y, colour);
                line(x2 - radius + y, y1 + radius - x, x2 - radius + y, y2 - radius + x, colour);

                line(x1 + radius - y, y1 + radius - x, x1 + radius - y, y2 - radius + x, colour);
                line(x1 + radius - x, y1 + radius - y, x1 + radius - x, y2 - radius + y, colour);
            }
        }
    }
}

void hV_Screen_Buffer::dRectangle(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t colour)
{
    rectangle(x0, y0, x0 + dx - 1, y0 + dy - 1, colour);
}

void hV_Screen_Buffer::dRoundedRectangle(uint16_t x0, uint16_t y0, uint16_t dx, uint16_t dy, uint16_t radius, uint16_t colour)
{
    roundedRectangle(x0, y0, x0 + dx - 1, y0 + dy - 1, radius, colour);
}

void hV_Screen_Buffer::_triangleArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t colour)
{
    int16_t wx1 = (int16_t)x1;
    int16_t wy1 = (int16_t)y1;
    int16_t wx2 = (int16_t)x2;
    int16_t wy2 = (int16_t)y2;
    int16_t wx3 = (int16_t)x3;
    int16_t wy3 = (int16_t)y3;
    int16_t wx4 = wx1;
    int16_t wy4 = wy1;
    int16_t wx5 = wx1;
    int16_t wy5 = wy1;

    bool changed1 = false;
    bool changed2 = false;

    int16_t dx1 = abs(wx2 - wx1);
    int16_t dy1 = abs(wy2 - wy1);

    int16_t dx2 = abs(wx3 - wx1);
    int16_t dy2 = abs(wy3 - wy1);

    int16_t signx1 = (wx2 >= wx1) ? +1 : -1;
    int16_t signx2 = (wx3 >= wx1) ? +1 : -1;

    int16_t signy1 = (wy2 >= wy1) ? +1 : -1;
    int16_t signy2 = (wy3 >= wy1) ? +1 : -1;

    if (dy1 > dx1)
    {
        swap(dx1, dy1); // swap values
        changed1 = true;
    }

    if (dy2 > dx2)
    {
        swap(dx2, dy2); // swap values
        changed2 = true;
    }

    int16_t e1 = 2 * dy1 - dx1;
    int16_t e2 = 2 * dy2 - dx2;

    for (int i = 0; i <= dx1; i++)
    {
        line(wx4, wy4, wx5, wy5, colour);

        while (e1 >= 0)
        {
            if (changed1)
            {
                wx4 += signx1;
            }
            else
            {
                wy4 += signy1;
            }
            e1 = e1 - 2 * dx1;
        }

        if (changed1)
        {
            wy4 += signy1;
        }
        else
        {
            wx4 += signx1;
        }

        e1 = e1 + 2 * dy1;

        while (wy5 != wy4)
        {
            while (e2 >= 0)
            {
                if (changed2)
                {
                    wx5 += signx2;
                }
                else
                {
                    wy5 += signy2;
                }
                e2 = e2 - 2 * dx2;
            }

            if (changed2)
            {
                wy5 += signy2;
            }
            else
            {
                wx5 += signx2;
            }

            e2 = e2 + 2 * dy2;
        }
    }
}

void hV_Screen_Buffer::triangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, uint16_t colour)
{
    if ((x1 == x2) and (y1 == y2))
    {
        line(x3, y3, x1, y1, colour);
    }
    else if ((x1 == x3) and (y1 == y3))
    {
        line(x2, y2, x3, y3, colour);
    }
    else if ((x2 == x3) and (y2 == y3))
    {
        line(x1, y1, x2, y2, colour);
    }
    else if (_penSolid)
    {
        bool b = true;

        // Graham Scan + Andrew's Monotone Chain Algorithm
        // Sort by ascending y
        while (b)
        {
            b = false;
            if ((b == false) and (y1 > y2))
            {
                swap(x1, x2);
                swap(y1, y2);
                b = true;
            }
            if ((b == false) and (y2 > y3))
            {
                swap(x3, x2);
                swap(y3, y2);
                b = true;
            }
        }

        if (y2 == y3)
        {
            _triangleArea(x1, y1, x2, y2, x3, y3, colour);
        }
        else if (y1 == y2)
        {
            _triangleArea(x3, y3, x1, y1, x2, y2, colour);
        }
        else
        {
            uint16_t x4 = (uint16_t)((int32_t)x1 + (y2 - y1) * (x3 - x1) / (y3 - y1));
            uint16_t y4 = y2;

            _triangleArea(x1, y1, x2, y2, x4, y4, colour);
#if defined(ESP8266)
            hV_HAL_delayMilliseconds(1);
#else
            hV_HAL_delayMicroseconds(1000); // delay(1);
#endif // ESP8266
            hV_HAL_delayMicroseconds(1000); // delay(1);
            _triangleArea(x3, y3, x2, y2, x4, y4, colour);
#if defined(ESP8266)
            hV_HAL_delayMilliseconds(1);
#else
            hV_HAL_delayMicroseconds(1000); // delay(1);
#endif // ESP8266
        }
    }
    else
    {
        line(x1, y1, x2, y2, colour);
        line(x2, y2, x3, y3, colour);
        line(x3, y3, x1, y1, colour);
    }
}

bool hV_Screen_Buffer::_inValue(int16_t value, int16_t valueLow, int16_t valueHigh)
{
    if (valueLow <= valueHigh)
    {
        return ((valueLow <= value) and (value < valueHigh));
    }
    else
    {
        return ((valueHigh <= value) and (value < valueLow));
    }
}

bool hV_Screen_Buffer::_inCycle(int16_t value, int16_t valueLow, int16_t valueHigh)
{
    if (valueLow <= valueHigh)
    {
        return ((valueLow < value) and (value < valueHigh));
    }
    else
    {
        return ((valueHigh <= value) != (value < valueLow));
    }
}

bool hV_Screen_Buffer::_inSector(int16_t valueStart, int16_t valueEnd,
                                 int16_t sectorLow, int16_t sectorHigh,
                                 int16_t criteriaStart, int16_t criteriaEnd,
                                 int16_t criteriaLow, int16_t criteriaHigh,
                                 int16_t criteria)
{
    bool flag = false;

    flag |= !_inValue(valueStart, sectorLow, sectorHigh) and _inValue(valueEnd, sectorLow, sectorHigh) and _inValue(criteria, criteriaLow, criteriaEnd);
    flag |= _inValue(valueStart, sectorLow, sectorHigh) and _inValue(valueEnd, sectorLow, sectorHigh) and _inValue(criteria, criteriaStart, criteriaEnd);
    flag |= _inValue(valueStart, sectorLow, sectorHigh) and !_inValue(valueEnd, sectorLow, sectorHigh) and _inValue(criteria, criteriaStart, criteriaHigh);
    flag |= ((valueStart <= sectorLow) and (sectorHigh <= valueEnd)) or ((sectorHigh <= valueEnd) and (valueEnd <= valueStart)) or ((valueStart <= sectorLow) and (valueEnd <= valueStart));

    return flag;
}

void hV_Screen_Buffer::arc(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t start, uint16_t end, uint16_t colour)
{
    // Normalisation
    start += 360;
    end += 360;
    start %= 360;
    end %= 360;

    // Relative coordinates
    int16_t dxi = +(radius * sin32x100(start * 100)) / 100;
    int16_t dyi = -(radius * cos32x100(start * 100)) / 100;
    int16_t dxf = +(radius * sin32x100(end * 100)) / 100;
    int16_t dyf = -(radius * cos32x100(end * 100)) / 100;
    int16_t p = +(radius * cos32x100(4500)) / 100;

    // Absolute coordinates
    int16_t x0i, y0i, x0f, y0f;
    int16_t x1i, y1i, x1f, y1f;
    int16_t x2i, y2i, x2f, y2f;
    int16_t x3i, y3i, x3f, y3f;
    int16_t x4i, y4i, x4f, y4f;
    int16_t x5i, y5i, x5f, y5f;
    int16_t x6i, y6i, x6f, y6f;
    int16_t x7i, y7i, x7f, y7f;

    int16_t f = 1 - radius;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * radius;
    int16_t x = 0;
    int16_t y = radius;

    bool flag0i = false;
    bool flag1i = false;
    bool flag2i = false;
    bool flag3i = false;
    bool flag4i = false;
    bool flag5i = false;
    bool flag6i = false;
    bool flag7i = false;

    // Patch for angles 45 135 225 315
    bool flag0f = (_inCycle(start, 0, 45) or _inCycle(end, 0, 45) or (end == 45));
    bool flag1f = (_inCycle(start, 45, 90) or _inCycle(end, 45, 90));
    bool flag2f = (_inCycle(start, 90, 135) or _inCycle(end, 90, 135) or (end == 135));
    bool flag3f = (_inCycle(start, 135, 180) or _inCycle(end, 135, 180));
    bool flag4f = (_inCycle(start, 180, 225) or _inCycle(end, 180, 225) or (end == 225));
    bool flag5f = (_inCycle(start, 225, 270) or _inCycle(end, 225, 270));
    bool flag6f = (_inCycle(start, 270, 315) or _inCycle(end, 270, 315) or (end == 315));
    bool flag7f = (_inCycle(start, 315, 360) or _inCycle(end, 315, 360));

    while (x <= y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }

        x++;
        ddF_x += 2;
        f += ddF_x;

        // Octant 0: 0~45 criteria= +x in x : x0 + x, y0 - y
        if (_inSector(start, end, 0, 45, dxi, dxf, 0, p, +x))
        {
            point(x0 + x, y0 - y, colour);
            if (_penSolid)
            {
                if (!flag0i)
                {
                    flag0i = true;
                    x0i = x;
                    y0i = y;
                }
                x0f = x;
                y0f = y;

                if (flag0i and flag0f)
                {
                    line(x0 + x0i, y0 - y, x0 + x, y0 - y, colour);
                }
                if (_inCycle(360, start, end))
                {
                    line(x0, y0 - y, x0 + x, y0 - y, colour); // 0, -1
                }
                if (_inCycle(45, start, end))
                {
                    line(x0 + x, y0 - x, x0 + x, y0 - y, colour); // p, -p
                }
            }
        }

        // Octant 1: 45~90 criteria= -x in y : x0 + y, y0 - x
        if (_inSector(start, end, 45, 90, dyi, dyf, -p, 0, -x))
        {
            point(x0 + y, y0 - x, colour);
            if (_penSolid)
            {
                if (!flag1i)
                {
                    flag1i = true;
                    x1i = x;
                    y1i = y;
                }
                x1f = x;
                y1f = y;

                if (flag1i and flag1f)
                {
                    line(x0 + y, y0 - x1i, x0 + y, y0 - x, colour);
                }
                if (_inCycle(45, start, end))
                {
                    line(x0 + x, y0 - x, x0 + y, y0 - x, colour); // p, -p
                }
                if (_inCycle(90, start, end))
                {
                    line(x0 + y, y0, x0 + y, y0 - x, colour); // 1, 0
                }
            }
        }

        // Octant 2: 90~135 criteria= +x in y : x0 + y, y0 + x
        if (_inSector(start, end, 90, 135, dyi, dyf, 0, p, +x))
        {
            point(x0 + y, y0 + x, colour);
            if (_penSolid)
            {
                if (!flag2i)
                {
                    flag2i = true;
                    x2i = x;
                    y2i = y;
                }
                x2f = x;
                y2f = y;

                if (flag2i and flag2f)
                {
                    line(x0 + y, y0 + x2i, x0 + y, y0 + x, colour); //
                }
                if (_inCycle(90, start, end))
                {
                    line(x0 + y, y0, x0 + y, y0 + x, colour); // 1, 0
                }
                if (_inCycle(135, start, end))
                {
                    line(x0 + x, y0 + x, x0 + y, y0 + x, colour); // p, p
                }
            }
        }

        // Octant 3: 135~180 criteria= +x in x : x0 + x, y0 + y
        if (_inSector(start, end, 135, 180, dxi, dxf, p, 0, +x))
        {
            point(x0 + x, y0 + y, colour);
            if (_penSolid)
            {
                if (!flag3i)
                {
                    flag3i = true;
                    x3i = x;
                    y3i = y;
                }
                x3f = x;
                y3f = y;
                if (flag3i and flag3f)
                {
                    line(x0 + x3i, y0 + y, x0 + x, y0 + y, colour);
                }
                if (_inCycle(135, start, end))
                {
                    line(x0 + x, y0 + x, x0 + x, y0 + y, colour); // p, p
                }
                if (_inCycle(180, start, end))
                {
                    line(x0, y0 + y, x0 + x, y0 + y, colour); // 0, 1
                }
            }
        }

        // Octant 4: 180~225 criteria= -x in x : x0 - x, y0 + y
        if (_inSector(start, end, 180, 225, dxi, dxf, 0, -p, -x))
        {
            point(x0 - x, y0 + y, colour);
            if (_penSolid)
            {
                if (!flag4i)
                {
                    flag4i = true;
                    x4i = x;
                    y4i = y;
                }
                x4f = x;
                y4f = y;
                if (flag4i and flag4f)
                {
                    line(x0 - x4i, y0 + y, x0 - x, y0 + y, colour);
                }
                if (_inCycle(180, start, end))
                {
                    line(x0, y0 + y, x0 - x, y0 + y, colour); // 0, 1
                }
                if (_inCycle(225, start, end))
                {
                    line(x0 - x, y0 + x, x0 - x, y0 + y, colour); // -p, p
                }
            }
        }

        // Octant 5: 225~270 criteria= +x in y : x0 - y, y0 + x
        if (_inSector(start, end, 225, 270, dyi, dyf, p, 0, +x))
        {
            point(x0 - y, y0 + x, colour);
            if (_penSolid)
            {
                if (!flag5i)
                {
                    flag5i = true;
                    x5i = x;
                    y5i = y;
                }
                x5f = x;
                y5f = y;
                if (flag5i and flag5f)
                {
                    line(x0 - y, y0 + x5i, x0 - y, y0 + x, colour); //
                }
                if (_inCycle(225, start, end))
                {
                    line(x0 - x, y0 + x, x0 - y, y0 + x, colour); // -p, p
                }
                if (_inCycle(270, start, end))
                {
                    line(x0 - y, y0, x0 - y, y0 + x, colour); // -1, 0
                }
            }
        }

        // Octant 6: 270~335 criteria= -x in y : x0 - y, y0 - x
        if (_inSector(start, end, 270, 315, dyi, dyf, 0, -p, -x))
        {
            point(x0 - y, y0 - x, colour);
            if (_penSolid)
            {
                if (!flag6i)
                {
                    flag6i = true;
                    x6i = x;
                    y6i = y;
                }
                x6f = x;
                y6f = y;
                if (flag6i and flag6f)
                {
                    line(x0 - y, y0 - x6i, x0 - y, y0 - x, colour); //
                }
                if (_inCycle(270, start, end))
                {
                    line(x0 - y, y0, x0 - y, y0 - x, colour); // -1, 0
                }
                if (_inCycle(315, start, end))
                {
                    line(x0 - x, y0 - x, x0 - y, y0 - x, colour); // -p, -p
                }
            }
        }

        // Octant 7: 335~360 criteria= -x in x : x0 - x, y0 - y
        if (_inSector(start, end, 315, 360, dxi, dxf, -p, 0, -x))
        {
            point(x0 - x, y0 - y, colour);
            if (_penSolid)
            {
                if (!flag7i)
                {
                    flag7i = true;
                    x7i = x;
                    y7i = y;
                }
                x7f = x;
                y7f = y;
                if (flag7i and flag7f)
                {
                    line(x0 - x7i, y0 - y, x0 - x, y0 - y, colour);
                }
                if (_inCycle(315, start, end))
                {
                    line(x0 - x, y0 - x, x0 - x, y0 - y, colour); // -p, -p
                }
                if (_inCycle(360, start, end))
                {
                    line(x0, y0 - y, x0 - x, y0 - y, colour); // 0, -1
                }
            }
        }
    }

    if (_penSolid)
    {
        // Octant 0: 0~ 45 criteria= +x in x : x0 + x, y0 - y
        if (flag0f and (abs(x0i) < 512) and (abs(y0i) < 512) and (abs(x0f) < 512) and (abs(y0f) < 512))
        {
            triangle(x0, y0, x0 + x0i, y0 - y0i, x0 + x0f, y0 - y0f, colour);
        }

        // Octant 1: 45~ 90 criteria= -x in y : x0 + y, y0 - x
        if (flag1f and (abs(x1i) < 512) and (abs(y1i) < 512) and (abs(x1f) < 512) and (abs(y1f) < 512))
        {
            triangle(x0, y0, x0 + y1i, y0 - x1i, x0 + y1f, y0 - x1f, colour);
        }

        // Octant 2: 90~135 criteria= +x in y : x0 + y, y0 + x
        if (flag2f and (abs(x2i) < 512) and (abs(y2i) < 512) and (abs(x2f) < 512) and (abs(y2f) < 512))
        {
            triangle(x0, y0, x0 + y2i, y0 + x2i, x0 + y2f, y0 + x2f, colour);
        }

        // Octant 3: 135~180 criteria= +x in x : x0 + x, y0 + y
        if (flag3f and (abs(x3i) < 512) and (abs(y3i) < 512) and (abs(x3f) < 512) and (abs(y3f) < 512))
        {
            triangle(x0, y0, x0 + x3i, y0 + y3i, x0 + x3f, y0 + y3f, colour);
        }

        // Octant 4: 180~225 criteria= -x in x : x0 - x, y0 + y
        if (flag4f and (abs(x4i) < 512) and (abs(y4i) < 512) and (abs(x4f) < 512) and (abs(y4f) < 512))
        {
            triangle(x0, y0, x0 - x4i, y0 + y4i, x0 - x4f, y0 + y4f, colour);
        }

        // Octant 5: 225~270 criteria= +x in y : x0 - y, y0 + x
        if (flag5f and (abs(x5i) < 512) and (abs(y5i) < 512) and (abs(x5f) < 512) and (abs(y5f) < 512))
        {
            triangle(x0, y0, x0 - y5i, y0 + x5i, x0 - y5f, y0 + x5f, colour);
        }

        // Octant 6: 270~335 criteria= -x in y : x0 - y, y0 - x
        if (flag6f and (abs(x6i) < 512) and (abs(y6i) < 512) and (abs(x6f) < 512) and (abs(y6f) < 512))
        {
            triangle(x0, y0, x0 - y6i, y0 - x6i, x0 - y6f, y0 - x6f, colour);
        }

        // Octant 7: 335~360 criteria= -x in x : x0 - x, y0 - y
        if (flag7f and (abs(x7i) < 512) and (abs(y7i) < 512) and (abs(x7f) < 512) and (abs(y7f) < 512))
        {
            triangle(x0, y0, x0 - x7i, y0 - y7i, x0 - x7f, y0 - y7f, colour);
        }

        if (_inCycle(0, start, end))
        {
            line(x0, y0, x0, y0 - radius, colour);
        }
        if (_inCycle(45, start, end))
        {
            line(x0, y0, x0 + p, y0 - p, colour);
        }
        if (_inCycle(90, start, end))
        {
            line(x0, y0, x0 + radius, y0, colour);
        }
        if (_inCycle(135, start, end))
        {
            line(x0, y0, x0 + p, y0 + p, colour);
        }
        if (_inCycle(180, start, end))
        {
            line(x0, y0, x0, y0 + radius, colour);
        }
        if (_inCycle(225, start, end))
        {
            line(x0, y0, x0 - p, y0 + p, colour);
        }
        if (_inCycle(270, start, end))
        {
            line(x0, y0, x0 - radius, y0, colour);
        }
        if (_inCycle(335, start, end))
        {
            line(x0, y0, x0 - p, y0 - p, colour);
        }
    }
}

void hV_Screen_Buffer::level(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t start, uint16_t end, uint16_t colour)
{
    // Normalisation
    start += 180;
    end += 180;
    start %= 180;
    end %= 180;
    if (start > end)
    {
        swap(start, end);
    }

    // Relative coordinates
    // +50 for rounding to smooth aliasing
    int16_t dxi = +(radius * sin32x100(start * 100) + 50) / 100;
    int16_t dyi = -(radius * cos32x100(start * 100) + 50) / 100;
    int16_t dxf = +(radius * sin32x100(end * 100) + 50) / 100;
    int16_t dyf = -(radius * cos32x100(end * 100) + 50) / 100;
    int16_t p = +(radius * cos32x100(4500) + 50) / 100;

    line(x0 - dxi, y0 + dyi, x0 + dxi, y0 + dyi, colour);
    line(x0 - dxf, y0 + dyf, x0 + dxf, y0 + dyf, colour);

    // // Absolute coordinates
    // int16_t xi = x0 + dxi;
    // int16_t xf = x0 + dxf;
    // int16_t yi = y0 + dyi;
    // int16_t yf = y0 + dyf;

    int16_t x0i, y0i, x0f, y0f;
    int16_t x1i, y1i, x1f, y1f;
    int16_t x2i, y2i, x2f, y2f;
    int16_t x3i, y3i, x3f, y3f;

    int16_t f = 1 - radius;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * radius;
    int16_t x = 0;
    int16_t y = radius;

    bool flag0i = false;
    bool flag1i = false;
    bool flag2i = false;
    bool flag3i = false;
    /*
        bool flag4i = false;
        bool flag5i = false;
        bool flag6i = false;
        bool flag7i = false;
    */

    // Patch for angles 45 135 225 315
    bool flag0f = (_inCycle(start, 0, 45) or _inCycle(end, 0, 45) or (end == 45));
    bool flag1f = (_inCycle(start, 45, 90) or _inCycle(end, 45, 90) or (start == 45));
    bool flag2f = (_inCycle(start, 90, 135) or _inCycle(end, 90, 135) or (end == 135));
    bool flag3f = (_inCycle(start, 135, 180) or _inCycle(end, 135, 180) or (start == 135));
    /*
        bool flag4f = ( _inCycle(start, 180, 225) or _inCycle(end, 180, 225) or (end == 225) );
        bool flag5f = ( _inCycle(start, 225, 270) or _inCycle(end, 225, 270) );
        bool flag6f = ( _inCycle(start, 270, 315) or _inCycle(end, 270, 315) or (end == 315) );
        bool flag7f = ( _inCycle(start, 315, 360) or _inCycle(end, 315, 360) );
    */

    while (x <= y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }

        x++;
        ddF_x += 2;
        f += ddF_x;

        // Octant 0: 0~45 criteria= +x in x : x0 + x, y0 - y
        if (_inSector(start, end, 0, 45, dxi, dxf, 0, p, +x))
        {
            point(x0 + x, y0 - y, colour);
            point(x0 - x, y0 - y, colour);
            if (_penSolid)
            {
                if (!flag0i)
                {
                    flag0i = true;
                    x0i = x;
                    y0i = y;
                }
                x0f = x;
                y0f = y;

                if (flag0i and flag0f)
                {
                    line(x0 - x, y0 - y, x0 + x, y0 - y, colour);
                }
                if (_inCycle(360, start, end))
                {
                    line(x0 - x, y0 - y, x0 + x, y0 - y, colour); // 0, -1
                }
                if (_inCycle(45, start, end))
                {
                    line(x0 - x, y0 - y, x0 + x, y0 - y, colour); // p, -p
                }
            }
        }

        // Octant 1: 45~90 criteria= -x in y : x0 + y, y0 - x
        if (_inSector(start, end, 45, 90, dyi, dyf, -p, 0, -x))
        {
            point(x0 + y, y0 - x, colour);
            point(x0 - y, y0 - x, colour);
            if (_penSolid)
            {
                if (!flag1i)
                {
                    flag1i = true;
                    x1i = x;
                    y1i = y;
                }
                x1f = x;
                y1f = y;

                if (flag1i and flag1f)
                {
                    line(x0 - y, y0 - x, x0 + y, y0 - x, colour);
                }
                if (_inCycle(45, start, end))
                {
                    line(x0 - y, y0 - x, x0 + y, y0 - x, colour); // p, -p
                }
                if (_inCycle(90, start, end))
                {
                    line(x0 - y, y0 - x, x0 + y, y0 - x, colour); // 1, 0
                }
            }
        }

        // Octant 2: 90~135 criteria= +x in y : x0 + y, y0 + x
        if (_inSector(start, end, 90, 135, dyi, dyf, 0, p, +x))
        {
            point(x0 + y, y0 + x, colour);
            point(x0 - y, y0 + x, colour);
            if (_penSolid)
            {
                if (!flag2i)
                {
                    flag2i = true;
                    x2i = x;
                    y2i = y;
                }
                x2f = x;
                y2f = y;

                if (flag2i and flag2f)
                {
                    line(x0 - y, y0 + x, x0 + y, y0 + x, colour); //
                }
                if (_inCycle(90, start, end))
                {
                    line(x0 - y, y0 + x, x0 + y, y0 + x, colour); // 1, 0
                }
                if (_inCycle(90, start, end))
                {
                    line(x0 - y, y0, x0 + y, y0, colour); // 1, 0
                }
                if (_inCycle(135, start, end))
                {
                    line(x0 - y, y0 + x, x0 + y, y0 + x, colour); // p, p
                }
            }
        }

        // Octant 3: 135~180 criteria= +x in x : x0 + x, y0 + y
        if (_inSector(start, end, 135, 180, dxi, dxf, p, 0, +x))
        {
            point(x0 + x, y0 + y, colour);
            point(x0 - x, y0 + y, colour);
            if (_penSolid)
            {
                if (!flag3i)
                {
                    flag3i = true;
                    x3i = x;
                    y3i = y;
                }
                x3f = x;
                y3f = y;
                if (flag3i and flag3f)
                {
                    line(x0 - x, y0 + y, x0 + x, y0 + y, colour);
                }
                if (_inCycle(135, start, end))
                {
                    line(x0 - x, y0 + y, x0 + x, y0 + y, colour); // p, p
                }
                if (_inCycle(180, start, end))
                {
                    line(x0 - x, y0 + y, x0 + x, y0 + y, colour); // 0, 1
                }
            }
        }
        /*
            // octant 4: 180~225 criteria= -x in x : x0 - x, y0 + y
            if (_inSector(start, end, 180, 225, dxi, dxf, 0, -p, -x))
            {
            point(x0 - x, y0 + y, colour);
            point(x0 + x, y0 + y, colour);
            if (_penSolid)
            {
            if (!flag4i)
            {
            flag4i = true;
            x4i = x;
            y4i = y;
            }
            x4f = x;
            y4f = y;
            if (flag4i and flag4f) line(x0 + x, y0 + y, x0 - x, y0 + y, colour);
            if (_inCycle(180, start, end)) line(x0 + x, y0 + y, x0 - x, y0 + y, colour); // 0, 1
            }
            }
        */
    }

    if (_penSolid)
    {
        /*
            // Patch against strange values xyi xyf even for authorised octants
            if (flag0f and ((abs(x0i) > 512) or (abs(y0i) > 512) or (abs(x0f) > 512) or (abs(y0f) > 512))) flag0f = false;
            if (flag1f and ((abs(x1i) > 512) or (abs(y1i) > 512) or (abs(x1f) > 512) or (abs(y1f) > 512))) flag1f = false;
            if (flag2f and ((abs(x2i) > 512) or (abs(y2i) > 512) or (abs(x2f) > 512) or (abs(y2f) > 512))) flag2f = false;
            if (flag3f and ((abs(x3i) > 512) or (abs(y3i) > 512) or (abs(x3f) > 512) or (abs(y3f) > 512))) flag3f = false;
            if (flag4f and ((abs(x4i) > 512) or (abs(y4i) > 512) or (abs(x4f) > 512) or (abs(y4f) > 512))) flag4f = false;
            if (flag5f and ((abs(x5i) > 512) or (abs(y5i) > 512) or (abs(x5f) > 512) or (abs(y5f) > 512))) flag5f = false;
            if (flag6f and ((abs(x6i) > 512) or (abs(y6i) > 512) or (abs(x6f) > 512) or (abs(y6f) > 512))) flag6f = false;
            if (flag7f and ((abs(x7i) > 512) or (abs(y7i) > 512) or (abs(x7f) > 512) or (abs(y7f) > 512))) flag7f = false;

            // octant 0: 0~ 45 criteria= +x in x : x0 + x, y0 - y
            if (flag0f) triangle(x0, y0, x0 + x0i, y0 - y0i, x0 + x0f, y0 - y0f, colour);

            // octant 1: 45~ 90 criteria= -x in y : x0 + y, y0 - x
            if (flag1f) triangle(x0, y0, x0 + y1i, y0 - x1i, x0 + y1f, y0 - x1f, colour);

            // octant 2: 90~135 criteria= +x in y : x0 + y, y0 + x
            if (flag2f) triangle(x0, y0, x0 + y2i, y0 + x2i, x0 + y2f, y0 + x2f, colour);

            // octant 3: 135~180 criteria= +x in x : x0 + x, y0 + y
            if (flag3f) triangle(x0, y0, x0 + x3i, y0 + y3i, x0 + x3f, y0 + y3f, colour);

            // octant 4: 180~225 criteria= -x in x : x0 - x, y0 + y
            if (flag4f) triangle(x0, y0, x0 - x4i, y0 + y4i, x0 - x4f, y0 + y4f, colour);

            // octant 5: 225~270 criteria= +x in y : x0 - y, y0 + x
            if (flag5f) triangle(x0, y0, x0 - y5i, y0 + x5i, x0 - y5f, y0 + x5f, colour);

            // octant 6: 270~335 criteria= -x in y : x0 - y, y0 - x
            if (flag6f) triangle(x0, y0, x0 - y6i, y0 - x6i, x0 - y6f, y0 - x6f, colour);

            // octant 7: 335~360 criteria= -x in x : x0 - x, y0 - y
            if (flag7f) triangle(x0, y0, x0 - x7i, y0 - y7i, x0 - x7f, y0 - y7f, colour);
        */
        /*
            colour = myColours.violet;

            // octant 0: 0~ 45 criteria= +x in x : x0 + x, y0 - y
            if (flag0f and (abs(x0i) < 512) and (abs(y0i) < 512) and (abs(x0f) < 512) and (abs(y0f) < 512))
            {
            // triangle(x0, y0, x0 + x0i, y0 - y0i, x0 + x0f, y0 - y0f, colour);
            line(x0 - x0i, y0 - y0i, x0 + x0i, y0 - y0i, colour);
            line(x0 - x0f, y0 - y0f, x0 + x0f, y0 - y0f, colour);
            }
            // octant 1: 45~ 90 criteria= -x in y : x0 + y, y0 - x
            if (flag1f and (abs(x1i) < 512) and (abs(y1i) < 512) and (abs(x1f) < 512) and (abs(y1f) < 512))
            {
            // triangle(x0, y0, x0 + y1i, y0 - x1i, x0 + y1f, y0 - x1f, colour);
            line(x0 - y1i, y0 - x1i, x0 + y1i, y0 - x1i, colour);
            line(x0 - y1f, y0 - x1f, x0 + y1f, y0 - x1f, colour);
            }
            // octant 2: 90~135 criteria= +x in y : x0 + y, y0 + x
            if (flag2f and (abs(x2i) < 512) and (abs(y2i) < 512) and (abs(x2f) < 512) and (abs(y2f) < 512))
            {
            // triangle(x0, y0, x0 + y2i, y0 + x2i, x0 + y2f, y0 + x2f, colour);
            line(x0 - y2i, y0 + x2i, x0 + y2i, y0 + x2i, colour);
            line(x0 - y2f, y0 + x2f, x0 + y2f, y0 + x2f, colour);
            }
            // octant 3: 135~180 criteria= +x in x : x0 + x, y0 + y
            if (flag3f and (abs(x3i) < 512) and (abs(y3i) < 512) and (abs(x3f) < 512) and (abs(y3f) < 512))
            {
            // triangle(x0, y0, x0 + x3i, y0 + y3i, x0 + x3f, y0 + y3f, colour);
            line(x0 - x3i, y0 + y3i, x0 + x3i, y0 + y3i, colour);
            line(x0 - x3f, y0 + y3f, x0 + x3f, y0 + y3f, colour);
            }
            // // octant 4: 180~225 criteria= -x in x : x0 - x, y0 + y
            // if (flag4f and (abs(x4i) < 512) and (abs(y4i) < 512) and (abs(x4f) < 512) and (abs(y4f) < 512))
            // triangle(x0, y0, x0 - x4i, y0 + y4i, x0 - x4f, y0 + y4f, colour);
            //
            // // octant 5: 225~270 criteria= +x in y : x0 - y, y0 + x
            // if (flag5f and (abs(x5i) < 512) and (abs(y5i) < 512) and (abs(x5f) < 512) and (abs(y5f) < 512))
            // triangle(x0, y0, x0 - y5i, y0 + x5i, x0 - y5f, y0 + x5f, colour);
            //
            // // octant 6: 270~335 criteria= -x in y : x0 - y, y0 - x
            // if (flag6f and (abs(x6i) < 512) and (abs(y6i) < 512) and (abs(x6f) < 512) and (abs(y6f) < 512))
            // triangle(x0, y0, x0 - y6i, y0 - x6i, x0 - y6f, y0 - x6f, colour);
            //
            // // octant 7: 335~360 criteria= -x in x : x0 - x, y0 - y
            // if (flag7f and (abs(x7i) < 512) and (abs(y7i) < 512) and (abs(x7f) < 512) and (abs(y7f) < 512))
            // triangle(x0, y0, x0 - x7i, y0 - y7i, x0 - x7f, y0 - y7f, colour);
            //
        */
        if (_inCycle(0, start, end))
        {
            line(x0, y0 - radius, x0, y0 - radius, colour);
        }
        if (_inCycle(45, start, end))
        {
            line(x0 - p, y0 - p - 1, x0 + p, y0 - p - 1, colour);
        }
        if (_inCycle(90, start, end))
        {
            line(x0 - radius, y0, x0 + radius, y0, colour);
        }
        if (_inCycle(135, start, end))
        {
            line(x0 - p, y0 + p + 1, x0 + p, y0 + p + 1, colour);
        }
        /*
            if (_inCycle(180, start, end)) line(x0, y0 + radius, x0, y0 + radius, colour);
            if (_inCycle(225, start, end)) line(x0 + p, y0 + p, x0 - p, y0 + p, colour);
            if (_inCycle(270, start, end)) line(x0 + radius, y0, x0 - radius, y0, colour);
            if (_inCycle(335, start, end)) line(x0 + p, y0 - p, x0 - p, y0 - p, colour);
        */
    } // pen solid
}

// Touch functions
bool hV_Screen_Buffer::isTouch()
{
    return (_touchTrim > 0);
}
bool hV_Screen_Buffer::isTouchEvent()
{
    return _touchEvent;
}

bool hV_Screen_Buffer::getTouch(uint16_t & x, uint16_t & y, uint16_t & z, uint16_t & t)
{
    if (_touchTrim == 0)
    {
        return false;
    }

    bool _result = false;
    uint16_t x0, y0, z0, t0;

    hV_HAL_delayMilliseconds(16); // non-blocking delay to prevent freeze from I²C acquisition
    _getRawTouch(x0, y0, z0, t0); // Physical screen
    z = z0;
    t = t0;

    if (z > _touchTrim)
    {
        x0 = checkRange((uint16_t)(x0), (uint16_t)(_touchXmin), (uint16_t)(_touchXmax));
        y0 = checkRange((uint16_t)(y0), (uint16_t)(_touchYmin), (uint16_t)(_touchYmax));

        // Raw coordinates on physical screen to logical screen coordinates
        switch (_orientation)
        {
            case 0: // ok

                x = hV_HAL_map(x0, _touchXmin, _touchXmax, 0, _screenSizeH - 1);
                y = hV_HAL_map(y0, _touchYmin, _touchYmax, 0, _screenSizeV - 1);
                break;

            case 1: // ok

                x = hV_HAL_map(y0, _touchYmin, _touchYmax, 0, _screenSizeV - 1);
                y = hV_HAL_map(x0, _touchXmin, _touchXmax, _screenSizeH - 1, 0);
                break;

            case 2: // ok

                x = hV_HAL_map(x0, _touchXmin, _touchXmax, _screenSizeH - 1, 0);
                y = hV_HAL_map(y0, _touchYmin, _touchYmax, _screenSizeV - 1, 0);
                break;

            case 3: // ok

                x = hV_HAL_map(y0, _touchYmin, _touchYmax, _screenSizeV - 1, 0);
                y = hV_HAL_map(x0, _touchXmin, _touchXmax, 0, _screenSizeH - 1);
                break;
        }

        uint16_t _screenMarginX = screenMarginX();
        uint16_t _screenMarginY = screenMarginY();

        // Screen coordinates to margin coordinates
        if ((x > _screenMarginX) and (y > _screenMarginY)) // low margin
        {
            x -= _screenMarginX;
            y -= _screenMarginY;

            if ((x < screenSizeX()) and (y < screenSizeY())) // high margin
            {
                _result = true;
            }
        }
    }

    // hV_HAL_log(LEVEL_INFO, "%4ix%4i ? %2i", x, y, _result);
    return _result;
}

bool hV_Screen_Buffer::getTouchInterrupt()
{
    return _getInterruptTouch();
}

void hV_Screen_Buffer::clearTouch()
{
    uint16_t tx, ty, tz, tt;
    // while (getTouchInterrupt())
    while (getTouch(tx, ty, tz, tt))
    {
        hV_HAL_delayMilliseconds(10);
    }
    _touchEvent = TOUCH_EVENT_NONE;
}

bool hV_Screen_Buffer::isReadable()
{
    return _flagRead;
}
bool hV_Screen_Buffer::isStorage()
{
    return _flagStorage;
}

void hV_Screen_Buffer::_displayTarget(uint16_t x0, uint16_t y0, uint16_t colour)
{
    setPenSolid(false);
    circle(x0, y0, 8, colour);
    circle(x0, y0, 7, colour);
    circle(x0, y0, 6, colour);
}

// Touch functions
void hV_Screen_Buffer::_getRawTouch(uint16_t & x0, uint16_t & y0, uint16_t & z0, uint16_t & t0)
{
    x0 = 0;
    y0 = 0;
    z0 = 0;
    t0 = 0;
}

bool hV_Screen_Buffer::_getInterruptTouch()
{
    return false;
}

// Area functions
bool hV_Screen_Buffer::isArea()
{
    return (_fsmArea > 0);
}

// Font functions
void hV_Screen_Buffer::setFontSolid(bool flag)
{
    _f_setFontSolid(flag);
}

uint8_t hV_Screen_Buffer::addFont(font_s fontName)
{
    return _f_addFont(fontName);
}

void hV_Screen_Buffer::selectFont(uint8_t font)
{
    _f_selectFont(font);
}

uint8_t hV_Screen_Buffer::getFont()
{
    return _f_fontSize;
}

uint8_t hV_Screen_Buffer::fontMax()
{
    return _f_fontMax();
}

uint16_t hV_Screen_Buffer::characterSizeX(uint8_t character)
{
    uint16_t result = 0;
    if ((_f_font.kind & 0x40) == 0x40) // Monospaced font
    {
        result = _f_font.maxWidth + _f_fontSpaceX;
    }
    else
    {
        result = _f_characterSizeX(character);
    }

    return result;
}

uint16_t hV_Screen_Buffer::characterSizeY()
{
    return _f_characterSizeY();
}

#if (STRING_MODE == USE_STRING_OBJECT)
uint16_t hV_Screen_Buffer::stringSizeX(String text)
{
    return _f_stringSizeX(text);
}
#elif (STRING_MODE == USE_CHAR_ARRAY)
uint16_t hV_Screen_Buffer::stringSizeX(const char * text)
{
    return _f_stringSizeX(text);
}
#endif // STRING_MODE

#if (STRING_MODE == USE_STRING_OBJECT)
uint8_t hV_Screen_Buffer::stringLengthToFitX(String text, uint16_t pixels)
{
    return _f_stringLengthToFitX(text, pixels);
}
#elif (STRING_MODE == USE_CHAR_ARRAY)
uint8_t hV_Screen_Buffer::stringLengthToFitX(const char * text, uint16_t pixels)
{
    return _f_stringLengthToFitX(text, pixels);
}
#endif

void hV_Screen_Buffer::setFontSpaceX(uint8_t number)
{
    _f_setFontSpaceX(number);
}

void hV_Screen_Buffer::setFontSpaceY(uint8_t number)
{
    _f_setFontSpaceY(number);
}

uint8_t hV_Screen_Buffer::_getCharacter(uint8_t character, uint8_t index)
{
    return _f_getCharacter(character, index);
}

#if (STRING_MODE == USE_STRING_OBJECT)

void hV_Screen_Buffer::gText(uint16_t x0, uint16_t y0,
                             String text,
                             uint16_t textColour,
                             uint16_t backColour)
#if (FONT_MODE == USE_FONT_TERMINAL)
{
    uint8_t c;
    uint8_t line, line1, line2, line3;
    uint16_t x, y;
    uint8_t i, j, k;
#if (MAX_FONT_SIZE > 0)
    if (_f_fontSize == 0)
    {
        for (k = 0; k < text.length(); k++)
        {
            c = text.charAt(k) - ' ';

            for (i = 0; i < 6; i++)
            {
                line = _f_getCharacter(c, i);

                for (j = 0; j < 8; j++)
                    if (bitRead(line, j))
                    {
                        point(x0 + 6 * k + i, y0 + j, textColour);
                    }
                    else if (_f_fontSolid)
                    {
                        point(x0 + 6 * k + i, y0 + j, backColour);
                    }
            }
        }
    }
#if (MAX_FONT_SIZE > 1)
    else if (_f_fontSize == 1)
    {
        for (k = 0; k < text.length(); k++)
        {
            c = text.charAt(k) - ' ';

            for (i = 0; i < 8; i++)
            {
                line = _f_getCharacter(c, 2 * i);
                line1 = _f_getCharacter(c, 2 * i + 1);

                for (j = 0; j < 8; j++)
                {
                    if (bitRead(line, j))
                    {
                        point(x0 + 8 * k + i, y0 + j, textColour);
                    }
                    else if (_f_fontSolid)
                    {
                        point(x0 + 8 * k + i, y0 + j, backColour);
                    }
                    if (bitRead(line1, j))
                    {
                        point(x0 + 8 * k + i, y0 + 8 + j, textColour);
                    }
                    else if ((_f_fontSolid) and (j < 4))
                    {
                        point(x0 + 8 * k + i, y0 + 8 + j, backColour);
                    }
                }
            }
        }
    }
#if (MAX_FONT_SIZE > 2)
    else if (_f_fontSize == 2)
    {

        for (k = 0; k < text.length(); k++)
        {
            c = text.charAt(k) - ' ';

            for (i = 0; i < 12; i++)
            {
                line = _f_getCharacter(c, 2 * i);
                line1 = _f_getCharacter(c, 2 * i + 1);

                for (j = 0; j < 8; j++)
                {
                    if (bitRead(line, j))
                    {
                        point(x0 + 12 * k + i, y0 + j, textColour);
                    }
                    else if (_f_fontSolid)
                    {
                        point(x0 + 12 * k + i, y0 + j, backColour);
                    }
                    if (bitRead(line1, j))
                    {
                        point(x0 + 12 * k + i, y0 + 8 + j, textColour);
                    }
                    else if (_f_fontSolid)
                    {
                        point(x0 + 12 * k + i, y0 + 8 + j, backColour);
                    }
                }
            }
        }
    }
#if (MAX_FONT_SIZE > 3)
    else if (_f_fontSize == 3)
    {
        for (k = 0; k < text.length(); k++)
        {
            c = text.charAt(k) - ' ';
            for (i = 0; i < 16; i++)
            {
                line = _f_getCharacter(c, 3 * i);
                line1 = _f_getCharacter(c, 3 * i + 1);
                line2 = _f_getCharacter(c, 3 * i + 2);
                for (j = 0; j < 8; j++)
                {
                    if (bitRead(line, j))
                    {
                        point(x0 + 16 * k + i, y0 + j, textColour);
                    }
                    else if (_f_fontSolid)
                    {
                        point(x0 + 16 * k + i, y0 + j, backColour);
                    }
                    if (bitRead(line1, j))
                    {
                        point(x0 + 16 * k + i, y0 + 8 + j, textColour);
                    }
                    else if (_f_fontSolid)
                    {
                        point(x0 + 16 * k + i, y0 + 8 + j, backColour);
                    }
                    if (bitRead(line2, j))
                    {
                        point(x0 + 16 * k + i, y0 + 16 + j, textColour);
                    }
                    else if (_f_fontSolid)
                    {
                        point(x0 + 16 * k + i, y0 + 16 + j, backColour);
                    }
                }
            }
        }
    }
#endif // end MAX_FONT_SIZE > 3
#endif // end MAX_FONT_SIZE > 2
#endif // end MAX_FONT_SIZE > 1
#endif // end MAX_FONT_SIZE > 0
}
#else
{
    uint8_t character;
    uint8_t line;
    uint16_t x, y;
    uint8_t z;
    // uint8_t highTextColour = highByte(textColour);
    // uint8_t lowTextColour = lowByte(textColour);
    // uint8_t highBackColour = highByte(backColour);
    // uint8_t lowBackColour = lowByte(backColour);
    uint16_t textWidth = 0;
    uint16_t width; // variable
    uint16_t height = _f_characterSizeY(); // fixed
    // uint8_t kind = _getFontKind();
    bool isMonospace = ((_f_getFontKind() & 0x40) == 0x40);
    bool isHighDef = ((_f_getFontKind() & 0x80) == 0x80);
    uint16_t maxWidth = _f_getFontMaxWidth();

    if (!isHighDef) // Normal definition
    {
        for (uint8_t k = 0; k < text.length(); k++)
        {
            character = text.charAt(k);
            width = _f_characterSizeX(character) - _f_fontSpaceX;

            uint16_t bytes = roundUp(width, 8);
            uint8_t limit;

            uint8_t monoCharacter = 0;
            if (isMonospace) // Monospaced font
            {
                // Difference = displayed width - defined width
                monoCharacter = maxWidth - width;
            }
            // monoCharacter is now added to _f_fontSpaceX
            // Total _f_fontSpaceX + monoCharacter shared between preCharacter and postCharacter
            uint8_t preCharacter = (_f_fontSpaceX + monoCharacter) / 2;
            uint8_t postCharacter = (_f_fontSpaceX + monoCharacter) - preCharacter;

            if ((preCharacter > 0) and (_f_fontSolid))
            {
                bool oldPen = _penSolid;
                setPenSolid(true);
                dRectangle(x0 + textWidth, y0, preCharacter + 1, height, backColour);
                _penSolid = oldPen;
            }
            textWidth += preCharacter;

            for (uint16_t i = 0; i < height; i++)
            {
                y = i;
                for (uint16_t j = 0; j < bytes; j++)
                {
                    line = _f_getCharacter(character, i * bytes + j);

                    limit = 8;
                    if (width < j * 8 + 8)
                    {
                        limit = width % 8;
                    }

                    line >>= (8 - limit);
                    for (uint16_t l = limit; l > 0; l--)
                    {
                        x = j * 8 + limit - l;
                        if (bitRead(line, l - 1))
                        {
                            _setPoint(x0 + textWidth + x, y0 + y, textColour);
                        }
                        else if (_f_fontSolid)
                        {
                            _setPoint(x0 + textWidth + x, y0 + y, backColour);
                        }
                    }
                }
            }

            textWidth += width;
            if ((postCharacter > 0) and (_f_fontSolid))
            {
                bool oldPen = _penSolid;
                setPenSolid(true);
                dRectangle(x0 + textWidth, y0, postCharacter + 1, height, backColour);
                _penSolid = oldPen;
            }
            textWidth += postCharacter;
        }
    }
    else // High definition
    {
        uint16_t colour1;
        colour1 = myColours.weightedAverage(textColour, 2, backColour, 2);
        // uint8_t highColour1 = highByte(colour1);
        // uint8_t lowColour1 = lowByte(colour1);

        uint16_t colour2;
        colour2 = myColours.weightedAverage(textColour, 3, backColour, 1);
        // uint8_t highColour2 = highByte(colour2);
        // uint8_t lowColour2 = lowByte(colour2);

        for (uint8_t k = 0; k < text.length(); k++)
        {
            character = text.charAt(k);
            width = _f_characterSizeX(character) - _f_fontSpaceX;

            uint32_t bytes = roundUp(width * 2, 8);
            uint8_t limit;

            uint8_t monoCharacter = 0;
            if (isMonospace) // Monospaced font
            {
                // Difference = displayed width - defined width
                monoCharacter = maxWidth - width;
            }
            // monoCharacter is now added to _f_fontSpaceX
            // Total _f_fontSpaceX + monoCharacter shared between preCharacter and postCharacter
            uint8_t preCharacter = (_f_fontSpaceX + monoCharacter) / 2;
            uint8_t postCharacter = (_f_fontSpaceX + monoCharacter) - preCharacter;

            if ((preCharacter > 0) and (_f_fontSolid))
            {
                bool oldPen = _penSolid;
                setPenSolid(true);
                dRectangle(x0 + textWidth, y0, preCharacter + 1, height, backColour);
                _penSolid = oldPen;
            }
            textWidth += preCharacter;

            for (uint16_t i = 0; i < height; i++)
            {
                y = i;
                for (uint16_t j = 0; j < bytes; j++)
                {
                    line = _f_getCharacter(character, i * bytes + j);

                    limit = 4;
                    if (width < j * 4 + 4)
                    {
                        limit = width % 4;
                    }

                    line >>= (4 - limit) * 2;
                    for (uint32_t l = limit; l > 0; l--)
                    {
                        x = j * 4 + limit - l;
                        z = bitRead(line, 2 * l - 2) + bitRead(line, 2 * l - 1) * 2;

                        switch (z)
                        {
                            case 3:

                                _setPoint(x0 + textWidth + x, y0 + y, textColour);
                                break;

                            case 2:

                                _setPoint(x0 + textWidth + x, y0 + y, colour2);
                                break;

                            case 1:

                                _setPoint(x0 + textWidth + x, y0 + y, colour1);
                                break;

                            default:

                                break;
                        }
                    }
                }
            }
            textWidth += width;

            if ((postCharacter > 0) and (_f_fontSolid))
            {
                bool oldPen = _penSolid;
                setPenSolid(true);
                dRectangle(x0 + textWidth, y0, postCharacter + 1, height, backColour);
                _penSolid = oldPen;
            }
            textWidth += postCharacter;
        }
    }
}
#endif // FONT_MODE

#elif (STRING_MODE == USE_CHAR_ARRAY)

void hV_Screen_Buffer::gText(uint16_t x0, uint16_t y0,
                             const char * text,
                             uint16_t textColour,
                             uint16_t backColour)
#if (FONT_MODE == USE_FONT_TERMINAL)
{
    uint8_t c;
    uint8_t line, line1, line2, line3;
    uint16_t x, y;
    uint8_t i, j, k;
#if (MAX_FONT_SIZE > 0)
    if (_f_fontSize == 0)
    {
        for (k = 0; k < strlen(text); k++)
        {
            c = text[k] - ' ';

            for (i = 0; i < 6; i++)
            {
                line = _f_getCharacter(c, i);

                for (j = 0; j < 8; j++)
                    if (bitRead(line, j))
                    {
                        point(x0 + 6 * k + i, y0 + j, textColour);
                    }
                    else if (_f_fontSolid)
                    {
                        point(x0 + 6 * k + i, y0 + j, backColour);
                    }
            }
        }
    }
#if (MAX_FONT_SIZE > 1)
    else if (_f_fontSize == 1)
    {
        for (k = 0; k < strlen(text); k++)
        {
            c = text[k] - ' ';

            for (i = 0; i < 8; i++)
            {
                line = _f_getCharacter(c, 2 * i);
                line1 = _f_getCharacter(c, 2 * i + 1);

                for (j = 0; j < 8; j++)
                {
                    if (bitRead(line, j))
                    {
                        point(x0 + 8 * k + i, y0 + j, textColour);
                    }
                    else if (_f_fontSolid)
                    {
                        point(x0 + 8 * k + i, y0 + j, backColour);
                    }
                    if (bitRead(line1, j))
                    {
                        point(x0 + 8 * k + i, y0 + 8 + j, textColour);
                    }
                    else if ((_f_fontSolid) and (j < 4))
                    {
                        point(x0 + 8 * k + i, y0 + 8 + j, backColour);
                    }
                }
            }
        }
    }
#if (MAX_FONT_SIZE > 2)
    else if (_f_fontSize == 2)
    {

        for (k = 0; k < strlen(text); k++)
        {
            c = text[k] - ' ';

            for (i = 0; i < 12; i++)
            {
                line = _f_getCharacter(c, 2 * i);
                line1 = _f_getCharacter(c, 2 * i + 1);

                for (j = 0; j < 8; j++)
                {
                    if (bitRead(line, j))
                    {
                        point(x0 + 12 * k + i, y0 + j, textColour);
                    }
                    else if (_f_fontSolid)
                    {
                        point(x0 + 12 * k + i, y0 + j, backColour);
                    }
                    if (bitRead(line1, j))
                    {
                        point(x0 + 12 * k + i, y0 + 8 + j, textColour);
                    }
                    else if (_f_fontSolid)
                    {
                        point(x0 + 12 * k + i, y0 + 8 + j, backColour);
                    }
                }
            }
        }
    }
#if (MAX_FONT_SIZE > 3)
    else if (_f_fontSize == 3)
    {
        for (k = 0; k < strlen(text); k++)
        {
            c = text[k] - ' ';
            for (i = 0; i < 16; i++)
            {
                line = _f_getCharacter(c, 3 * i);
                line1 = _f_getCharacter(c, 3 * i + 1);
                line2 = _f_getCharacter(c, 3 * i + 2);
                for (j = 0; j < 8; j++)
                {
                    if (bitRead(line, j))
                    {
                        point(x0 + 16 * k + i, y0 + j, textColour);
                    }
                    else if (_f_fontSolid)
                    {
                        point(x0 + 16 * k + i, y0 + j, backColour);
                    }
                    if (bitRead(line1, j))
                    {
                        point(x0 + 16 * k + i, y0 + 8 + j, textColour);
                    }
                    else if (_f_fontSolid)
                    {
                        point(x0 + 16 * k + i, y0 + 8 + j, backColour);
                    }
                    if (bitRead(line2, j))
                    {
                        point(x0 + 16 * k + i, y0 + 16 + j, textColour);
                    }
                    else if (_f_fontSolid)
                    {
                        point(x0 + 16 * k + i, y0 + 16 + j, backColour);
                    }
                }
            }
        }
    }
#endif // end MAX_FONT_SIZE > 3
#endif // end MAX_FONT_SIZE > 2
#endif // end MAX_FONT_SIZE > 1
#endif // end MAX_FONT_SIZE > 0
}
#else
{
    uint8_t character;
    uint8_t line;
    uint16_t x, y;
    uint8_t z;
    // uint8_t highTextColour = highByte(textColour);
    // uint8_t lowTextColour = lowByte(textColour);
    // uint8_t highBackColour = highByte(backColour);
    // uint8_t lowBackColour = lowByte(backColour);
    uint16_t textWidth = 0;
    uint16_t width; // variable
    uint16_t height = _f_characterSizeY(); // fixed
    // uint8_t kind = _getFontKind();
    bool isMonospace = ((_f_getFontKind() & 0x40) == 0x40);
    bool isHighDef = ((_f_getFontKind() & 0x80) == 0x80);
    uint16_t maxWidth = _f_getFontMaxWidth();

    if (!isHighDef) // Normal definition
    {
        for (uint8_t k = 0; k < strlen(text); k++)
        {
            character = text[k];
            width = _f_characterSizeX(character) - _f_fontSpaceX;

            uint16_t bytes = roundUp(width, 8);
            uint8_t limit;

            uint8_t monoCharacter = 0;
            if (isMonospace) // Monospaced font
            {
                // Difference = displayed width - defined width
                monoCharacter = maxWidth - width;
            }
            // monoCharacter is now added to _f_fontSpaceX
            // Total _f_fontSpaceX + monoCharacter shared between preCharacter and postCharacter
            uint8_t preCharacter = (_f_fontSpaceX + monoCharacter) / 2;
            uint8_t postCharacter = (_f_fontSpaceX + monoCharacter) - preCharacter;

            if ((preCharacter > 0) and (_f_fontSolid))
            {
                bool oldPen = _penSolid;
                setPenSolid(true);
                dRectangle(x0 + textWidth, y0, preCharacter + 1, height, backColour);
                _penSolid = oldPen;
            }
            textWidth += preCharacter;

            for (uint16_t i = 0; i < height; i++)
            {
                y = i;
                for (uint16_t j = 0; j < bytes; j++)
                {
                    line = _f_getCharacter(character, i * bytes + j);

                    limit = 8;
                    if (width < j * 8 + 8)
                    {
                        limit = width % 8;
                    }

                    line >>= (8 - limit);
                    for (uint16_t l = limit; l > 0; l--)
                    {
                        x = j * 8 + limit - l;
                        if (bitRead(line, l - 1))
                        {
                            _setPoint(x0 + textWidth + x, y0 + y, textColour);
                        }
                        else if (_f_fontSolid)
                        {
                            _setPoint(x0 + textWidth + x, y0 + y, backColour);
                        }
                    }
                }
            }

            textWidth += width;
            if ((postCharacter > 0) and (_f_fontSolid))
            {
                bool oldPen = _penSolid;
                setPenSolid(true);
                dRectangle(x0 + textWidth, y0, postCharacter + 1, height, backColour);
                _penSolid = oldPen;
            }
            textWidth += postCharacter;
        }
    }
    else // High definition
    {
        uint16_t colour1;
        colour1 = myColours.weightedAverage(textColour, 2, backColour, 2);
        // uint8_t highColour1 = highByte(colour1);
        // uint8_t lowColour1 = lowByte(colour1);

        uint16_t colour2;
        colour2 = myColours.weightedAverage(textColour, 3, backColour, 1);
        // uint8_t highColour2 = highByte(colour2);
        // uint8_t lowColour2 = lowByte(colour2);

        for (uint8_t k = 0; k < strlen(text); k++)
        {
            character = text[k];
            width = _f_characterSizeX(character) - _f_fontSpaceX;

            uint32_t bytes = roundUp(width * 2, 8);
            uint8_t limit;

            uint8_t monoCharacter = 0;
            if (isMonospace) // Monospaced font
            {
                // Difference = displayed width - defined width
                monoCharacter = maxWidth - width;
            }
            // monoCharacter is now added to _f_fontSpaceX
            // Total _f_fontSpaceX + monoCharacter shared between preCharacter and postCharacter
            uint8_t preCharacter = (_f_fontSpaceX + monoCharacter) / 2;
            uint8_t postCharacter = (_f_fontSpaceX + monoCharacter) - preCharacter;

            if ((preCharacter > 0) and (_f_fontSolid))
            {
                bool oldPen = _penSolid;
                setPenSolid(true);
                dRectangle(x0 + textWidth, y0, preCharacter + 1, height, backColour);
                _penSolid = oldPen;
            }
            textWidth += preCharacter;

            for (uint16_t i = 0; i < height; i++)
            {
                y = i;
                for (uint16_t j = 0; j < bytes; j++)
                {
                    line = _f_getCharacter(character, i * bytes + j);

                    limit = 4;
                    if (width < j * 4 + 4)
                    {
                        limit = width % 4;
                    }

                    line >>= (4 - limit) * 2;
                    for (uint32_t l = limit; l > 0; l--)
                    {
                        x = j * 4 + limit - l;
                        z = bitRead(line, 2 * l - 2) + bitRead(line, 2 * l - 1) * 2;

                        switch (z)
                        {
                            case 3:

                                _setPoint(x0 + textWidth + x, y0 + y, textColour);
                                break;

                            case 2:

                                _setPoint(x0 + textWidth + x, y0 + y, colour2);
                                break;

                            case 1:

                                _setPoint(x0 + textWidth + x, y0 + y, colour1);
                                break;

                            default:

                                break;
                        }
                    }
                }
            }
            textWidth += width;

            if ((postCharacter > 0) and (_f_fontSolid))
            {
                bool oldPen = _penSolid;
                setPenSolid(true);
                dRectangle(x0 + textWidth, y0, postCharacter + 1, height, backColour);
                _penSolid = oldPen;
            }
            textWidth += postCharacter;
        }
    }
}
#endif // FONT_MODE

#endif // STRING_MODE

// Other functions
bool hV_Screen_Buffer::isIntensity()
{
    return _flagIntensity;
}

void hV_Screen_Buffer::setIntensity(uint8_t intensity)
{
    _intensity = intensity;
    _setIntensity(_intensity);
}

uint8_t hV_Screen_Buffer::getIntensity()
{
    return _intensity;
}

bool hV_Screen_Buffer::isEnergy()
{
    return _flagEnergy;
}

void hV_Screen_Buffer::setBacklight(bool state)
{
    _setBacklight(state);
}

void hV_Screen_Buffer::setEnergy(uint32_t seconds)
{
    _msEnergy = (uint32_t)seconds * 1000;
    _chronoEnergy = hV_HAL_getMilliseconds() + _msEnergy;
    _stateEnergy = true;
}

void hV_Screen_Buffer::updateEnergy(bool state)
{
    if (_msEnergy > 0)
    {
        if (state)
        {
            _chronoEnergy = hV_HAL_getMilliseconds() + _msEnergy;
            if (!_stateEnergy)
            {
                setBacklight(true);
            }
            _stateEnergy = true;
        }
        else if (hV_HAL_getMilliseconds() > _chronoEnergy)
        {
            if (_stateEnergy)
            {
                setBacklight(false);
            }
            _stateEnergy = false;
        }
    }
}

void hV_Screen_Buffer::_setIntensity(uint8_t intensity)
{
    ;
}

void hV_Screen_Buffer::_setBacklight(bool flag)
{
    ;
}

void hV_Screen_Buffer::_setWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    ;
}

font_s hV_Screen_Buffer::getFontDetails()
{
    return _f_font;
}

bool hV_Screen_Buffer::getFontSolid()
{
    return _f_fontSolid;
}
bool hV_Screen_Buffer::getPenSolid()
{
    return _penSolid;
}

// void hV_Screen_Buffer::setMarginHV(uint16_t marginH, uint16_t marginV)
// {
//     _screenMarginH = marginH;
//     _screenMarginV = marginV;
// }

void hV_Screen_Buffer::setMarginXY(uint16_t marginX, uint16_t marginY)
{
    switch (_orientation)
    {
        case 0:
        case 2:

            _screenMarginH = marginX;
            _screenMarginV = marginY;
            break;

        case 1:
        case 3:

            _screenMarginH = marginY;
            _screenMarginV = marginX;
            break;
    }
}

// uint16_t hV_Screen_Buffer::getMarginH()
// {
//     return _screenMarginH;
// }
// 
// uint16_t hV_Screen_Buffer::getMarginV()
// {
//     return _screenMarginV;
// }

uint16_t hV_Screen_Buffer::screenMarginX()
{
    switch (_orientation)
    {
        case 1:
        case 3:

            return _screenMarginV; // _maxX
            break;

        // case 0:
        // case 2:
        default:

            return _screenMarginH; // _maxX
            break;
    }
    return 0;
}

uint16_t hV_Screen_Buffer::screenMarginY()
{
    switch (_orientation)
    {
        case 1:
        case 3:

            return _screenMarginH; // _maxY
            break;

        // case 0:
        // case 2:
        default:

            return _screenMarginV; // _maxY
            break;
    }
    return 0;
}

void hV_Screen_Buffer::showImage(uint16_t x, uint16_t y, uint16_t width, uint16_t length, uint8_t *data, uint16_t colour)
{
    uint16_t b = width/8;

    for (uint16_t l = 0; l < length; l++)
    {
//        Serial.println();
//        Serial.printf("%03d |", l);
        for (uint16_t w = 0; w < width; w++)
        {
            if(data[(l*b)+(w/8)] & 0x80>>(w%8))
            {
                _setPoint(x+w, y+l, colour);
            }
        }
    }
}