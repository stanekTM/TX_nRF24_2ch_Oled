# RC TX nRF24 2ch Oled
Simple surface 2 channel RC transmitter for cars, boats and tanks.
The code is written for Arduino and contains only the necessary functions of modern transmitters for DIY racing cars, where only the steering and throttle channel is enough without unnecessary functions.
The hardware includes a low-cost nRF24L01+ transceiver and an ATmega328P processor with an OLED screen.

This RC transmitter works with RC receiver from my repository [RX_nRF24L01_Telemetry_Motor_Servo](https://github.com/stanekTM/RX_nRF24L01_Telemetry_Motor_Servo)

Thank you to the original authors "Gabapentin" [Arduino-RC-6CH-Radio-control](https://github.com/Gabapentin/Arduino-RC-6CH-Radio-control) and "doohans" [arduino_surface_TX_4ch](https://github.com/doohans/arduino_surface_TX_4ch) for code inspiration.

NOTE: :) This transmitter is programmed in a small apartment with two kids, a permissive girlfriend, and zero privacy, where there is a lot of yelling, crying, and joy. It serves only for the need of self-relaxation. Please excuse my mistakes. I lost my private RC/electronics lab after moving.

## The firmware includes
* Two proportional channels 1000us - 2000us
* Adjustable left and right endpoints 0 - 100%
* Adjustable sub trims +-125us
* Adjustable exponential curves in steps 1 - 9
* Channel reverse
* Calibration of potentiometers
* Memory for ten models
* Five character model name
* RX voltage telemetry

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
