
//************************************************************************************************************************************************************************
// Macro for read pots, joysticks, values, applying calibration and rules
//************************************************************************************************************************************************************************
void read_pots()
{
  for (int i = 0; i < CHANNELS; i++)
  {
    int pots_control_val = MID_CONTROL_VAL;

    raw_pots[i] = read_adc(i);
    
    //Serial.println(pots_value[1]); //print value on a serial monitor

    // only for throttle and steering ch
    if (i < 2)
    {
      // Applying calibration mapping
      // In case of surface TX, Left and Right rotation rate should be same.
      // So, Longer side length is used for both side
      int gap = calibration[i][1] - centerPos[i];
      int gapTemp = centerPos[i] - calibration[i][0];

      // Select longer side
      if (gap < gapTemp) gap = gapTemp;

      // Calculate Center offset
      int centerOffset = POT_CENTER - centerPos[i];

      // Applying initial value with center offset
      pots[i] = raw_pots[i] + centerOffset;

      // range out correction
      if (pots[i] < calibration[i][0] + centerOffset) pots[i] = calibration[i][0] + centerOffset;
      if (pots[i] > calibration[i][1] + centerOffset) pots[i] = calibration[i][1] + centerOffset;
      
      
      // EPA
      int epaVal = 0;
      int epaVal_bwd = 0;
      
      epaVal = 500 - (500 * epa[i] / 100);
      
      if (i == 1) epaVal_bwd = 500 - (500 * epa[2] / 100);
      
      unsigned short trimServoMid = MID_CONTROL_VAL + subTrim[i]; 
      unsigned short trimServoMin = MIN_CONTROL_VAL + epaVal + subTrim[i];
      unsigned short trimServoMax = MAX_CONTROL_VAL - epaVal + subTrim[i];

      // Convert Analog Value to pots value
      if (pots[i]  < (POT_CENTER - deadBand))
      {
        if (i == 1) trimServoMin = MIN_CONTROL_VAL + epaVal_bwd + subTrim[i];
        
        pots_control_val = map(pots[i], POT_CENTER - gap, POT_CENTER - deadBand, trimServoMin, trimServoMid);
        
        // EXPO
        if (expo[i] > 0) pots_control_val = calc_expo(trimServoMid, pots_control_val, trimServoMin, expo[i]);
      }
      else if (pots[i]  > (POT_CENTER + deadBand))
      {
        pots_control_val = map(pots[i], POT_CENTER + deadBand, POT_CENTER + gap - 1, trimServoMid, trimServoMax);
        
        // EXPO
        if (expo[i] > 0) pots_control_val = calc_expo(trimServoMid, pots_control_val, trimServoMax, expo[i]);
      }
      else
      {
        pots_control_val = trimServoMid;
      }
      
      
      // Check Servo Reversing and applying Reverse value if necessary
      if (bitRead(servoReverse, i) == 1)
      {
        pots_control_val = MAX_CONTROL_VAL - pots_control_val + MIN_CONTROL_VAL;
      }
      
      
      //Min Max Validation
      if (pots_control_val < MIN_CONTROL_VAL) pots_control_val = MIN_CONTROL_VAL;
      if (pots_control_val > MAX_CONTROL_VAL) pots_control_val = MAX_CONTROL_VAL;
    }
    else
    {
      // mapping pot value CH3
      if (i == 2)
      {
        if (bitRead(servoReverse, 2) == 1)
        {
          pots_control_val = map(read_adc(2), calibration[i][0], calibration[i][1], MAX_CONTROL_VAL, MIN_CONTROL_VAL);
        }
        else
        {
          pots_control_val = map(read_adc(2), calibration[i][0], calibration[i][1], MIN_CONTROL_VAL, MAX_CONTROL_VAL);
        }
      }
      
      // mapping pot value CH4
      if (i == 3)
      {
        if (bitRead(servoReverse, 3) == 1)
        {
          pots_control_val = map(read_adc(3), calibration[i][0], calibration[i][1], MAX_CONTROL_VAL, MIN_CONTROL_VAL);
        }
        else
        {
          pots_control_val = map(read_adc(3), calibration[i][0], calibration[i][1], MIN_CONTROL_VAL, MAX_CONTROL_VAL);
        }
      }
    }
    
    pots_value[i] = pots_control_val;
  }
}
 
