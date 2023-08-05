
//************************************************************************************************************************************************************************
// Macro to calculate average ADC value, checking battery voltage alarm and value for battery bar.
// For "TX_MONITORED_VOLTAGE" do not under default value in config file because arduino will be more unstable.
//************************************************************************************************************************************************************************
float val_TX_battery;            // input battery pin value
int calc_bar_batt;               // Calculating correct voltage value voltage for battery bar
int calc_max_batt;
int calc_min_batt;
unsigned char perc_batt;         // Battery bar level indicator (20 pixel) only Min to Max values

void TX_batt_check()
{
  // battery voltage from average from 0 to 4.2V LiPo
  val_TX_battery = read_adc(7) * (TX_BATTERY_VOLTAGE / 1023);

  // Calculating correct value voltage for battery bar
  calc_bar_batt = (val_TX_battery * 10)/* + correct_volt 2 */;
  calc_max_batt = TX_BATTERY_VOLTAGE * 10;
  calc_min_batt = TX_MONITORED_VOLTAGE * 10;
  
  //Serial.println(calc_min_batt);

  // Mapping voltage for drawing battery bar
  perc_batt = map(calc_bar_batt, calc_min_batt, calc_max_batt, 1, 10);
  perc_batt = constrain(perc_batt, 1, 10);
  
  //Serial.println(perc_batt);


  // Low battery beep alarm
  //if (perc_batt <= 1 || battStatus == 0) {
  if (perc_batt <= 1 )
  {
    // Set byte 0 to mantain battery alarm even voltage floating +/- on minBat value
    battStatus = 0;

    // Set value to mantain battery bar to min value even voltage is under minBat
    perc_batt = 1;

    // Recall pulse for buzzer
    beep();
  }
  else
  {
    // Buzzer alarm OFF
    noTone(PIN_BUZZER);
  }
}
 
