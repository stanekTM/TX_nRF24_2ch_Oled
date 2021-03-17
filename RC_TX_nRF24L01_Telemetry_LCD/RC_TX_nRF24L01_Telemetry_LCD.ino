
//******************************************************************************************************************************************************
// Add communication nRF24L01P. Fixed RF channel, fixed address.                                                                                       *
// Support for Arduino-based receivers and RF24 libraries from this repository https://github.com/stanekTM/RC_RX_nRF24L01_Telemetry_Motor_Driver_Servo *
// Thanks to the original authors "Gabapentin" https://github.com/Gabapentin/Arduino-RC-6CH-Radio-control                                              *
// and "doohans" https://github.com/doohans/arduino_surface_TX_4ch for code inspiration.                                                               *
//******************************************************************************************************************************************************


#include <SPI.h>      // Arduino standard library
#include <EEPROM.h>   // Arduino standard library
#include <U8g2lib.h>  // https://github.com/olikraus/u8g2 v2.27.6
#include "Config.h"   // Load static and variable configuration settings
#include <RF24.h>     // https://github.com/nRF24/RF24 v1.3.9
#include <nRF24L01.h>

//************************************************************************************************************************************************************************
// initial main settings *************************************************************************************************************************************************
//************************************************************************************************************************************************************************
void setup() {

//  Serial.begin(9600); //print value ​​on a serial monitor

  radio_setup();
  
  
  //------------------------------------------------------------------
  // LCD config with U8G2 library display init (mandatory)
  //------------------------------------------------------------------
//  u8g2.setBusClock(800000); //max 800000
  u8g2.begin();
//  u8g2.setFlipMode(1);   
//  u8g2.setContrast(10);
  u8g2.setFont(u8g2_font_6x10_tr); // Set default font type used for all display sessions (mandatory)
  

  boot_screen(); // print boot screen

  read_adc_setup();


  //-----------------------------------------------------------------
  // Debouncing mechanical buttons
  // NOTE: For input pin buttons is necessary to mount on every pin
  // 0,1uF/100nF(104)ceramic capacitors from pin to GND
  //-----------------------------------------------------------------
  pinMode(pin_buttonUp, INPUT_PULLUP);
  pinMode(pin_buttonDown, INPUT_PULLUP);
  pinMode(pin_buttonSelect, INPUT_PULLUP);
  pinMode(pin_buttonExit, INPUT_PULLUP);
  pinMode(pin_buzzer, OUTPUT);

//  delay(100); // Delay before reading button (about charge capacitor pulse on pin)


  //-------------------------------------------------------------------------------------
  // Default state config parameters
  //-------------------------------------------------------------------------------------
  // SERVO DIRECTION bit mask: 0 Normal, 1 Reverse
  servoReverse = 0b00000000;
  

  // EPA and SUB TRIM default values (only for first two channels)
  for (int i = 0; i < CHANNELS - 2; i++) {
    epa[i] = 100;
    subTrim[i] = 0;
  }
  
  // Default MODEL NAME 5 byte
  for (int i = 0; i < 5; i++) {
    modelName[i] = 0x5f;
  }


  // NOTE: SHOULD BE USED FOR THE FIRST TIME AFTER CALIBRATION !!!
  resetEeprom_screen(); // print "ERASE DATA" screen


  // Load data from Eeprom
  modelActual = storedDataEeprom(255);
}

//************************************************************************************************************************************************************************
// program loop **********************************************************************************************************************************************************
//************************************************************************************************************************************************************************
void loop() {

  //----------------------------------------------------------------------------
  // Start Calibration screen if buttonSelect is pressed on power on 
  //----------------------------------------------------------------------------
  if (calibStatus == 1 && read_button() == 2) {
    // Recall calibration procedure
    Calibration();
  }
  // Set condition 0 to continue loop if calibration procedure is not selected
  else {
    calibStatus = 0;
  }


  receive_time();
  send_and_receive_data();
  
  RX_batt_check();         // Checking RX battery status
  TX_batt_check();         // Checking TX battery status

  read_button_exit();      // Macro for read button status definitions
  read_pots();             // Macro for read pots, joysticks, values, applying calibration and rules
  select();                // Select screen, calibration, step control for channels/values
}
 
