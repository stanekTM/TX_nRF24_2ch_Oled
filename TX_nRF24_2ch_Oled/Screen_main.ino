
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
  
  //u8g2.firstPage(); do {
  
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
    u8g2.setCursor(17, 9);
    u8g2.print(tx_batt_volt);
    
    // Print "V"
    strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[7])));
    u8g2.setCursor(50, 9);
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
  u8g2.drawHLine(0, 10, 128);
  
  // Print "model"
  strcpy_P(name_buffer, (char*)pgm_read_word(&(channel_name[11])));
  u8g2.setCursor(0, 21);
  u8g2.print(name_buffer);
  
  // Print number of which model in use
  u8g2.setCursor(40, 21);
  u8g2.print(modelActual + 1);
  
  // Print "MODEL NAME"
  //u8g2.setFont(u8g2_font_VCR_OSD_tr); // height 15 pixels (dafont)
  u8g2.drawStr(54, 21, modelName);
  
  
  // Drawing only first 2 channels
  for (int i = 0; i < CHANNELS; i++)
  {
    // Draw boxes/frames for every channel
    //            (20, 40 + (i * 16), 108, 8)
    u8g2.drawFrame(20, 40 + (i * 16), 108, 8); //128 - (108 / 2) = 74
    
    // Drawing vertical middle/center separation line
    //            (74, 41 + (i * 16), 6)
    u8g2.drawVLine(74, 41 + (i * 16), 6);
    
    // Define value bar reference
    unsigned int valBar;
    
    // Value bars subdivision (pots value / 50)
    valBar = map(pots_value[i], MIN_CONTROL_VAL, MAX_CONTROL_VAL, 0, 100);
    
    // Drawing cursor in every channel bar
    if (valBar < 50)
    {           //(21 + valBar, 41 + (i * 16), 54 - valBar, 6)
      u8g2.drawBox(21 + valBar, 41 + (i * 16), 54 - valBar, 6);
    }
    else if (valBar > 50)
    {           //(74, 41 + (i * 16), valBar - 46, 6)
      u8g2.drawBox(74, 41 + (i * 16), valBar - 45, 6);
    }
    
    
    // REVERSE
    unsigned short subTrimVal;
    
    subTrimVal = map(subTrim[i], 0, SUB_TRIM_MAX, 0, 50);
    
    // Check reverse and applying reverse value if necessary
    if (bitRead(reverse, i) == 1)
    {
      subTrimVal = -subTrimVal;
    }
    
    
    //u8g2.setFont(u8g2_font_5x7_tr); // height 6 pixels (X11)
    
    if (subTrim[i] > 0)
    {
      // Print SUB TRIM value
      u8g2.setCursor(66, 39 + (i * 16));
      u8g2.print(subTrim[i]);
    }
    else if (subTrim[i] < 0)
    {
      // Print SUB TRIM value
      u8g2.setCursor(66, 39 + (i * 16));
      u8g2.print(subTrim[i]);
    }
    
    
    // Drawing REVERSE channel status for every channel
    if (bitRead(reverse, i) == 1)
    {
      // Print "REV"
      strcpy_P(name_buffer, (char*)pgm_read_word(&(channel_name[9])));
      u8g2.setCursor(23, 39 + (i * 16));
      u8g2.print(name_buffer);
    }
    
    
    // Drawing EXPO
    if (expo[i] > 0)
    {
      // Print "EXPO"
      strcpy_P(menu_buffer, (char*)pgm_read_word(&(menu_name[6])));
      u8g2.setCursor(102, 39 + (i * 16));
      u8g2.print(menu_buffer);
      
      // Print EXPO value
      u8g2.setCursor(124, 39 + (i * 16));
      u8g2.print(expo[i]);
    }
    
    //u8g2.setFont(u8g2_font_6x10_tr); // height 7 pixels (X11)
    //u8g2.setFont(u8g2_font_t0_14_tr); // *height 7 pixels (X11)*
    
    // Print channel items name "CH1 and CH2"
    strcpy_P(name_buffer, (char*)pgm_read_word(&(channel_name[i])));
    u8g2.setCursor(0, 40 + (i * 16));
    u8g2.print(name_buffer);
  }
  // End drawing only first 2 channels
  
  //u8g2.setFont(u8g2_font_6x10_tr); // height 7 pixels (X11)
  
  //} while (u8g2.nextPage());
}
 
