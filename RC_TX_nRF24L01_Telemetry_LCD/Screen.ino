//************************************************************************************************************************************************************************
// Drawing BOOT screen display
//************************************************************************************************************************************************************************
void boot_screen()
{
  u8g2.firstPage(); do {

    // Print version string
    u8g2.setCursor(4, 28);
    u8g2.print(ver_str);

  } while (u8g2.nextPage());
  
  delay(1000);
}

//************************************************************************************************************************************************************************
// Drawing MAIN screen display
//************************************************************************************************************************************************************************
// this is the state machine, which will replace the do - while loop
void draw_main_screen() 
{
  static uint8_t is_next_page = 0;
  
  //call to first page, if required
  if (is_next_page == 0)
  {
    u8g2.firstPage();
    is_next_page = 1;
  } 

  //draw our screen
  main_screen();
  
  //call to next page
  if (u8g2.nextPage() == 0)
  {
    is_next_page = 0;      // ensure, that first page is called
  }  
}

void main_screen() {

  // Set memory buffer for text strings
  char chName_buffer[22];
  char char_buffer[21];
  char msg_buffer[9];

//  u8g2.firstPage(); do {

//  read_pots(); // Macro again for stable ppm pulse

    u8g2.setFont(u8g2_font_6x10_tr);
    
    // Print "MODEL"
    strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[18])));
    u8g2.setCursor(0, 7);
    u8g2.print(chName_buffer);

    // Print number of which model in use
    u8g2.setCursor(32, 7);
    u8g2.print(modelActual + 1);
    

    // Print ">"
    strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[14])));
    u8g2.setCursor(43, 7);
    u8g2.print(char_buffer);

    // Print MODEL NAME
    u8g2.drawStr(50, 7, modelName);

    
    // Print value TX battery
    u8g2.setCursor(91, 7);
    u8g2.print(raw_TX_batt / 100);

    // Print "V"
    strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[18])));
    u8g2.setCursor(109, 7);
    u8g2.print(char_buffer);

    // Battery symbol contruction
    u8g2.drawFrame(116, 0, 11, 7);      // Battery box
    u8g2.drawBox(116, 1, perc_batt, 5); // level bar          
    u8g2.drawVLine(127, 2, 3);          // Battery nipple plus pole


    if (RFstate)
    {
      // Print "RX-RF OFF!"
      strcpy_P(msg_buffer, (char*)pgm_read_word(&(messages[1])));
      u8g2.setCursor(0, 25);
      u8g2.print(msg_buffer);
    }
    else if (RXbattstate)
    {
      // Print "RXbatt"
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[10])));
      u8g2.setCursor(0, 25);
      u8g2.print(chName_buffer);
      
      // Print "LOW!"
      strcpy_P(msg_buffer, (char*)pgm_read_word(&(messages[6])));
      u8g2.setCursor(38, 25);
      u8g2.print(msg_buffer);
    }
    else
    {
      // Print "RXbatt"
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[10])));
      u8g2.setCursor(0, 25);
      u8g2.print(chName_buffer);

      // Print value RX battery
      u8g2.setCursor(38, 25);
      u8g2.print(payload.RXbatt);
    
      // Print "V"
      strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[18])));
      u8g2.setCursor(56, 25);
      u8g2.print(char_buffer);
    }


    u8g2.setFont(u8g2_font_5x7_tr);
    
    // SUB TRIM
//    u8g2.setFontMode(1);
//    u8g2.setDrawColor(2);
//    u8g2.drawBox(88, 11, 8, 18);        // Trim Box
    
    // Print SUB TRIM value
    for (int i = 0; i < 2; i++) {

      // Print channel items name "STR, THR"
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[i])));
      u8g2.setCursor(91, 22 + i * 8);
      u8g2.print(chName_buffer);
      
      // SUB TRIM value
      u8g2.setCursor(109, 22 + i * 8);
      u8g2.print(subTrim[i]);
    }

