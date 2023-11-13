
//*********************************************************************************************************************
// Drawing BOOT screen display
//*********************************************************************************************************************
void boot_screen() {
  
  u8g2.firstPage(); do {
    
    // Print version string
    u8g2.setCursor(4, 28);
    u8g2.print(ver_str);

  } while (u8g2.nextPage());
  
  delay(1000);
}

//*********************************************************************************************************************
// Drawing MAIN screen display
//*********************************************************************************************************************
// this is the state machine, which will replace the do - while loop
void draw_main_screen()
{
  static uint8_t is_next_page = 0;
  
  // call to first page, if required
  if (is_next_page == 0)
  {
    u8g2.firstPage();
    is_next_page = 1;
  }
  
  // draw our screen
  main_screen();
  
  // call to next page
  if (u8g2.nextPage() == 0)
  {
    is_next_page = 0; // ensure, that first page is called 
  }  
}

//------------------------------------------------------------------------
void main_screen()
{
  // Set memory buffer for text strings
  char name_buffer[13];
  char char_buffer[8];
  char msg_buffer[11];
  char menu_buffer[7];
  
  //u8g2.firstPage(); do {
  
  read_pots(); // Macro again for stable pots value
  
  u8g2.setFont(u8g2_font_6x10_tr);
  
  
  // Print "MODEL" ************************************************
  strcpy_P(name_buffer, (char*)pgm_read_word(&(channel_name[11])));
  u8g2.setCursor(0, 22);
  u8g2.print(name_buffer);
  
  // Print number of which model in use
  u8g2.setCursor(32, 22);
  u8g2.print(modelActual + 1);
  
  // Print "MODEL NAME"
  u8g2.drawStr(48, 22, modelName);
  
  
  // Print "TX" *********************************************
  strcpy_P(msg_buffer, (char*)pgm_read_word(&(message[10])));
  u8g2.setCursor(0, 7);
  u8g2.print(msg_buffer);
  
  if (tx_low_batt)
  {
    // Print "low!"
    strcpy_P(msg_buffer, (char*)pgm_read_word(&(message[6])));
    u8g2.setCursor(14, 7);
    u8g2.print(msg_buffer);
  }
  else
  {
    // Print TX battery voltage
    u8g2.setCursor(14, 7);
    u8g2.print(tx_batt_volt);
    
    // Print "V"
    strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[7])));
    u8g2.setCursor(39, 7);
    u8g2.print(char_buffer);
  }
  
  
  // Print "RX" ********************************************
  strcpy_P(msg_buffer, (char*)pgm_read_word(&(message[9])));
  u8g2.setCursor(84, 7);
  u8g2.print(msg_buffer);
  
  if (rf_state)
  {
    if (rx_low_batt)
    {
      // Print "low!"
     strcpy_P(msg_buffer, (char*)pgm_read_word(&(message[6])));
     u8g2.setCursor(98, 7);
     u8g2.print(msg_buffer);
    }
    else
    {
    // Print RX battery voltage
    u8g2.setCursor(98, 7);
    u8g2.print(rx_batt_volt);
    
    // Print "V"
    strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[7])));
    u8g2.setCursor(123, 7);
    u8g2.print(char_buffer);
    }
  }
  else
  {
    // Print "off!"
    strcpy_P(msg_buffer, (char*)pgm_read_word(&(message[1])));
    u8g2.setCursor(98, 7);
    u8g2.print(msg_buffer);
  }
  rf_state = 0;
  
  
  
  // Drawing only first 2 channels ***************************************
  u8g2.setFont(u8g2_font_5x7_tr);
  
  for (int i = 0; i < 2; i++)
  {
    // Draw boxes/frames for every channel
    u8g2.drawFrame(13, 40 + (i * 16), 102, 8); //13, 40 + (i * 16), 102, 8

    // Drawing vertical middle/center separation line
    u8g2.drawVLine(64, 38 + (i * 16), 9); //64, 38 + (i * 16), 9
    
    // Define value bar reference
    unsigned int valBar;
    
    // Value bars subdivision (pots value / 50)
    valBar = map(pots_value[i], MIN_CONTROL_VAL, MAX_CONTROL_VAL, 0, 100);
    
    // Drawing cursor in every channel bar
    if (valBar < 50)
    {
      u8g2.drawBox(14 + valBar, 41 + (i * 16), 50 - valBar, 6); //64 - 50 = 14
    }
    else if (valBar > 50)
    {
      u8g2.drawBox(65, 41 + (i * 16), valBar - 50, 6);          //64 + 1 = 65
    }
    
    
    unsigned short subTrimVal;
    
    subTrimVal = map(subTrim[i], 0, 500, 0, 50);
    
    // Check Servo Reversing and applying Reverse value if necessary
    if (bitRead(servoReverse, i) == 1)
    {
      subTrimVal = -subTrimVal;
    }
    
    
    // EPA
    unsigned short minMaxValid = 0;
    short epa_1 = epa[i];
    short epa_2 = epa[i];
    //Serial.println(epa[i]);
    if (i == 1)
    {
      if (bitRead(servoReverse, i) == 1)
      {
        epa_2 = epa[2]; //2*
      }
      else
      {
        epa_1 = epa[2]; //2*
      }
    }
    
    //64 - 1 = 63
    minMaxValid = 63 - (50 * (epa_1 / 100)) + subTrimVal; //113 - 63 = 50
    
    if (minMaxValid < 13)
    {
      minMaxValid = 13; //63 - 50 = 13
    }

    if (minMaxValid > 113)
    {
      minMaxValid = 113; //100 + 13 = 113
      u8g2.drawVLine(minMaxValid, 41 + (i * 16), 6);
    }
    
    
    minMaxValid = 64 + (50 * (epa_2 / 100)) + subTrimVal; //114 - 64 = 50
    
    if (minMaxValid < 14)
    {
      minMaxValid = 14; //64 - 50 = 14
    }
    
    if (minMaxValid > 114)
    {
      minMaxValid = 114; //100 + 14 = 114
      u8g2.drawVLine(minMaxValid, 41 + (i * 16), 6);
    }
    
    
    // Print channel items name "CH1 and CH2"
    strcpy_P(name_buffer, (char*)pgm_read_word(&(channel_name[i])));
    u8g2.setCursor(0, 39 + (i * 16));
    u8g2.print(name_buffer);
    
    
    if (subTrim[i] > 0)
    {
      // Print SUB TRIM value
      u8g2.setCursor(66, 39 + (i * 16));
      u8g2.print(subTrim[i]);
    }
    else if (subTrim[i] < 0)
    {
      // Print SUB TRIM value
      u8g2.setCursor(66, 39 + (i * 16));
      u8g2.print(subTrim[i]);
    }
    
    
    // Drawing REV channel status for every channel
    if (bitRead(servoReverse, i) == 1)
    {
      // Print "REV"
      strcpy_P(name_buffer, (char*)pgm_read_word(&(channel_name[9])));
      u8g2.setCursor(23, 39 + (i * 16));
      u8g2.print(name_buffer);
    }
    
    
    // Drawing EXPO
    if (expo[i] > 0)
    {
      // Print "EXPO"
      strcpy_P(menu_buffer, (char*)pgm_read_word(&(menu_name[6])));
      u8g2.setCursor(102, 39 + (i * 16));
      u8g2.print(menu_buffer);
      
      // Print EXPO value
      u8g2.setCursor(124, 39 + (i * 16));
      u8g2.print(expo[i]);
    }
  }
  // End drawing only first 2 channels
  
  
  // Drawing CH3 and CH4    
  /*for (int i = 2; i < CHANNELS; i++)
  {
    // Print "CH3 and CH4"
    strcpy_P(name_buffer, (char*)pgm_read_word(&(channel_name[i])));
    u8g2.setCursor(81, 3 + (i * 8));
    u8g2.print(name_buffer);
    
    // Print CH3 and CH4 value in %
    unsigned int value[i];
    
    value[i] = map(pots_value[i], MIN_CONTROL_VAL, MAX_CONTROL_VAL, 0, 100);
    u8g2.setCursor(98, 3 + (i * 8));
    u8g2.print(value[i]);
    
    // Drawing REV channel status for every channel
    if (bitRead(servoReverse, i) == 1)
    {
      // Print "REV"
      strcpy_P(name_buffer, (char*)pgm_read_word(&(channel_name[9])));
      u8g2.setCursor(114, 3 + (i * 8));
      u8g2.print(name_buffer);
    } 
  }*/
  // End drawing CH3 and CH4
  
  //} while (u8g2.nextPage());
  
  u8g2.setFont(u8g2_font_6x10_tr); // set font menu, submenu
}

