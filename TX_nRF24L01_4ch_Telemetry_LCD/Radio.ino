
//*********************************************************************************************************************
// Setup radio comunication
//*********************************************************************************************************************
RF24 radio(PIN_CE, PIN_CSN); //setup CE and CSN pins
uint8_t invert_address = ~address[5]; //invert bits for reading so that telemetry packets have a different address

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
  radio.openReadingPipe(1, invert_address);
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
  uint8_t rssi;  //not used yet
  float batt_A1;
  float batt_A2; //not used yet
};
telemetry_packet_size telemetry_packet;

//*********************************************************************************************************************
// Gain time after losing RF data or turning off the RX, display "RX off!" and disable the RX battery status
//*********************************************************************************************************************
unsigned long rx_time = 0;
int RFstate;
int RXbattstate;

void receive_time()
{
  if (millis() - rx_time > 1000) //1s
  {
    RFstate = 1;
    RXbattstate = 0;
  }
}

//*********************************************************************************************************************
// RX battery status check. If the battery voltage RX is < RX_MONITORED_VOLTAGE = the TX display reports "RXbatt LOW!"
// at 1s interval and the RX LED flashes at 0.5s interval
//*********************************************************************************************************************
unsigned long RXbattTime = 0;

void RX_batt_check()
{
  if (telemetry_packet.batt_A1 <= RX_MONITORED_VOLTAGE)
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
  }
  
  if (telemetry_packet.batt_A1 >= RX_MONITORED_VOLTAGE)
  {
    RXbattstate = 0;
  }
  
  //Serial.println(telemetry_packet.batt_A1);
}

//*********************************************************************************************************************
// send and receive data **********************************************************************************************
//*********************************************************************************************************************
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
      
      rx_time = millis();
      RFstate = 0;
    }
  }
}
 
