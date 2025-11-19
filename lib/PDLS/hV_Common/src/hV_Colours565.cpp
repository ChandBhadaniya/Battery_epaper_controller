//
// hV_Colours565.cpp
// Class library C++ code
// ----------------------------------
//
// Project Pervasive Displays Library Suite
// Based on highView technology
//
// Created by Rei Vilo, 31 Dec 2013
//
// Copyright (c) Rei Vilo, 2010-2023
// Licence All rights reserved
//
// See hV_Colours565.h for references
//
// Release 611: Added support for red and yellow colour screens

// Library header
#include "hV_Colours565.h"

// Code
hV_Colours565::hV_Colours565()
{
    ;
}

// Colour
uint16_t hV_Colours565::calculate(uint8_t red, uint8_t green, uint8_t blue)
{
    return (red >> 3) << 11 | (green >> 2) << 5 | (blue >> 3);
}

void hV_Colours565::split(uint16_t rgb, uint8_t & red, uint8_t & green, uint8_t & blue)
{
    // rgb16 = red5 green6 blue5
    red   = ((rgb & 0b1111100000000000) >> 11) * 255 / 31 ; // << 3;
    green = ((rgb & 0b0000011111100000) >>  5) * 255 / 63 ; // << 2;
    blue  = ((rgb & 0b0000000000011111)) * 255 / 31 ;       // << 3;
}

uint16_t hV_Colours565::halve(uint16_t rgb)
{
    // rgb16 = red5 green6 blue5
    return (((rgb & 0b1111100000000000) >> 12) << 11 | \
            ((rgb & 0b0000011111100000) >>  6) <<  5 | \
            ((rgb & 0b0000000000011111) >>  1));
}

uint16_t hV_Colours565::average(uint16_t rgb1, uint16_t rgb2)
{
    uint8_t r1, g1, b1, r2, g2, b2;
    uint16_t r, g, b;
    split(rgb1, r1, g1, b1);
    split(rgb2, r2, g2, b2);
    r = (uint16_t)(r1 + r2) / 2;
    g = (uint16_t)(g1 + g2) / 2;
    b = (uint16_t)(b1 + b2) / 2;
    return calculate((uint8_t)r, (uint8_t)g, (uint8_t)b);
}

uint16_t hV_Colours565::weightedAverage(uint16_t rgb1, uint8_t weight1, uint16_t rgb2, uint8_t weight2)
{
    if (weight1 == 0)
    {
        return rgb2;
    }
    if (weight2 == 0)
    {
        return rgb1;
    }

    uint8_t r1, g1, b1, r2, g2, b2;
    uint16_t r, g, b;
    uint16_t weight = weight1 + weight2;
    split(rgb1, r1, g1, b1);
    split(rgb2, r2, g2, b2);
    r = (uint16_t)(r1 * weight1 + r2 * weight2) / weight;
    g = (uint16_t)(g1 * weight1 + g2 * weight2) / weight;
    b = (uint16_t)(b1 * weight1 + b2 * weight2) / weight;
    return calculate((uint8_t)r, (uint8_t)g, (uint8_t)b);
}

uint16_t hV_Colours565::reverse(uint16_t rgb)
{
    // rgb16 = red5 green6 blue5
    return (uint16_t)(rgb ^ 0b1111111111111111);
}

uint16_t hV_Colours565::code(uint8_t value09)
{
    bool flag = (value09 > 9);
    value09 %= 10;
    uint16_t colour;

    uint16_t colours[10] = {black, brown, red, orange, yellow, green, blue, violet, grey, white};
    colour = colours[value09];

    if (flag)
    {
        return halve(colour);
    }
    else
    {
        return colour;
    }
}

hV_Colours565 myColours = hV_Colours565();