//    u8g2.setDrawColor(1);
//    u8g2.drawFrame(88, 11, 40, 18);             // Trim Box
//    u8g2.setFontMode(0);


    // Print first 2 channels reference with input pots order

    // Print "STR"
    strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[0])));
    u8g2.setCursor(0, 41);
    u8g2.print(chName_buffer);
    
    // Print "THR"
    strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[1])));
    u8g2.setCursor(0, 53);
    u8g2.print(chName_buffer);

    //Drawing vertical middle/center separation line
    u8g2.drawVLine(66, 32, 24); //63 + 3, 32, 24

    // Drawing only first 2 channels
    for (int i = 0; i < 2; i++) {

      // Define value bar reference
      unsigned int valBar;

      // Value bars subdivision (ppm value / 55)
      valBar = map(ppm[i], servoMin, servoMax, 0, 100);

      // Draw boxes/frames for every channel
      u8g2.drawFrame(16, 34 + (i * 12), 101, 8); //13 + 3, 34 + (i * 12), 101, 8    /*/ + 3/*/

      // Drawing cursor in every channel bar
      if      (valBar < 50) {
        u8g2.drawBox(17 + valBar, 35 + (i * 12), 50 - valBar, 6); //17 + valBar, 35 + (i * 12), 50 - valBar, 6
      }                                                           
      else if (valBar > 50) {                                     //67 - 17 = 50
        u8g2.drawBox(67, 35 + (i * 12), valBar - 50, 6);          //64 + 3, 35 + (i * 12), valBar - 50, 6
      }


      unsigned short subTrimVal = map(subTrim[i], 0, 500, 0, 50);

      // Check Servo Reversing and applying Reverse value if necessary
      if (bitRead(servoReverse, i) == 1) {
        subTrimVal = -subTrimVal;
      }


      unsigned short minMaxValid = 0;
      
      // EPA
      short epa_1 = epa[i];
      short epa_2 = epa[i];
      
      if (i == 1) {
        
        if (bitRead(servoReverse, i) == 1) {
          epa_2 = epa[2];
          }
          else {
            epa_1 = epa[2];
          }
        }

        minMaxValid = 66 - (50 * epa_1 / 100) + subTrimVal; //63 - (50 * epa_1 / 100
        if (minMaxValid < 16) minMaxValid = 16;             //13) minMaxValid = 13   //66 - 16 = 50
        if (minMaxValid > 116) minMaxValid = 116;           //113) minMaxValid = 113
        u8g2.drawVLine(minMaxValid, 39 + (i * 12), 2);      //39 + (i * 12), 2

        

        minMaxValid = 66 + (50 * epa_2 / 100) + subTrimVal; //63 + (50 * epa_2 / 100
        if (minMaxValid < 16) minMaxValid = 16;             //13) minMaxValid = 13   //116 - 66 = 50
        if (minMaxValid > 116) minMaxValid = 116;           //113) minMaxValid = 113
        u8g2.drawVLine(minMaxValid, 39 + (i * 12), 2);      //39 + (i * 12), 2


      // Print Reverse/Normal channel status for every channel
      if (bitRead(servoReverse, i) == 1) {

        // Print "R"
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[3])));
        u8g2.setCursor(119, 41 + i * 12);
        u8g2.print(char_buffer);
      }
      else {

        // Print "N"
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[4])));
        u8g2.setCursor(119, 41 + i * 12);
        u8g2.print(char_buffer);
      }

      // EXPO
      if (expo[i] > 0) {
        // Print "E"
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[19])));
        u8g2.setCursor(124, 41 + i * 12);
        u8g2.print(char_buffer);
      }
    }


      //If pot CH3 print value in %
      unsigned int CH3value;
      CH3value = map(ppm[2], servoMin, servoMax, 0, 100);

      // Print "CH3"
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[2])));
      u8g2.setCursor(0, 64);
      u8g2.print(chName_buffer);

      // Print CH3 value
      u8g2.setCursor(20, 64);
      u8g2.print(CH3value);
      

      //If pot CH4 print value in %
      unsigned int CH4value;
      CH4value = map(ppm[3], servoMin, servoMax, 0, 100);

      // Print "CH4"
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[3])));
      u8g2.setCursor(93, 64);
      u8g2.print(chName_buffer);

      // Print CH4 value
      u8g2.setCursor(113, 64);
      u8g2.print(CH4value);

//  } while (u8g2.nextPage());
  
  u8g2.setFont(u8g2_font_6x10_tr); // set font menu, submenu
}

