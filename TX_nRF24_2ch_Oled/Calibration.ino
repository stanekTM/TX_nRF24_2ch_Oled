
//*********************************************************************************************************************
// Calibration and screen
//*********************************************************************************************************************
void Calibration()
{
  while (calibStatus == 1)
  {
    // Reading MIN and MAX value for every channel
    unsigned int raw_pots;
    
    for (int ch = 0; ch < CHANNELS; ch++)
    {
      // Get value from every ADC ports
      raw_pots = analogRead(ch);
      
      // Get MIN values
      if (raw_pots < pot_calib_min[ch])
      {
        pot_calib_min[ch] = raw_pots;
      }
      
      // Get MAX values
      if (raw_pots > pot_calib_max[ch])
      {
        pot_calib_max[ch] = raw_pots;
      }
    }
    
    // Print calibration "MIN-MAX" real time channels
    calib_MinMax_screen();
    
    delay(10);
    
    // Set calibStatus = 0 to exit calibration procedure by pressing button UP
    if (read_button() == 1)
    {
      calibStatus = 0;
    }
  }
  calibStatus = 1;
  
  
  // button UP not pressed check
  while (read_button() != 0)
  {
    delay(10);
  }
  
  
  while (calibStatus == 1)
  {
    for (int ch = 0; ch < CHANNELS; ch++)
    {
      // Get value from every ADC ports
      pot_calib_mid[ch] = analogRead(ch);
    }
    
    // Print calibration "CENTER" real time channels
    calib_center_screen();
    
    delay(10);
    
    // Set calibStatus = 0 to exit calibration procedure by pressing button UP
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
  for (int ch = 0; ch < CHANNELS; ch++)
  {
    // Save MIN calibration values for channels
    posEeprom = 1000 + (ch * 4);
    EEPROMUpdateInt(posEeprom, pot_calib_min[ch]);
    
    // Save MAX calibration values for channels
    posEeprom += 2;
    EEPROMUpdateInt(posEeprom, pot_calib_max[ch]);
  }
  
  // Save CENTER calibration values from Eeprom
  for (int ch = 0; ch < CHANNELS; ch++)
  {
    // Save CENTER calibration values for channels
    posEeprom = 1016 + (ch * 2);
    EEPROMUpdateInt(posEeprom, pot_calib_mid[ch]);
  }
  
  delay(2000); // Screen message for 2sec
}

//*********************************************************************************************************************
// Print calibration "MIN-MAX" real time channels
//*********************************************************************************************************************
void calib_MinMax_screen()
{
  // Set memory buffer for text strings
  char msg_buffer[11];
  char word_buffer[13];
  char char_buffer[8];
  
  u8g2.firstPage(); do {
    
    read_pots(); // Macro again for stable pots value
    
    // Print "MIN-MAX"
    strcpy_P(msg_buffer, (char*)pgm_read_word(&(message[3])));
    u8g2.setCursor(0, 9);
    u8g2.print(msg_buffer);
    
    // Print "CALIBRATION"
    strcpy_P(msg_buffer, (char*)pgm_read_word(&(message[2])));
    u8g2.setCursor(52, 9);
    u8g2.print(msg_buffer);
    
    // Drawing horizontal line under header and vertical/middle line
    u8g2.drawHLine(0, 10, 128);
    u8g2.drawVLine(64, 12, 15);
    
    // Print "CH1" and value
    strcpy_P(word_buffer, (char*)pgm_read_word(&(word_name[0])));
    u8g2.setCursor(0, 22);
    u8g2.print(word_buffer);
    u8g2.setCursor(30, 22);
    u8g2.print(analogRead(0));
    
    // Print "CH2" and value
    strcpy_P(word_buffer, (char*)pgm_read_word(&(word_name[1])));
    u8g2.setCursor(71, 22);
    u8g2.print(word_buffer);
    u8g2.setCursor(101, 22);
    u8g2.print(analogRead(1));
    
    // Print "Move the Pots"
    strcpy_P(msg_buffer, (char*)pgm_read_word(&(message[8])));
    u8g2.setCursor(20, 44);
    u8g2.print(msg_buffer);
    
    // Print "SAVE DATA"
    strcpy_P(msg_buffer, (char*)pgm_read_word(&(message[0])));
    u8g2.setCursor(0, 64);
    u8g2.print(msg_buffer);
    
    // Print "="
    strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[10])));
    u8g2.setCursor(66, 64);
    u8g2.print(char_buffer);
    
    // Print "UP"
    strcpy_P(msg_buffer, (char*)pgm_read_word(&(message[9])));
    u8g2.setCursor(76, 64);
    u8g2.print(msg_buffer);
    
  } while (u8g2.nextPage());
}

