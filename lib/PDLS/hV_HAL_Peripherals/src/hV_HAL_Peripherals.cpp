//
// hV_HAL_Peripherals.cpp
// Library C++ code
// ----------------------------------
//
// Details Light hardware abstraction layer for peripherals
// Project highVieww Library Suite
//
// Created by Rei Vilo, 21 Dec 2020
//
// Copyright (c) Rei Vilo, 2010-2023
// Licence All rights reserved
//
// See hV_HAL_Peripherals.h and ReadMe.txt for references
//
// Release 508: Added TivaC SPI settings exception
// Release 509: Added Arduino mbed and RP2040 exceptions
// Release 510: Added hV_HAL_Debug_flush
// Release 521: Added RasPiArduino exceptions
// Release 521: Added Arduino SAMD exceptions
// Release 527: Added pseudo-random function
// Release 529: Improved stability for SPI
// Release 530: Added ESP32 exceptions
// Release 531: Stability enhancements
// Release 532: Added support for MRAA
// Release 533: Added support for Mbed-OS
// Release 540: Improved messages
// Release 544: Tested Feather M0 and M4, nRF52832 and nRF52840, RP2040 boards
// Release 550: Tested Xiao ESP32-C3 with SPI exception
// Release 605: Improved consistency
// Release 607: Added log messages
// Release 607: Refactored peripherals
// Release 608: Improved log options
// Release 609: Cleaned unused functions
// Release 610: Added SPI speed parameter for faster screens
// Release 611: Improved log options
//

// Library header
#include "hV_HAL_Peripherals.h"
#include "definations_cpp.h"                // SYS function prototypes
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h> 
#include "hV_Constants.h"

#define TAG    "EPD"

volatile bool isTransferDone = false;

/* This function will be called by SPI PLIB when transfer is completed */
void SPIEventHandler(uintptr_t context )
{
    /* De-assert the chip select */
    //EEPROM_CS_Set();

    isTransferDone = true;
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
    const long run = in_max - in_min;
    if(run == 0){
        return -1; // AVR returns -1, SAM returns 0
    }
    const long rise = out_max - out_min;
    const long delta = x - in_min;
    return (delta * rise) / run + out_min;
}

//
// === General section
//
void hV_HAL_begin()
{
    hV_HAL_Serial_begin(hV_HAL_Serial_Speed);
    hV_HAL_delayMilliseconds(500);

    hV_HAL_Serial_crlf();
    hV_HAL_log(LEVEL_SYSTEM, "Begin");

    hV_HAL_GPIO_begin();
    hV_HAL_SPI_begin();
}

void hV_HAL_end()
{
    hV_HAL_log(LEVEL_SYSTEM, "End");

    hV_HAL_exit(0);
}

void hV_HAL_exit(uint8_t code)
{
    hV_HAL_log(LEVEL_SYSTEM, "Exit with code %i", code);

#if defined(ARDUINO_ARCH_PIDUINO)

    exit(code);

#else

    hV_HAL_log(LEVEL_SYSTEM, "Loop");
    hV_HAL_Serial_crlf();
    while (true)
    {
        hV_HAL_delayMilliseconds(1000);
    }

#endif // ARDUINO_ARCH_PIDUINO
}
//
// === End of General section
//

//
// === GPIO section
//
void hV_HAL_GPIO_begin()
{
    // Empty
}

void hV_HAL_GPIO_define(uint8_t pin, uint8_t mode)
{

}

void hV_HAL_GPIO_set(uint8_t pin)
{
    switch (pin)
    {
        case 	EPD_SS:
            /* code */
            break;
        case 	EPD_DC:
            EPD_DC_Set(); 
            break;
        case 	EPD_RESET:
            EPD_RESET_Set(); 
            break;
        case 	EPD_BUSY:
            EPD_BUSY_Set();
            break;
        default:
            break;
    }
}

void hV_HAL_GPIO_clear(uint8_t pin)
{
    switch (pin)
    {
        case 	EPD_SS:
            /* code */
            break;
        case 	EPD_DC:
            EPD_DC_Clear();
            break;
        case 	EPD_RESET:
            EPD_RESET_Clear();
            break;
        case 	EPD_BUSY:
            EPD_BUSY_Clear();
            break;
        default:
            break;
    }
}

uint8_t hV_HAL_GPIO_get(uint8_t pin)
{
    uint8_t ret;
    switch (pin)
    {
        case 	EPD_SS:
            /* code */
            break;  
        case 	EPD_DC:
            ret = EPD_DC_Get();
            break;
        case 	EPD_RESET:
            ret = EPD_RESET_Get();
            break;
        case 	EPD_BUSY:
            ret = EPD_BUSY_Get();
            break;
        default:
            break;
    }
    return ret;
}