//************************************************************************************************************************************************************************
// Drawing MENU screen display
//************************************************************************************************************************************************************************
// this is the state machine, which will replace the do - while loop
void draw_menu_screen() 
{
  static uint8_t is_next_page = 0;
  
  // call to first page, if required
  if (is_next_page == 0)
  {
    u8g2.firstPage();
    is_next_page = 1;
  } 

  //draw our screen
  menu_screen();
  
  //call to next page
  if (u8g2.nextPage() == 0)
  {
    is_next_page = 0;      // ensure, that first page is called
  }  
}

void menu_screen() {

  // Set memory buffer for text strings
  char chName_buffer[22];
  char menu_buffer[8];
  char char_buffer[21];

//  u8g2.firstPage(); do {

//  read_pots(); // Macro again for stable ppm pulse
/*
    // Print "MODEL"
    strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[18])));
    u8g2.setCursor(0, 7);
    u8g2.print(chName_buffer);

    // Print number of which model in use
    u8g2.setCursor(32, 7);
    u8g2.print(modelActual + 1);

    // Print MODEL NAME
    u8g2.drawStr(48, 7, modelName);
*/

    // Print "/"
    strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[16])));
    u8g2.setCursor(117, 7);
    u8g2.print(char_buffer);

    u8g2.setCursor(111, 7);
    u8g2.print(menuPage + 1);

    u8g2.setCursor(123, 7);
    u8g2.print( (MENU_COUNT - 1) / 5 + 1);  // Total Menu Count / menu count per page + 1

    // Drawing horizontal line under header
    u8g2.drawHLine(0, 8, 128);

    for (int i = 1; i < 6; i++) {

      if (i + (5 * menuPage) > MENU_COUNT)
      break;

      // Print main menu items "SERVO DIRECTION, EPA, MODEL SELECTION, SAVE MODEL DATA, SUB TRIM, MODEL NAME, EXPO"
      strcpy_P(menu_buffer, (char*)pgm_read_word(&(menu_name[i + (5 * menuPage) - 1])));

      if (i + (5 * menuPage) == menuSubActual) {

        // Print "*"
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[13])));
        u8g2.setCursor(5, 9 + (i * 10));
        u8g2.print(char_buffer);

        // Print main menu items
        u8g2.setCursor(14, 9 + (i * 10));
        u8g2.print(menu_buffer);
      }
      else {

        // Print main menu items
        u8g2.setCursor(20, 9 + (i * 10));
        u8g2.print(menu_buffer);
      }
    }
    
//  } while (u8g2.nextPage());
}

//************************************************************************************************************************************************************************
// Drawing SERVO DIRECTION screen display
//************************************************************************************************************************************************************************
// this is the state machine, which will replace the do - while loop
void draw_servo_dir_screen() 
{
  static uint8_t is_next_page = 0;
  
  // call to first page, if required
  if (is_next_page == 0)
  {
    u8g2.firstPage();
    is_next_page = 1;
  } 

  // draw our screen
  servo_dir_screen();
  
  // call to next page
  if (u8g2.nextPage() == 0)
  {
    is_next_page = 0;      // ensure, that first page is called
  }  
}

void servo_dir_screen() {

  // Set memory buffer for text strings
  char menu_buffer[8];
  char chName_buffer[22];
  char char_buffer[21];

//  u8g2.firstPage(); do {

//  read_pots(); // Macro again for stable ppm pulse


    // Print SERVO DIRECTION channels list
    for (int i = 0; i < CHANNELS; i++) {

      if (i == menuSubActual - 1) {
        // Print "["
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[8])));
        u8g2.setCursor(5, 20 + i * 13);
        u8g2.print(char_buffer);

        // Print "="
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[10])));
        u8g2.setCursor(31, 20 + i * 13);
        u8g2.print(char_buffer);

        // Print "]"
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[9])));
        u8g2.setCursor(57, 20 + i * 13);
        u8g2.print(char_buffer);      
      }
      

      // Print channel items name "STR, THR, CH3, CH4"
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[i])));
      u8g2.setCursor(12, 20 + i * 13);
      u8g2.print(chName_buffer);
      
      
      if (bitRead(servoReverse, i) == 1) {
        // Print "REV"
        strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[14])));
        u8g2.setCursor(38, 20 + i * 13);
        u8g2.print(chName_buffer);
      }
      else {
        // Print "NOR"
        strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[13])));
        u8g2.setCursor(38, 20 + i * 13);
        u8g2.print(chName_buffer);
      }
    }


    // Drawing dynamic graphics items
    for (int i = 0; i < CHANNELS; i++) {
      u8g2.drawHLine(72, 20 + i * 13, 45);
      u8g2.drawVLine(94, 20 + i * 13 - 4, 4);
      u8g2.drawBox(map(ppm[i], 1000 , 2000, 74, 114) - 1, 18 + (i * 13), 3, 2);
    }


    // Print "SERVO DIRECTION"
    strcpy_P(menu_buffer, (char*)pgm_read_word(&(menu_name[0])));
    u8g2.setCursor(0, 7);
    u8g2.print(menu_buffer);

    // Drawing horizontal line under header
    u8g2.drawHLine(0, 8, 128);
    
