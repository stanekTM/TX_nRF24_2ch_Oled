//************************************************************************************************************************************************************************
// Config LCD display ****************************************************************************************************************************************************
//************************************************************************************************************************************************************************
U8G2_SSD1306_128X64_NONAME_1_HW_I2C
u8g2(U8G2_R0, U8X8_PIN_NONE);       //https://github.com/olikraus/u8g2/wiki/u8g2setupcpp#constructor-name

//hardware I2C
//----- SDA      A4 
//----- SCL      A5 

//************************************************************************************************************************************************************************
// Config radio comunication *********************************************************************************************************************************************
//************************************************************************************************************************************************************************
const byte addresses[][6] = {"tx001", "rx002"};

//pins for nRF24L01
#define CE       9
#define CSN      10

//hardware SPI
//----- MOSI     11 
//----- MISO     12 
//----- SCK      13 

//************************************************************************************************************************************************************************
// Config pin assignment *************************************************************************************************************************************************
//************************************************************************************************************************************************************************
#define buttonUp     2     // Button for Up/Prev functions
#define buttonDown   3     // Button for Down/Next functions
#define buttonSelect 4     // Button for Menu/Select functions
#define buttonExit   5     // Button for Exit
#define buzzer       6     // Buzzer pin (buzzer with standalone sound generator)

//************************************************************************************************************************************************************************
// Config PPM settings ***************************************************************************************************************************************************
//************************************************************************************************************************************************************************
#define servoMid  1500     // Servo center value (us)
#define servoMin  1000     // Min ppm output to be mapped (us)
#define servoMax  2000     // Max ppm output to be mapped (us)

//************************************************************************************************************************************************************************
// Config global TX param ************************************************************************************************************************************************
//************************************************************************************************************************************************************************
#define CHANNELS  4          // Number of channels 4
#define MODELS    10         // Total memory models 30
#define potCenter 512        // Pot Center reference value
#define numBytesPerModel 25  // Maximum bytes for data storage per model 32/25
#define epaMax           200 // Maximum EPA value

//************************************************************************************************************************************************************************
//************************************************************************************************************************************************************************
//************************************************************************************************************************************************************************
unsigned short int ppm[CHANNELS];            // ppm output array
unsigned short int pots[CHANNELS];           // Input ADC data array
unsigned short int calibration[CHANNELS][2]; // Min/Max value array for read channels and store calibration
unsigned short int raw_Pots[CHANNELS];       // Read raw input pots array
unsigned short int centerPos[2];             // Center Position. Only for Throttle and Steering

//************************************************************************************************************************************************************************
// Servo management parameters *******************************************************************************************************************************************
//************************************************************************************************************************************************************************
unsigned char epaSelection = 0xFF;          // Default value for EPA Selection
unsigned char expoSelection = 0xFF;         // Default value for EXPO Selection
unsigned short int subTrimSelection = 0xFF; // Default value for SUB TRIM
unsigned char servoReverse;                 // Reading bit status
unsigned char epa[3];                       // EPA value array
unsigned char expo[2];                      // EXPO value array                     
short subTrim[2];                           // SUB TRIM channel array
unsigned short int deadBand = 0;            // Deadband center stick value (25 suggested value) 30

//************************************************************************************************************************************************************************
// Battery voltage calculation, checking status for alarm, drawing battery bar *******************************************************************************************
//************************************************************************************************************************************************************************
#define minBat    33     // Minimum battery voltage for alarm (33 = 3.3V)
#define maxBat    42     // Maximum battery voltage (42 = 4.2V)
float raw_TX_batt;       // raw input battery pin value
int calc_bar_batt;       // Calculating correct voltage value voltage for battery bar
unsigned char perc_batt; // Battery bar level indicator (20 pixel) only Min to Max values

//************************************************************************************************************************************************************************
// MODEL NAME ************************************************************************************************************************************************************
//************************************************************************************************************************************************************************
char modelName[5];                       // MODEL NAME 5 char
unsigned char modelNameSelection = 0xFF; // Default value for MODEL NAME Selection

//************************************************************************************************************************************************************************
// Checking conditions control status for switch and calibration *************************************************************************************************
//************************************************************************************************************************************************************************
boolean calibStatus = 1; // Checking status to enter in calibration procedure
boolean battStatus = 1;
boolean exitStatus = 1;

//************************************************************************************************************************************************************************
//************************************************************************************************************************************************************************
//************************************************************************************************************************************************************************
byte pulseBuzzer; // Sound pulse for battery alarm

//************************************************************************************************************************************************************************
//************************************************************************************************************************************************************************
//************************************************************************************************************************************************************************
#define actualModelEepromAddr 1023

//************************************************************************************************************************************************************************
// Menu management variables *********************************************************************************************************************************************
//************************************************************************************************************************************************************************
#define MENU_COUNT 7             // Total Menu Count
unsigned char menuActual = 0;
unsigned char menuSubActual = 1;
unsigned char modelActual = 0;   // Added for Model Menu management
unsigned char menuSubModel = 0;  // Added for Model Menu management
unsigned char screen = 0;
unsigned char menuPage = 0;      // for Menu Page
unsigned char modelPage = 0;     // for Model Page

