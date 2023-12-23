
//*********************************************************************************************************************
// Drawing MAIN screen display
//*********************************************************************************************************************
// this is the state machine, which will replace the do - while loop
void draw_main_screen()
{
  static uint8_t is_next_page = 0;
  
  // call to first page, if required
  if (is_next_page == 0)
  {
    u8g2.firstPage();
    is_next_page = 1;
  }
  
  // draw our screen
  main_screen();
  
  // call to next page
  if (u8g2.nextPage() == 0)
  {
    is_next_page = 0; // ensure, that first page is called 
  }  
}

//------------------------------------------------------------------------
void main_screen()
{
  // Set memory buffer for text strings
  char name_buffer[13];
  char char_buffer[8];
  char msg_buffer[11];
  char menu_buffer[7];
  
  read_pots(); // Macro again for stable pots value
  
  // Print "TX"
  strcpy_P(msg_buffer, (char*)pgm_read_word(&(message[10])));
  u8g2.setCursor(0, 9);
  u8g2.print(msg_buffer);
  
  if (tx_low_batt)
  {
    // Print "low!"
    strcpy_P(msg_buffer, (char*)pgm_read_word(&(message[6])));
    u8g2.setCursor(18, 9);
    u8g2.print(msg_buffer);
  }
  else
  {
    // Print TX battery voltage
    u8g2.setCursor(15, 9);
    u8g2.print(tx_batt_volt);
    
    // Print "V"
    strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[7])));
    u8g2.setCursor(45, 9);
    u8g2.print(char_buffer);
  }
  
  
  // Print "RX"
  strcpy_P(msg_buffer, (char*)pgm_read_word(&(message[9])));
  u8g2.setCursor(72, 9);
  u8g2.print(msg_buffer);
  
  if (rf_state)
  {
    if (rx_low_batt)
    {
      // Print "low!"
     strcpy_P(msg_buffer, (char*)pgm_read_word(&(message[6])));
     u8g2.setCursor(90, 9);
     u8g2.print(msg_buffer);
    }
    else
    {
    // Print RX battery voltage
    u8g2.setCursor(88, 9);
    u8g2.print(rx_batt_volt);
    
    // Print "V"
    strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[7])));
    u8g2.setCursor(122, 9);
    u8g2.print(char_buffer);
    }
  }
  else
  {
    // Print "off!"
    strcpy_P(msg_buffer, (char*)pgm_read_word(&(message[1])));
    u8g2.setCursor(90, 9);
    u8g2.print(msg_buffer);
  }
  rf_state = 0;
  
  
  // Drawing horizontal line under header
  //u8g2.drawHLine(0, 10, 128);
  
  // Print number of which model in use
  u8g2.setCursor(113, 26); //61
  u8g2.print(modelActual + 1);
  
  // Print MODEL NAME "MODEL"
  u8g2.setFont(u8g2_font_VCR_OSD_tr); // height 15 pixels (dafont)
  
  u8g2.setCursor(52, 27); //0
  u8g2.print(modelName);
  
  
  // Drawing only first 2 channels
  for (int i = 0; i < CHANNELS; i++)
  {
    // Draw boxes/frames for every channel
    u8g2.drawFrame(22, 36 + (i * 20), 106, 8); //128 - (106 / 2) = 75
    
    // Drawing vertical middle/center separation line
    u8g2.drawVLine(75, 34 + (i * 20), 6);
    
    // Define value bar reference
    unsigned int valBar;
    
    // Value bars subdivision (pots value / 50)
    valBar = map(pots_value[i], MIN_CONTROL_VAL, MAX_CONTROL_VAL, 0, 100);
    
    // Drawing cursor in every channel bar
    if (valBar < 50)
    {
      u8g2.drawBox(23 + valBar, 37 + (i * 20), 53 - valBar, 6);
    }
    else if (valBar > 50)
    {
      u8g2.drawBox(75, 37 + (i * 20), valBar - 45, 6);
    }
    
    
    // REVERSE
    unsigned short subTrimVal;
    
    subTrimVal = map(subTrim[i], 0, SUB_TRIM_MAX, 0, 50);
    
    // Check reverse and applying reverse value if necessary
    if (bitRead(reverse, i) == 1)
    {
      subTrimVal = -subTrimVal;
    }
    
    u8g2.drawHLine(0, 10 + (i * 17), 128);
    
    u8g2.setFont(u8g2_font_5x7_tr); // height 6 pixels (X11)
    
    if (subTrim[i] > 0)
    {
      // Print SUB TRIM value
      u8g2.setCursor(67, 35 + (i * 20));
      u8g2.print(subTrim[i]);
    }
    else if (subTrim[i] < 0)
    {
      // Print SUB TRIM value
      u8g2.setCursor(62, 35 + (i * 20));
      u8g2.print(subTrim[i]);
    }
    
    
    // Drawing REVERSE channel status for every channel
    if (bitRead(reverse, i) == 1)
    {
      // Print "REV"
      strcpy_P(name_buffer, (char*)pgm_read_word(&(channel_name[9])));
      u8g2.setCursor(26, 35 + (i * 20));
      u8g2.print(name_buffer);
    }
    
    
    // Drawing EXPO
    if (expo[i] > 0)
    {
      // Print "EXPO"
      strcpy_P(menu_buffer, (char*)pgm_read_word(&(menu_name[6])));
      u8g2.setCursor(102, 35 + (i * 20));
      u8g2.print(menu_buffer);
      
      // Print EXPO value
      u8g2.setCursor(124, 35 + (i * 20));
      u8g2.print(expo[i]);
    }
    
    
    // Print channel items name "CH1 and CH2"
    u8g2.setFont(u8g2_font_7x13_tr); // height 9 pixels (X11)
    
    strcpy_P(name_buffer, (char*)pgm_read_word(&(channel_name[i])));
    u8g2.setCursor(0, 44 + (i * 20));
    u8g2.print(name_buffer);
  }
  // End drawing only first 2 channels
}
 
