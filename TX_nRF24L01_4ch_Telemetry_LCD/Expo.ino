
//*********************************************************************************************************************
// EXPO Eq.
// Y = X * EXPO((X-EndPoint)/dv))
// proper dv value : 300 ~ 700
//*********************************************************************************************************************
int calc_expo(unsigned short trim_ppm_mid, unsigned short value, unsigned short in_max, unsigned short dv)
{
  if (value == trim_ppm_mid)
  {
    return value;
  }
  
  if (dv <= 0)
  {
    return value;
  }
  
  dv = 9 - dv; //1 ~ 9 -> 9 ~ 1 conversion
  
  dv = map(dv, 1, 9, 300, 700);
  
  if (value > trim_ppm_mid)
  {
    value = (value - trim_ppm_mid) * exp((((double)value - trim_ppm_mid) - (in_max - trim_ppm_mid)) / dv) + trim_ppm_mid;
    return value;
  }
  else
  {
    unsigned short trim_value = trim_ppm_mid - MID_CONTROL_VAL;
    value = (MAX_CONTROL_VAL + trim_value) - value + (MIN_CONTROL_VAL + trim_value);
    in_max = (MAX_CONTROL_VAL + trim_value) - in_max + (MIN_CONTROL_VAL + trim_value);
    
    value = (value - trim_ppm_mid) * exp((((double)value - trim_ppm_mid) - (in_max - trim_ppm_mid)) / dv) + trim_ppm_mid;
    return (MAX_CONTROL_VAL + trim_value) - value + (MIN_CONTROL_VAL + trim_value);
  }
}
 
