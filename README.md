# Sonoff_basic + OTA

## Arduino IDE

In the Arduino IDE for sonoff select from Tools Board Generic ESP8266 Module and set the following options:

- Upload Using: Serial
- Flash Mode: DOUT
- Flash Frequency: 40MHz
- CPU Frequency: 80MHz
- Flash Size: 1M (64K SPIFFS)
- Debug Port: Disabled
- Debug Level: None
- Reset Method: ck
- Upload Speed: 115200
- Port: Your COM port connected to sonoff

Необходимо установить Python 2.7.x https://www.python.org/downloads/
При установке нужно включить опцию `Add python.exe to Path`
