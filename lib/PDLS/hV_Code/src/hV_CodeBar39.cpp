//
// hV_CodeBar39.cpp
// Library C++ code
// ----------------------------------
//
// Details Generate and display a QR-code
// Project Pervasive Displays Library Suite
//
// Created by Rei Vilo, 21 Jan 2021
//
// Copyright (c) Rei Vilo, 2010-2023
// Licence All rights reserved
//
// See hV_CodeBar.h and ReadMe.txt for references
//
// Release 605: Improved result with RESULT_SUCCESS and RESULT_ERROR
//

// Library header
#include "hV_CodeBar39.h"

// Other libraries

// Ratio 1/3: "0"=x "1"=xxx
const uint16_t code39[] =
{
    0b1000111010111010, // 32 ( )
    0b0000000000000000, // 33 ()
    0b0000000000000000, // 34 ()
    0b0000000000000000, // 35 ()
    0b1000100010001010, // 36 ($)
    0b1010001000100010, // 37 (%)
    0b0000000000000000, // 38 ()
    0b0000000000000000, // 39 ()
    0b0000000000000000, // 40 ()
    0b0000000000000000, // 41 ()
    0b1000101110111010, // 42 (*)
    0b1000101000100010, // 43 (+)
    0b0000000000000000, // 44 ()
    0b1000101011101110, // 45 (-)
    0b1110001010111010, // 46 (.)
    0b1000100010100010, // 47 (/)
    0b1010001110111010, // 48 (0)
    0b1110100010101110, // 49 (1)
    0b1011100010101110, // 50 (2)
    0b1110111000101010, // 51 (3)
    0b1010001110101110, // 52 (4)
    0b1110100011101010, // 53 (5)
    0b1011100011101010, // 54 (6)
    0b1010001011101110, // 55 (7)
    0b1110100010111010, // 56 (8)
    0b1011100010111010, // 57 (9)
    0b0000000000000000, // 58 ()
    0b0000000000000000, // 59 ()
    0b0000000000000000, // 60 ()
    0b0000000000000000, // 61 ()
    0b0000000000000000, // 62 ()
    0b0000000000000000, // 63 ()
    0b0000000000000000, // 64 ()
    0b1110101000101110, // 65 (A)
    0b1011101000101110, // 66 (B)
    0b1110111010001010, // 67 (C)
    0b1010111000101110, // 68 (D)
    0b1110101110001010, // 69 (E)
    0b1010111010001110, // 69 (E)
    0b1011101110001010, // 70 (F)
    0b1010100011101110, // 71 (G)
    0b1110101000111010, // 72 (H)
    0b1011101000111010, // 73 (I)
    0b1010111000111010, // 74 (J)
    0b1110101010001110, // 75 (K)
    0b1011101010001110, // 76 (L)
    0b1110001110101010, // 76 (L)
    0b1110111010100010, // 77 (M)
    0b1110101110100010, // 79 (O)
    0b1011101110100010, // 80 (P)
    0b1010101110001110, // 81 (Q)
    0b1110101011100010, // 82 (R)
    0b1011101011100010, // 83 (S)
    0b1010111011100010, // 84 (T)
    0b1110001010101110, // 85 (U)
    0b1000111010101110, // 86 (V)
    0b1000101110101110, // 88 (X)
    0b1110001011101010, // 89 (Y)
    0b1000111011101010 // 90 (Z)
};

