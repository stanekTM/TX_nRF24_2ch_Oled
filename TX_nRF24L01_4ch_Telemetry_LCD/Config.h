
//*********************************************************************************************************************
// Version
//*********************************************************************************************************************
const char ver_str[] = "TX stanekTM v 1.0";

//*********************************************************************************************************************
// pins connecting
//*********************************************************************************************************************

//joystick 1               A0
//joystick 2               A1
//potentiometer 3          A2
//potentiometer 4          A3

//pin battery              A7

//pins button
#define PIN_BUTTON_UP      2 //for Up/Prev functions
#define PIN_BUTTON_DOWN    3 //for Down/Next functions
#define PIN_BUTTON_SELECT  4 //for Menu/Select functions
#define PIN_BUTTON_EXIT    5 //for Exit

//pin buzzer (buzzer with standalone sound generator)
//#define PIN_BUZZER         6

//pins for nRF24L01
#define PIN_CE             9
#define PIN_CSN            10
//          MOSI           11 hardware SPI
//          MISO           12 hardware SPI
//          SCK            13 hardware SPI

//pins LCD display
//          SDA            A4 hardware I2C
//          SCL            A5 hardware I2C

//*********************************************************************************************************************
// Config radio comunication
//*********************************************************************************************************************
//setting RF channels address (5 bytes number or character)
const byte address[] = "jirka";

//RF communication channel settings (0-125, 2.4Ghz + 76 = 2.476Ghz)
#define RADIO_CHANNEL  76

// this structure defines the sent data in bytes (structure size max. 32 bytes)
struct rc_packet_size
{
  unsigned int ch1;
  unsigned int ch2;
  //unsigned int ch3;
  //unsigned int ch4;
  //unsigned int ch5;
};
rc_packet_size rc_packet;

// this struct defines data, which are embedded inside the ACK payload
struct telemetry_packet_size
{
  byte rssi;    // not used yet
  byte batt_A1;
  byte batt_A2; // not used yet
};
telemetry_packet_size telemetry_packet;

//*********************************************************************************************************************
// Config LCD display
//*********************************************************************************************************************
U8G2_SSD1306_128X64_NONAME_1_HW_I2C
u8g2(U8G2_R0, U8X8_PIN_NONE);       //https://github.com/olikraus/u8g2/wiki/u8g2setupcpp#constructor-name

//*********************************************************************************************************************
// Battery voltage settings TX/RX, minimum battery voltage for alarm
//*********************************************************************************************************************
#define TX_BATTERY_VOLTAGE    4.2  // Maximum battery voltage
#define TX_MONITORED_VOLTAGE  3.35 // Minimum battery voltage for alarm

#define RX_BATTERY_VOLTAGE    4.2  // Maximum battery voltage
#define RX_MONITORED_VOLTAGE  3.35 // Minimum battery voltage for alarm

//*********************************************************************************************************************
// setting the control range value
//*********************************************************************************************************************
#define MIN_CONTROL_VAL  1000
#define MID_CONTROL_VAL  1500
#define MAX_CONTROL_VAL  2000

//*********************************************************************************************************************
// Config global TX param
//*********************************************************************************************************************
#define CHANNELS                 4    // Number of channels 4
#define MODELS                   10   // Total memory models 30
#define NUM_BYTES_PER_MODEL      25   // Maximum bytes for data storage per model 32/25
#define ACTUAL_MODEL_EEPROM_ADDR 1023

//*********************************************************************************************************************
//
//*********************************************************************************************************************
#define POT_CENTER 512                   // Pot Center reference value
unsigned short int pots[CHANNELS];       // Input ADC data array
unsigned short int pots_value[CHANNELS]; // pots value output array
unsigned short int pot_calib_min[] = {0, 0, 0, 0};
unsigned short int pot_calib_mid[] = {512, 512, 512, 512};
unsigned short int pot_calib_max[] = {1023, 1023, 1023, 1023};

//*********************************************************************************************************************
// Servo management parameters
//*********************************************************************************************************************
unsigned short int deadBand = 10;           // Deadband center stick value (25 suggested value)
#define EPA_MAX 100                         // Maximum EPA value
unsigned char epa[3];                       // EPA value array
unsigned char epaSelection = 0xFF;          // Default value for EPA Selection
unsigned char expoSelection = 0xFF;         // Default value for EXPO Selection
unsigned short int subTrimSelection = 0xFF; // Default value for SUB TRIM
unsigned char servoReverse;                 // Reading bit status
unsigned char expo[2];                      // EXPO value array
short subTrim[2];                           // SUB TRIM channel array

