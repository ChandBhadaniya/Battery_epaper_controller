///
/// @file hV_Font.h
/// @brief Header
/// @details Image structure
/// @n
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @author Rei Vilo
/// @date 21 Jul 2023
/// @version 612
///
/// @copyright (c) Rei Vilo, 2010-2023
/// @copyright All rights reserved
///
/// @see ReadMe.txt for references
///

// SDK
#include "hV_HAL_Peripherals.h"

#ifndef hV_IMAGE_HEADER_STRUCTURE_RELEASE
///
/// @brief hV_Font release number
///
#define hV_IMAGE_HEADER_STRUCTURE_RELEASE 612

#include "hV_Configuration.h"

///
/// @brief Structure for image
///
/// @brief Image structure for header
/// @details Internal MCU Flash memory
///
struct image_s
{
    uint16_t minWindowH;
    uint16_t minWindowV;
    uint16_t maxWindowH;
    uint16_t maxWindowV;

    uint8_t depth; ///< number of colours
    uint32_t size; ///< size of the image in bytes
    const uint8_t * table; ///< image definition
};

#endif // hV_IMAGE_HEADER_STRUCTURE_RELEASE
