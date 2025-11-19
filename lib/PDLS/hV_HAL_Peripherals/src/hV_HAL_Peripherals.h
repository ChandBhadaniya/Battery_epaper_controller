///
/// @file hV_HAL_Peripherals.h
/// @brief Light hardware abstraction layer for peripherals
///
/// @details Project Pervasive Displays Library Suite
/// @n Based on highView technology
///
/// @author Rei Vilo
/// @date 21 Jun 2023
/// @version 611
///
/// @copyright (c) Rei Vilo, 2010-2023
/// @copyright All rights reserved
///
/// * Evaluation edition: for professionals or organisations, no commercial usage
/// * Commercial edition: for professionals or organisations, for commercial usage
///

#ifndef hV_HAL_PERIPHERALS_RELEASE
///
/// @brief Release
///
#define hV_HAL_PERIPHERALS_RELEASE 611

#include <iostream>
#include <algorithm>
#include <cctype> // Required for isdigit()

// Other libraries
#include "hV_Constants.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "common_def.h"

using namespace std;

#define TAG_EPD    "EPD"

#define lowByte(w) ((uint8_t) ((w) & 0xff))
#define highByte(w) ((uint8_t) ((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) ((bitvalue) ? bitSet(value, bit) : bitClear(value, bit))

// avr-libc defines _NOP() since 1.6.2
#ifndef _NOP
#define _NOP() do { __asm__ volatile ("nop"); } while (0)
#endif

long map(long x, long in_min, long in_max, long out_min, long out_max);

///
/// @brief FRAM management
/// @details for MSP430FR MCUs
///
#ifndef PLACE_IN_FRAM
#define PLACE_IN_FRAM
#endif

///
/// @brief Serial configuration
/// @{
///
/// @brief Serial port
///
// #define hV_HAL_Serial Serial

#define hV_HAL_Serial_Speed 115200
#define SPI_SPEED_4_MHz 4000000
#define SPI_SPEED_8_MHz 8000000
#define SPI_SPEED_16_MHz 16000000
#define hV_HAL_SPI_SPEED SPI_SPEED_8_MHz
#define hV_HAL_SPI_CLOCK SPI_SPEED_8_MHz

/// @}

/// @} Specific configuration

///
/// @name General
/// @{
#define LOW               0x0
#define HIGH              0x1

//GPIO FUNCTIONS
#define INPUT             0x01
// Changed OUTPUT from 0x02 to behave the same as Arduino pinMode(pin,OUTPUT) 
// where you can read the state of pin even when it is set as OUTPUT
#define OUTPUT            0x03 
#define PULLUP            0x04
#define INPUT_PULLUP      0x05
#define PULLDOWN          0x08
#define INPUT_PULLDOWN    0x09
#define OPEN_DRAIN        0x10
#define OUTPUT_OPEN_DRAIN 0x12
#define ANALOG            0xC0
///
/// @brief General initialisation
/// @details hV_HAL_Serial initialised at hV_HAL_Serial_Speed
///
void hV_HAL_begin();

///
/// @brief General end
/// @note same as hV_HAL_exit(0)
///
void hV_HAL_end();

///
/// @brief General exit
/// @param code default = 0 = success, otherwise error
/// @note Only used on Linux
///
void hV_HAL_exit(uint8_t code = 0);

/// @}

///
/// @name GPIO
/// @see https://www.arduino.cc/reference/en/language/functions/digital-io/
/// @{

///
/// @brief GPIO initialisation
/// @details Required for GPIO expander
/// @warning begin() does not initialise SPI
/// @warning begin() initialises I2C
///
void hV_HAL_GPIO_begin();

///
/// @brief Define GPIO mode
///
/// @param pin pin number
/// @note Same as hV_HAL_GPIO_pinMode(pin, mode);
///
void hV_HAL_GPIO_define(uint8_t pin, uint8_t mode);

///
/// @brief Set GPIO
///
/// @param pin pin number
/// @note Same as hV_HAL_GPIO_digitalWrite(pin, HIGH);
///
void hV_HAL_GPIO_set(uint8_t pin);

