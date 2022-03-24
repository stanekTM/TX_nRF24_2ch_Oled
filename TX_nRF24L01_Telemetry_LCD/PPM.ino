//************************************************************************************************************************************************************************
// Macro for read pots, joysticks, values, applying calibration and rules
//************************************************************************************************************************************************************************
void read_pots() {
  
  for (int i = 0; i < CHANNELS; i++) {
    
    int tempPPM = servo_mid;

    raw_Pots[i] = read_adc(i);

//    Serial.println(ppm[1]); //print value ​​on a serial monitor

    // only for throttle and steering ch
    if ( i < 2) {
      
      // Applying calibration mapping
      // In case of surface TX, Left and Right rotation rate should be same.
      // So, Longer side length is used for both side
      int gap = calibration[i][1] - centerPos[i];
      int gapTemp = centerPos[i] - calibration[i][0];

      // Select longer side
      if (gap < gapTemp) gap = gapTemp;

      // Calculate Center offset
      int centerOffset = potCenter - centerPos[i];

      // Applying initial value with center offset
      pots[i] = raw_Pots[i] + centerOffset;

      // range out correction
      if (pots[i] < calibration[i][0] + centerOffset) pots[i] = calibration[i][0] + centerOffset;
      if (pots[i] > calibration[i][1] + centerOffset) pots[i] = calibration[i][1] + centerOffset;
      
      
      // EPA
      int epaVal = 0;
      int epaVal_bwd = 0;
      
      epaVal = 500 - (500 * epa[i] / 100);
      
      if (i == 1) epaVal_bwd = 500 - (500 * epa[2] / 100);
      
      unsigned short trimServoMid = servo_mid + subTrim[i]; 
      unsigned short trimServoMin = servo_min + epaVal + subTrim[i];
      unsigned short trimServoMax = servo_max - epaVal + subTrim[i];

      // Convert Analog Value to PPM Value
      if (pots[i]  < (potCenter - deadBand)) {
        
        if (i == 1) trimServoMin = servo_min + epaVal_bwd + subTrim[i];
        
        tempPPM = map(pots[i], potCenter - gap, potCenter - deadBand, trimServoMin, trimServoMid);
        
        // EXPO
        if (expo[i] > 0) tempPPM = calc_expo(trimServoMid, tempPPM, trimServoMin, expo[i]);
      }
      else if (pots[i]  > (potCenter + deadBand)) {
        tempPPM = map(pots[i], potCenter + deadBand, potCenter + gap - 1, trimServoMid, trimServoMax);
        
        // EXPO
        if (expo[i] > 0) tempPPM = calc_expo(trimServoMid, tempPPM, trimServoMax, expo[i]);
      }
      else {
        tempPPM = trimServoMid;
      }
      
      
      // Check Servo Reversing and applying Reverse value if necessary
      if (bitRead(servoReverse, i) == 1) {
        tempPPM = servo_max - tempPPM + servo_min;
      }
      
      
      //Min Max Validation
      if (tempPPM < servo_min) tempPPM = servo_min;
      if (tempPPM > servo_max) tempPPM = servo_max;
    }
    else {
      
      // ppm mapping pot CH3
      if (i == 2) {
        
        if (bitRead(servoReverse, 2) == 1) {
          tempPPM = map(read_adc(2), calibration[i][0], calibration[i][1], servo_max, servo_min);
        }
        else {
          tempPPM = map(read_adc(2), calibration[i][0], calibration[i][1], servo_min, servo_max);
        }
      }
      
      // ppm mapping pot CH4
      if (i == 3) {
        
        if (bitRead(servoReverse, 3) == 1) {
          tempPPM = map(read_adc(3), calibration[i][0], calibration[i][1], servo_max, servo_min);
        }
        else {
          tempPPM = map(read_adc(3), calibration[i][0], calibration[i][1], servo_min, servo_max);
        }
      }
    }
    ppm[i] = tempPPM;
  }
}
 