//*********************************************************************************************************************
// Drawing MENU screen display
//*********************************************************************************************************************
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
  
  // draw our screen
  menu_screen();
  
  // call to next page
  if (u8g2.nextPage() == 0)
  {
    is_next_page = 0; // ensure, that first page is called
  }  
}

//------------------------------------------------------------------------
void menu_screen()
{

  // Set memory buffer for text strings
  //char name_buffer[13];
  char menu_buffer[7];
  char char_buffer[8];
  
  //u8g2.firstPage(); do {
  
  read_pots(); // Macro again for stable pots value

/*
  // Print "MODEL"
  strcpy_P(name_buffer, (char*)pgm_read_word(&(channel_name[11])));
  u8g2.setCursor(0, 7);
  u8g2.print(name_buffer);

  // Print number of which model in use
  u8g2.setCursor(32, 7);
  u8g2.print(modelActual + 1);

  // Print MODEL NAME
  u8g2.drawStr(48, 7, modelName);
*/

  // Print "/"
  strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[6])));
  u8g2.setCursor(117, 7);
  u8g2.print(char_buffer);

  u8g2.setCursor(111, 7);
  u8g2.print(menuPage + 1);

  u8g2.setCursor(123, 7);
  u8g2.print((MENU_COUNT - 1) / 5 + 1); // Total Menu Count / menu count per page + 1

  // Drawing horizontal line under header
  u8g2.drawHLine(0, 8, 128);
  
  for (int i = 1; i < 6; i++)
  {
    if (i + (5 * menuPage) > MENU_COUNT)
    {
      break;
    }

    // Print main menu items "SERVO DIRECTION, EPA, MODEL SELECTION, SAVE MODEL DATA, SUB TRIM, MODEL NAME, EXPO"
    strcpy_P(menu_buffer, (char*)pgm_read_word(&(menu_name[i + (5 * menuPage) - 1])));
    
    if (i + (5 * menuPage) == menuSubActual)
    {
      // Print ">"
      strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[5])));
      u8g2.setCursor(5, 9 + (i * 10));
      u8g2.print(char_buffer);

      // Print main menu items
      u8g2.setCursor(14, 9 + (i * 10));
      u8g2.print(menu_buffer);
    }
    else
    {
      // Print main menu items
      u8g2.setCursor(20, 9 + (i * 10));
      u8g2.print(menu_buffer);
    }
  }
  
  //} while (u8g2.nextPage());
}

