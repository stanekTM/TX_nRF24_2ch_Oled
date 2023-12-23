
//*********************************************************************************************************************
// Drawing MODEL SELECT screen display
//*********************************************************************************************************************
// this is the state machine, which will replace the do - while loop
void draw_model_select_screen()
{
  static uint8_t is_next_page = 0;
  
  // call to first page, if required
  if (is_next_page == 0)
  {
    u8g2.firstPage();
    is_next_page = 1;
  }
  
  // draw our screen
  model_select_screen();
  
  // call to next page
  if (u8g2.nextPage() == 0)
  {
    is_next_page = 0; // ensure, that first page is called
  }
}

//------------------------------------------------------------------------
void model_select_screen()
{
  // Set memory buffer for text strings
  char char_buffer[9];
  
  // Print MODEL NAME "MODEL"
  u8g2.setCursor(0, 9);
  u8g2.print(modelName);
  
  // Print number of which model in use
  u8g2.setCursor(40, 9);
  u8g2.print(modelActual + 1);
  
  /*
  // Print "1 or 2"
  u8g2.setCursor(107, 9);
  u8g2.print(modelPage + 1);
  
  // Print "/"
  strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[6])));
  u8g2.setCursor(114, 9);
  u8g2.print(char_buffer);
  
  // Total model count/model count per page + 1
  u8g2.setCursor(122, 9);
  u8g2.print((MODELS - 1) / MODELS + 1);
  */
  
  // Drawing horizontal line under header
  u8g2.drawHLine(0, 10, 128);
  
  
  // For Eeprom position reference
  int tempModelNoIdx;
  unsigned int eepromPos;
  
  // Print MODEL SELECT list
  for (int i = 0; i < 5; i++)
  {
    // Left Section Start
    tempModelNoIdx = i + (MODELS * modelPage);
    
    if (tempModelNoIdx > MODELS)
    break;
    
    u8g2.setCursor(50, 21 + (i * 10));
    u8g2.print(tempModelNoIdx + 1);
    
    // Define start position for Eeprom write/update (32 * [0,1,2,3,4])
    eepromPos = NUM_BYTES_PER_MODEL * tempModelNoIdx;
    
    // Print MODEL NAME
    for (int j = 0; j < 5; j++)
    {
      u8g2.setCursor(9 + (j * 8), 21 + (i * 10));
      char ch = EEPROM.read(eepromPos++);
      u8g2.print(ch);
    }
    
    if (tempModelNoIdx == menuSubModel)
    {
      // Print ">"
      strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[5])));
      u8g2.setCursor(0, 21 + (i * 10));
      u8g2.print(char_buffer);
    }
    // Left Section End
    
    
    // Right Section Start
    tempModelNoIdx = (i + 5) + (MODELS * modelPage);
    
    if (tempModelNoIdx > MODELS)
    break;
    
    u8g2.setCursor(115, 21 + (i * 10));
    u8g2.print(tempModelNoIdx + 1);
    
    // Define start position for Eeprom write/update (25 * [0,1,2,3,4])
    eepromPos = NUM_BYTES_PER_MODEL * tempModelNoIdx;
    
    // Print MODEL NAME
    for (int j = 0; j < 5; j++)
    {
      u8g2.setCursor(74 + (j * 8), 21 + (i * 10));
      char ch = EEPROM.read(eepromPos++);
      u8g2.print(ch);
    }
    
    if (tempModelNoIdx == menuSubModel)
    {
      // Print ">"
      strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[5])));
      u8g2.setCursor(65, 21 + (i * 10));
      u8g2.print(char_buffer);
    }
    // Right Section End
  }
}
 
