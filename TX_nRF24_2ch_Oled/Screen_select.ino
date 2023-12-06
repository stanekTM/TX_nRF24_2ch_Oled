
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
  char name_buffer[13];
  
  // For Eeprom position reference
  unsigned int eepromPos;
  int tempModelNoIdx;
  
  //u8g2.firstPage(); do {
  
  // Print "MODEL"
  strcpy_P(name_buffer, (char*)pgm_read_word(&(channel_name[11])));
  u8g2.setCursor(0, 9);
  u8g2.print(name_buffer);
  
  // Print number of which model in use
  u8g2.setCursor(32, 9);
  u8g2.print(modelActual + 1);
  
  /*
  u8g2.setCursor(111, 9);
  u8g2.print(modelPage + 1);

  // Print "/"
  strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[6])));
  u8g2.setCursor(117, 9);
  u8g2.print(char_buffer);
  
  u8g2.setCursor(123, 9);
  u8g2.print((MODELS - 1) / MODELS + 1);  // Total model Count / model count per page + 1
  */
  
  // Drawing horizontal line under header
  u8g2.drawHLine(0, 10, 128);
  
  
  // Print MODEL SELECT list
  for (int i = 0; i < 5; i++)
  {
    // Left Section Start
    tempModelNoIdx = i + (MODELS * modelPage);
    
    if (tempModelNoIdx > MODELS)
    break;
    
    u8g2.setCursor(8, 21 + i * 10);
    u8g2.print(tempModelNoIdx + 1);
    
    // Define start position for Eeprom write/update (32 * [0,1,2,3,4])
    eepromPos = NUM_BYTES_PER_MODEL * tempModelNoIdx;
    
    for (int j = 0; j < 5; j++)
    {
      u8g2.setCursor(23 + (j * 6), 21 + i * 10);
      char ch = EEPROM.read(eepromPos++);
      u8g2.print(ch);
    }
    
    if (tempModelNoIdx == menuSubModel)
    {
      // Print "["
      strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[2])));
      u8g2.setCursor(0, 21 + i * 10);
      u8g2.print(char_buffer);
      
      // Print "]"
      strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[3])));
      u8g2.setCursor(55, 21 + i * 10);
      u8g2.print(char_buffer);
    }
    // Left Section End
    
    
    // Right Section Start
    tempModelNoIdx = (i + 5) + (MODELS * modelPage);
    
    if (tempModelNoIdx > MODELS)
    break;
    
    u8g2.setCursor(75, 21 + i * 10);
    u8g2.print(tempModelNoIdx + 1);
    
    // Define start position for Eeprom write/update (25 * [0,1,2,3,4])
    eepromPos = NUM_BYTES_PER_MODEL * tempModelNoIdx;
    
    for (int j = 0; j < 5; j++)
    {
      u8g2.setCursor(90 + (j * 6), 21 + i * 10);
      char ch = EEPROM.read(eepromPos++);
      u8g2.print(ch);
    }
    
    if (tempModelNoIdx == menuSubModel)
    {
      // Print "["
      strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[2])));
      u8g2.setCursor(67, 21 + i * 10);
      u8g2.print(char_buffer);
      
      // Print "]"
      strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[3])));
      u8g2.setCursor(122, 21 + i * 10);
      u8g2.print(char_buffer);
    }
    // Right Section End
  }
  
  //} while (u8g2.nextPage());
}
 
