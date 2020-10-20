//************************************************************************************************************************************************************************
// Read input buttons status and send value to loop
//************************************************************************************************************************************************************************
unsigned long buttonTime = 0;

unsigned char read_button() {

  if (millis() >= buttonTime + 150) //1000 (1second)
  {
  // buttonSelect
  if ((PIND & (1 << buttonSelect)) == 0) {
    buttonTime = millis();
    return 2;
  }
  // buttonUp
  if ((PIND & (1 << buttonUp)) == 0) {
    buttonTime = millis();
    return 1;
  }
  // buttonDown
  if ((PIND & (1 << buttonDown)) == 0) {
    buttonTime = millis();
    return 3;
  }
  return 0;
 }
}

//************************************************************************************************************************************************************************
// Macro for read button status definitions
//************************************************************************************************************************************************************************
void read_button_exit() {

  // excute only one time after buttonExit is up
  if (exitStatus == 1 && !bit_is_clear(PIND, buttonExit)) {
    // buttonExit pushed
    exitStatus = !exitStatus;
    
    // buttonExit can be cancle button. Go main screen
    if (screen != 0) {
      
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

  if (bit_is_clear(PIND, buttonExit)) {
    // buttonExit up
    exitStatus = 1;
  }
}
 
