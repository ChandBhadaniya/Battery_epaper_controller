
#include "disp.h"
#include "disp_common.h"
//#include "../lib/Sensors/Sensirion_Core/src/SensirionCore.h"
//#include "PDLS_EXT3_Advanced_Fast.h"
#include "hV_HAL_Peripherals/src/hV_HAL_Peripherals.h"
#include "PDLS_EXT3_Advanced_Fast/src/PDLS_EXT3_Advanced_Fast.h"
#include "hV_HAL_Peripherals/src/hV_HAL_Peripherals.h"
#include "hV_Common/src/hV_Configuration.h"
#include "hV_Common/src/hV_Screen_Buffer.h"
#include "font_render.h"
#include "buttons.h"
#include "hvac.h"
#include "../UI/ui.h"
#include "gfx.h"
#include "Font_InterMedium24.h"
//#include "../lib/CapSense/capsense.h"
//#include "sht4x.h"
//#include "scd4x.h"
//#include "relay.h"
//#include "ntc_temp_sensor.h"
#include "settings.h"


#include "hV_HAL_Peripherals.h"

// Configuration
#include "hV_Configuration.h"

#include "hV_GUI_Fast/src/hV_GUI.h"

// Set parameters
//#define DISPLAY_GUI_TEXT 1

// Define variables and constants
//#include "PDLS_EXT3_Advanced_Touch.h"

#define TAG        "DISP"

// Define variables and constants
Screen_EPD_EXT3_Fast myScreen(eScreen_EPD_EXT3_266_0C_Fast, boardESP32S2EPC);
uint8_t is_scd4x_sensor_present = 0;

static GUI myGUI(&myScreen);

//const uint8_t myOrientation = 3; // ORIENTATION_LANDSCAPE
uint8_t fontExtra, fontLarge, fontMedium, fontSmall;


#ifdef __cplusplus
extern "C" {
#endif

// Utilities
void wait(uint8_t second)
{
    for (uint8_t i = second; i > 0; i--)
    {
        delay(100);
        hV_HAL_Debug_print(" > %i  \r", i);
        delay(900);
    }
    hV_HAL_Debug_print("         \r");
}

void displayText()
{
    myScreen.setOrientation(myOrientation);
    uint16_t x, y, dx, dy;
    uint16_t tx, ty, tz, tt;

    x = myScreen.screenSizeX();
    y = myScreen.screenSizeY();
    dx = x / 6;
    dy = y / 4;

    myGUI.delegate(false);

    myGUI.dLabel(0, 0, x, dy, "Text", myColours.black, myColours.white, -1, 1, fontLarge);

    Text myText1(&myGUI);
    myText1.dDefine(dx, dy, dx * 4, dy, 0, 0, fontLarge);
    myText1.setOption(optionWithoutFrame);
    myText1.draw("Normal text");

    Text myText2(&myGUI);
    myText2.dDefine(dx, dy * 2, dx * 4, dy, 0, 0, fontLarge);
    myText2.setOption(optionWithFrame);
    myText2.draw("Text in box");

//    drawNext();
    myScreen.flushFast();

    myGUI.delegate(true);
    while (1)
    {
        hV_HAL_delayMilliseconds(10);
    }
}

// Add setup code
void setup_text_example()
{
    hV_HAL_begin();

    // Serial console already initialised by hV_HAL_begin()
    // hV_HAL_Serial_begin(115200);
    // hV_HAL_delayMilliseconds(500);
    hV_HAL_log(LEVEL_INFO, __FILE__);
    hV_HAL_log(LEVEL_INFO, __DATE__ " " __TIME__);
    hV_HAL_Serial_crlf();

    // Screen
    myScreen.begin();
    hV_HAL_Serial_crlf();

    // Add fonts from header files
    fontSmall = myScreen.addFont(Font_InterMedium24);
    fontSmall -= (fontSmall > 0) ? 1 : 0;
    fontMedium = myScreen.addFont(Font_InterMedium24);
    fontMedium -= (fontMedium > 0) ? 1 : 0;
    fontLarge = myScreen.addFont(Font_InterMedium24);
    fontLarge -= (fontLarge > 0) ? 1 : 0;
    fontExtra = myScreen.addFont(Font_InterMedium24);
    fontExtra -= (fontExtra > 0) ? 1 : 0;

    // Prepare GUI
    myGUI.begin();
    myGUI.delegate(true);
    myGUI.setColours(myColours.black, myColours.white, myColours.grey);

//    beginNext(); // prepare Next button

#if (DISPLAY_GUI_TEXT == 1)
    hV_HAL_log(LEVEL_INFO, "DISPLAY_GUI_TEXT... ");
    myScreen.clear();
    displayText();
    hV_HAL_log(LEVEL_INFO, "done");
    hV_HAL_Serial_crlf();

    wait(4);
#endif // DISPLAY_GUI_TEXT

    // Against possible ghosting
    myScreen.regenerate();

    hV_HAL_exit(0);
}

void disp_init(void)
{
    hV_HAL_begin();
//    board_i2c_init();
//    touch_button_read_task_init();

//    if(scd4x_init() == 1)
//    {
//        is_scd4x_sensor_present = 1;
//    }
//    sht4x_init();
	
//	relay_init();
//    ntc_temp_init();

    // Serial console
    hV_HAL_Debug_begin(115200);
    delay(500);
//    ESP_LOGI(TAG, "");
//    ESP_LOGI(TAG, "");
//    ESP_LOGI(TAG, "=== " __FILE__);
//    ESP_LOGI(TAG, "=== " __DATE__ " " __TIME__);
//    ESP_LOGI(TAG, "");

    // Screen
    ESP_LOGI(TAG, "begin... ");
    myScreen.begin();
    ESP_LOGI(TAG, "clear... ");
    myScreen.clear();
    ESP_LOGI(TAG, "flushMode... ");
    myScreen.flushMode(UPDATE_GLOBAL);
    ui_init();
    ESP_LOGI(TAG, "done");

    // hvac_report_conf();
    // hvac_report_sys();
    // hvac_report_status();
    text_font_init();
    set_disp(get_disp_type());
    ui_task();
    // hV_HAL_exit(0);
}

// Add loop code
void loop_example()
{
    hV_HAL_delayMilliseconds(1000);
}


void disp_loop(void)
{
    status.device_uptime = (millis() / 1000);
    
//    if(settings.flags & FLAGS_HEC_SET_RELAY_ON)
//    {
//        relay_set_duty_cycle(100);
//    }
//    else
//    {
//        relay_set_duty_cycle(0);
//    }

    hvac_read_sensors();
    hvac_task();
    ui_task();
}

#ifdef __cplusplus
} // closing brace for extern "C"
#endif