# RC TX nRF24 Stanek 2ch Oled
Simple surface 2 channel RC transmitter for cars, boats and robots.
The hardware includes a cheap, years-proven nRF24L01+ transceiver and an ATmega328P processor with an OLED screen.

**It is written for Arduino and includes:**
* Two proportional channels 1000us - 2000us
* Adjustable left and right EPA endpoints for each control channel 0 - 100%

This RC transmitter works with RC receiver from my repository [RX_nRF24L01_Telemetry_Motor_Servo](https://github.com/stanekTM/RX_nRF24L01_Telemetry_Motor_Servo)

Thanks to the original authors "Gabapentin" [Arduino-RC-6CH-Radio-control](https://github.com/Gabapentin/Arduino-RC-6CH-Radio-control) 
and "doohans" [arduino_surface_TX_4ch](https://github.com/doohans/arduino_surface_TX_4ch)
for code inspiration.

## Arduino pins
```
A0 - joystick 1
A1 - joystick 2

D2 - button Up/Prev
D3 - button Down/Next
D4 - button Menu/Select
D5 - button Exit

A7 - input TX battery

LCD I2C:
A4 - SDA
A5 - SCL

nRF24L01:
D9  - CE
D10 - CSN
D11 - MOSI
D12 - MISO
D13 - SCK
```

## Used libraries
* <RF24.h>    https://github.com/nRF24/RF24
* <U8g2lib.h> https://github.com/olikraus/u8g2
* <EEPROM.h>  Arduino standard library
* <SPI.h>     Arduino standard library