//*********************************************************************************************************************
// Drawing SERVO DIRECTION screen display
//*********************************************************************************************************************
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
    is_next_page = 0; // ensure, that first page is called
  }
}

//------------------------------------------------------------------------
void servo_dir_screen()
{
  // Set memory buffer for text strings
  char menu_buffer[7];
  char name_buffer[13];
  char char_buffer[8];
  
  //u8g2.firstPage(); do {
    
  read_pots(); // Macro again for stable pots value
  
  
  // Print "SERVO DIRECTION"
  strcpy_P(menu_buffer, (char*)pgm_read_word(&(menu_name[0])));
  u8g2.setCursor(0, 7);
  u8g2.print(menu_buffer);
  
  // Drawing horizontal line under header
  u8g2.drawHLine(0, 8, 128);
  
  // Drawing only first 4 channels
  for (int i = 0; i < CHANNELS; i++)
  {
    if (i == menuSubActual - 1)
    {
      // Print "["
      strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[2])));
      u8g2.setCursor(5, 20 + i * 13);
      u8g2.print(char_buffer);

      // Print "="
      strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[4])));
      u8g2.setCursor(31, 20 + i * 13);
      u8g2.print(char_buffer);

      // Print "]"
      strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[3])));
      u8g2.setCursor(57, 20 + i * 13);
      u8g2.print(char_buffer);
    }
    
    
    if (bitRead(servoReverse, i) == 1)
    {
      // Print "REV"
      strcpy_P(name_buffer, (char*)pgm_read_word(&(channel_name[9])));
      u8g2.setCursor(38, 20 + i * 13);
      u8g2.print(name_buffer);
    }
    else
    {
      // Print "NOR"
      strcpy_P(name_buffer, (char*)pgm_read_word(&(channel_name[8])));
      u8g2.setCursor(38, 20 + i * 13);
      u8g2.print(name_buffer);
    }
    
    
    // Print channel items name "CH1, CH2, CH3, CH4"
    strcpy_P(name_buffer, (char*)pgm_read_word(&(channel_name[i])));
    u8g2.setCursor(12, 20 + i * 13);
    u8g2.print(name_buffer);


    // Drawing dynamic graphics items
    u8g2.drawHLine(72, 20 + i * 13, 45);
    u8g2.drawVLine(94, 20 + i * 13 - 4, 4);
    u8g2.drawBox(map(pots_value[i], MIN_CONTROL_VAL, MAX_CONTROL_VAL, 74, 114) - 1, 18 + (i * 13), 3, 2);
  }
  // End drawing only first 4 channels
  
  //} while (u8g2.nextPage());
}

