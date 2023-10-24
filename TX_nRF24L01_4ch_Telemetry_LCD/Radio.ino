
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
// RX battery status check. If the battery voltage RX is < RX_MONITORED_VOLTAGE = the TX display reports "RXbatt LOW!"
// at 1s interval and the RX LED flashes at 0.5s interval
//*********************************************************************************************************************
unsigned long RX_batt_time = 0;
int RX_batt;
bool RX_batt_state = 0;

void RX_batt_check()
{
  RX_batt = ((telemetry_packet.batt_A1 * 4) / 2) - 70;
  
  
  if (RX_batt <= RX_MONITORED_VOLTAGE * 100)
  {
    //RX_batt_state = 1;
    
    if (millis() - RX_batt_time > 1000) //1s
    {
      RX_batt_time = millis();
      
      if (RX_batt_state)
      {
        RX_batt_state = 0;
      }
      else
      {
        RX_batt_state = 1;
      }
    }
  }




  /*if (telemetry_packet.batt_A1 <= RX_MONITORED_VOLTAGE)
  {
    if (millis() - RXbattTime > 1000) //1s
    {
      RXbattTime = millis();
      
      if (RXbattstate)
      {
        RXbattstate = 0;
      }
      else
      {
        RXbattstate = 1;
      }
    }
  }*/
  
  //Serial.println(test2);
}

//*********************************************************************************************************************
// send and receive data **********************************************************************************************
//*********************************************************************************************************************
bool RF_state = 1;

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
      
      RF_state = 0;
      RX_batt_check();
    }
  }
  
/*  
  if (radio.write(&rc_packet, sizeof(rc_packet_size)))
  {
    if (radio.available())
    {
      radio.read(&telemetry_packet, sizeof(telemetry_packet_size));
      
      RFstate = 0;
    }
  }
*/

}
 
