
//*********************************************************************************************************************
// Drawing SAVE MODEL screen display
//*********************************************************************************************************************
void save_model_screen()
{
  // For Eeprom position reference
  unsigned int eepromBase;
  
  // Define start position for Eeprom write/update (32 * [0,1,2,3,4])
  eepromBase = NUM_BYTES_PER_MODEL * modelActual;
  
  // Save Actual Model data
  EEPROM.update(ACTUAL_MODEL_EEPROM_ADDR, modelActual);
  
  // For write/Update REVERSE and EPA position
  unsigned int eepromPos = eepromBase;
  
  // Save MODEL NAME
  for (int i = 0; i < 5; i++)
  {
    EEPROM.update(eepromPos++, modelName[i]);
  }
  
  // Save REVERSE data
  EEPROM.update(eepromPos++, reverse);
  
  // Save SUB TRIM center stick values for two channels in every model memory bank
  for (int i = 0; i < CHANNELS; i++)
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
  for (int i = 0; i < CHANNELS; i++)
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
 
