
//*********************************************************************************************************************
// Read input buttons status and send value to loop
//*********************************************************************************************************************
unsigned long button_time = 0;

unsigned char read_button()
{
  if (millis() - button_time > 200)
  {
    if (digitalRead(PIN_BUTTON_SELECT) == LOW)
    {
      button_time = millis();
      return 2;
    }
    
    if (digitalRead(PIN_BUTTON_UP) == LOW)
    {
      button_time = millis();
      return 1;
    }
    
    if (digitalRead(PIN_BUTTON_DOWN) == LOW)
    {
      button_time = millis();
      return 3;
    }
  }
  return 0;
}

//*********************************************************************************************************************
// Macro for read button status definitions
//*********************************************************************************************************************
void read_button_exit()
{
  bool exitStatus = 1;
  
  // excute only one time after button EXIT is up
  if (exitStatus == 1 && digitalRead(PIN_BUTTON_EXIT) == LOW)
  {
    // button EXIT pushed
    exitStatus = !exitStatus;
    
    // button EXIT can be cancle button. Go main screen
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
  
  if (digitalRead(PIN_BUTTON_EXIT) == LOW)
  {
    // button EXIT
    exitStatus = 1;
  }
}
 
