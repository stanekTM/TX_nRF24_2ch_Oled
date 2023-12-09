
//*********************************************************************************************************************
// Drawing SAVE MODEL screen display
//*********************************************************************************************************************
void save_model_screen()
{
  // For eeprom position reference
  unsigned int eepromBase;
  
  // Define start position for eeprom write/update (32 * [0,1,2,3,4])
  eepromBase = NUM_BYTES_PER_MODEL * modelActual;
  
  // Save ACTUAL MODEL DATA
  EEPROM.update(ACTUAL_MODEL_EEPROM_ADDR, modelActual);
  
  // For write/update REVERSE and EPA position
  unsigned int eepromPos = eepromBase;
  
  // Save MODEL NAME 5 byte
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
  
  // Start of save data message screen
  u8g2.firstPage(); do {
    
    // Set memory buffer for text strings
    char msg_buffer[11];
    
    // Print "SAVE DATA"
    strcpy_P(msg_buffer, (char*)pgm_read_word(&(message[0])));
    u8g2.setCursor(28, 20);
    u8g2.print(msg_buffer);
    
    // Print MODEL NAME "MODEL"
    u8g2.setCursor(36, 50);
    u8g2.print(modelName);
    
    // Print number of which model in use
    u8g2.setCursor(77, 50);
    u8g2.print(modelActual + 1);
    
  } while (u8g2.nextPage());
  
  delay(5000); // Screen message for 1sec
  
  screen--;
  menuActual = 0;
  menuSubActual = 4;
}
 
