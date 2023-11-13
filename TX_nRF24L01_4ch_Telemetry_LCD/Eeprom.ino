
//*********************************************************************************************************************
// Macro for read selected model data from Eeprom
//*********************************************************************************************************************
unsigned char storedDataEeprom(unsigned char mod)
{
  // For Eeprom position reference
  unsigned int eepromBase;
  
  // Read which model to upload data
  if (mod == 255)
  {
    mod = EEPROM.read(ACTUAL_MODEL_EEPROM_ADDR);
  }
  
  // Define start position for Eeprom read (25 * [0,1,2,3,4])
  eepromBase = NUM_BYTES_PER_MODEL * mod;
  
  unsigned int eepromPos = eepromBase;
  
  // read MODEL NAME
  for (int i = 0; i < 5; i++)
  {
    modelName[i] = EEPROM.read(eepromPos++);
  }
  
  // Read SERVO DIRECTION from first position
  servoReverse = EEPROM.read(eepromPos++);
  
  // Read SUB TRIM offset values for selected model
  for (int i = 0; i < 2; i++)
  {
    subTrim[i] = EEPROMReadInt(eepromPos);
    eepromPos += 2;
  }
  
  // Read EPA values after first position
  for (int i = 0; i < 4; i++)
  {
    epa[i] = EEPROM.read(eepromPos++);
  }
  
  // Read EXPO values after first position
  for (int i = 0; i < 2; i++)
  {
    expo[i] = EEPROM.read(eepromPos++);
  } 
  
  // Read MIN, MAX, CENTER values in Eeprom
  unsigned int posEeprom;
  
  // Read MIN and MAX calibration values from Eeprom
  for (int i = 0; i < CHANNELS; i++)
  {
    // Read MIN calibration values for channels
    posEeprom = 1000 + (i * 4);
    pot_calib_min[i] = EEPROMReadInt(posEeprom);

    // Read MAX calibration values for channels
    posEeprom += 2;
    pot_calib_max[i] = EEPROMReadInt(posEeprom);
  }
  
  // Read CENTER calibration values from Eeprom
  for (int i = 0; i < 2; i++)
  {
    // Read CENTER calibration values for channels
    posEeprom = 1016 + (i * 2);
    pot_calib_mid[i] = EEPROMReadInt(posEeprom);
  }
  return mod;
}

//*********************************************************************************************************************
// Read and Update Eeprom data format
//*********************************************************************************************************************
/*
void EEPROMWriteInt(int p_address, int p_value)
{
  // Write a 16bit value in Eeprom
  byte Byte1 = ((p_value >> 0) & 0xFF);
  byte Byte2 = ((p_value >> 8) & 0xFF);

  EEPROM.write(p_address, Byte1);
  EEPROM.write(p_address + 1, Byte2);
}
*/

int EEPROMReadInt(int p_address)
{
  // Read a 16 bit value in Eeprom
  byte Byte1 = EEPROM.read(p_address);
  byte Byte2 = EEPROM.read(p_address + 1);
  
  long firstTwoBytes = ((Byte1 << 0) & 0xFF) + ((Byte2 << 8) & 0xFF00);
  
  return (firstTwoBytes);
}

// will update only changed value to preserve eeprom writing cycles
void EEPROMUpdateInt(int p_address, int p_value)
{
  // Write a 16bit value in Eeprom
  byte Byte1 = ((p_value >> 0) & 0xFF);
  byte Byte2 = ((p_value >> 8) & 0xFF);
  
  EEPROM.update(p_address, Byte1);
  EEPROM.update(p_address + 1, Byte2);
}

//*********************************************************************************************************************
// Hold button DOWN on power ON to initialize default memory data
// Will only write data settings to default (not erase calibration data)
// NOTE: SHOULD BE USED FOR THE FIRST TIME AFTER CALIBRATION !!!
//*********************************************************************************************************************
void resetEeprom_screen()
{
  if (digitalRead(PIN_BUTTON_EXIT) == LOW)
  {
    bool isWait = true;
    
    // Set memory buffer for text strings
    char name_buffer[13];
    char msg_buffer[11];
    char char_buffer[8];
    
    u8g2.firstPage(); do {
      
      // Print "ERASE DATA ?"
      strcpy_P(msg_buffer, (char*)pgm_read_word(&(message[5])));
      u8g2.setCursor(30, 20);
      u8g2.print(msg_buffer);
      
      // Print "Y"
      strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[0])));
      u8g2.setCursor(25, 40);
      u8g2.print(char_buffer);
      
      // Print "="
      strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[4])));
      u8g2.setCursor(32, 40);
      u8g2.print(char_buffer);
      
      // Print "DOWN"
      strcpy_P(name_buffer, (char*)pgm_read_word(&(channel_name[5])));
      u8g2.setCursor(39, 40);
      u8g2.print(name_buffer);
      
      // Print "N"
      strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[1])));
      u8g2.setCursor(75, 40);
      u8g2.print(char_buffer);
      
      // Print "="
      strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[4])));
      u8g2.setCursor(82, 40);
      u8g2.print(char_buffer);
      
      // Print "UP"
      strcpy_P(name_buffer, (char*)pgm_read_word(&(channel_name[4])));
      u8g2.setCursor(89, 40);
      u8g2.print(name_buffer);
      
    } while (u8g2.nextPage());
    
    while (isWait)
    {
      switch (read_button())
      {
        // button UP
        case 1:
        isWait = false;
        break;
        
        // button DOWN
        case 3:
        isWait = false;
        
        // Recall "Reset to default" macro
        resetEeprom();
        
        u8g2.firstPage(); do {
          
          // Print "ERASE DATA"
          strcpy_P(msg_buffer, (char*)pgm_read_word(&(message[5])));
          u8g2.setCursor(35, 20);
          u8g2.print(msg_buffer);
          
        } while (u8g2.nextPage());
        
        break;
      }
    }
  }
}

//*********************************************************************************************************************
// This macro will be used to clear eeprom and set default parameters if necessary
//*********************************************************************************************************************
void resetEeprom()
{
  // For write SERVO DIRECTION and EPA position
  unsigned int eepromPos;
  
  // Writing default model[0]
  EEPROM.update(ACTUAL_MODEL_EEPROM_ADDR, 0);
  
  // Start writing default values for every model memory bank
  for (int j = 0; j < MODELS; j++)
  {
    // Define start position for Eeprom storing (32 * [0,1,2,3,4...])
    eepromPos = NUM_BYTES_PER_MODEL * j;
    
    // MODEL NAME 5 byte
    for (int i = 0; i < 5; i++)
    {
      EEPROM.update(eepromPos++, 0x5f);
    }
    
    // Writing SERVO DIRECTION default value in first address of start position
    EEPROM.update(eepromPos++, 0x00);
    
    // Writing SUB TRIM offset values for two channels in every model memory bank
    for (int i = 0; i < 2; i++)
    {
      // Writing SUB TRIM stick values for every channels
      EEPROMUpdateInt(eepromPos, 0);
      eepromPos += 2;
    }
    
    // Writing EPA values for every channels in every model memory bank
    // Writing values will start after first address of start position
    for (int i = 0; i < 4; i++)
    {
      // Writing default EPA for channels
      EEPROM.update(eepromPos++, 100);
    }
    
    // Writing EXPO values for every channels in every model memory bank
    // Writing values will start after first address of start position
    for (int i = 0; i < 2; i++)
    {
      // Writing default EXPO for channels
      EEPROM.update(eepromPos++, 0);
    }
  }
}
 