///
/// @brief Clear GPIO
///
/// @param pin pin number
/// @note Same as hV_HAL_GPIO_digitalWrite(pin, LOW);
///
void hV_HAL_GPIO_clear(uint8_t pin);

///
/// @brief Get GPIO
///
/// @param pin pin number
/// @return uint8_t HIGH = true or LOW = false
/// @note Same as hV_HAL_GPIO_digitalRead(pin);
///
uint8_t hV_HAL_GPIO_get(uint8_t pin);

///
/// @brief Wait for GPIO state
///
/// @param pin pin number
/// @param state state to wait for, HIGH or LOW
/// @note Overtime management should be placed here.
///
void hV_HAL_GPIO_waitFor(uint8_t pin, uint8_t state);

/// @}

///
/// @name Time
/// @see https://www.arduino.cc/reference/en/#time
/// @{

///
/// @brief Microsecond non-blocking delay
///
/// @param us delay, us
///
void hV_HAL_delayMicroseconds(uint32_t us);

///
/// @brief Millisecond non-blocking delay
///
/// @param ms delay, ms
///
void hV_HAL_delayMilliseconds(uint32_t ms);

///
/// @brief Get milliseconds since start-up
///
/// @return uint32_t milliseconds since start-up
/// @note Limit of 2^32 ms or ~49.7 days
///
uint32_t hV_HAL_getMilliseconds();

/// @}

///
/// @name Serial console
/// @brief Formatted output to hV_HAL_Serial
/// @see https://www.arduino.cc/reference/en/language/functions/communication/serial/
/// @{

///
/// @brief Initialise console
/// @param speed in bps, eg. 115200
/// @deprecated Console already initialised at 115200 by hV_HAL_Serial_begin() (6.0.7).
///
void hV_HAL_Serial_begin(uint32_t speed = 115200);

///
/// @brief Format and send to console
/// @param format see https://www.cplusplus.com/reference/cstdio/printf/ for tokens
///
void hV_HAL_Serial_printf(const char * format, ...);

///
/// @brief Print carriage-return line-feed
///
void hV_HAL_Serial_crlf();

/// @}

///
/// @name Log system
/// @{
///

///
/// @brief Log level messages
/// @{
#define LEVEL_CRITICAL 0x0001 ///< * Non-handled error, stop or exit
#define LEVEL_ERROR 0x0002 ///< + Handled error, continue
#define LEVEL_WARNING 0x0004 ///< ! Warning
#define LEVEL_INFO 0x0008 ///< . Information
#define LEVEL_DEBUG 0x0010 ///< - Debug
#define LEVEL_SYSTEM 0x0020 ///< = System
#define FILTER_ALL 0x00ff ///< Filter for all log levels

#define WITH_COMMENT 0x0100 ///< = Option with leading //
#define WITH_NO_CRLF 0x0200 ///< Option with no CR-LF
#define WITH_NO_LEVEL 0x0400 ///< Option with no level
/// @}
///

///
/// @brief Send debug message to console
/// @param level Debug level message
/// @param format see https://www.cplusplus.com/reference/cstdio/printf/ for tokens
/// @note With final CR-LF
///
void hV_HAL_log(uint16_t level, const char * format, ...);

///
/// @brief Set filter for log levels
/// @param filter default = FILTER_ALL
/// @note Numbers are bit-based and or-combinable
///
void hV_HAL_setFilter(uint8_t filter = FILTER_ALL);

///
/// @brief Set option for log messages
/// @param option WITH_COMMENT = default, WITH_NO_CRLF, WITH_NO_LEVEL
/// @note Numbers are bit-based and or-combinable
///
void hV_HAL_setOption(uint16_t option = WITH_COMMENT);

/// @}

///
/// @name SPI bus
/// @see https://www.arduino.cc/en/Reference/SPI
/// @warning Energia does not implement beginTransaction...endTransaction
///
/// @{

#if defined(ENERGIA) // Energia does not feature SPISettings except MSP430 and TivaC

