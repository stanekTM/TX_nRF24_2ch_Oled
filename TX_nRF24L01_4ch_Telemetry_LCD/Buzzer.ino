
//*********************************************************************************************************************
// Macro for buzzer beep battery alarm
//*********************************************************************************************************************
byte pulseBuzzer; // Sound pulse for battery alarm

void beep()
{
  if (pulseBuzzer < 2)
  {
    tone(PIN_BUZZER, 900);
  }
  else if (pulseBuzzer > 2 && pulseBuzzer < 4)
  {
    noTone(PIN_BUZZER);
  }
  else if (pulseBuzzer > 4 && pulseBuzzer < 7)
  {
    tone(PIN_BUZZER, 900);
  }
  else if (pulseBuzzer > 7 && pulseBuzzer < 9)
  {
    noTone(PIN_BUZZER);
  }
  
  if (pulseBuzzer > 80)
  {
    pulseBuzzer = 0;
  }
  
  pulseBuzzer++;

/*
 // Another indication when an LED is used instead of a buzzer
  if (pulseBuzzer < 5)
  {
    digitalWrite(PIN_BUZZER, HIGH);
  }
  else
  {
    digitalWrite(PIN_BUZZER, LOW);
  }
  
  if (pulseBuzzer > 100)
  {
    pulseBuzzer = 0;
  }
  
  pulseBuzzer++;
*/  
}
 
