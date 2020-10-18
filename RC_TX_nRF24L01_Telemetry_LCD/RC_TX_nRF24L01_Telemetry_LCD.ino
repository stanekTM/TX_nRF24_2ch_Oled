//************************************************************************************************************************************************************************
//************************************************************************************************************************************************************************
//************************************************************************************************************************************************************************
#include <SPI.h>      // Arduino standard library
#include <EEPROM.h>   // Arduino standard library
#include <U8g2lib.h>  // https://github.com/olikraus/u8g2
#include "Config.h"   // Load static and variable configuration settings
#include <RF24.h>     // https://github.com/nRF24/RF24
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
  u8g2.setBusClock(800000); //max 800000
  u8g2.begin();
//  u8g2.setFlipMode(1);   
//  u8g2.setContrast(10);
  // Set default font type used for all display sessions (mandatory)
  u8g2.setFont(u8g2_font_6x10_tr);
  

  boot_screen(); // print boot screen

  read_adc_setup();


  //-----------------------------------------------------------------
  // Debouncing mechanical buttons
  // NOTE: For input pin buttons is necessary to mount on every pin
  // 0,1uF/100nF(104)ceramic capacitors from pin to GND
  //-----------------------------------------------------------------
  pinMode(buttonUp, INPUT_PULLUP);
  pinMode(buttonDown, INPUT_PULLUP);
  pinMode(buttonSelect, INPUT_PULLUP);
  pinMode(buttonExit, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);

//  delay(100); // Delay before reading button (about charge capacitor pulse on pin)


/*
  //-------------------------------------------
  // Default state values for ppm
  //-------------------------------------------
  ppm[0] = servoCenter; // channel 1 (STR)
  ppm[1] = servoCenter; // channel 2 (THR)
  ppm[2] = servoMin;    // channel 3 (CH3)
  ppm[3] = servoMin;    // channel 4 (CH4)
*/  

  //-------------------------------------------------------------------------------------
  // Default state config parameters
  //-------------------------------------------------------------------------------------

  // SERVO DIRECTION bit mask: 0 Normal, 1 Reverse
  servoReverse = 0b00000000;
  

  // EPA and SUB TRIM default values (only for first two channels)
  for (int i = 0; i < CHANNELS - 2; i++) {
    epa[i] = 75;
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

  receive_time();
  send_and_receive_data();
  
  RX_batt_check();         // Checking RX battery status
  TX_batt_check();         // Checking TX battery status

  read_button_exit();      // Macro for read button status definitions
  read_pots();             // Macro for read pots, sticks, values, applying calibration and rules
  select();                // Select screen, calibration, step control for channels/values
}
 