//  } while (u8g2.nextPage());
}

//************************************************************************************************************************************************************************
// Drawing EPA screen display
//************************************************************************************************************************************************************************
// this is the state machine, which will replace the do - while loop
void draw_epa_screen() 
{
  static uint8_t is_next_page = 0;
  
  // call to first page, if required
  if (is_next_page == 0)
  {
    u8g2.firstPage();
    is_next_page = 1;
  } 

  // draw our screen
  epa_screen();
  
  // call to next page
  if (u8g2.nextPage() == 0)
  {
    is_next_page = 0;      // ensure, that first page is called
  }  
}

void epa_screen() {
  
  // Set memory buffer for text strings
  char menu_buffer[8];
  char chName_buffer[22];
  char char_buffer[21];

//  u8g2.firstPage(); do {

//  read_pots(); // Macro again for stable ppm pulse
  
    // Print "EPA"
    strcpy_P(menu_buffer, (char*)pgm_read_word(&(menu_name[1])));
    u8g2.setCursor(0, 7);
    u8g2.print(menu_buffer);

    // Drawing horizontal line under header
    u8g2.drawHLine(0, 8, 128);

    
    unsigned char counterTemp = 0;

    // Print EPA channels list
    for (int i = 0; i < 3; i++) {
    
      // Print channel items name "STR, THR"
      if (i > 1) {
        // Print "THR"
        strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[1])));
      }
      else {
        strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[i])));
      }
        
      u8g2.setCursor(2, 20 + i * 13);
      u8g2.print(chName_buffer);
  
    if (i == 1) {
      // Print "FWD"
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[20])));
      u8g2.setCursor(24, 20 + i * 13);
      u8g2.print(chName_buffer);
    }
    
    if (i == 2) {
      // Print "BWD"
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[21])));
      u8g2.setCursor(24, 20 + i * 13);
      u8g2.print(chName_buffer);
    }
      
    if (menuSubActual - 1 == counterTemp) {
      if (epaSelection == counterTemp) {
      
        // Print "["
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[8])));
        u8g2.setCursor(59, 20 + i * 13);
        u8g2.print(char_buffer);

        // Print "]"
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[9])));
        u8g2.setCursor(85, 20 + i * 13);
        u8g2.print(char_buffer);
      }
      else {
        // Print ">"
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[14])));
        u8g2.setCursor(59, 20 + i * 13);
        u8g2.print(char_buffer);
      }
    }
    
    // EPA value
    u8g2.setCursor(66, 20 + i * 13);
    u8g2.print(epa[i]);
    
    counterTemp++;
  }

// } while (u8g2.nextPage());
}

//************************************************************************************************************************************************************************
// Drawing MODEL SELECTION screen display
//************************************************************************************************************************************************************************
// this is the state machine, which will replace the do - while loop
void draw_model_sel_screen() 
{
  static uint8_t is_next_page = 0;
  
  // call to first page, if required
  if (is_next_page == 0)
  {
    u8g2.firstPage();
    is_next_page = 1;
  } 

  // draw our screen
  model_sel_screen();
  
  // call to next page
  if (u8g2.nextPage() == 0)
  {
    is_next_page = 0;      // ensure, that first page is called
  }  
}

