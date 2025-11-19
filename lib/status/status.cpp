#include "settings.h"
#include "status.h"
#include "ui.h"
#include "gfx.h"
#include "font_render.h"

#define error_no 4
 const char * error_list[error_no]= {NULL,NULL};
 uint16_t error_flag_value[error_no] = {

	ERROR_AIRTEMP,
	ERROR_FLOOR_TEMP ,
	ERROR_CO2 ,
 };

hec_extended_status_s hec_extended_status;

void get_error_status()
{
//    int j = 0;
//    for (int i = 0; i < 5; i++)
//    {
////        if ((status.error & error_flag_value[i]) && i < error_no)
////        {
////            error_list[j++] = text_error[i][lang];
////        }
//    }
//    uint16_t x, y, yoffset = 6;
//    for (uint i = 0; i < j; i++)
//    {
//        if (j > 4)
//            j = 4;
//        x = 0;
//        y = 172 + (i * 16) - yoffset;
//        text_render_center_horizontal(error_list[i], font_16px, LCD_W, &x, &y,0);
//    }
}