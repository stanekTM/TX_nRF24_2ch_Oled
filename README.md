# RC_TX_nRF24L01_Telemetry_LCD
It is a surface 4-channel RC transmitter for direct control of the nRF24L01 transceiver on the Arduino with the ATmega328P for cars and boats. 
It contains the telemetry of the monitored voltage RX displayed on the LCD display. 
  
Thanks to the original authors "Gabapentin" [Arduino-RC-6CH-Radio-control](https://github.com/Gabapentin/Arduino-RC-6CH-Radio-control) 
and "doohans" [arduino_surface_TX_4ch](https://github.com/doohans/arduino_surface_TX_4ch)

This firmware has been completely redesigned for its requirements, but is not yet fully completed.

This RC transmitter works with a simple RC receiver from my repository [RC_RX_nRF24L01_Telemetry_Motor_Driver_Servo](https://github.com/stanekTM/RC_RX_nRF24L01_Telemetry_Motor_Driver_Servo)

### Arduino pins:
* D2 - button Up/Prev
* D3 - button Down/Next
* D4 - button Menu/Select
* D5 - button Exit
* D6 - buzzer
* A7 - input TX battery
#
LCD I2C:
* A4 - SDA
* A5 - SCL
#
nRF24L01:
* D9  - CE
* D10 - CSN
* D11 - MOSI
* D12 - MISO
* D13 - SCK

### Code:
[RC_TX_nRF24L01_Telemetry_LCD](https://github.com/stanekTM/RC_TX_nRF24L01_Telemetry_LCD/tree/main/RC_TX_nRF24L01_Telemetry_LCD)

### Used libraries:
* <SPI.h>     Arduino standard library
* <EEPROM.h>  Arduino standard library
* <U8g2lib.h> https://github.com/olikraus/u8g2
* <RF24.h>    https://github.com/nRF24/RF24
