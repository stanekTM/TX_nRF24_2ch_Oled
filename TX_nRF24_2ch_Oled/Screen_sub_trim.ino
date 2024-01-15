
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
  char word_buffer[13];
  char char_buffer[9];
  
  read_pots(); // Macro again for stable pots value
  
  // Print "SUB TRIM"
  strcpy_P(menu_buffer, (char*)pgm_read_word(&(menu_name[4])));
  u8g2.setCursor(0, 9);
  u8g2.print(menu_buffer);
  
  // Drawing horizontal line under header
  u8g2.drawHLine(0, 10, 128);
  
  
  unsigned char temp_Counter = 0;
  
  // Drawing only first 2 channels
  for (int i = 0; i < CHANNELS; i++)
  {
    // Print channel items name "CH1, CH2"
    strcpy_P(word_buffer, (char*)pgm_read_word(&(word_name[i])));
    u8g2.setCursor(10, 21 + (i * 10));
    u8g2.print(word_buffer);
    
    // Print SUB TRIM value
    if (subTrim[i] < 0)
    {
      u8g2.setCursor(44, 21 + (i * 10));
      u8g2.print(subTrim[i]);
    }
    else if (subTrim[i] > 0)
    {
      u8g2.setCursor(51, 21 + (i * 10));
      u8g2.print(subTrim[i]);
    }
    
    // Print PPM value
    u8g2.setCursor(87, 21 + (i * 10));
    u8g2.print(pots_value[i]);
    
    // Print "us"
    strcpy_P(word_buffer, (char*)pgm_read_word(&(word_name[6])));
    u8g2.setCursor(115, 21 + (i * 10));
    u8g2.print(word_buffer);
    
    
    if (menuSubActual - 1 == temp_Counter)
    {
      if (subTrimSelection == temp_Counter)
      {
        // Print ">"
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[5])));
        u8g2.setCursor(0, 21 + (i * 10));
        u8g2.print(char_buffer);
        
        // Print "("
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[2])));
        u8g2.setCursor(39, 21 + (i * 10));
        u8g2.print(char_buffer);
        
        // Print ")"
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[3])));
        u8g2.setCursor(72, 21 + (i * 10));
        u8g2.print(char_buffer);
      }
      else
      {
        // Print ">"
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[5])));
        u8g2.setCursor(0, 21 + (i * 10));
        u8g2.print(char_buffer);
      }
    }
    
    temp_Counter++;
  }
}
 
