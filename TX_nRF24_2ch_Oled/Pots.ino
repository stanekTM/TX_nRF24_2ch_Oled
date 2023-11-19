
//*********************************************************************************************************************
// Macro for read pots, joysticks, values, applying calibration and rules
//*********************************************************************************************************************
void read_pots()
{
  for (int ch = 0; ch < CHANNELS; ch++)
  {
    int value_pots = MID_CONTROL_VAL;
    
    pots[ch] = analogRead(ch);
    
    // MIN range correction
    //if (pots[ch] < pot_calib_min[ch]) pots[ch] = pot_calib_min[ch];
    
    // MAX range correction
    //if (pots[ch] > pot_calib_max[ch]) pots[ch] = pot_calib_max[ch];
    
    
    // EPA check
    unsigned short left_epa_value = 0;
    unsigned short right_epa_value = 0;
    
    if (ch == 0)
    {
      right_epa_value = 500 - (500 * epa[0] / EPA_MAX);
       left_epa_value = 500 - (500 * epa[2] / EPA_MAX);
    }
    
    if (ch == 1)
    {
       left_epa_value = 500 - (500 * epa[1] / EPA_MAX);
      right_epa_value = 500 - (500 * epa[3] / EPA_MAX);
    }
    
    unsigned short mid_control_trim = MID_CONTROL_VAL + subTrim[ch];
    unsigned short min_control_trim = MIN_CONTROL_VAL + left_epa_value;
    unsigned short max_control_trim = MAX_CONTROL_VAL - left_epa_value;
    
    
    // Convert analog value to pots value
    if (pots[ch] < (pot_calib_mid[ch] - DEAD_ZONE))
    {
      if (ch == 0) min_control_trim = MIN_CONTROL_VAL + right_epa_value;
      
      value_pots = map(pots[ch], pot_calib_min[ch], pot_calib_mid[ch] - DEAD_ZONE, min_control_trim, mid_control_trim);
      
      // EXPO
      if (expo[ch] > 0) value_pots = calc_expo(mid_control_trim, value_pots, min_control_trim, expo[ch]);
      
    }
    else if (pots[ch] > (pot_calib_mid[ch] + DEAD_ZONE))
    {
      if (ch == 1) max_control_trim = MAX_CONTROL_VAL - right_epa_value;
      
      value_pots = map(pots[ch], pot_calib_mid[ch] + DEAD_ZONE, pot_calib_max[ch], mid_control_trim, max_control_trim);
      
      // EXPO
      if (expo[ch] > 0) value_pots = calc_expo(mid_control_trim, value_pots, max_control_trim, expo[ch]);
      
    }
    else
    {
      value_pots = mid_control_trim;
    }
    
    
    // Check reverse and applying reverse value if necessary
    if (bitRead(reverse, ch) == 1) value_pots = MAX_CONTROL_VAL - value_pots + MIN_CONTROL_VAL;
    
    //Min Max validation
    //if (value_pots < MIN_CONTROL_VAL) value_pots = MIN_CONTROL_VAL;
    
    //if (value_pots > MAX_CONTROL_VAL) value_pots = MAX_CONTROL_VAL;
    
    
    pots_value[ch] = value_pots;

    //Serial.println(pots_value[1]);
  }
}
 
