//
// hV_CodeBar128.cpp
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
// http://www.keepautomation.com/online_barcode_generator/code_128/ and https://www.barcodefaq.com/1d/code-128/#CalculationExamples
//
// Release 605: Improved result with RESULT_SUCCESS and RESULT_ERROR
//

// Library header
#include "hV_CodeBar128.h"

// Other libraries

// Ratio 1/3: "0"=x "1"=xxx
const uint16_t code128[] =
{
    0b11011001100, // 32 (space)
    0b11001101100, // 33 (!)
    0b11001100110, // 34 (")
    0b10010011000, // 35 (#)
    0b10010001100, // 36 ($)
    0b10001001100, // 37 (%)
    0b10011001000, // 38 (&)
    0b10011000100, // 39 ()
    0b10001100100, // 40 (()
    0b11001001000, // 41 ())
    0b11001000100, // 42 (*)
    0b11000100100, // 43 (+)
    0b10110011100, // 44 (,)
    0b10011011100, // 45 (-)
    0b10011001110, // 46 (.)
    0b10111001100, // 47 (/)
    0b10011101100, // 48 (0)
    0b10011100110, // 49 (1)
    0b11001110010, // 50 (2)
    0b11001011100, // 51 (3)
    0b11001001110, // 52 (4)
    0b11011100100, // 53 (5)
    0b11001110100, // 54 (6)
    0b11101101110, // 55 (7)
    0b11101001100, // 56 (8)
    0b11100101100, // 57 (9)
    0b11100100110, // 58 (:)
    0b11101100100, // 59 (;)
    0b11100110100, // 60 (<)
    0b11100110010, // 61 (=)
    0b11011011000, // 62 (>)
    0b11011000110, // 63 (?)
    0b11000110110, // 64 (@)
    0b10100011000, // 65 (A)
    0b10001011000, // 66 (B)
    0b10001000110, // 67 (C)
    0b10110001000, // 68 (D)
    0b10001101000, // 69 (E)
    0b10001100010, // 70 (F)
    0b11010001000, // 71 (G)
    0b11000101000, // 72 (H)
    0b11000100010, // 73 (I)
    0b10110111000, // 74 (J)
    0b10110001110, // 75 (K)
    0b10001101110, // 76 (L)
    0b10111011000, // 77 (M)
    0b10111000110, // 78 (N)
    0b10001110110, // 79 (O)
    0b11101110110, // 80 (P)
    0b11010001110, // 81 (Q)
    0b11000101110, // 82 (R)
    0b11011101000, // 83 (S)
    0b11011100010, // 84 (T)
    0b11011101110, // 85 (U)
    0b11101011000, // 86 (V)
    0b11101000110, // 87 (W)
    0b11100010110, // 88 (X)
    0b11101101000, // 89 (Y)
    0b11101100010, // 90 (Z)
    0b11100011010, // 91 ([)
    0b11101111010, // 92 (\)
    0b11001000010, // 93 (])
    0b11110001010, // 94 (^)
    0b10100110000, // 95 (_)
    0b10100001100, // 96 (`)
    0b10010110000, // 97 (a)
    0b10010000110, // 98 (b)
    0b10000101100, // 99 (c)
    0b10000100110, // 100 (d)
    0b10110010000, // 101 (e)
    0b10110000100, // 102 (f)
    0b10011010000, // 103 (g)
    0b10011000010, // 104 (h)
    0b10000110100, // 105 (i)
    0b10000110010, // 106 (j)
    0b11000010010, // 107 (k)
    0b11001010000, // 108 (l)
    0b11110111010, // 109 (m)
    0b11000010100, // 110 (n)
    0b10001111010, // 111 (o)
    0b10100111100, // 112 (p)
    0b10010111100, // 113 (q)
    0b10010011110, // 114 (r)
    0b10111100100, // 115 (s)
    0b10011110100, // 116 (t)
    0b10011110010, // 117 (u)
    0b11110100100, // 118 (v)
    0b11110010100, // 119 (w)
    0b11110010010, // 120 (x)
    0b11011011110, // 121 (y)
    0b11011110110, // 122 (z)
    0b11110110110, // 123 ({)
    0b10101111000, // 124 (|)
    0b10100011110, // 125 (})
    0b10001011110, // 126 (~)
    0b10111101000, // 127 (DEL)
};

const uint16_t startB = 0b11010010000; // Start Code B
const uint16_t stop13 = 0b1100011101011; // Stop 13-bit

