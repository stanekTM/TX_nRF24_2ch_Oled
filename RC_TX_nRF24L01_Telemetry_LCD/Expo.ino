//************************************************************************************************************************************************************************
// EXPO Eq.
// Y=X*EXPO((X-EndPoint)/dV))
// proper dV value : 300 ~ 700
//************************************************************************************************************************************************************************
int calc_expo(unsigned short  trimServoCenter, unsigned short  value, unsigned short inMax, unsigned short dV) {

  if (value == trimServoCenter) return value;
  if (dV <= 0) return value;

  dV = 9 - dV; //1 ~ 9 -> 9 ~ 1 conversion

  dV = map(dV, 1, 9, 300, 700);

  if (value > trimServoCenter) {
    value = (value - trimServoCenter) * exp((((double)value - trimServoCenter) - (inMax - trimServoCenter)) / dV ) + trimServoCenter;
    return value;
    }
    else {
      unsigned short trimVal = trimServoCenter - servoCenter;
      value = (servoMax + trimVal) - value + (servoMin + trimVal);
      inMax = (servoMax + trimVal) - inMax + (servoMin + trimVal);

    value = (value - trimServoCenter) * exp((((double)value - trimServoCenter) - (inMax - trimServoCenter)) / dV ) + trimServoCenter;
    return (servoMax + trimVal) - value + (servoMin + trimVal);
  }
}
 
