// Configuration
#include "hV_Configuration.h"

// The Arduino IDE doesn't allow to select the libraries, hence this condition.
#if (FONT_MODE == USE_FONT_HEADER)

#ifndef PXSANSREGULAR16FONT_RELEASE
#define PXSANSREGULAR16FONT_RELEASE 507

#include "hV_Font.h"

#if (hV_FONT_HEADER_STRUCTURE_RELEASE < 507)

#error Required hV_FONT_HEADER_STRUCTURE_RELEASE 507

#endif // hV_FONT_HEADER_STRUCTURE_RELEASE

extern const font_s Font_PXSansRegular16;

#endif // PXSANSREGULAR16FONT_RELEASE

#endif // FONT_MODE 

