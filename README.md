# RC TX nRF24L01 4ch
Surface 4 channel RC transmitter nRF24L01 with ATmega328P/16Mhz, Arduino Nano or Pro Mini for cars, boats and robots. 
It contains the telemetry of the monitored voltage RX displayed on the LCD display.

Note: I use (Arduino) ATmega328P 5V/16Mhz and supply VCC only with 3.3V voltage. 
  
Thanks to the original authors "Gabapentin" [Arduino-RC-6CH-Radio-control](https://github.com/Gabapentin/Arduino-RC-6CH-Radio-control) 
and "doohans" [arduino_surface_TX_4ch](https://github.com/doohans/arduino_surface_TX_4ch)
for code inspiration.

The firmware has been completely redesigned according to my requirements. 

This RC transmitter works with RC receiver from my repository [RX_nRF24L01_Telemetry_Motor_Servo](https://github.com/stanekTM/RX_nRF24L01_Telemetry_Motor_Servo)

## Arduino pins
```
A0 - joystick 1
A1 - joystick 2
A2 - potentiometer 3
A3 - potentiometer 4

D2 - button Up/Prev
D3 - button Down/Next
D4 - button Menu/Select
D5 - button Exit

D6 - buzzer
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
* <RF24.h>    https://github.com/nRF24/RF24 v1.3.9
* <U8g2lib.h> https://github.com/olikraus/u8g2 v2.27.6
* <EEPROM.h>  Arduino standard library
* <SPI.h>     Arduino standard library
#
Jiri StanekTM