// Code
#if (STRING_MODE == USE_STRING_OBJECT)
bool codeBar39(hV_Screen_Virtual * screen, uint16_t x0, uint16_t y0, String text, uint16_t height, uint16_t frontColour, uint16_t backColour, bool displayText)
{
    if (text.length() > 29)
    {
        return RESULT_ERROR;
    }

    uint16_t multiplier = 1;
    // w= total width
    // getSizeCodeBar39() includes *
    // 12 white spaces left and right
    uint16_t w = (12 + getSizeCodeBar39(text) + 12) * multiplier;
    text = "*" + text + "*";
    char letter;
    uint16_t code;

    // Background for bars
    screen->setPenSolid(true);
    screen->dRectangle(x0, y0, w, (height + 4) * multiplier, backColour);
    uint16_t dx = 12 * multiplier; // 12 white spaces left
    uint16_t dy = 2 * multiplier; // 2 white lines on top

    for (uint8_t i = 0; i < text.length(); i++)
    {
        letter = text.charAt(i);
        if ((letter >= 'a') and (letter <= 'z'))
        {
            letter = letter - 'a' + 'A';
        }
        code = code39[letter - ' '];

        for (uint8_t j = 16; j > 0; j--)
        {
            screen->dRectangle(x0 + dx + (16 * i + (16 - j)) * multiplier, y0 + dy, multiplier, height * multiplier, bitRead(code, j - 1) ? frontColour : backColour);
        }
    }

    if (displayText)
    {
        // Background for text
        screen->selectFont(0);
        y0 += (height + 2) * multiplier;
        screen->dRectangle(x0, y0, w, screen->characterSizeY(), backColour);

        for (uint8_t i = 0; i < text.length(); i++)
        {
            letter = text.charAt(i);
            if ((letter >= 'a') and (letter <= 'z'))
            {
                letter = letter - 'a' + 'A';
            }
            uint16_t dw = (16 * multiplier - screen->characterSizeX(letter)) / 2;
            screen->gText(x0 + dx + (16 * i) * multiplier + dw, y0, String(letter), frontColour, backColour);
        }
    }
    screen->setPenSolid(false);

    return RESULT_SUCCESS;
}

#elif (STRING_MODE == USE_CHAR_ARRAY)

bool codeBar39(hV_Screen_Virtual * screen, uint16_t x0, uint16_t y0, const char * text, uint16_t height, uint16_t frontColour, uint16_t backColour, bool displayText)
{
    if (strlen(text) > 29)
    {
        return RESULT_ERROR;
    }

    uint16_t multiplier = 1;
    // w= total width
    // getSizeCodeBar39() includes *
    // 12 white spaces left and right
    uint16_t w = (12 + getSizeCodeBar39(text) + 12) * multiplier;
    char work[40];
    memset(work, 0x00, 40);
    strcpy(work, "*");
    strcat(work, text);
    strcat(work, "*");
    char letter;
    uint16_t code;

    // Background for bars
    screen->setPenSolid(true);
    screen->dRectangle(x0, y0, w, (height + 4) * multiplier, backColour);
    uint16_t dx = 12 * multiplier; // 12 white spaces left
    uint16_t dy = 2 * multiplier; // 2 white lines on top

    for (uint8_t i = 0; i < strlen(work); i++)
    {
        letter = work[i];
        if ((letter >= 'a') and (letter <= 'z'))
        {
            letter = letter - 'a' + 'A';
        }
        code = code39[letter - ' '];

        for (uint8_t j = 16; j > 0; j--)
        {
            screen->dRectangle(x0 + dx + (16 * i + (16 - j)) * multiplier, y0 + dy, multiplier, height * multiplier, bitRead(code, j - 1) ? frontColour : backColour);
        }
    }

    if (displayText)
    {
        // Background for work
        screen->selectFont(0);
        y0 += (height + 2) * multiplier;
        screen->dRectangle(x0, y0, w, screen->characterSizeY(), backColour);

        char two[2];
        two[1] = 0x00;
        for (uint8_t i = 0; i < strlen(work); i++)
        {
            letter = work[i];
            if ((letter >= 'a') and (letter <= 'z'))
            {
                letter = letter - 'a' + 'A';
            }
            two[0] = letter;
            uint16_t dw = (16 * multiplier - screen->characterSizeX(letter)) / 2;
            screen->gText(x0 + dx + (16 * i) * multiplier + dw, y0, two, frontColour, backColour);
        }
    }
    screen->setPenSolid(false);

    return RESULT_SUCCESS;
}

#endif // STRING_MODE

#if (STRING_MODE == USE_STRING_OBJECT)

uint16_t getSizeCodeBar39(String text)
{
    return (text.length() + 2) * 16;
}

#elif (STRING_MODE == USE_CHAR_ARRAY)

uint16_t getSizeCodeBar39(const char * text)
{
    // StartB + text + CRC + Stop 13-bit
    return (strlen(text) + 2) * 16;
}

#endif // STRING_MODE
