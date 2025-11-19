

#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "peripheral/sercom/usart/plib_sercom1_usart.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const uint8_t myOrientation;

#define LCD_W         152 
#define LCD_H         296

#define LCD_HW_V2       1
#define DEV_MODEL_1101	0x1101
#define DEV_MODEL_1002	0x1002
#define DEV_MODEL_1003	0x1003
#define CONFIG_FW_VERSION   0x0102
#define LCD_HW_VER      1

#define TASK_PRIORITY_DEVICE            8
#define TASK_PRIORITY_MB_SLAVE          7
#define TASK_PRIORITY_tOUCH             6
#define TASK_PRIORITY_FORCE_CO2_CALIBRATION  5

#define MAX_SCD4x_READ_ERR_COUNT  4
#define MAX_SHT4x_READ_ERR_COUNT  12
#define MAX_NTC_READ_ERR_COUNT    6


//#define MODBUS_STATUS_LENGTH                 (47)
//#define MODBUS_CONFIG_BUF_LENGTH             (10)
//#define MODBUS_SETTING_LENGTH                (50)
//#define MODBUS_OTAHEADER_LENGTH              (8)
//#define MODBUS_OTADATA_LENGTH                (125)
//#define MODBUS_VEC_DATA_LENGTH               (10)  

// Define ESP_LOGI macro
#define ESP_LOGI(tag, format, ...) \
    do { \
        char buffer[128]; \
        char newline[] = "\r\n"; \
        int len = snprintf(buffer, sizeof(buffer), "[%s] " format, tag, ##__VA_ARGS__); \
        if (len > 0 && len < sizeof(buffer)) { \
            SERCOM1_USART_Write(buffer, len); \
            SERCOM1_USART_Write(newline, 2); \
        } \
    } while(0)

#define ESP_LOGE(tag, format, ...) \
    do { \
        char buffer[128]; \
        char newline[] = "\r\n"; \
        int len = snprintf(buffer, sizeof(buffer), "[%s] " format, tag, ##__VA_ARGS__); \
        if (len > 0 && len < sizeof(buffer)) { \
            SERCOM1_USART_Write(buffer, len); \
            SERCOM1_USART_Write(newline, 2); \
        } \
    } while(0)

#define ESP_LOGW(tag, format, ...) \
    do { \
        char buffer[128]; \
        char newline[] = "\r\n"; \
        int len = snprintf(buffer, sizeof(buffer), "[%s] " format, tag, ##__VA_ARGS__); \
        if (len > 0 && len < sizeof(buffer)) { \
            SERCOM1_USART_Write(buffer, len); \
            SERCOM1_USART_Write(newline, 2); \
        } \
    } while(0)


typedef int esp_err_t;

/* Definitions for error constants. */
#define ESP_OK          0       /*!< esp_err_t value indicating success (no error) */
#define ESP_FAIL        -1      /*!< Generic esp_err_t code indicating failure */

#define ESP_ERR_NO_MEM              0x101   /*!< Out of memory */
#define ESP_ERR_INVALID_ARG         0x102   /*!< Invalid argument */
#define ESP_ERR_INVALID_STATE       0x103   /*!< Invalid state */
#define ESP_ERR_INVALID_SIZE        0x104   /*!< Invalid size */
#define ESP_ERR_NOT_FOUND           0x105   /*!< Requested resource not found */
#define ESP_ERR_NOT_SUPPORTED       0x106   /*!< Operation or feature not supported */
#define ESP_ERR_TIMEOUT             0x107   /*!< Operation timed out */
#define ESP_ERR_INVALID_RESPONSE    0x108   /*!< Received response was invalid */
#define ESP_ERR_INVALID_CRC         0x109   /*!< CRC or checksum was invalid */
#define ESP_ERR_INVALID_VERSION     0x10A   /*!< Version was invalid */
#define ESP_ERR_INVALID_MAC         0x10B   /*!< MAC address was invalid */
#define ESP_ERR_NOT_FINISHED        0x10C   /*!< Operation has not fully completed */
#define ESP_ERR_NOT_ALLOWED         0x10D   /*!< Operation is not allowed */

#define ESP_ERR_WIFI_BASE           0x3000  /*!< Starting number of WiFi error codes */
#define ESP_ERR_MESH_BASE           0x4000  /*!< Starting number of MESH error codes */
#define ESP_ERR_FLASH_BASE          0x6000  /*!< Starting number of flash error codes */
#define ESP_ERR_HW_CRYPTO_BASE      0xc000  /*!< Starting number of HW cryptography module error codes */
#define ESP_ERR_MEMPROT_BASE        0xd000  /*!< Starting number of Memory Protection API error codes */

unsigned long micros();
unsigned long millis();
void delay(uint32_t ms);
void delayMicroseconds(uint32_t us);
uint8_t wait_for_s(uint32_t time, uint32_t *Couinter);
uint8_t wait_for_ms(uint32_t time, uint32_t *Couinter);

#ifdef __cplusplus
} // closing brace for extern "C"
#endif

#endif //_COMMON_H_
