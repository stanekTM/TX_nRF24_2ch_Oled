
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
  char char_buffer[9];
  
  read_pots(); // Macro again for stable pots value
  
  // Print "EPA"
  strcpy_P(menu_buffer, (char*)pgm_read_word(&(menu_name[0])));
  u8g2.setCursor(0, 9);
  u8g2.print(menu_buffer);
  
  // Drawing horizontal line under header
  u8g2.drawHLine(0, 10, 128);
  
  // Print "left"
  strcpy_P(name_buffer, (char*)pgm_read_word(&(channel_name[2])));
  u8g2.setCursor(0, 21);
  u8g2.print(name_buffer);
  
  // Print "right"
  strcpy_P(name_buffer, (char*)pgm_read_word(&(channel_name[3])));
  u8g2.setCursor(89, 21);
  u8g2.print(name_buffer);
  
  //u8g2.drawVLine(64, 12, 52); // test center
  
  
  int section_epa;
  
  for (int i = 0; i < 2; i++)
  {
    // Print channel items name "CH1, CH2"
    strcpy_P(name_buffer, (char*)pgm_read_word(&(channel_name[i])));
    u8g2.setCursor(53, 33 + i * 10);
    u8g2.print(name_buffer);
    
    
    // Left section start
    section_epa = i;
    
    // EPA value
    u8g2.setCursor(7, 33 + i * 10);
    u8g2.print(epa[section_epa]);
    
    
    if (menuSubActual - 1 == section_epa)
    {
      if (epaSelection == section_epa)
      {
        // Print "("
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[2])));
        u8g2.setCursor(0, 33 + i * 10);
        u8g2.print(char_buffer);
        
        // Print ")"
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[3])));
        u8g2.setCursor(30, 33 + i * 10);
        u8g2.print(char_buffer);
      }
      else
      {
        // Print "<"
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[4])));
        u8g2.setCursor(32, 33 + i * 10);
        u8g2.print(char_buffer);
      }
    }
    // Left section end
    
    
    // Right section start
    section_epa = i + 2;
    
    // EPA value
    u8g2.setCursor(100, 33 + i * 10);
    u8g2.print(epa[section_epa]);
    
    
    if (menuSubActual - 1 == section_epa)
    {
      if (epaSelection == section_epa)
      {
        // Print "("
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[2])));
        u8g2.setCursor(93, 33 + i * 10);
        u8g2.print(char_buffer);
        
        // Print ")"
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[3])));
        u8g2.setCursor(123, 33 + i * 10);
        u8g2.print(char_buffer);
      }
      else
      {
        // Print ">"
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[5])));
        u8g2.setCursor(91, 33 + i * 10);
        u8g2.print(char_buffer);
      }
    }
    // Right section end
  }
}
 
