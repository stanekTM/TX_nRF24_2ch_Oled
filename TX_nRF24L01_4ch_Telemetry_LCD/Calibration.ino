
//************************************************************************************************************************************************************************
// Calibration and screen ************************************************************************************************************************************************
//************************************************************************************************************************************************************************
void Calibration()
{
  // Setting default mid value reference for Min and Max calibration
  for (int i = 0; i < CHANNELS; i++)
  {
    calibration[i][0] = 512;
    calibration[i][1] = 512;
  }
  
  while (calibStatus == 1)
  {
    // Reading MIN and MAX value for every channel
    unsigned int raw_pots;
    
    for (int i = 0; i < CHANNELS; i++)
    {
      // Get value from every ADC ports
      raw_pots = read_adc(i);
      
      // Get MIN values
      if (raw_pots <= calibration[i][0])
      {
        calibration[i][0] = raw_pots;
      }

      // Get MAX values
      if (raw_pots >= calibration[i][1])
      {
        calibration[i][1] = raw_pots;
      }
    }
    
    // Print calibration "MIN-MAX" real time channels
    calib_MinMax_screen();
    
    delay(10);
    
    // Set calibStatus = 0 to exit calibration procedure by pressing buttonUp
    if (read_button() == 1)
    {
      calibStatus = 0;
    }
  }
  
  calibStatus = 1;
  
  // buttonUp not pressed check
  while (read_button() != 0)
  {
    delay(10);
  }
  
  // Setting default mid value reference for CENTER calibration
  // only for Throttle and Steering
  for (int i = 0; i < 2; i++)
  {
    centerPos[i] = 512;
  }
  
  while (calibStatus == 1)
  {
    for (int i = 0; i < 2; i++)
    {
      // Get value from every ADC ports
      centerPos[i] = read_adc(i);
    }
    
    // Print calibration "CENTER" real time channels
    calib_center_screen();
    
    delay(10);
    
    // Set calibStatus = 0 to exit calibration procedure by pressing buttonUp
    if (read_button() == 1)
    {
      calibStatus = 0;
    }
  }
  
  // Print calibration message "SAVE DATA"
  calib_save_screen();
  
  
  // Save MIN, MAX and CENTER values in Eeprom
  unsigned int posEeprom;
  
  // Save MIN and MAX calibration values from Eeprom
  for (int i = 0; i < CHANNELS; i++)
  {
    // Save MIN calibration values for channels
    posEeprom = 1000 + (i * 4);
    EEPROMUpdateInt(posEeprom, calibration[i][0]);

    // Save MAX calibration values for channels
    posEeprom += 2;
    EEPROMUpdateInt(posEeprom, calibration[i][1]);
  }
  
  // Save CENTER calibration values from Eeprom
  for (int i = 0; i < 2; i++)
  {
    // Save CENTER calibration values for channels
    posEeprom = 1016 + (i * 2);
    EEPROMUpdateInt(posEeprom, centerPos[i]);
  }
  
  delay(2000); // Screen message for 2sec
}

//************************************************************************************************************************************************************************
// Print calibration "MIN-MAX" real time channels
//************************************************************************************************************************************************************************
void calib_MinMax_screen()
{
  // Set memory buffer for text strings
  char msg_buffer[9];
  char chName_buffer[14];
  char char_buffer[10];

  u8g2.firstPage(); do {
  
  read_pots(); // Macro again for stable pots value
    
    // Print "MIN-MAX"
    strcpy_P(msg_buffer, (char*)pgm_read_word(&(messages[7])));
    u8g2.setCursor(7, 7);
    u8g2.print(msg_buffer);

    // Print "CALIBRATION"
    strcpy_P(msg_buffer, (char*)pgm_read_word(&(messages[2])));
    u8g2.setCursor(55, 7);
    u8g2.print(msg_buffer);

    // Drawing horizontal line under header and vertical/middle line
    u8g2.drawHLine(0, 8, 128);
    u8g2.drawVLine(64, 10, 30);

    // Print "STR" and value
    strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[0])));
    u8g2.setCursor(5, 20);
    u8g2.print(chName_buffer);
    u8g2.setCursor(30, 20);
    u8g2.print(read_adc(0));

    // Print "THR" and value
    strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[1])));
    u8g2.setCursor(70, 20);
    u8g2.print(chName_buffer);
    u8g2.setCursor(95, 20);
    u8g2.print(read_adc(1));

    // Print "CH3" and value
    strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[2])));
    u8g2.setCursor(5, 35);
    u8g2.print(chName_buffer);
    u8g2.setCursor(30, 35);
    u8g2.print(read_adc(2));

    // Print "CH4" and value
    strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[3])));
    u8g2.setCursor(70, 35);
    u8g2.print(chName_buffer);
    u8g2.setCursor(95, 35);
    u8g2.print(read_adc(3));

    // Print "Move the Pots"
    strcpy_P(msg_buffer, (char*)pgm_read_word(&(messages[4])));
    u8g2.setCursor(2, 54);
    u8g2.print(msg_buffer);

    // Print "SAVE DATA"
    strcpy_P(msg_buffer, (char*)pgm_read_word(&(messages[0])));
    u8g2.setCursor(2, 64);
    u8g2.print(msg_buffer);

    // Print "="
    strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[4])));
    u8g2.setCursor(60, 64);
    u8g2.print(char_buffer);

    // Print "UP"
    strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[4])));
    u8g2.setCursor(70, 64);
    u8g2.print(chName_buffer);
      
  } while (u8g2.nextPage());
}