void model_sel_screen() {

  // Set memory buffer for text strings
  char char_buffer[21];
  char chName_buffer[22];

  // For Eeprom position reference
  unsigned int eepromPos;
  int tempModelNoIdx;

//  u8g2.firstPage(); do {

    // Print "MODEL"
    strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[18])));
    u8g2.setCursor(0, 7);
    u8g2.print(chName_buffer);

    // Print number of which model in use
    u8g2.setCursor(32, 7);
    u8g2.print(modelActual + 1);

    // Print "/"
    strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[16])));
    u8g2.setCursor(117, 7);
    u8g2.print(char_buffer);

    u8g2.setCursor(111, 7);
    u8g2.print(modelPage + 1);

    u8g2.setCursor(123, 7);
    u8g2.print( (MODELS - 1) / 10 + 1);  // Total model Count / model count per page + 1

    // Drawing horizontal line under header
    u8g2.drawHLine(0, 8, 128);

    
    // Print MODEL SELECTION list
    for (int i = 0; i < 5; i++) {
      //--------------------------------------------------------------------
      // Left Section Start
      //--------------------------------------------------------------------
      tempModelNoIdx = i + (10 * modelPage);
      if (tempModelNoIdx > MODELS) break;

      u8g2.setCursor(8, 19 + i * 10);
      u8g2.print(tempModelNoIdx + 1);

      // Define start position for Eeprom write/update (32 * [0,1,2,3,4])
      eepromPos = numBytesPerModel * tempModelNoIdx;

      for (int j = 0; j < 5; j++) {
        u8g2.setCursor(23 + (j * 6), 19 + i * 10);
        char ch = EEPROM.read(eepromPos++);
        u8g2.print(ch);
      }

      if (tempModelNoIdx == menuSubModel) {

        // Print "["
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[8])));
        u8g2.setCursor(0, 19 + i * 10);
        u8g2.print(char_buffer);
        
        // Print "]"
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[9])));
        u8g2.setCursor(55, 19 + i * 10);
        u8g2.print(char_buffer);
      }
      // Left Section End

      //--------------------------------------------------------------------
      // Right Section Start
      //--------------------------------------------------------------------
      tempModelNoIdx = (i + 5) + (10 * modelPage);
      if (tempModelNoIdx > MODELS) break;

      u8g2.setCursor(75, 19 + i * 10);
      u8g2.print(tempModelNoIdx + 1);

      // Define start position for Eeprom write/update (25 * [0,1,2,3,4])
      eepromPos = numBytesPerModel * tempModelNoIdx;

      for (int j = 0; j < 5; j++) {
        u8g2.setCursor(90 + (j * 6), 19 + i * 10);
        char ch = EEPROM.read(eepromPos++);
        u8g2.print(ch);
      }

      if (tempModelNoIdx == menuSubModel) {

        // Print "["
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[8])));
        u8g2.setCursor(67, 19 + i * 10);
        u8g2.print(char_buffer);

        // Print "]"
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[9])));
        u8g2.setCursor(122, 19 + i * 10);
        u8g2.print(char_buffer);
      }
      // Right Section End
    }

//  } while (u8g2.nextPage());
}

//************************************************************************************************************************************************************************
// Drawing SAVE MODEL DATA screen display
//************************************************************************************************************************************************************************
void save_model_screen() {

  // For Eeprom position reference
  unsigned int eepromBase;

  // For write/Update SUB TRIM stored values
  //unsigned int posEeprom;

  // Define start position for Eeprom write/update (32 * [0,1,2,3,4])
  eepromBase = numBytesPerModel * modelActual;

  // For write/Update SERVO DIRECTION and EPA position
  unsigned int eepromPos = eepromBase;

  // Save Actual Model data
  EEPROM.update(actualModelEepromAddr, modelActual);

  // Save MODEL NAME
  for (int i = 0; i < 5; i++) {
    EEPROM.update(eepromPos++, modelName[i]);
  }

  // Save SERVO DIRECTION data
  EEPROM.update(eepromPos++, servoReverse);

  // Save SUB TRIM center stick values for two channels in every model memory bank
  for (int i = 0; i < 2; i++) {

    // Save center stick values for every channels
    EEPROMUpdateInt(eepromPos, subTrim[i]);
    eepromPos += 2;
  }

  // Save EPA data
  for (int i = 0; i < 3; i++) {

    EEPROM.update(eepromPos, epa[i]);
    eepromPos++;
  }

  // Save EXPO data
  for (int i = 0; i < 2; i++) {

    EEPROM.update(eepromPos, expo[i]);
    eepromPos++;
  }
  
  // Start of Save Data message screen ---------------------------------------------------------------------
  u8g2.firstPage(); do {

    // Set memory buffer for text strings
    char msg_buffer[9];
    char chName_buffer[22];

    // Print "SAVE DATA"
    strcpy_P(msg_buffer, (char*)pgm_read_word(&(messages[0])));
    u8g2.setCursor(40, 12);
    u8g2.print(msg_buffer);

    // Print "MODEL"
    strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[18])));
    u8g2.setCursor(43, 35);
    u8g2.print(chName_buffer);

    u8g2.setCursor(78, 35);
    u8g2.print(modelActual + 1);

    // Print MODEL NAME 5 byte
    for (int i = 0; i < 5; i++) {
      u8g2.setCursor(47 + i * 7, 55);
      u8g2.print(modelName[i]);
    }
    
  } while (u8g2.nextPage());

  delay(1000); // Screen message for 2sec

  screen--;
  menuActual = 0;
  menuSubActual = 4;
}