//*********************************************************************************************************************
// Drawing EPA screen display
//*********************************************************************************************************************
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
    is_next_page = 0; // ensure, that first page is called
  }
}

//------------------------------------------------------------------------
void epa_screen()
{
  // Set memory buffer for text strings
  char menu_buffer[7];
  char name_buffer[13];
  char char_buffer[8];
  
  //u8g2.firstPage(); do {
  
  read_pots(); // Macro again for stable pots value
  
  
  // Print "EPA"
  strcpy_P(menu_buffer, (char*)pgm_read_word(&(menu_name[1])));
  u8g2.setCursor(0, 7);
  u8g2.print(menu_buffer);
  
  // Drawing horizontal line under header
  u8g2.drawHLine(0, 8, 128);
  
  unsigned char counterTemp = 0;
  
  // Print EPA channels list
  for (int i = 0; i < 4; i++)
  {
    // Print channel items name "CH1, CH2"
    if (i > 0) //1*
    {
      // Print "CH2"
      strcpy_P(name_buffer, (char*)pgm_read_word(&(channel_name[1])));
    }
    else
    {
      strcpy_P(name_buffer, (char*)pgm_read_word(&(channel_name[i])));
    }
    
    u8g2.setCursor(10, 20 + i * 13);
    u8g2.print(name_buffer);
    
    if (i == 1)
    {
      // Print "FWD"
      strcpy_P(name_buffer, (char*)pgm_read_word(&(channel_name[12])));
      u8g2.setCursor(32, 20 + i * 13);
      u8g2.print(name_buffer);
    }
    
    if (i == 2)
    {
      // Print "BWD"
      strcpy_P(name_buffer, (char*)pgm_read_word(&(channel_name[6])));
      u8g2.setCursor(32, 20 + i * 13);
      u8g2.print(name_buffer);
    }
    
    if (menuSubActual - 1 == counterTemp)
    {
      if (epaSelection == counterTemp)
      {
        // Print ">"
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[5])));
        u8g2.setCursor(2, 20 + i * 13);
        u8g2.print(char_buffer);
        
        // Print "["
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[2])));
        u8g2.setCursor(59, 20 + i * 13);
        u8g2.print(char_buffer);
        
        // Print "]"
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[3])));
        u8g2.setCursor(85, 20 + i * 13);
        u8g2.print(char_buffer);
      }
      else
      {
        // Print ">"
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[5])));
        u8g2.setCursor(2, 20 + i * 13);
        u8g2.print(char_buffer);
      }
    }
    
    // EPA value
    u8g2.setCursor(66, 20 + i * 13);
    u8g2.print(epa[i]);
    
    counterTemp++;
  }
  
  //} while (u8g2.nextPage());
}

//*********************************************************************************************************************
// Drawing MODEL SELECTION screen display
//*********************************************************************************************************************
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
    is_next_page = 0; // ensure, that first page is called
  }  
}

