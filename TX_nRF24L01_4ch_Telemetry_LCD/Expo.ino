
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
    unsigned short trimVal = trimServoMid - mid_control_val;
    value = (max_control_val + trimVal) - value + (min_control_val + trimVal);
    inMax = (max_control_val + trimVal) - inMax + (min_control_val + trimVal);
    
    value = (value - trimServoMid) * exp((((double)value - trimServoMid) - (inMax - trimServoMid)) / dV ) + trimServoMid;
    return (max_control_val + trimVal) - value + (min_control_val + trimVal);
  }
}
 
