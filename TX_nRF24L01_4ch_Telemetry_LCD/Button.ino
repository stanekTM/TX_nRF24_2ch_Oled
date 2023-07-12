
//************************************************************************************************************************************************************************
// Read input buttons status and send value to loop
//************************************************************************************************************************************************************************
unsigned long button_time = 0;

unsigned char read_button()
{
  if (millis() - button_time > 150) //150ms
  {
    // buttonSelect
    if ((PIND & (1 << PIN_BUTTON_SELECT)) == 0)
    {
      button_time = millis();
      return 2;
    }
    
    // buttonUp
    if ((PIND & (1 << PIN_BUTTON_UP)) == 0)
    {
      button_time = millis();
      return 1;
    }
    
    // buttonDown
    if ((PIND & (1 << PIN_BUTTON_DOWN)) == 0)
    {
      button_time = millis();
      return 3;
    }
  }
  return 0;
}

//************************************************************************************************************************************************************************
// Macro for read button status definitions
//************************************************************************************************************************************************************************
void read_button_exit()
{
  // excute only one time after buttonExit is up
  if (exitStatus == 1 && !bit_is_clear(PIND, PIN_BUTTON_EXIT))
  {
    // buttonExit pushed
    exitStatus = !exitStatus;
    
    // buttonExit can be cancle button. Go main screen
    if (screen != 0)
    {
      screen = 0;
      menuSubActual = 1;
      menuActual = 0;
      menuPage=0;

      epaSelection = 0xFF;
      subTrimSelection = 0xFF;
      modelNameSelection = 0xFF;
      expoSelection = 0xFF;
      
      return;
    }
  }
  
  if (bit_is_clear(PIND, PIN_BUTTON_EXIT))
  {
    // buttonExit up
    exitStatus = 1;
  }
}
 