#ifndef ENERGIA_ARCH_MSP430
#ifndef ENERGIA_ARCH_MSP430ELF
#ifndef ENERGIA_ARCH_TIVAC
struct SPISettings
{
    uint32_t clock; // in Hz, max SPI_CLOCK_MAX = 16000000
    uint8_t bitOrder; // LSBFIRST, MSBFIRST
    uint8_t dataMode; // SPI_MODE0..3
};
#endif // ENERGIA_ARCH_TIVAC
#endif // ENERGIA_ARCH_MSP430ELF
#endif // ENERGIA_ARCH_MSP430

#endif // ENERGIA

///
/// @brief Configure and start SPI
/// @param speed SPI speed in Hz, hV_HAL_SPI_SPEED = default
/// Other values are SPI_SPEED_4_MHz, SPI_SPEED_8_MHz, SPI_SPEED_16_MHz
/// @note Other parameters are
/// * Bit order: MSBFIRST
/// * Data mode: SPI_MODE0
/// @warning SPI ESP32 uses pins SCK = 14, MISO = 12 and MOSI = 13
///
void hV_HAL_SPI_begin(uint32_t speed = hV_HAL_SPI_SPEED);

///
/// @brief End SPI
///
void hV_HAL_SPI_end();

///
/// @brief Combined write and read of a single byte
/// @param data byte
/// @return read byte
///
uint8_t hV_HAL_SPI_transfer(uint8_t data);

///
/// @brief Combined write and read
///
/// @param data array of uint8_t to send
/// @param size length
/// @note Array may be updated with read data
///
void hV_HAL_SPI_transfer(uint8_t * data, size_t size);

/// @}

///
/// @name Wire bus
/// @see https://www.arduino.cc/en/Reference/Wire
///
/// @{

///
/// @brief Configure and start Wire bus
/// @note Master mode only
///
void hV_HAL_Wire_begin();

///
/// @brief End Wire bus
///
void hV_HAL_Wire_end();

///
/// @brief Combined write and read
///
/// @param[in] address I2C device address
/// @param[in] dataWrite buffer to write
/// @param[in] sizeWrite number of bytes
/// @param[out] dataRead buffer to read
/// @param[in] sizeRead number of bytes
/// @note If sizeRead = 0, no read performed
///
void hV_HAL_Wire_transfer(uint8_t address, uint8_t * dataWrite, size_t sizeWrite, uint8_t * dataRead = 0, size_t sizeRead = 0);

/// @}

///
/// @name Miscellaneous
/// @{

///
/// @brief Pseudo-random number
///
/// @param maxNumber maximum random number
/// @return uint32_t 0..number - 1
///
uint32_t hV_HAL_random(uint32_t maxNumber);

///
/// @brief Rescale a value
///
/// @param x input value
/// @param in_min minimal input value
/// @param in_max maximum input value
/// @param out_min minimum output value
/// @param out_max maximum output value
/// @return long output value
/// @note Formula is
/// @code {.cpp}
/// (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
/// @endcode
///
long hV_HAL_map(long x, long in_min, long in_max, long out_min, long out_max);

/// @}

///
/// @name Deprecated synonyms for GPIO
/// @{

///
/// @brief Define GPIO mode
///
/// @param pin pin number
/// @param mode OUTPUT, INPUT, INPUT_PULLUP, INPUT_PULLDOWN
/// @deprecated Use hV_HAL_GPIO_define() instead  (6.1.0).
///
void hV_HAL_GPIO_pinMode(uint8_t pin, uint8_t mode);

///
/// @brief Set or clear GPIO
///
/// @param pin pin number
/// @param value HIGH = true or LOW = false
/// @note Synonym of hV_HAL_GPIO_set() and hV_HAL_GPIO_clear()
/// @deprecated Use hV_HAL_set() and hV_HAL_clear() instead (6.0.7).
///
void hV_HAL_GPIO_digitalWrite(uint8_t pin, uint8_t value);

///
/// @brief Get GPIO
///
/// @param pin pin number
/// @return uint8_t HIGH = true or LOW = false
/// @note Synonym of hV_HAL_GPIO_get()
/// @deprecated Use hV_HAL_get() instead  (6.1.0).
///
uint8_t hV_HAL_GPIO_digitalRead(uint8_t pin);

/// @}

///
/// @name Deprecated synonyms for Time
/// @{