void hV_HAL_GPIO_waitFor(uint8_t pin, uint8_t state)
{
    while (hV_HAL_GPIO_get(pin) != state)
    {
        hV_HAL_delayMilliseconds(4); // non-blocking
    }
}

//
// --- Synonyms of GPIO section
//
void hV_HAL_GPIO_digitalWrite(uint8_t pin, uint8_t value)
{
    if(value == HIGH)
    {
        hV_HAL_GPIO_set(pin);
    }
    else
    {
        hV_HAL_GPIO_clear(pin);
    }
}

uint8_t hV_HAL_GPIO_digitalRead(uint8_t pin)
{
    return hV_HAL_GPIO_get(pin);
}

void hV_HAL_GPIO_pinMode(uint8_t pin, uint8_t mode)
{
    ;
}
//
// === End of GPIO section
//

//
// === Time section
//
void hV_HAL_delayMicroseconds(uint32_t us)
{
    delayMicroseconds(us);
}

void hV_HAL_delayMilliseconds(uint32_t ms)
{
    delay(ms);
}

uint32_t hV_HAL_getMilliseconds()
{
    return millis();
}

//
// --- Synonyms of Time section
//
void hV_HAL_delay(uint32_t ms)
{
    hV_HAL_delayMilliseconds(ms);
}

SPISettings _settingScreen;

#ifndef SPI_CLOCK_MAX
#define SPI_CLOCK_MAX 16000000
#endif

void hV_HAL_SPI_configure()
{
    hV_HAL_SPI_begin();
    // hV_HAL_SPI_beginTransaction(_settingScreen);
}

void hV_HAL_SPI_begin(uint32_t speed)
{
    // esp_err_t ret;
    // spi_bus_config_t buscfg={
    //     .mosi_io_num=EPD_MOSI,
    //     .miso_io_num=EPD_MISO,
    //     .sclk_io_num=EPD_SCK,
    //     .quadwp_io_num=-1,
    //     .quadhd_io_num=-1,
    //     .max_transfer_sz=500
    // };
    // spi_device_interface_config_t devcfg = {
    //     .command_bits = 0,
    //     .address_bits = 0,
    //     .dummy_bits = 0,                      // need flags=SPI_DEVICE_HALFDUPLEX
    //     .mode = 0,                            // SPI mode 0
    //     .clock_speed_hz = SPI_MASTER_FREQ_8M, // Clock out at 8 MHz
    //     .spics_io_num = -1,                   // CS pin
    //     .queue_size = 1,                      // We want to be able to queue 7 transactions at a time
    // };

    // //Initialize the SPI bus
    // ret=spi_bus_initialize(EPD_HOST, &buscfg, SPI_DMA_DISABLED);
    // ESP_ERROR_CHECK(ret);

    // ret=spi_bus_add_device(EPD_HOST, &devcfg, &spi);
    // ESP_ERROR_CHECK(ret);

    /* Register callback with the SPI PLIB */
//    SERCOM0_SPI_CallbackRegister(SPIEventHandler, (uintptr_t) 0);
}

void hV_HAL_SPI_end()
{

}

uint8_t hV_HAL_SPI_transfer(uint8_t data)
{
    hV_HAL_SPI_transfer(&data, 1);
    return 1;
}

void hV_HAL_SPI_transfer(uint8_t * data, size_t size)
{
    // esp_err_t ret;
    // spi_transaction_t t;
    // if (size == 0)
    // return;                                   // no need to send anything
    // memset(&t, 0, sizeof(t));                   // Zero out the transaction
    // t.length = size * 8;                         // Len is in bytes, transaction length is in bits.
    // t.tx_buffer = data;                         // Data
    // t.user = (void *)1;                         // D/C needs to be set to 1
    // ret = spi_device_polling_transmit(spi, &t); // Transmit!
    // ESP_ERROR_CHECK(ret);                      // Should have had no issues.
    //EEPROM_CS_Clear();
    SERCOM0_SPI_Write(data, size);
    if (isTransferDone == true)
    {
        isTransferDone = false;
    }
}
//
// === End of SPI section
//

//
// === Wire section
//
void hV_HAL_Wire_begin()
{

}

void hV_HAL_Wire_end()
{

}

void hV_HAL_Wire_transfer(uint8_t address, uint8_t * dataWrite, size_t sizeWrite, uint8_t * dataRead, size_t sizeRead)
{
  
}

