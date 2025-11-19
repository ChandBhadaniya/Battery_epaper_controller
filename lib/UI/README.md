#Add esp board suppot to arduino.

- Add below URL to file->preferences-> Additional Board URLs:
```
https://dl.espressif.com/dl/package_esp32_index.json, http://arduino.esp8266.com/stable/package_esp8266com_index.json, https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json
```
- Then Go to 

Board manager and install esp32 lib.


#Changes for error:
```
undefined reference to `HardwareSerial::begin(unsigned long, unsigned int, signed char, signed char, bool, unsigned long)
```
- file: 
//Windows
C:\Users\Ravi\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.3\cores\esp32\HardwareSerial.cpp

//Ubuntu
/home/airobot/.arduino15/packages/esp32/hardware/esp32/2.0.3/cores/esp32/HardwareSerial.cpp
- Edit defienition: 
```
void HardwareSerial::begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, bool invert, unsigned long timeout_ms, uint8_t rxfifo_full_thrhd)
```
- Remove rxfifo_full_thrhd and define locally as below
```

void HardwareSerial::begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, bool invert, unsigned long timeout_ms)
{
    uint8_t rxfifo_full_thrhd = 112;
```
- Also edit declaration for this
```
file: C:\Users\Ravi\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.3\cores\esp32\HardwareSerial.h
file: /home/airobot/.arduino15/packages/esp32/hardware/esp32/2.0.3/cores/esp32/HardwareSerial.h

- void begin(unsigned long baud, uint32_t config=SERIAL_8N1, int8_t rxPin=-1, int8_t txPin=-1, bool invert=false, unsigned long timeout_ms = 20000UL, uint8_t rxfifo_full_thrhd = 112);

+ void begin(unsigned long baud, uint32_t config=SERIAL_8N1, int8_t rxPin=-1, int8_t txPin=-1, bool invert=false, unsigned long timeout_ms = 20000UL);
```

#Change pinout of ESP32S2

- Copy directory "esp32s2" to path "C:\Users\Ravi\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.3\variants"