//------------------------------------------------------------------------
void model_sel_screen()
{
  // Set memory buffer for text strings
  char char_buffer[8];
  char name_buffer[13];

  // For Eeprom position reference
  unsigned int eepromPos;
  int tempModelNoIdx;
  
  //u8g2.firstPage(); do {
  
  // Print "MODEL"
  strcpy_P(name_buffer, (char*)pgm_read_word(&(channel_name[11])));
  u8g2.setCursor(0, 7);
  u8g2.print(name_buffer);

  // Print number of which model in use
  u8g2.setCursor(32, 7);
  u8g2.print(modelActual + 1);

  // Print "/"
  strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[6])));
  u8g2.setCursor(117, 7);
  u8g2.print(char_buffer);

  u8g2.setCursor(111, 7);
  u8g2.print(modelPage + 1);

  u8g2.setCursor(123, 7);
  u8g2.print((MODELS - 1) / 10 + 1);  // Total model Count / model count per page + 1

  // Drawing horizontal line under header
  u8g2.drawHLine(0, 8, 128);

    
  // Print MODEL SELECTION list
  for (int i = 0; i < 5; i++)
  {
    // Left Section Start
    tempModelNoIdx = i + (10 * modelPage);
    
    if (tempModelNoIdx > MODELS)
    break;
    
    u8g2.setCursor(8, 19 + i * 10);
    u8g2.print(tempModelNoIdx + 1);

    // Define start position for Eeprom write/update (32 * [0,1,2,3,4])
    eepromPos = NUM_BYTES_PER_MODEL * tempModelNoIdx;
    
    for (int j = 0; j < 5; j++)
    {
      u8g2.setCursor(23 + (j * 6), 19 + i * 10);
      char ch = EEPROM.read(eepromPos++);
      u8g2.print(ch);
    }
    
    if (tempModelNoIdx == menuSubModel)
    {
      // Print "["
      strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[2])));
      u8g2.setCursor(0, 19 + i * 10);
      u8g2.print(char_buffer);
        
      // Print "]"
      strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[3])));
      u8g2.setCursor(55, 19 + i * 10);
      u8g2.print(char_buffer);
    }
    // Left Section End
    
    
    // Right Section Start
    tempModelNoIdx = (i + 5) + (10 * modelPage);
    
    if (tempModelNoIdx > MODELS)
    break;
    
    u8g2.setCursor(75, 19 + i * 10);
    u8g2.print(tempModelNoIdx + 1);

    // Define start position for Eeprom write/update (25 * [0,1,2,3,4])
    eepromPos = NUM_BYTES_PER_MODEL * tempModelNoIdx;
    
    for (int j = 0; j < 5; j++)
    {
      u8g2.setCursor(90 + (j * 6), 19 + i * 10);
      char ch = EEPROM.read(eepromPos++);
      u8g2.print(ch);
    }
    
    if (tempModelNoIdx == menuSubModel)
    {
      // Print "["
      strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[2])));
      u8g2.setCursor(67, 19 + i * 10);
      u8g2.print(char_buffer);

      // Print "]"
      strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[3])));
      u8g2.setCursor(122, 19 + i * 10);
      u8g2.print(char_buffer);
    }
    // Right Section End
  }
  
  //} while (u8g2.nextPage());
}

//*********************************************************************************************************************
// Drawing SAVE MODEL DATA screen display
//*********************************************************************************************************************
void save_model_screen()
{
  // For Eeprom position reference
  unsigned int eepromBase;

  // Define start position for Eeprom write/update (32 * [0,1,2,3,4])
  eepromBase = NUM_BYTES_PER_MODEL * modelActual;

  // Save Actual Model data
  EEPROM.update(ACTUAL_MODEL_EEPROM_ADDR, modelActual);

  // For write/Update SERVO DIRECTION and EPA position
  unsigned int eepromPos = eepromBase;

  // Save MODEL NAME
  for (int i = 0; i < 5; i++)
  {
    EEPROM.update(eepromPos++, modelName[i]);
  }

  // Save SERVO DIRECTION data
  EEPROM.update(eepromPos++, servoReverse);

  // Save SUB TRIM center stick values for two channels in every model memory bank
  for (int i = 0; i < 2; i++)
  {
    // Save CENTER stick values
    EEPROMUpdateInt(eepromPos, subTrim[i]);
    eepromPos += 2;
  }

  // Save EPA data
  for (int i = 0; i < 4; i++)
  {
    EEPROM.update(eepromPos, epa[i]);
    eepromPos++;
  }
  
  // Save EXPO data
  for (int i = 0; i < 2; i++)
  {
    EEPROM.update(eepromPos, expo[i]);
    eepromPos++;
  }

  // Start of Save Data message screen
  u8g2.firstPage(); do {

    // Set memory buffer for text strings
    char msg_buffer[11];
    char name_buffer[13];

    // Print "SAVE DATA"
    strcpy_P(msg_buffer, (char*)pgm_read_word(&(message[0])));
    u8g2.setCursor(40, 12);
    u8g2.print(msg_buffer);

    // Print "MODEL"
    strcpy_P(name_buffer, (char*)pgm_read_word(&(channel_name[11])));
    u8g2.setCursor(43, 35);
    u8g2.print(name_buffer);

    u8g2.setCursor(78, 35);
    u8g2.print(modelActual + 1);

    // Print MODEL NAME 5 byte
    for (int i = 0; i < 5; i++)
    {
      u8g2.setCursor(47 + i * 7, 55);
      u8g2.print(modelName[i]);
    }
    
  } while (u8g2.nextPage());
  
  delay(1000); // Screen message for 1sec
  
  screen--;
  menuActual = 0;
  menuSubActual = 4;
}

