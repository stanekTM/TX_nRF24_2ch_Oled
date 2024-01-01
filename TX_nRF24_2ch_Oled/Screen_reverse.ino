
//*********************************************************************************************************************
// Drawing REVERSE screen display
//*********************************************************************************************************************
// this is the state machine, which will replace the do - while loop
void draw_reverse_screen()
{
  static uint8_t is_next_page = 0;
  
  // call to first page, if required
  if (is_next_page == 0)
  {
    u8g2.firstPage();
    is_next_page = 1;
  }
  
  // draw our screen
  reverse_screen();
  
  // call to next page
  if (u8g2.nextPage() == 0)
  {
    is_next_page = 0; // ensure, that first page is called
  }
}

//------------------------------------------------------------------------
void reverse_screen()
{
  // Set memory buffer for text strings
  char menu_buffer[7];
  char name_buffer[13];
  char char_buffer[9];
  
  read_pots(); // Macro again for stable pots value
  
  // Print "REVERSE"
  strcpy_P(menu_buffer, (char*)pgm_read_word(&(menu_name[2])));
  u8g2.setCursor(0, 9);
  u8g2.print(menu_buffer);
  
  // Drawing horizontal line under header
  u8g2.drawHLine(0, 10, 128);
  
  
  // Drawing only first 2 channels
  for (int i = 0; i < CHANNELS; i++)
  {
    // Print channel items name "CH1, CH2"
    strcpy_P(name_buffer, (char*)pgm_read_word(&(channel_name[i])));
    u8g2.setCursor(8, 21 + (i * 10));
    u8g2.print(name_buffer);
    
    // Print "="
    strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[10])));
    u8g2.setCursor(37, 21 + (i * 10));
    u8g2.print(char_buffer);
    
    
    if (bitRead(reverse, i) == 1)
    {
      // Print "REV"
      strcpy_P(name_buffer, (char*)pgm_read_word(&(channel_name[9])));
      u8g2.setCursor(53, 21 + (i * 10));
      u8g2.print(name_buffer);
    }
    else
    {
      // Print "NOR"
      strcpy_P(name_buffer, (char*)pgm_read_word(&(channel_name[8])));
      u8g2.setCursor(53, 21 + (i * 10));
      u8g2.print(name_buffer);
    }
    
    
    // Drawing dynamic graphics items
    u8g2.drawHLine(83, 20 + (i * 10), 45);
    
    u8g2.drawVLine(105, 16 + (i * 10), 4);
    //                                                                83, 125), 18 + (i * 10), 3, 2)
    u8g2.drawBox(map(pots_value[i], MIN_CONTROL_VAL, MAX_CONTROL_VAL, 83, 125), 18 + (i * 10), 3, 2);
    
    
    if (menuSubActual - 1 == i)
    {
      // Print ">"
      strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[5])));
      u8g2.setCursor(0, 21 + (i * 10));
      u8g2.print(char_buffer);
    }
  }
  // End drawing only first 2 channels
}
 
