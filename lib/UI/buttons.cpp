
#include "common_def.h"
#include "capsense.h"
//#include "esp_log.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TAG 							"CAP"
#define BUTTON_LONG_PRESS_TIME_MS		10000
#define BUTTON_VERYLONG_PRESS_TIME_MS	3000

const buttons_e buttons[TOUCH_BUTTON_NUM]={BTN_BACK, BTN_LEFT, BTN_OK, BTN_RIGHT, BTN_ONOFF};
const buttons_longpress_e buttons_longp[TOUCH_BUTTON_NUM]={BTN_LONGP_BACK, BTN_LONGP_LEFT, BTN_LONGP_OK, BTN_LONGP_RIGHT, BTN_LONGP_LEFTRIGHT};

static uint32_t touchdetected, last_touchdetected;
static uint8_t	button_event_updated = 0;

uint8_t button_scan()
{
	static buttons_e t[TOUCH_BUTTON_NUM];
	static uint32_t press_time[TOUCH_BUTTON_NUM];
	uint8_t i=0;
	uint32_t now_time = 0;      // TODO:
	for(i=0; i<TOUCH_BUTTON_NUM; i++)
	{
		if(capsense_get_touch() & buttons[i])
		{
//			if(t[i] == BTN_NONE)
//			{
//				press_time[i] = now_time;
//				t[i] = buttons[i];
//			}

			if(t[i] == BTN_LEFT || t[i] == BTN_RIGHT)
			{
				if(((now_time - press_time[1]) > BUTTON_VERYLONG_PRESS_TIME_MS) && ((now_time - press_time[3]) > BUTTON_VERYLONG_PRESS_TIME_MS))
				{
					for(uint8_t j=0; j<TOUCH_BUTTON_NUM; j++)
					{
						t[j] = BTN_NONE;
						press_time[j] = 0;
					}
					touchdetected = buttons_longp[4];
//					capsense_clear_touch();
				}
			}
			if(t[i] == BTN_OK )
			{
				if(!((now_time - press_time[i]) < BUTTON_VERYLONG_PRESS_TIME_MS))
				{
					for(uint8_t j=0; j<TOUCH_BUTTON_NUM; j++)
					{
						t[j] = BTN_NONE;
						press_time[j] = 0;
					}
					touchdetected |= buttons_longp[i];
					ESP_LOGI(TAG, "Longgggg press(%d) = %d\r\n", i, buttons_longp[i]);
					capsense_clear_touch();
				}
			}
		}
		else if(t[i] != BTN_NONE && touchdetected == BTN_NONE)
		{
			if((now_time - press_time[i]) > BUTTON_LONG_PRESS_TIME_MS)
			{
				touchdetected |= buttons_longp[i];
#ifdef DEBUG_LOGS
				ESP_LOGI(TAG, "Long press(%d) = %d\r\n", i, buttons_longp[i]);
#endif
			}
			else
			{
				touchdetected |= t[i];
			}
			t[i] = BTN_NONE;
			press_time[i] = 0;
		}
	}
	return touchdetected;
}

uint32_t buttons_get_events()
{
	uint32_t ret;
	ret = touchdetected;
	touchdetected = BTN_NONE;
	return ret;
}

uint8_t is_buttons_event_updated(void)
{
	if(button_event_updated)
	{
		last_touchdetected = millis();
		button_event_updated = 0;
		return 1;
	}
	return 0;
}

uint32_t get_time_since_last_interaction()
{
	return last_touchdetected;
}

uint32_t buttons_probe()
{
	return touchdetected;
}

uint32_t buttons_check()
{
    uint32_t touch = buttons_get_events();

	if(touch != BTN_NONE)
	{
		button_event_updated = 1;
	}

    if(touch & BTN_BACK){
        ESP_LOGI(TAG, "BTN_BACK detected");
    }
    if(touch & BTN_LEFT){
        ESP_LOGI(TAG, "BTN_LEFT detected");
    }
    if(touch & BTN_RIGHT){
        ESP_LOGI(TAG, "BTN_RIGHT detected");
    }
	if(touch & BTN_OK){
        ESP_LOGI(TAG, "BTN_OK detected");
    }
	if(touch & BTN_ONOFF){
        ESP_LOGI(TAG, "BTN_ONOFF detected");
    }
    if(touch & BTN_LONGP_BACK){
        ESP_LOGI(TAG, "BTN_LONGP_BACK detected");
    }
    if(touch & BTN_LONGP_LEFT){
        ESP_LOGI(TAG, "BTN_LONGP_LEFT detected");
    }
    if(touch & BTN_LONGP_RIGHT){
        ESP_LOGI(TAG, "BTN_LONGP_RIGHT detected");
    }
	if(touch & BTN_LONGP_OK){
        ESP_LOGI(TAG, "BTN_LONGP_OK detected");
    }
	if(touch & BTN_LONGP_LEFTRIGHT){
        ESP_LOGI(TAG, "BTN_LONGP_LEFTRIGHT detected");
    }

	return touch;
}

void reset_button_tp()
{
	ESP_LOGI(TAG, "reset_button_tp Enter");
//    tp_example_set_thresholds();
	ESP_LOGI(TAG, "reset_button_tp Exit");
}

#ifdef __cplusplus
} // closing brace for extern "C"
#endif