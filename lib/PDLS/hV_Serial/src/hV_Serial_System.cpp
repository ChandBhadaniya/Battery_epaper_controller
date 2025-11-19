//
// hV_Serial_System.cpp
// Library C++ code
// ----------------------------------
//
// Details Save screen content to BPM file on SD card
// Project Pervasive Displays Library Suite
//
// Created by Rei Vilo, 16 Feb 2022
//
// Copyright (c) Rei Vilo, 2010-2023
// Licence All rights reserved
//
// See hV_Serial_System.h and ReadMe.txt for references
//
// Release 509: Renamed beginFileSystem suspendFileSystem
// Release 522: Added support for Linux
// Release 529: Added pixmap format
// Release 530: Added support for external SRAM
// Release 607: Added log messages
// Release 609: Deprecated partial update
// Release 611: Added check on fast screens
//

// Library header
#include "hV_Serial_System.h"

Serial_System::Serial_System()
{
    ;
}

bool Serial_System::beginFileSystem(uint8_t pinCS, uint8_t port, int8_t pinDetect, int level)
{
    return true;
}

bool Serial_System::endFileSystem()
{
    return true;
}

bool Serial_System::suspendFileSystem()
{
    return true;
}

bool Serial_System::resumeFileSystem()
{
    return true;
}

#if (STRING_MODE == USE_STRING_OBJECT)

bool Serial_System::setFolder(String name)
{
    memset(_folderName, 0, sizeof(_folderName));
    name.toCharArray(_folderName, 63);
    return RESULT_SUCCESS;
}

#elif (STRING_MODE == USE_CHAR_ARRAY)

bool Serial_System::setFolder(const char * name)
{
    memset(_folderName, 0, sizeof(_folderName));
    strncpy(_folderName, name, 63);
    if ((char)_folderName[strlen(_folderName) + 1] != '/')
    {
        strcat(_folderName, "/");
    }

    return RESULT_SUCCESS;
}

#endif // STRING_MODE

