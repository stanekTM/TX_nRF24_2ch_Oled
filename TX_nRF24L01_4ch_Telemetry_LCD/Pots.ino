
//*********************************************************************************************************************
// Macro for read pots, joysticks, values, applying calibration and rules
//*********************************************************************************************************************
int ch;
void read_pots()
{
  for (ch = 0; ch < CHANNELS; ch++)
  {
    int pots_control_val = MID_CONTROL_VAL;
    
    raw_pots[ch] = analogRead(ch);
    
    // only for throttle and steering ch
    if (ch < 2)
    {
      // Applying calibration mapping
      // In case of surface TX, Left and Right rotation rate should be same.
      // So, Longer side length is used for both side
      int gap = calibration[ch][1] - centerPos[ch];
      int gapTemp = centerPos[ch] - calibration[ch][0];
      
      // Select longer side
      if (gap < gapTemp)
      {
        gap = gapTemp;
      }
      
      // Calculate Center offset
      int centerOffset = POT_CENTER - centerPos[ch];
      
      // Applying initial value with center offset
      pots[ch] = raw_pots[ch] + centerOffset;
      
      // range out correction
      if (pots[ch] < calibration[ch][0] + centerOffset)
      {
        pots[ch] = calibration[ch][0] + centerOffset;
      }
      
      if (pots[ch] > calibration[ch][1] + centerOffset)
      {
        pots[ch] = calibration[ch][1] + centerOffset;
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
        
        pots_control_val = map(pots[ch], POT_CENTER - gap, POT_CENTER - deadBand, trimServoMin, trimServoMid);
        
        // EXPO
        if (expo[ch] > 0)
        {
          pots_control_val = calc_expo(trimServoMid, pots_control_val, trimServoMin, expo[ch]);
        }
      }
      else if (pots[ch] > (POT_CENTER + deadBand))
      {
        pots_control_val = map(pots[ch], POT_CENTER + deadBand, POT_CENTER + gap - 1, trimServoMid, trimServoMax);
        
        // EXPO
        if (expo[ch] > 0)
        {
          pots_control_val = calc_expo(trimServoMid, pots_control_val, trimServoMax, expo[ch]);
        }
      }
      else
      {
        pots_control_val = trimServoMid;
      }
      
      // Check Servo Reversing and applying Reverse value if necessary
      if (bitRead(servoReverse, ch) == 1)
      {
        pots_control_val = MAX_CONTROL_VAL - pots_control_val + MIN_CONTROL_VAL;
      }
      
      //Min Max Validation
      if (pots_control_val < MIN_CONTROL_VAL)
      {
        pots_control_val = MIN_CONTROL_VAL;
      }
      
      if (pots_control_val > MAX_CONTROL_VAL)
      {
        pots_control_val = MAX_CONTROL_VAL;
      }
    }
    else
    {
      // mapping pot value CH3
      if (ch == 2)
      {
        if (bitRead(servoReverse, 2) == 1)
        {
          pots_control_val = map(analogRead(2), calibration[ch][0], calibration[ch][1], MAX_CONTROL_VAL, MIN_CONTROL_VAL);
        }
        else
        {
          pots_control_val = map(analogRead(2), calibration[ch][0], calibration[ch][1], MIN_CONTROL_VAL, MAX_CONTROL_VAL);
        }
      }
      
      // mapping pot value CH4
      if (ch == 3)
      {
        if (bitRead(servoReverse, 3) == 1)
        {
          pots_control_val = map(analogRead(3), calibration[ch][0], calibration[ch][1], MAX_CONTROL_VAL, MIN_CONTROL_VAL);
        }
        else
        {
          pots_control_val = map(analogRead(3), calibration[ch][0], calibration[ch][1], MIN_CONTROL_VAL, MAX_CONTROL_VAL);
        }
      }
    }
    
    pots_value[ch] = pots_control_val;
  }
}
 