//*********************************************************************************************************************
// MODEL NAME
//*********************************************************************************************************************
char modelName[5];                       // MODEL NAME 5 char
unsigned char modelNameSelection = 0xFF; // Default value for MODEL NAME Selection

//*********************************************************************************************************************
// Checking conditions control status for switch and calibration
//*********************************************************************************************************************
bool calibStatus = 1; // Checking status to enter in calibration procedure

//*********************************************************************************************************************
// Menu management variables
//*********************************************************************************************************************
#define MENU_COUNT  7 // Total Menu Count
unsigned char menuActual    = 0;
unsigned char menuSubActual = 1;
unsigned char modelActual   = 0; // Added for Model Menu management
unsigned char menuSubModel  = 0; // Added for Model Menu management
unsigned char screen        = 0;
unsigned char menuPage      = 0; // for Menu Page
unsigned char modelPage     = 0; // for Model Page

//*********************************************************************************************************************
// Using PROGMEM for characters and text strings to save RAM memory
// Array text strings for drawing menu "menu_buffer[7]"
//*********************************************************************************************************************
const char menu_name_0[] PROGMEM = "SERVO DIRECTION";
const char menu_name_1[] PROGMEM = "EPA";
const char menu_name_2[] PROGMEM = "MODEL SELECTION";
const char menu_name_3[] PROGMEM = "SAVE MODEL DATA";
const char menu_name_4[] PROGMEM = "SUB TRIM";
const char menu_name_5[] PROGMEM = "MODEL NAME";
const char menu_name_6[] PROGMEM = "EXPO";

const char* const menu_name[] PROGMEM = {
  menu_name_0,
  menu_name_1,
  menu_name_2,
  menu_name_3,
  menu_name_4,
  menu_name_5,
  menu_name_6
};

//*********************************************************************************************************************
// Array text strings (short) for Channel/Pots names, status, model for all screens "name_buffer[13]"
//*********************************************************************************************************************
const char channel_name_0[] PROGMEM = "CH1";
const char channel_name_1[] PROGMEM = "CH2";
const char channel_name_2[] PROGMEM = "CH3";
const char channel_name_3[] PROGMEM = "CH4";
const char channel_name_4[] PROGMEM = "UP";
const char channel_name_5[] PROGMEM = "DOWN";
const char channel_name_6[] PROGMEM = "BWD";
const char channel_name_7[] PROGMEM = "TRIM";
const char channel_name_8[] PROGMEM = "NOR";
const char channel_name_9[] PROGMEM = "REV";
const char channel_name_10[] PROGMEM = "PPM";
const char channel_name_11[] PROGMEM = "MODEL";
const char channel_name_12[] PROGMEM = "FWD";

const char* const channel_name[] PROGMEM = {
  channel_name_0,
  channel_name_1,
  channel_name_2,
  channel_name_3,
  channel_name_4,
  channel_name_5,
  channel_name_6,
  channel_name_7,
  channel_name_8,
  channel_name_9,
  channel_name_10,
  channel_name_11,
  channel_name_12
};

//*********************************************************************************************************************
// Array for single characters "char_buffer[8]"
//*********************************************************************************************************************
const char one_char_0[] PROGMEM = "Y";
const char one_char_1[] PROGMEM = "N";
const char one_char_2[] PROGMEM = "[";
const char one_char_3[] PROGMEM = "]";
const char one_char_4[] PROGMEM = "=";
const char one_char_5[] PROGMEM = ">";
const char one_char_6[] PROGMEM = "/";
const char one_char_7[] PROGMEM = "V";
//const char one_char_8[] PROGMEM = ".";

const char* const one_char[] PROGMEM = {
  one_char_0,
  one_char_1,
  one_char_2,
  one_char_3,
  one_char_4,
  one_char_5,
  one_char_6,
  one_char_7,
  //one_char_8
};

//*********************************************************************************************************************
// Array text strings for screen messages "msg_buffer[11]"
//*********************************************************************************************************************
const char message_0[] PROGMEM = "SAVE DATA";
const char message_1[] PROGMEM = "off!";
const char message_2[] PROGMEM = "CALIBRATION";
const char message_3[] PROGMEM = "Don't";
const char message_4[] PROGMEM = "Move the Pots";
const char message_5[] PROGMEM = "ERASE DATA";
const char message_6[] PROGMEM = "low!";
const char message_7[] PROGMEM = "MIN-MAX";
const char message_8[] PROGMEM = "CENTER";
const char message_9[] PROGMEM = "RX";
const char message_10[] PROGMEM = "TX";

const char* const message[] PROGMEM = {
  message_0,
  message_1,
  message_2,
  message_3,
  message_4,
  message_5,
  message_6,
  message_7,
  message_8,
  message_9,
  message_10
};
 