//************************************************************************************************************************************************************************
// Print calibration "CENTER" real time channels
//************************************************************************************************************************************************************************
void calib_center_screen()
{
  // Set memory buffer for text strings
  char msg_buffer[9];
  char chName_buffer[14];
  char char_buffer[10];
  
  u8g2.firstPage(); do {
  
  read_pots(); // Macro again for stable pots value
    
    // Print "CENTER"
    strcpy_P(msg_buffer, (char*)pgm_read_word(&(messages[8])));
    u8g2.setCursor(10, 7);
    u8g2.print(msg_buffer);

    // Print "CALIBRATION"
    strcpy_P(msg_buffer, (char*)pgm_read_word(&(messages[2])));
    u8g2.setCursor(55, 7);
    u8g2.print(msg_buffer);

    // Drawing horizontal line under header and vertical/middle line
    u8g2.drawHLine(0, 8, 128);
    u8g2.drawVLine(64, 10, 30);

    // Print "STR" and value
    strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[0])));
    u8g2.setCursor(5, 20);
    u8g2.print(chName_buffer);
    u8g2.setCursor(30, 20);
    u8g2.print(read_adc(0));

    // Print "THR" and value
    strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[1])));
    u8g2.setCursor(70, 20);
    u8g2.print(chName_buffer);
    u8g2.setCursor(95, 20);
    u8g2.print(read_adc(1));

    // Print "Don't"
    strcpy_P(msg_buffer, (char*)pgm_read_word(&(messages[3])));
    u8g2.setCursor(2, 54);
    u8g2.print(msg_buffer);

    // Print "Move the Pots"
    strcpy_P(msg_buffer, (char*)pgm_read_word(&(messages[4])));
    u8g2.setCursor(37, 54);
    u8g2.print(msg_buffer);

    // Print "SAVE DATA"
    strcpy_P(msg_buffer, (char*)pgm_read_word(&(messages[0])));
    u8g2.setCursor(2, 64);
    u8g2.print(msg_buffer);

    // Print "="
    strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[4])));
    u8g2.setCursor(60, 64);
    u8g2.print(char_buffer);

    // Print "UP"
    strcpy_P(chName_buffer, (char*)pgm_read_word(&(channel_name[4])));
    u8g2.setCursor(70, 64);
    u8g2.print(chName_buffer);
      
  } while (u8g2.nextPage());
}

//************************************************************************************************************************************************************************
// Print calibration message "SAVE DATA"
//************************************************************************************************************************************************************************
void calib_save_screen()
{
  // Set memory buffer for text strings
  char msg_buffer[9];

  u8g2.firstPage(); do {
    
    // Print "SAVE DATA"
    strcpy_P(msg_buffer, (char*)pgm_read_word(&(messages[0])));
    u8g2.setCursor(30, 15);
    u8g2.print(msg_buffer);

    // Print "CALIBRATION"
    strcpy_P(msg_buffer, (char*)pgm_read_word(&(messages[2])));
    u8g2.setCursor(27, 30);
    u8g2.print(msg_buffer);

  } while (u8g2.nextPage());
}
 