//*********************************************************************************************************************
// Drawing SUB TRIM screen display
//*********************************************************************************************************************
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
    is_next_page = 0; // ensure, that first page is called
  }  
}

//------------------------------------------------------------------------
void sub_trim_screen()
{
  // Set memory buffer for text strings
  char menu_buffer[7];
  char name_buffer[13];
  char char_buffer[8];
  
  //u8g2.firstPage(); do {
  
  read_pots(); // Macro again for stable pots value
  
  
  // Print "SUB TRIM"
  strcpy_P(menu_buffer, (char*)pgm_read_word(&(menu_name[4])));
  u8g2.setCursor(0, 7);
  u8g2.print(menu_buffer);

  // Drawing horizontal line under header
  u8g2.drawHLine(0, 8, 128);
    

  unsigned char temp_Counter = 0;

  // Print SUB TRIM channels list
  for (int i = 0; i < 2; i++)
  {
    // Print channel items name "CH1, CH2"
    strcpy_P(name_buffer, (char*)pgm_read_word(&(channel_name[i])));
    u8g2.setCursor(10, 20 + i * 13);
    u8g2.print(name_buffer);
    
    if (menuSubActual - 1 == temp_Counter)
    {
      if (subTrimSelection == temp_Counter)
      {
        // Print ">"
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[5])));
        u8g2.setCursor(2, 20 + i * 13);
        u8g2.print(char_buffer);

        // Print "["
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[2])));
        u8g2.setCursor(50, 20 + i * 13);
        u8g2.print(char_buffer);

        // Print "]"
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[3])));
        u8g2.setCursor(85, 20 + i * 13);
        u8g2.print(char_buffer);
      }
      else
      {
        // Print ">"
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[5])));
        u8g2.setCursor(2, 20 + i * 13);
        u8g2.print(char_buffer);
      }
    }
    
    // Print SUB TRIM value
    u8g2.setCursor(58, 20 + i * 13);
    u8g2.print(subTrim[i]);


    // Print "PPM"
    strcpy_P(name_buffer, (char*)pgm_read_word(&(channel_name[10])));
    u8g2.setCursor(98, 20 + i * 13);
    u8g2.print(name_buffer);

    temp_Counter++;
  }
  
  //} while (u8g2.nextPage());
}

//*********************************************************************************************************************
// Drawing MODEL NAME screen display
//*********************************************************************************************************************
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
    is_next_page = 0; // ensure, that first page is called
  }  
}

