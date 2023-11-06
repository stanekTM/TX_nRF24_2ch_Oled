
//*********************************************************************************************************************
// TX battery voltage display 0-4.2V and if TX battery voltage < TX_MONITORED_VOLTAGE = the display reports "TX low!"
//*********************************************************************************************************************
float tx_batt_volt;
bool tx_low_batt = 0;

void TX_batt_check()
{
  tx_batt_volt = analogRead(7) * (TX_BATTERY_VOLTAGE / 1023);
  
  tx_low_batt = tx_batt_volt <= TX_MONITORED_VOLTAGE;
  
  //Serial.println(tx_batt_volt);
}

//*********************************************************************************************************************
// Voltage calculation from RX telemetry "byte 0-255". 
// Battery voltage display 0-4.2V and if RX battery voltage < RX_MONITORED_VOLTAGE = display reports "RX low!"
//*********************************************************************************************************************
float rx_batt_volt;
bool rx_low_batt = 0;

void RX_batt_check()
{
  rx_batt_volt = telemetry_packet.batt_A1 * (RX_BATTERY_VOLTAGE / 255);
  
  rx_low_batt = rx_batt_volt <= RX_MONITORED_VOLTAGE;
  
  //Serial.println(rx_batt_volt);
}
 
