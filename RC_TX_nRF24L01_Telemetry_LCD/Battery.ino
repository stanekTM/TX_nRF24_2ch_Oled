//************************************************************************************************************************************************************************
// Macro to calculate average ADC value, checking battery voltage alarm and value for battery bar
// For minBat do not under default value in config file because arduino will be more unstable.
// About 16%(presumed)of battery is enough to landing ASAP
//************************************************************************************************************************************************************************
void TX_batt_check() {

  // Mapping battery voltage from average from 0 to 4.2V LiPo
  raw_TX_batt = map(read_adc(7), 0, 1023, 0, 420);

  // Calculating correct value voltage for battery bar
  calc_bar_batt = (raw_TX_batt / 10)/* + correct_volt 2 */;

//  Serial.println(calc_bar_batt); // print value ​​on a serial monitor

  // Mapping voltage for drawing battery bar
  perc_batt = map(calc_bar_batt, minBat, maxBat, 1, 10);
  perc_batt = constrain(perc_batt, 1, 10);

//  Serial.println(perc_batt); // print value ​​on a serial monitor
  

  // Low battery beep alarm
  //if (perc_batt <= 1 || battStatus == 0) {
  if (perc_batt <= 1 ) {

    // Set byte 0 to mantain battery alarm even voltage floating +/- on minBat value
    battStatus = 0;

    // Set value to mantain battery bar to min value even voltage is under minBat
    perc_batt = 1;

    // Recall pulse for buzzer
    beep();
  }

  // Buzzer alarm OFF
  else {
    //PORTD &= ~_BV(buzzer);
    noTone(buzzer);    
  }
}
 
