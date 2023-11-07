
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
// send and receive data **********************************************************************************************
//*********************************************************************************************************************
bool rf_state = 0;

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
      
      rf_state = 1;
      
      RX_batt_check();
    }
  }
  
/*  
  if (radio.write(&rc_packet, sizeof(rc_packet_size)))
  {
    if (radio.available())
    {
      radio.read(&telemetry_packet, sizeof(telemetry_packet_size));
      
      rf_state = 1;
    }
  }
*/

}
 
