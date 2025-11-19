

//#include "sdkconfig.h"

#define DEVICE_MODEL_NO             0x1001
#define DEVICE_FW_VERSION_MAJOR     1
#define DEVICE_FW_VERSION_MINOR     4

#define HARDWARE_VERSION_01_01      257

#define MODBUS_STATUS_LENGTH                 (43)
#define MODBUS_CONFIG_BUF_LENGTH             (10)
#define MODBUS_SETTING_LENGTH                (46)
#define MODBUS_OTAHEADER_LENGTH              (4)
#define MODBUS_OTADATA_LENGTH                (125)

#define TEMP_INVALID            0x7FFF
#define HUM_INVALID             0xFFFF
#define PM_INVALID              0xFFFF
#define CO2_INVALID             0xFFFF

#define USER_INTEREACTION_TIME_MS               180000
#define REFRESH_TIME_WHILE_INTERACTION_MS       30000
#define REFRESH_TIME_WHILE_INITIALIZATION_MS    15000
#define DEFAULT_SCREEN_REFRESH_TIME_MS          300000

#define DEVICE_MODBUS_MAX_ADDRESS           20
#define DEVICE_PAIRING_TIMEOUT              30    //SECONDS

#define DEBUG_UART_PORT         0
#define DEBUG_TX                43
#define DEBUG_RX                44

#define I2C_UART_PORT           0
#define I2C_SDA                 41
#define I2C_SCL                 42

#define EPD_HOST                SPI2_HOST
#define EPD_MISO                46
#define EPD_MOSI                20
#define EPD_SCK                 3
//#define EPD_SS                 17
//#define EPD_RESET               18
//#define EPD_DC                    19
//#define EPD_BUSY                17
//#define EPD_RESET               18
//#define EPD_DC                  8

#define RF_HOST                 SPI3_HOST
#define RF_SS                   45
#define RF_MOSI                 38
#define RF_MISO                 40
#define RF_SCK                  39
#define RF_RST                  48
#define RF_DIO0                 47
#define RF_NA                   0xFF


#define AI_NTC                  1

#define T2                      TOUCH_PAD_NUM4
#define T3                      TOUCH_PAD_NUM5
#define T4                      TOUCH_PAD_NUM6
#define T5                      TOUCH_PAD_NUM7
#define TSH                     TOUCH_PAD_NUM14

#define MODBUS_UART_PORT        1
#define MODBUS_SEL              11
#define MODBUS_TX               12
#define MODBUS_RX               13
#define MODBUS_BAUDRATE         230400

#define LED                     21
#define RELAY                   14