//*********************************************************************************************************************
// Print calibration "CENTER" real time channels
//*********************************************************************************************************************
void calib_center_screen()
{
  // Set memory buffer for text strings
  char msg_buffer[11];
  char word_buffer[13];
  char char_buffer[8];
  
  u8g2.firstPage(); do {
    
    read_pots(); // Macro again for stable pots value
    
    // Print "CENTER"
    strcpy_P(msg_buffer, (char*)pgm_read_word(&(message[4])));
    u8g2.setCursor(0, 9);
    u8g2.print(msg_buffer);
    
    // Print "CALIBRATION"
    strcpy_P(msg_buffer, (char*)pgm_read_word(&(message[2])));
    u8g2.setCursor(52, 9);
    u8g2.print(msg_buffer);
    
    // Drawing horizontal line under header and vertical/middle line
    u8g2.drawHLine(0, 10, 128);
    u8g2.drawVLine(64, 12, 15);
    
    // Print "CH1" and value
    strcpy_P(word_buffer, (char*)pgm_read_word(&(word_name[0])));
    u8g2.setCursor(0, 22);
    u8g2.print(word_buffer);
    u8g2.setCursor(30, 22);
    u8g2.print(analogRead(0));
    
    // Print "CH2" and value
    strcpy_P(word_buffer, (char*)pgm_read_word(&(word_name[1])));
    u8g2.setCursor(71, 22);
    u8g2.print(word_buffer);
    u8g2.setCursor(101, 22);
    u8g2.print(analogRead(1));
    
    // Print "Don't"
    strcpy_P(msg_buffer, (char*)pgm_read_word(&(message[7])));
    u8g2.setCursor(0, 44);
    u8g2.print(msg_buffer);
    
    // Print "Move the Pots"
    strcpy_P(msg_buffer, (char*)pgm_read_word(&(message[8])));
    u8g2.setCursor(38, 44);
    u8g2.print(msg_buffer);
    
    // Print "SAVE DATA"
    strcpy_P(msg_buffer, (char*)pgm_read_word(&(message[0])));
    u8g2.setCursor(0, 64);
    u8g2.print(msg_buffer);
    
    // Print "="
    strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[10])));
    u8g2.setCursor(66, 64);
    u8g2.print(char_buffer);
    
    // Print "UP"
    strcpy_P(msg_buffer, (char*)pgm_read_word(&(message[9])));
    u8g2.setCursor(76, 64);
    u8g2.print(msg_buffer);
    
  } while (u8g2.nextPage());
}

//*********************************************************************************************************************
// Print calibration message "SAVE DATA"
//*********************************************************************************************************************
void calib_save_screen()
{
  // Set memory buffer for text strings
  char msg_buffer[11];
  
  u8g2.firstPage(); do {
    
    // Print "SAVE DATA"
    strcpy_P(msg_buffer, (char*)pgm_read_word(&(message[0])));
    u8g2.setCursor(25, 20);
    u8g2.print(msg_buffer);
    
    // Print "CALIBRATION"
    strcpy_P(msg_buffer, (char*)pgm_read_word(&(message[2])));
    u8g2.setCursor(25, 40);
    u8g2.print(msg_buffer);
    
  } while (u8g2.nextPage());
}
 
