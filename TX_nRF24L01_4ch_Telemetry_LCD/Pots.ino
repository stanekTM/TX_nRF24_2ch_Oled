
//*********************************************************************************************************************
// Macro for read pots, joysticks, values, applying calibration and rules
//*********************************************************************************************************************
void read_pots()
{
  for (int ch = 0; ch < CHANNELS; ch++)
  {
    int value_pots = MID_CONTROL_VAL;
    
    pots[ch] = analogRead(ch);
    
    // Applying calibration mapping
    // In case of surface TX, Left and Right rotation rate should be same
    // So, Longer side length is used for both side
    int gap = pot_calib_max[ch] - pot_calib_mid[ch];
    int gapTemp = pot_calib_mid[ch] - pot_calib_min[ch];
    
    // Select longer side
    if (gap < gapTemp) gap = gapTemp;
    
    // MIN range correction
    //if (pots[ch] < pot_calib_min[ch]) pots[ch] = pot_calib_min[ch];
    
    // MAX range correction
    //if (pots[ch] > pot_calib_max[ch]) pots[ch] = pot_calib_max[ch];
    
    
    // EPA check
    int epaVal = 0;
    int epaVal_bwd = 0;
    
    if (ch == 0)
    {
      epaVal_bwd = 500 - (500 * epa[0] / EPA_MAX);
          epaVal = 500 - (500 * epa[2] / EPA_MAX);
    }
    
    if (ch == 1)
    {
          epaVal = 500 - (500 * epa[1] / EPA_MAX);
      epaVal_bwd = 500 - (500 * epa[3] / EPA_MAX);
    }
    
    unsigned short mid_control_trim = MID_CONTROL_VAL + subTrim[ch];
    unsigned short min_control_trim = MIN_CONTROL_VAL + epaVal + subTrim[ch];
    unsigned short max_control_trim = MAX_CONTROL_VAL - epaVal + subTrim[ch];
    
    
    // Convert analog value to pots value
    if (pots[ch] < (pot_calib_mid[ch] - deadBand))
    {
      if (ch == 0) min_control_trim = MIN_CONTROL_VAL + epaVal_bwd + subTrim[ch];
      
      //value_pots = map(pots[ch], pot_calib_mid[ch] - gap, pot_calib_mid[ch] - deadBand, min_control_trim, mid_control_trim);
      value_pots = map(pots[ch], pot_calib_min[ch], pot_calib_mid[ch] - deadBand, min_control_trim, mid_control_trim);
      
      // EXPO
      if (expo[ch] > 0) value_pots = calc_expo(mid_control_trim, value_pots, min_control_trim, expo[ch]);
      
    }
    else if (pots[ch] > (pot_calib_mid[ch] + deadBand))
    {
      if (ch == 1) max_control_trim = MAX_CONTROL_VAL - epaVal_bwd + subTrim[ch];
      
      //value_pots = map(pots[ch], pot_calib_mid[ch] + deadBand, pot_calib_mid[ch] + gap - 1, mid_control_trim, max_control_trim);
      value_pots = map(pots[ch], pot_calib_mid[ch] + deadBand, pot_calib_max[ch], mid_control_trim, max_control_trim);
      
      // EXPO
      if (expo[ch] > 0) value_pots = calc_expo(mid_control_trim, value_pots, max_control_trim, expo[ch]);
      
    }
    else
    {
      value_pots = mid_control_trim;
    }
    
    
    // Check Servo Reversing and applying Reverse value if necessary
    if (bitRead(servoReverse, ch) == 1) value_pots = MAX_CONTROL_VAL - value_pots + MIN_CONTROL_VAL;
    
    //Min Max Validation
    //if (value_pots < MIN_CONTROL_VAL) value_pots = MIN_CONTROL_VAL;
    
    //if (value_pots > MAX_CONTROL_VAL) value_pots = MAX_CONTROL_VAL;
    
    
    pots_value[ch] = value_pots;
    //Serial.println(pots_value[1]);
  }
}
 