//************************************************************************************************************************************************************************
// Drawing SUB TRIM screen display
//************************************************************************************************************************************************************************
// this is the state machine, which will replace the do - while loop
void draw_sub_trim_screen() 
{
  static uint8_t is_next_page = 0;
  
  // call to first page, if required
  if (is_next_page == 0)
  {
    u8g2.firstPage();
    is_next_page = 1;
  } 

  // draw our screen
  sub_trim_screen();
  
  // call to next page
  if (u8g2.nextPage() == 0)
  {
    is_next_page = 0;      // ensure, that first page is called
  }  
}

void sub_trim_screen() {

  // Set memory buffer for text strings
  char menu_buffer[8];
  char chName_buffer[22];
  char char_buffer[21];

//  u8g2.firstPage(); do {

//  read_pots(); // Macro again for stable ppm pulse

    // Print "SUB TRIM"
    strcpy_P(menu_buffer, (char*)pgm_read_word(&(menu_name[4])));
    u8g2.setCursor(0, 7);
    u8g2.print(menu_buffer);

    // Drawing horizontal line under header
    u8g2.drawHLine(0, 8, 128);
    

    unsigned char temp_Counter = 0;

    // Print SUB TRIM channels list
    for (int i = 0; i < 2; i++) {

      // Print channel items name "STR, THR"
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[i])));
      u8g2.setCursor(22, 20 + i * 13);
      u8g2.print(chName_buffer);

      if (menuSubActual - 1 == temp_Counter) {
        if (subTrimSelection == temp_Counter) {

          // Print ">"
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[14])));
          u8g2.setCursor(10, 20 + i * 13);
          u8g2.print(char_buffer);

          // Print "["
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[8])));
          u8g2.setCursor(50, 20 + i * 13);
          u8g2.print(char_buffer);

          // Print "]"
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[9])));
          u8g2.setCursor(85, 20 + i * 13);
          u8g2.print(char_buffer);
        }
        else {

          // Print ">"
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[14])));
          u8g2.setCursor(10, 20 + i * 13);
          u8g2.print(char_buffer);
        }
      }

      // Print SUB TRIM value
      u8g2.setCursor(58, 20 + i * 13);
      u8g2.print(subTrim[i]);


      // Print "PPM"
      strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[17])));
      u8g2.setCursor(98, 20 + i * 13);
      u8g2.print(chName_buffer);

      temp_Counter++;
    }

//  } while (u8g2.nextPage());
}

//************************************************************************************************************************************************************************
// Drawing MODEL NAME screen display
//************************************************************************************************************************************************************************
// this is the state machine, which will replace the do - while loop
void draw_model_name_screen() 
{
  static uint8_t is_next_page = 0;
  
  // call to first page, if required
  if (is_next_page == 0)
  {
    u8g2.firstPage();
    is_next_page = 1;
  } 

  // draw our screen
  model_name_screen();
  
  // call to next page
  if (u8g2.nextPage() == 0)
  {
    is_next_page = 0;      // ensure, that first page is called
  }  
}