//************************************************************************************************************************************************************************
// Version ***************************************************************************************************************************************************************
//************************************************************************************************************************************************************************
const char ver_str[] = "version 0.1 nRF24L01";

//************************************************************************************************************************************************************************
// Using PROGMEM for characters and text strings to save RAM memory
// Array text strings for drawing menu
//************************************************************************************************************************************************************************
const char menu_0[] PROGMEM = "SERVO DIRECTION";
const char menu_1[] PROGMEM = "EPA";
const char menu_2[] PROGMEM = "MODEL SELECTION";
const char menu_3[] PROGMEM = "SAVE MODEL DATA";
const char menu_4[] PROGMEM = "SUB TRIM";
const char menu_5[] PROGMEM = "MODEL NAME";
const char menu_6[] PROGMEM = "EXPO";
const char menu_7[] PROGMEM = "free text";

const char* const menu_name[] PROGMEM = {
  menu_0,
  menu_1,
  menu_2,
  menu_3,
  menu_4,
  menu_5,
  menu_6,
  menu_7
};

//************************************************************************************************************************************************************************
// Array text strings (short) for Channel/Pots names, status, model for all screens *******************************************************************************
//************************************************************************************************************************************************************************
const char chName_0[] PROGMEM = "STR";
const char chName_1[] PROGMEM = "THR";
const char chName_2[] PROGMEM = "CH3";
const char chName_3[] PROGMEM = "CH4";
const char chName_4[] PROGMEM = "UP";
const char chName_5[] PROGMEM = "free text";
const char chName_6[] PROGMEM = "free text"; 
const char chName_7[] PROGMEM = "free text";
const char chName_8[] PROGMEM = "free text";
const char chName_9[] PROGMEM = "SEL";
const char chName_10[] PROGMEM = "RXbatt";
const char chName_11[] PROGMEM = "free text";
const char chName_12[] PROGMEM = "free text";
const char chName_13[] PROGMEM = "NOR";
const char chName_14[] PROGMEM = "REV";
const char chName_15[] PROGMEM = "free text";
const char chName_16[] PROGMEM = "free text";
const char chName_17[] PROGMEM = "PPM";
const char chName_18[] PROGMEM = "MODEL";
const char chName_19[] PROGMEM = "free text";
const char chName_20[] PROGMEM = "FWD";
const char chName_21[] PROGMEM = "BWD";

const char* const channel_name[] PROGMEM = {
  chName_0,
  chName_1,
  chName_2,
  chName_3,
  chName_4,
  chName_5,
  chName_6,
  chName_7,
  chName_8,
  chName_9,
  chName_10,
  chName_11,
  chName_12,
  chName_13,
  chName_14,
  chName_15,
  chName_16,
  chName_17,
  chName_18,
  chName_19,
  chName_20,
  chName_21
};

//************************************************************************************************************************************************************************
// Array for single characters *******************************************************************************************************************************************
//************************************************************************************************************************************************************************
const char character_0[] PROGMEM = "free text";
const char character_1[] PROGMEM = "free text";
const char character_2[] PROGMEM = "Y";
const char character_3[] PROGMEM = "R";
const char character_4[] PROGMEM = "N";
const char character_5[] PROGMEM = "free text";
const char character_6[] PROGMEM = "free text";
const char character_7[] PROGMEM = "free text";
const char character_8[] PROGMEM = "[";
const char character_9[] PROGMEM = "]";
const char character_10[] PROGMEM = "=";
const char character_11[] PROGMEM = "free text";
const char character_12[] PROGMEM = "free text";
const char character_13[] PROGMEM = "*";
const char character_14[] PROGMEM = ">";
const char character_15[] PROGMEM = "?";
const char character_16[] PROGMEM = "/";
const char character_17[] PROGMEM = "free text";
const char character_18[] PROGMEM = "V";
const char character_19[] PROGMEM = "E";
const char character_20[] PROGMEM = "free text";

const char* const one_char[] PROGMEM = {
  character_0,
  character_1,
  character_2,
  character_3,
  character_4,
  character_5,
  character_6,
  character_7,
  character_8,
  character_9,
  character_10,
  character_11,
  character_12,
  character_13,
  character_14,
  character_15,
  character_16,
  character_17,
  character_18,
  character_19,
  character_20
};

//************************************************************************************************************************************************************************
// Array text strings for screen messages ********************************************************************************************************************************
//************************************************************************************************************************************************************************
const char Message_0[] PROGMEM = "SAVE DATA";
const char Message_1[] PROGMEM = "RX-RF OFF!";
const char Message_2[] PROGMEM = "CALIBRATION";
const char Message_3[] PROGMEM = "Don't";
const char Message_4[] PROGMEM = "Move the Pots";
const char Message_5[] PROGMEM = "ERASE DATA";
const char Message_6[] PROGMEM = "LOW!";
const char Message_7[] PROGMEM = "MIN-MAX";
const char Message_8[] PROGMEM = "CENTER";

const char* const messages[] PROGMEM = {
  Message_0,
  Message_1,
  Message_2,
  Message_3,
  Message_4,
  Message_5,
  Message_6,
  Message_7,
  Message_8
};
 