// Code
#if (STRING_MODE == USE_STRING_OBJECT)
bool codeBar128(hV_Screen_Virtual * screen, uint16_t x0, uint16_t y0, String text, uint16_t height, uint16_t frontColour, uint16_t backColour, bool displayText)
{
    if (text.length() > 29)
    {
        return RESULT_ERROR;
    }

    uint16_t multiplier = 1;
    // w = total width
    // getSizeCodeBar128() includes startB, CRC and stop13
    // 12 white spaces left and right
    uint16_t w = (12 + getSizeCodeBar128(text) + 12) * multiplier;
    char letter;
    uint16_t code;
    uint32_t checksum = 104;

    // Background for bars
    screen->setPenSolid(true);
    screen->dRectangle(x0, y0, w, (height + 4) * multiplier, backColour);
    uint16_t dx = 12 * multiplier; // 12 white spaces left
    uint16_t dy = 2 * multiplier; // 2 white lines on top

    // startB
    for (uint8_t j = 11; j > 0; j--)
    {
        screen->dRectangle(x0 + dx + (11 * 0 + 11 - j) * multiplier, y0 + dy, multiplier, height * multiplier, bitRead(startB, j - 1) ? frontColour : backColour);
    }
    dx += 11 * multiplier;

    // text
    for (uint8_t i = 0; i < text.length(); i++)
    {
        letter = text.charAt(i);
        code = code128[letter - ' '];
        checksum += (uint32_t)(letter - ' ') * (i + 1);

        for (uint8_t j = 11; j > 0; j--)
        {
            screen->dRectangle(x0 + dx + (11 * i + 11 - j) * multiplier, y0 + dy, multiplier, height * multiplier, bitRead(code, j - 1) ? frontColour : backColour);
        }
    }

    // checksum
    checksum %= 103;
    code = code128[checksum];
    for (uint8_t j = 11; j > 0; j--)
    {
        screen->dRectangle(x0 + dx + (11 * text.length() + 11 - j) * multiplier, y0 + dy, multiplier, height * multiplier, bitRead(code, j - 1) ? frontColour : backColour);
    }

    // stop13
    for (uint8_t j = 13; j > 0; j--)
    {
        screen->dRectangle(x0 + dx + (11 * text.length() + 11 + 13 - j) * multiplier, y0 + dy, multiplier, height * multiplier, bitRead(stop13, j - 1) ? frontColour : backColour);
    }

    // left sapce + StartB + CRC + Stop 13-bit + right space
    dx = (12 + 11 + 11 + 13 + 12) / 2;
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
            uint16_t dw = (11 * multiplier - screen->characterSizeX(letter)) / 2;
            screen->gText(x0 - 11 + 12 * multiplier + dx + (11 * i) * multiplier + dw, y0, String(letter), frontColour, backColour);
        }
    }
    screen->setPenSolid(false);

    return RESULT_SUCCESS;
}

#elif (STRING_MODE == USE_CHAR_ARRAY)

bool codeBar128(hV_Screen_Virtual * screen, uint16_t x0, uint16_t y0, const char * text, uint16_t height, uint16_t frontColour, uint16_t backColour, bool displayText)
{
    if (strlen(text) > 29)
    {
        return RESULT_ERROR;
    }

    uint16_t multiplier = 1;
    // w = total width
    // getSizeCodeBar128() includes startB, CRC and stop13
    // 12 white spaces left and right
    uint16_t w = (12 + getSizeCodeBar128(text) + 12) * multiplier;
    char letter;
    uint16_t code;
    uint32_t checksum = 104;

    // Background for bars
    screen->setPenSolid(true);
    screen->dRectangle(x0, y0, w, (height + 4) * multiplier, backColour);
    uint16_t dx = 12 * multiplier; // 12 white spaces left
    uint16_t dy = 2 * multiplier; // 2 white lines on top

    // startB
    for (uint8_t j = 11; j > 0; j--)
    {
        screen->dRectangle(x0 + dx + (11 * 0 + 11 - j) * multiplier, y0 + dy, multiplier, height * multiplier, bitRead(startB, j - 1) ? frontColour : backColour);
    }
    dx += 11 * multiplier;

    // text
    for (uint8_t i = 0; i < strlen(text); i++)
    {
        letter = text[i];
        code = code128[letter - ' '];
        checksum += (uint32_t)(letter - ' ') * (i + 1);

        for (uint8_t j = 11; j > 0; j--)
        {
            screen->dRectangle(x0 + dx + (11 * i + 11 - j) * multiplier, y0 + dy, multiplier, height * multiplier, bitRead(code, j - 1) ? frontColour : backColour);
        }
    }

    // checksum
    checksum %= 103;
    code = code128[checksum];
    for (uint8_t j = 11; j > 0; j--)
    {
        screen->dRectangle(x0 + dx + (11 * strlen(text) + 11 - j) * multiplier, y0 + dy, multiplier, height * multiplier, bitRead(code, j - 1) ? frontColour : backColour);
    }

    // stop13
    for (uint8_t j = 13; j > 0; j--)
    {
        screen->dRectangle(x0 + dx + (11 * strlen(text) + 11 + 13 - j) * multiplier, y0 + dy, multiplier, height * multiplier, bitRead(stop13, j - 1) ? frontColour : backColour);
    }

    // left sapce + StartB + CRC + Stop 13-bit + right space
    dx = (12 + 11 + 11 + 13 + 12) / 2;
    if (displayText)
    {
        // Background for text
        screen->selectFont(0);
        y0 += (height + 2) * multiplier;
        screen->dRectangle(x0, y0, w, screen->characterSizeY(), backColour);

        char two[2];
        two[1] = 0x00;
        for (uint8_t i = 0; i < strlen(text); i++)
        {
            letter = text[i];
            if ((letter >= 'a') and (letter <= 'z'))
            {
                letter = letter - 'a' + 'A';
            }
            two[0] = letter;
            uint16_t dw = (11 * multiplier - screen->characterSizeX(letter)) / 2;
            screen->gText(x0 - 11 + 12 * multiplier + dx + (11 * i) * multiplier + dw, y0, two, frontColour, backColour);
        }
    }
    screen->setPenSolid(false);

    return RESULT_SUCCESS;
}

#endif // STRING_MODE

#if (STRING_MODE == USE_STRING_OBJECT)

uint16_t getSizeCodeBar128(String text)
{
    // StartB + text + CRC + Stop 13-bit
    return 11 + text.length() * 11 + 11 + 13;
}

#elif (STRING_MODE == USE_CHAR_ARRAY)

uint16_t getSizeCodeBar128(const char * text)
{
    // StartB + text + CRC + Stop 13-bit
    return 11 + strlen(text) * 11 + 11 + 13;
}

#endif // STRING_MODE
