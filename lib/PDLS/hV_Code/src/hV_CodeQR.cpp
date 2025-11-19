//
// hV_CodeQR.cpp
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
// See hV_CodeQR.h and ReadMe.txt for references
//
// Release 509: New https://github.com/ricmoo/qrcode/ library
// Release 605: Improved result with RESULT_SUCCESS and RESULT_ERROR
//

// Library header
#include "hV_CodeQR.h"

// Other libraries
#include "qrcode.h"

// Code
#if (STRING_MODE == USE_STRING_OBJECT)

bool codeQR(hV_Screen_Virtual * screen, uint16_t x0, uint16_t y0, String text, uint8_t multiplier, uint16_t frontColour, uint16_t backColour)

#elif (STRING_MODE == USE_CHAR_ARRAY)

bool codeQR(hV_Screen_Virtual * screen, uint16_t x0, uint16_t y0, const char * text, uint8_t multiplier, uint16_t frontColour, uint16_t backColour)

#endif // STRING_MODE
{
#if (STRING_MODE == USE_STRING_OBJECT)

    if (text.length() > QR_MAX_LENGTH)
    {
        return RESULT_ERROR;
    }

#elif (STRING_MODE == USE_CHAR_ARRAY)

    if (strlen(text) > QR_MAX_LENGTH)
    {
        return RESULT_ERROR;
    }

#endif // STRING_MODE

    // The structure to manage the QR code
    QRCode qrcode;

    // Allocate a chunk of memory to store the QR code
    uint8_t qrcodeBytes[qrcode_getBufferSize(QR_VERSION)];

#if (STRING_MODE == USE_STRING_OBJECT)

    // Generate the QR-code
    qrcode_initText(&qrcode, qrcodeBytes, QR_VERSION, QR_ECC, text.c_str());

#elif (STRING_MODE == USE_CHAR_ARRAY)

    // Generate the QR-code
    qrcode_initText(&qrcode, qrcodeBytes, QR_VERSION, QR_ECC, text);

#endif // STRING_MODE

    uint16_t WD = qrcode.size;

    // Display the QR-code
    screen->setPenSolid(true);
    screen->dRectangle(x0, y0, (WD + 2) * multiplier, (WD + 2) * multiplier, backColour);
    x0 += multiplier;
    y0 += multiplier;

    for (uint8_t y = 0; y < WD; y++)
    {
        for (uint8_t x = 0; x < WD; x++)
        {
            screen->dRectangle(x0 + multiplier * x, y0 + multiplier * y, multiplier, multiplier, (qrcode_getModule(&qrcode, x, y) > 0) ? frontColour : backColour);
        }
    }
    screen->setPenSolid(false);

    return RESULT_SUCCESS;
}

uint16_t getSizeCodeQR(uint8_t multiplier)
{
    return (4 * QR_VERSION + 17 + 2) * multiplier;
    // return (WD + 2) * multiplier;
}