///
/// @brief Millisecond non-blocking delay
///
/// @param ms delay, ms
/// @note Synonym of hV_HAL_delayMilliseconds()
/// @deprecated Use hV_HAL_delayMilliseconds() instead  (6.1.0).
///
void hV_HAL_delay(uint32_t ms);

/// @}

///
/// @name Deprecated synonyms for Serial
/// @{

///
/// @brief Format and send to console
/// @param format see https://www.cplusplus.com/reference/cstdio/printf/ for tokens
/// @deprecated Use hV_HAL_Serial_printf() instead (6.1.0).
///
void hV_HAL_printf(const char * format, ...);

///
/// @brief Print carriage-return line-feed
/// @deprecated Use hV_HAL_Serial_crlf() instead (6.1.0).
///
void hV_HAL_crlf();

///
/// @brief Initialise console
/// @param speed in bps, eg. 115200
/// @deprecated Console already initialised at 115200 by hV_HAL_Serial_begin() (6.0.7).
///
void hV_HAL_Debug_begin(uint32_t speed = 115200);

///
/// @brief Force the output of the serial buffer
/// @deprecated No longer required (6.0.7).
///
void hV_HAL_Debug_flush();

///
/// @brief Format and send to console
/// @param format see https://www.cplusplus.com/reference/cstdio/printf/ for tokens
/// @deprecated Use hV_HAL_Serial_printf() instead (6.1.0).
///
void hV_HAL_Debug_print(const char * format, ...);

/// @brief Format and send to console
/// @param format see https://www.cplusplus.com/reference/cstdio/printf/ for tokens
/// @note With final CR-LF
/// @deprecated Use hV_HAL_Serial_printf() and hV_HAL_Serial_crlf() instead (6.1.0).
///
void hV_HAL_Debug_println(const char * format, ...);

///
/// @brief Print to serial console
///
/// @tparam T type of the element, [u]int{8|16|32}_t float String char*
/// @param text element to print
/// @warning Because of <template>, code follows definition in header file
/// @deprecated Use hV_HAL_Serial_printf() and hV_HAL_Serial_crlf() instead (6.1.0).
///
template <typename T>
void hV_HAL_Serial_print(T text)
{
    ESP_LOGI(TAG_EPD, "%s", text);
}

///
/// @brief Print to serial console with cr-lf at the end
///
/// @tparam T type of the element: [u]int{8|16|32}_t float String char*
/// @param text element to print
/// @warning Because of <template>, code follows definition in header file
/// @deprecated Use hV_HAL_Serial_printf() and hV_HAL_Serial_crlf() instead (6.1.0).
///
template <typename T>
void hV_HAL_Serial_println(T text)
{
    ESP_LOGI(TAG_EPD, "%s", text);
}

struct SPISettings
{
    uint32_t clock; // in Hz, max SPI_CLOCK_MAX = 16000000
    uint8_t bitOrder; // LSBFIRST, MSBFIRST
    uint8_t dataMode; // SPI_MODE0..3
};

void hV_HAL_SPI_beginTransaction(SPISettings _settingSPI);
void hV_HAL_SPI_endTransaction();


/// @}

///
/// @name Deprecated synonyms for SPI
/// @{

///
/// @brief Configure and start SPI
/// @details Parameters
/// * Clock: 4 MHz
/// * Bit order: MSBFIRST
/// * Data mode: SPI_MODE0
/// @deprecated Use hV_HAL_SPI_begin() instead  (6.1.0).
///
void hV_HAL_SPI_configure();

/// @}

///
/// @name Deprecated synonyms for Wire
/// @{

///
/// @brief Combined write and read
///
/// @param[in] address I2C device address
/// @param[in] dataWrite buffer to write
/// @param[in] sizeWrite number of bytes
/// @param[out] dataRead buffer to read
/// @param[in] sizeRead number of bytes
/// @note If sizeRead = 0, no read performed
///
/// @deprecated Use hV_HAL_Wire_transfer() instead (6.1.0).
///
void hV_HAL_Wire_writeRead(uint8_t address, uint8_t * dataWrite, size_t sizeWrite, uint8_t * dataRead = 0, size_t sizeRead = 0);

/// @}

#endif // hV_HAL_PERIPHERALS_RELEASE
