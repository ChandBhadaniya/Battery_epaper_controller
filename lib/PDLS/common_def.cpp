
#include "common_def.h"
#include "definitions.h"                // SYS function prototypes

const uint8_t myOrientation = 4; // ORIENTATION_VERTICAL

unsigned long micros()
{
    return 0;
}

unsigned long millis()
{
    return xTaskGetTickCount();
}

void delay(uint32_t ms)
{
    vTaskDelay(ms / portTICK_PERIOD_MS);
}

void delayMicroseconds(uint32_t us)
{
    uint64_t m;
    m = (us * 1000) / 1000;
    vTaskDelay(m / portTICK_PERIOD_MS);
}

uint8_t wait_for_s(uint32_t time, uint32_t *Couinter)
{
	if(*Couinter == 0)
	{
		*Couinter = millis();
	}
	else if(millis() > (*Couinter + (time * 1000)))
	{
		*Couinter = 0;
		return 1;
	}
	return 0;
}

uint8_t wait_for_ms(uint32_t time, uint32_t *Couinter)
{
	if(*Couinter == 0)
	{
		*Couinter = millis();
	}
	else if(millis() > (*Couinter + (time)))
	{
		*Couinter = 0;
		return 1;
	}
	return 0;
}


