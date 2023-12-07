
//*********************************************************************************************************************
// Drawing EXPO screen display
//*********************************************************************************************************************
// this is the state machine, which will replace the do - while loop
void draw_expo_screen()
{
  static uint8_t is_next_page = 0;
  
  // call to first page, if required
  if (is_next_page == 0)
  {
    u8g2.firstPage();
    is_next_page = 1;
  }
  
  // draw our screen
  expo_screen();
  
  // call to next page
  if (u8g2.nextPage() == 0)
  {
    is_next_page = 0; // ensure, that first page is called
  }
}

//------------------------------------------------------------------------
void expo_screen()
{
  // Set memory buffer for text strings
  char menu_buffer[7];
  char name_buffer[13];
  char char_buffer[9];
  
  //u8g2.firstPage(); do {
  
  read_pots(); // Macro again for stable pots value
  
  // Print "EXPO"
  strcpy_P(menu_buffer, (char*)pgm_read_word(&(menu_name[6])));
  u8g2.setCursor(0, 7);
  u8g2.print(menu_buffer);
  
  // Drawing horizontal line under header
  u8g2.drawHLine(0, 8, 128);

  
  // Drawing only first 2 channels
  for (int i = 0; i < CHANNELS; i++)
  {
    // Print channel items name "CH1, CH2"
    strcpy_P(name_buffer, (char*)pgm_read_word(&(channel_name[i])));
    u8g2.setCursor(10, 20 + i * 13);
    u8g2.print(name_buffer);
    
    // Print EXPO value
    u8g2.setCursor(37, 20 + i * 13);
    u8g2.print(expo[i]);
    
    
    if (i == menuSubActual - 1)
    {
      if (expoSelection == i)
      {
        // Print ">"
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[5])));
        u8g2.setCursor(2, 20 + i * 13);
        u8g2.print(char_buffer);

        // Print "("
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[2])));
        u8g2.setCursor(30, 20 + i * 13);
        u8g2.print(char_buffer);

        // Print ")"
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[3])));
        u8g2.setCursor(44, 20 + i * 13);
        u8g2.print(char_buffer);
      }
      else
      {
        // Print ">"
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[5])));
        u8g2.setCursor(2, 20 + i * 13);
        u8g2.print(char_buffer);
      }
      
      
      // Draw EXPO Graph
      u8g2.drawHLine(53, 36, 75);
      u8g2.drawVLine(92, 10, 54);
      
      u8g2.drawFrame(53, 10, 75, 54);
      u8g2.drawLine(52, 64, 128, 10);
      
      if (expo[i] > 0)
      {
        for (int j = 52; j <= 91; j++)
        {
          u8g2.drawPixel(j, map(calc_expo(MID_CONTROL_VAL,
          map(j, 52, 91, MIN_CONTROL_VAL, MID_CONTROL_VAL), MIN_CONTROL_VAL, expo[i]), MIN_CONTROL_VAL, MID_CONTROL_VAL, 64, 36));
        }
        
        for (int j = 91; j <= 128; j++)
        {
          u8g2.drawPixel(j, map(calc_expo(MID_CONTROL_VAL,
          map(j, 91, 128, MID_CONTROL_VAL, MAX_CONTROL_VAL), MAX_CONTROL_VAL, expo[i]), MID_CONTROL_VAL, MAX_CONTROL_VAL, 35, 9));
        }
      }
    }
  }
  
  //} while (u8g2.nextPage());
}
 
