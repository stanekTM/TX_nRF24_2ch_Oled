//************************************************************************************************************************************************************************
// EXPO Eq.
// Y=X*EXPO((X-EndPoint)/dV))
// proper dV value : 300 ~ 700
//************************************************************************************************************************************************************************
int calc_expo(unsigned short  trimServoMid, unsigned short  value, unsigned short inMax, unsigned short dV) {
  
  if (value == trimServoMid) return value;
  if (dV <= 0) return value;
  
  dV = 9 - dV; //1 ~ 9 -> 9 ~ 1 conversion

  dV = map(dV, 1, 9, 300, 700);
  
  if (value > trimServoMid) {
    value = (value - trimServoMid) * exp((((double)value - trimServoMid) - (inMax - trimServoMid)) / dV ) + trimServoMid;
    return value;
  }
  else {
    unsigned short trimVal = trimServoMid - servo_mid;
    value = (servo_max + trimVal) - value + (servo_min + trimVal);
    inMax = (servo_max + trimVal) - inMax + (servo_min + trimVal);
    
    value = (value - trimServoMid) * exp((((double)value - trimServoMid) - (inMax - trimServoMid)) / dV ) + trimServoMid;
    return (servo_max + trimVal) - value + (servo_min + trimVal);
  }
}
 
