
//************************************************************************************************************************************************************************
// Setup radio comunication **********************************************************************************************************************************************
//************************************************************************************************************************************************************************
RF24 radio(pin_CE, pin_CSN); //setup CE and CSN pins
uint8_t invert_address = ~address[5]; //Invert bits for reading so that telemetry packets have a different address

void radio_setup()
{
  radio.begin();
  radio.setAutoAck(true);          //ensure autoACK is enabled (default true)
  radio.enableAckPayload();        //enable Ack dynamic payloads. This only works on pipes 0&1 by default
  radio.enableDynamicPayloads();   //enable dynamic payloads on all pipes
  
  radio.setRetries(5, 5);          //set the number and delay of retries on failed submit (max. 15 x 250us delay (blocking !), max. 15 retries)
  
  radio.setChannel(radio_channel); //which RF channel to communicate on (0-125, 2.4Ghz + 76 = 2.476Ghz)
  radio.setDataRate(RF24_250KBPS); //RF24_250KBPS (fails for units without +), RF24_1MBPS, RF24_2MBPS
  radio.setPALevel(RF24_PA_MIN);   //RF24_PA_MIN (-18dBm), RF24_PA_LOW (-12dBm), RF24_PA_HIGH (-6dbm), RF24_PA_MAX (0dBm) 
  
  radio.stopListening();           //set the module as transmitter. Stop listening for incoming messages, and switch to transmit mode
  
  radio.openWritingPipe(address);           //open the writing pipe0 (RX_ADDR_P0 + TX_ADDR), but first call "stopListening"
  radio.openReadingPipe(1, invert_address); //open the reading pipe1 (RX_ADDR_P1)
}

//************************************************************************************************************************************************************************
// this structure defines the sent data in bytes (structure size max. 32 bytes) ******************************************************************************************
//************************************************************************************************************************************************************************
struct rc_packet_size
{
  unsigned int ch1;
  unsigned int ch2;
  unsigned int ch3;
  unsigned int ch4;
  unsigned int ch5; //unused channel, only adding byte array RX 5ch
};
rc_packet_size rc_packet; //create a variable with the above structure

//************************************************************************************************************************************************************************
// this struct defines data, which are embedded inside the ACK payload ***************************************************************************************************
//************************************************************************************************************************************************************************
struct telemetry_packet_size
{
  uint8_t rssi;     // not used yet
  float RX_batt_A1;
  float RX_batt_A2; // not used yet
};
telemetry_packet_size telemetry_packet;

//************************************************************************************************************************************************************************
// Gain time after losing RF data or turning off the RX, display "RX-RF OFF!" and disable the RX battery status **********************************************************
//************************************************************************************************************************************************************************
unsigned long lastRxTime = 0;
int RFstate;
int RXbattstate;

void receive_time()
{
  if(millis() >= lastRxTime + 1000) //1s
  {
    RFstate = 1;
    RXbattstate = 0;
  }
}

//************************************************************************************************************************************************************************
// RX battery status check ***********************************************************************************************************************************************
// If the battery voltage RX is < RX_monitored_voltage = the TX display reports "RXbatt LOW!" at 1s interval and the RX LED flashes at 0.5s interval *********************
//************************************************************************************************************************************************************************
unsigned long RXbattTime = 0;

void RX_batt_check()
{
  if (telemetry_packet.RX_batt_A1 <= RX_monitored_voltage)
  {
    if (millis() >= RXbattTime + 1000) //1s
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
  
  if (telemetry_packet.RX_batt_A1 >= RX_monitored_voltage)
  {
    RXbattstate = 0;
  }
//  Serial.println(telemetry_packet.RX_batt_A1); //print value ​​on a serial monitor
}

//************************************************************************************************************************************************************************
// send and receive data *************************************************************************************************************************************************
//************************************************************************************************************************************************************************
void send_and_receive_data()
{
  rc_packet.ch1 = pots_value[0]; //A0
  rc_packet.ch2 = pots_value[1]; //A1
  rc_packet.ch3 = pots_value[2]; //A2
  rc_packet.ch4 = pots_value[3]; //A3
  
  if (radio.write(&rc_packet, sizeof(rc_packet_size)))
  {
    if (radio.isAckPayloadAvailable())
    {
      radio.read(&telemetry_packet, sizeof(telemetry_packet_size));
      
      lastRxTime = millis(); //at this moment we have received the data
      RFstate = 0;
    }
  }
}
 