void model_name_screen() {

  // Set memory buffer for text strings
  char menu_buffer[8];
  char chName_buffer[6];
  char char_buffer[2];

//  u8g2.firstPage(); do {

    // Print "MODEL NAME"
    strcpy_P(menu_buffer, (char*)pgm_read_word(&(menu_name[5])));
    u8g2.setCursor(0, 7);
    u8g2.print(menu_buffer);
  
    // Print "MODEL"
    strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[18])));
    u8g2.setCursor(85, 7);
    u8g2.print(chName_buffer);

    // Print number of which model in use
    u8g2.setCursor(117, 7);
    u8g2.print(modelActual + 1);

    // Drawing horizontal line under header
    u8g2.drawHLine(0, 8, 128);
    

    u8g2.setFont(u8g2_font_VCR_OSD_tr);
    // Print MODEL NAME 5 byte
    for (int i = 0; i < 5; i++) {
      u8g2.setCursor(13 + (i * 23), 45);
      u8g2.print(modelName[i]);

      if (menuSubActual - 1 == i) {
        if (modelNameSelection == i) {

          // Print "["
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[8])));
          u8g2.setCursor(1 + (i * 23), 45);
          u8g2.print(char_buffer);

          // Print "]"
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[9])));
          u8g2.setCursor(25 + (i * 23), 45);
          u8g2.print(char_buffer);
        }
        else {

          // Print ">"
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[14])));
          u8g2.setCursor(1 + (i * 23), 45);
          u8g2.print(char_buffer);
        }
      }
    }
    
    u8g2.setFont(u8g2_font_6x10_tr);

//  } while (u8g2.nextPage());
}

//************************************************************************************************************************************************************************
// Drawing EXPO screen display
//************************************************************************************************************************************************************************
// this is the state machine, which will replace the do - while loop
void draw_expo_screen() 
{
  static uint8_t is_next_page = 0;
  
  // call to first page, if required
  if (is_next_page == 0)
  {
    u8g2.firstPage();
    is_next_page = 1;
  } 

  // draw our screen
  expo_screen();
  
  // call to next page
  if (u8g2.nextPage() == 0)
  {
    is_next_page = 0;      // ensure, that first page is called
  }  
}

void expo_screen() {

  // Set memory buffer for text strings
  char menu_buffer[8];
  char chName_buffer[22];
  char char_buffer[21];

//  u8g2.firstPage(); do {

//  read_pots(); // Macro again for stable ppm pulse

    // Print "EXPO"
    strcpy_P(menu_buffer, (char*)pgm_read_word(&(menu_name[6])));
    u8g2.setCursor(0, 7);
    u8g2.print(menu_buffer);

    // Drawing horizontal line under header
    u8g2.drawHLine(0, 8, 128);
    

    // Print EXPO channels list
    for (int i = 0; i < 2; i++) {

        // Print channel items name "STR, THR"
        strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[i])));
        u8g2.setCursor(7, 20 + i * 13);
        u8g2.print(chName_buffer);

        // Print EXPO value
        u8g2.setCursor(35, 20 + i * 13);
        u8g2.print(expo[i]);
        

      if (menuSubActual - 1 == i) {
        if (expoSelection == i) {

          // Print "["
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[8])));
          u8g2.setCursor(28, 20 + i * 13);
          u8g2.print(char_buffer);

          // Print "]"
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[9])));
          u8g2.setCursor(42, 20 + i * 13);
          u8g2.print(char_buffer);
        }
        else {

          // Print ">"
          strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[14])));
          u8g2.setCursor(0, 20 + i * 13);
          u8g2.print(char_buffer);
        }


        // Draw EXPO Graph
        u8g2.drawHLine(53, 36, 75);
        u8g2.drawVLine(92, 10, 54);

        u8g2.drawFrame(53, 10, 75, 54);
        u8g2.drawLine(52, 64, 128, 10);

        if (expo[i] > 0) {

          for (int j = 52; j <= 91; j++) {
            u8g2.drawPixel(j, map(calc_expo(servoCenter, map(j, 52, 91, servoMin, servoCenter), servoMin, expo[i]), servoMin, servoCenter, 64, 36));
          }

          for (int j = 91; j <= 128; j++) {
            u8g2.drawPixel(j, map(calc_expo(servoCenter, map(j, 91, 128, servoCenter, servoMax), servoMax, expo[i]), servoCenter, servoMax, 35, 9));
          }
        }
      }
    }

//  } while (u8g2.nextPage());
}
 
