
//*********************************************************************************************************************
// Macro for read pots, joysticks, values, applying calibration and rules
//*********************************************************************************************************************
void read_pots()
{
  for (int ch = 0; ch < CHANNELS; ch++)
  {
    int value_pots = MID_CONTROL_VAL;
    
    pots[ch] = analogRead(ch);
    
    // only for CH1 "steering" and CH2 "throttle"
    if (ch < 2)
    {
      // Applying calibration mapping
      // In case of surface TX, Left and Right rotation rate should be same.
      // So, Longer side length is used for both side
      int gap = pot_calib_max[ch] - pot_calib_mid[ch];
      int gapTemp = pot_calib_mid[ch] - pot_calib_min[ch];
      
      // Select longer side
      if (gap < gapTemp)
      {
        gap = gapTemp;
      }
      
      
      // MIN range correction
      if (pots[ch] < pot_calib_min[ch])
      {
        pots[ch] = pot_calib_min[ch];
      }
      
      // MAX range correction
      if (pots[ch] > pot_calib_max[ch])
      {
        pots[ch] = pot_calib_max[ch];
      }
      
      
      // EPA
      int epaVal = 0;
      int epaVal_bwd = 0;
      
      epaVal = 500 - (500 * epa[ch] / 100);
      
      if (ch == 1)
      {
        epaVal_bwd = 500 - (500 * epa[2] / 100);
      }
      
      
      unsigned short trimServoMid = MID_CONTROL_VAL + subTrim[ch];
      unsigned short trimServoMin = MIN_CONTROL_VAL + epaVal + subTrim[ch];
      unsigned short trimServoMax = MAX_CONTROL_VAL - epaVal + subTrim[ch];
      
      // Convert Analog Value to pots value
      if (pots[ch] < (POT_CENTER - deadBand))
      {
        if (ch == 1)
        {
          trimServoMin = MIN_CONTROL_VAL + epaVal_bwd + subTrim[ch];
        }
        
        value_pots = map(pots[ch], POT_CENTER - gap, POT_CENTER - deadBand, trimServoMin, trimServoMid);
        
        // EXPO
        if (expo[ch] > 0)
        {
          value_pots = calc_expo(trimServoMid, value_pots, trimServoMin, expo[ch]);
        }
      }
      else if (pots[ch] > (POT_CENTER + deadBand))
      {
        value_pots = map(pots[ch], POT_CENTER + deadBand, POT_CENTER + gap - 1, trimServoMid, trimServoMax);
        
        // EXPO
        if (expo[ch] > 0)
        {
          value_pots = calc_expo(trimServoMid, value_pots, trimServoMax, expo[ch]);
        }
      }
      else
      {
        value_pots = trimServoMid;
      }
      
      // Check Servo Reversing and applying Reverse value if necessary
      if (bitRead(servoReverse, ch) == 1)
      {
        value_pots = MAX_CONTROL_VAL - value_pots + MIN_CONTROL_VAL;
      }
      
      //Min Max Validation
      if (value_pots < MIN_CONTROL_VAL)
      {
        value_pots = MIN_CONTROL_VAL;
      }
      
      if (value_pots > MAX_CONTROL_VAL)
      {
        value_pots = MAX_CONTROL_VAL;
      }
    }
    /*else
    {
      // mapping pot value CH3
      if (ch == 2)
      {
        if (bitRead(servoReverse, 2) == 1)
        {
          value_pots = map(analogRead(2), pot_calib_min[ch], pot_calib_max[ch], MAX_CONTROL_VAL, MIN_CONTROL_VAL);
        }
        else
        {
          value_pots = map(analogRead(2), pot_calib_min[ch], pot_calib_max[ch], MIN_CONTROL_VAL, MAX_CONTROL_VAL);
        }
      }
      
      // mapping pot value CH4
      if (ch == 3)
      {
        if (bitRead(servoReverse, 3) == 1)
        {
          value_pots = map(analogRead(3), pot_calib_min[ch], pot_calib_max[ch], MAX_CONTROL_VAL, MIN_CONTROL_VAL);
        }
        else
        {
          value_pots = map(analogRead(3), pot_calib_min[ch], pot_calib_max[ch], MIN_CONTROL_VAL, MAX_CONTROL_VAL);
        }
      }
    }*/
    
    pots_value[ch] = value_pots;
  }
}
 