//------------------------------------------------------------------------
void model_name_screen()
{
  // Set memory buffer for text strings
  char menu_buffer[7];
  char name_buffer[13];
  char char_buffer[8];
  
  //u8g2.firstPage(); do {
  
  
  // Print "MODEL NAME"
  strcpy_P(menu_buffer, (char*)pgm_read_word(&(menu_name[5])));
  u8g2.setCursor(0, 7);
  u8g2.print(menu_buffer);
  
  // Print "MODEL"
  strcpy_P(name_buffer, (char*)pgm_read_word(&(channel_name[11])));
  u8g2.setCursor(85, 7);
  u8g2.print(name_buffer);

  // Print number of which model in use
  u8g2.setCursor(117, 7);
  u8g2.print(modelActual + 1);

  // Drawing horizontal line under header
  u8g2.drawHLine(0, 8, 128);
    

  u8g2.setFont(u8g2_font_VCR_OSD_tr);
  
  // Print MODEL NAME 5 byte
  for (int i = 0; i < 5; i++)
  {
    u8g2.setCursor(13 + (i * 23), 45);
    u8g2.print(modelName[i]);
    
    if (i == menuSubActual - 1)
    {
      if (modelNameSelection == i)
      {
        // Print "["
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[2])));
        u8g2.setCursor(1 + (i * 23), 45);
        u8g2.print(char_buffer);

        // Print "]"
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[3])));
        u8g2.setCursor(25 + (i * 23), 45);
        u8g2.print(char_buffer);
      }
      else
      {
        // Print ">"
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[5])));
        u8g2.setCursor(1 + (i * 23), 45);
        u8g2.print(char_buffer);
      }
    }
  }
  
  u8g2.setFont(u8g2_font_6x10_tr);
  
  //} while (u8g2.nextPage());
}

//*********************************************************************************************************************
// Drawing EXPO screen display
//*********************************************************************************************************************
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
    is_next_page = 0; // ensure, that first page is called
  }  
}

//------------------------------------------------------------------------
void expo_screen()
{
  // Set memory buffer for text strings
  char menu_buffer[7];
  char name_buffer[13];
  char char_buffer[8];
  
  //u8g2.firstPage(); do {
  
  read_pots(); // Macro again for stable pots value
  
  
  // Print "EXPO"
  strcpy_P(menu_buffer, (char*)pgm_read_word(&(menu_name[6])));
  u8g2.setCursor(0, 7);
  u8g2.print(menu_buffer);

  // Drawing horizontal line under header
  u8g2.drawHLine(0, 8, 128);
  
  // Print EXPO channels list
  for (int i = 0; i < 2; i++)
  {
    // Print channel items name "CH1, CH2"
    strcpy_P(name_buffer, (char*)pgm_read_word(&(channel_name[i])));
    u8g2.setCursor(10, 20 + i * 13);
    u8g2.print(name_buffer);

    // Print EXPO value
    u8g2.setCursor(37, 20 + i * 13);
    u8g2.print(expo[i]);
    
    
    if (i == menuSubActual - 1)
    {
      if (expoSelection == i)
      {
        // Print ">"
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[5])));
        u8g2.setCursor(2, 20 + i * 13);
        u8g2.print(char_buffer);

        // Print "["
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[2])));
        u8g2.setCursor(30, 20 + i * 13);
        u8g2.print(char_buffer);

        // Print "]"
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[3])));
        u8g2.setCursor(44, 20 + i * 13);
        u8g2.print(char_buffer);
      }
      else
      {
        // Print ">"
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[5])));
        u8g2.setCursor(2, 20 + i * 13);
        u8g2.print(char_buffer);
      }
      
      
      // Draw EXPO Graph
      u8g2.drawHLine(53, 36, 75);
      u8g2.drawVLine(92, 10, 54);

      u8g2.drawFrame(53, 10, 75, 54);
      u8g2.drawLine(52, 64, 128, 10);
      
      if (expo[i] > 0)
      {
        for (int j = 52; j <= 91; j++)
        {
          u8g2.drawPixel(j, map(calc_expo(MID_CONTROL_VAL,
          map(j, 52, 91, MIN_CONTROL_VAL, MID_CONTROL_VAL), MIN_CONTROL_VAL, expo[i]), MIN_CONTROL_VAL, MID_CONTROL_VAL, 64, 36));
        }
        
        for (int j = 91; j <= 128; j++)
        {
          u8g2.drawPixel(j, map(calc_expo(MID_CONTROL_VAL,
          map(j, 91, 128, MID_CONTROL_VAL, MAX_CONTROL_VAL), MAX_CONTROL_VAL, expo[i]), MID_CONTROL_VAL, MAX_CONTROL_VAL, 35, 9));
        }
      }
    }
  }
  
  //} while (u8g2.nextPage());
}
 