void hV_HAL_Wire_writeRead(uint8_t address, uint8_t * dataWrite, size_t sizeWrite, uint8_t * dataRead, size_t sizeRead)
{
 
 }
//
// === End of Wire section
//

//
// === Serial section
//
void hV_HAL_Serial_begin(uint32_t speed)
{
}

void hV_HAL_Serial_printf(const char * format, ...)
{
//    ESP_LOGI("LOG", "hV %s %s", charLevel[bit], buffer);
//    ;
     char buffer[256];
     memset(&buffer, 0x00, sizeof(buffer));
     va_list args;
     va_start(args, format);
     vsnprintf(buffer, 127, format, args);
     va_end(args);
     hV_HAL_Serial_print(buffer);
}

void hV_HAL_Serial_crlf()
{
    
}

//
// === Synonyms for Serial section
//
void hV_HAL_Debug_begin(uint32_t speed)
{
}

void hV_HAL_Debug_flush()
{
}

void hV_HAL_crlf()
{
}

void hV_HAL_Debug_print(const char * format, ...)
{
    // char buffer[256];
    // memset(&buffer, 0x00, sizeof(buffer));
    // va_list args;
    // va_start(args, format);
    // vsprintf(buffer, format, args);
    // va_end(args);
    // hV_HAL_Serial_print(buffer);
}

void hV_HAL_Debug_println(const char * format, ...)
{
    // char buffer[256];
    // memset(&buffer, 0x00, sizeof(buffer));
    // va_list args;
    // va_start(args, format);
    // vsprintf(buffer, format, args);
    // va_end(args);
    // hV_HAL_Serial_print(buffer);
}

void hV_HAL_Debug_printf(const char * format, ...)
{
    // char buffer[256];
    // memset(&buffer, 0x00, sizeof(buffer));
    // va_list args;
    // va_start(args, format);
    // vsnprintf(buffer, 127, format, args);
    // va_end(args);
    // hV_HAL_Serial_print(buffer);
}
//
// === End of Serial section
//

//
// === Log system
//
uint8_t _filter = FILTER_ALL;
uint16_t _option = 0;

void hV_HAL_log(uint16_t level, const char * format, ...)
{
    // --- Levels
    // #define LEVEL_CRITICAL 0x0001 ///< * Non-handled error, stop or exit
    // #define LEVEL_ERROR 0x0002 ///< + Handled error, continue
    // #define LEVEL_WARNING 0x0004 ///< ! Warning
    // #define LEVEL_INFO 0x0008 ///< . Information
    // #define LEVEL_DEBUG 0x0010 ///< - Debug
    // #define LEVEL_SYSTEM 0x0020 ///< = System
    // #define FILTER_ALL 0x00ff ///< Filter for all log levels
    // --- Options
    // #define WITH_COMMENT 0x0100 ///< = Option with leading //
    // #define WITH_NO_CRLF 0x0200 ///< Option with no CR-LF
    // #define WITH_NO_LEVEL 0x0400 ///< Option with no level

    level = level | _option;
    uint8_t _level = (level & 0xff);

    if (_level & _filter)
    {
        // const char * stringLevel[] = { "CRITICAL", "ERROR", "WARNING", "INFO", "DEBUG", "SYSTEM", "NOT", "UNKNOWN"};
        const char * charLevel[] = { "*", "+", "!", ".", "-", "=", "?", "?" };

        uint8_t bit = 0;
        bool flag = true;

        while ((flag and (bit < 8)))
        {
            if (bitRead(_level, bit) == 1)
            {
                flag = false;
            }
            else
            {
                bit += 1;
            }
        }

        // Leading comment
        if (level & WITH_COMMENT)
        {
            hV_HAL_Serial_printf("// ");
        }

        if (!(level & WITH_NO_LEVEL))
        {
            hV_HAL_Serial_printf("hV %s ", charLevel[bit]);
        }

        char buffer[256];
        memset(&buffer, 0x00, sizeof(buffer));
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, 127, format, args);
        va_end(args);

        hV_HAL_Serial_printf(buffer);

        // Ending CRLF
        // if (!bitRead(level, 7))
        if (!(level & WITH_NO_CRLF))
        {
            hV_HAL_Serial_crlf();
        }
    }
}

void hV_HAL_setFilter(uint8_t filter)
{
    _filter = filter;
}

void hV_HAL_setOption(uint16_t option)
{
    _option = (option & 0xff00);
}

//
// === End of Log system
//

//
// === Miscellaneous section
//
uint32_t hV_HAL_random(uint32_t maxNumber)
{
    return rand() % maxNumber;
}

long hV_HAL_map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
//
// === End of Miscellaneous section
//

