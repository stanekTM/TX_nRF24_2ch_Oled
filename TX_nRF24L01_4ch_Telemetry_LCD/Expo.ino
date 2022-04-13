
//************************************************************************************************************************************************************************
// EXPO Eq.
// Y=X*EXPO((X-EndPoint)/dV))
// proper dV value : 300 ~ 700
//************************************************************************************************************************************************************************
int calc_expo(unsigned short trimServoMid, unsigned short value, unsigned short inMax, unsigned short dV)
{
  if (value == trimServoMid) return value;
  if (dV <= 0) return value;
  
  dV = 9 - dV; //1 ~ 9 -> 9 ~ 1 conversion

  dV = map(dV, 1, 9, 300, 700);
  
  if (value > trimServoMid)
  {
    value = (value - trimServoMid) * exp((((double)value - trimServoMid) - (inMax - trimServoMid)) / dV ) + trimServoMid;
    return value;
  }
  else
  {
    unsigned short trimVal = trimServoMid - MID_CONTROL_VAL;
    value = (MAX_CONTROL_VAL + trimVal) - value + (MIN_CONTROL_VAL + trimVal);
    inMax = (MAX_CONTROL_VAL + trimVal) - inMax + (MIN_CONTROL_VAL + trimVal);
    
    value = (value - trimServoMid) * exp((((double)value - trimServoMid) - (inMax - trimServoMid)) / dV ) + trimServoMid;
    return (MAX_CONTROL_VAL + trimVal) - value + (MIN_CONTROL_VAL + trimVal);
  }
}
 
