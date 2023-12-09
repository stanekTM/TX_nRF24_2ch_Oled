
//*********************************************************************************************************************
// Drawing MODEL NAME screen display
//*********************************************************************************************************************
// this is the state machine, which will replace the do - while loop
void draw_model_name_screen()
{
  static uint8_t is_next_page = 0;
  
  // call to first page, if required
  if (is_next_page == 0)
  {
    u8g2.firstPage();
    is_next_page = 1;
  }
  
  // draw our screen
  model_name_screen();
  
  // call to next page
  if (u8g2.nextPage() == 0)
  {
    is_next_page = 0; // ensure, that first page is called
  }
}

//------------------------------------------------------------------------
void model_name_screen()
{
  // Set memory buffer for text strings
  char menu_buffer[7];
  char name_buffer[13];
  char char_buffer[9];
  
  // Print "MODEL NAME"
  strcpy_P(menu_buffer, (char*)pgm_read_word(&(menu_name[5])));
  u8g2.setCursor(0, 7);
  u8g2.print(menu_buffer);
  
  // Print MODEL NAME "MODEL"
  u8g2.setCursor(0, 9);
  u8g2.print(modelName);
  
  // Print number of which model in use
  u8g2.setCursor(117, 7);
  u8g2.print(modelActual + 1);
  
  // Drawing horizontal line under header
  u8g2.drawHLine(0, 8, 128);
  
  
  //u8g2.setFont(u8g2_font_VCR_OSD_tr); // height 15 pixels (dafont)
  
  // Print MODEL NAME 5 byte
  for (int i = 0; i < 5; i++)
  {
    u8g2.setCursor(13 + (i * 23), 45);
    u8g2.print(modelName[i]);
    
    if (i == menuSubActual - 1)
    {
      if (modelNameSelection == i)
      {
        // Print "("
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[2])));
        u8g2.setCursor(1 + (i * 23), 45);
        u8g2.print(char_buffer);
        
        // Print ")"
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[3])));
        u8g2.setCursor(25 + (i * 23), 45);
        u8g2.print(char_buffer);
      }
      else
      {
        // Print ">"
        strcpy_P(char_buffer, (char*)pgm_read_word(&(one_char[5])));
        u8g2.setCursor(1 + (i * 23), 45);
        u8g2.print(char_buffer);
      }
    }
  }
  
  //u8g2.setFont(u8g2_font_6x10_tr); // height 7 pixels (X11)
}
 
