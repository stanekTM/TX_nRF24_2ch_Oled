
//*********************************************************************************************************************
// Setup radio comunication
//*********************************************************************************************************************
RF24 radio(PIN_CE, PIN_CSN); //setup CE and CSN pins

void radio_setup()
{
  radio.begin();
  radio.setAutoAck(true);
  radio.enableAckPayload();
  radio.enableDynamicPayloads();
  radio.setRetries(5, 5);
  radio.setChannel(RADIO_CHANNEL);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MIN); //RF24_PA_MIN (-18dBm), RF24_PA_LOW (-12dBm), RF24_PA_HIGH (-6dbm), RF24_PA_MAX (0dBm)
  radio.stopListening();
  radio.openWritingPipe(address);
}

//*********************************************************************************************************************
// this structure defines the sent data in bytes (structure size max. 32 bytes)
//*********************************************************************************************************************
struct rc_packet_size
{
  unsigned int ch1;
  unsigned int ch2;
  //unsigned int ch3;
  //unsigned int ch4;
  //unsigned int ch5;
};
rc_packet_size rc_packet;

//*********************************************************************************************************************
// this struct defines data, which are embedded inside the ACK payload
//*********************************************************************************************************************
struct telemetry_packet_size
{
  byte rssi;    // not used yet
  byte batt_A1;
  byte batt_A2; // not used yet
};
telemetry_packet_size telemetry_packet;

//*********************************************************************************************************************
// RX battery status check.
// If RX battery voltage < RX_MONITORED_VOLTAGE = TX display reports "RX batt low!" in an interval of 1s
//*********************************************************************************************************************
unsigned int rx_batt_volt = 0;
bool low_batt_detect = 0;
bool previous_state_batt;
bool rx_batt_state = 1;
unsigned long rx_batt_time = 0;

void RX_batt_check()
{
  //rx_batt_volt = (telemetry_packet.batt_A1 * 4) / 2;

  rx_batt_volt = map(telemetry_packet.batt_A1, 0, 255, 0, 420 /*RX_BATTERY_VOLTAGE*/);
  
  low_batt_detect = rx_batt_volt <= 335 /*RX_MONITORED_VOLTAGE*/;
  
  if (low_batt_detect)
  {
    previous_state_batt = 1;
    
    if (millis() - rx_batt_time > 1000) //1s
    {
      rx_batt_time = millis();
      
      rx_batt_state = !rx_batt_state;
    }
  }
  low_batt_detect = previous_state_batt;
  
  Serial.println(low_batt_detect);
}

//*********************************************************************************************************************
// send and receive data **********************************************************************************************
//*********************************************************************************************************************
bool rf_off_state = 0;

void send_and_receive_data()
{
  rc_packet.ch1 = pots_value[0]; //A0
  rc_packet.ch2 = pots_value[1]; //A1
  //rc_packet.ch3 = pots_value[2]; //A2
  //rc_packet.ch4 = pots_value[3]; //A3
  
  
  if (radio.write(&rc_packet, sizeof(rc_packet_size)))
  {
    if (radio.isAckPayloadAvailable())
    {
      radio.read(&telemetry_packet, sizeof(telemetry_packet_size));
      
      rf_off_state = 1;
    }
  }
  
/*  
  if (radio.write(&rc_packet, sizeof(rc_packet_size)))
  {
    if (radio.available())
    {
      radio.read(&telemetry_packet, sizeof(telemetry_packet_size));
      
      rf_off_state = 1;
    }
  }
*/

}
 
