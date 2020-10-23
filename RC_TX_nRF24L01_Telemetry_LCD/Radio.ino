//************************************************************************************************************************************************************************
// Setup radio comunication **********************************************************************************************************************************************
//************************************************************************************************************************************************************************
RF24 radio(CE, CSN); //setup CE and CSN pins

void radio_setup()
{
  radio.begin(); 
  radio.setAutoAck(true);          //ensure autoACK is enabled
  radio.enableAckPayload();        //enable custom ack payloads on the acknowledge packets
  radio.enableDynamicPayloads();   //enable dynamically-sized payloads
  radio.setRetries(5, 5);          //set the number and delay of retries on failed submit (max. 15 x 250us delay (blocking !), max. 15 retries)
  
  radio.setChannel(76);            //which RF channel to communicate on (0-125, 2.4Ghz + 76 = 2.476Ghz) 
  radio.setDataRate(RF24_250KBPS); //RF24_250KBPS (fails for units without +), RF24_1MBPS, RF24_2MBPS
  radio.setPALevel(RF24_PA_MIN);   //RF24_PA_MIN (-18dBm), RF24_PA_LOW (-12dBm), RF24_PA_HIGH (-6dbm), RF24_PA_MAX (0dBm)
  
  radio.stopListening();           //set the module as transmitter. Stop listening for incoming messages, and switch to transmit mode
  
  radio.openWritingPipe(addresses[1]);    //open a pipe for writing via byte array. Call "stopListening" first
  radio.openReadingPipe(1, addresses[0]); //open all the required reading pipes
}

//************************************************************************************************************************************************************************
// this structure defines the sent data in bytes (structure size max. 32 bytes) ******************************************************************************************
//************************************************************************************************************************************************************************
struct packet
{
  unsigned int steering;
  unsigned int throttle;
  unsigned int ch3;
  unsigned int ch4;
  unsigned int ch5;
};
packet rc_data; //create a variable with the above structure

//************************************************************************************************************************************************************************
// this struct defines data, which are embedded inside the ACK payload ***************************************************************************************************
//************************************************************************************************************************************************************************
struct ackPayload
{
  float RXbatt;
};
ackPayload payload;

//************************************************************************************************************************************************************************
// Gain time after losing RF data or turning off the RX, display "RX-RF OFF!" and disable the RX battery status **********************************************************
//************************************************************************************************************************************************************************
unsigned long lastRxTime = 0;
int RFstate;
int RXbattstate;

void receive_time()
{
  if(millis() >= lastRxTime + 1000) //1000 (1second)
  {
    RFstate = 1;
    RXbattstate = 0;
  }
}

//************************************************************************************************************************************************************************
// RX battery status check ***********************************************************************************************************************************************
// If the battery voltage RX 1S LiPo is < 3.3V = the TX display reports "RXbatt LOW!" at 1000ms interval and the RX LED flashes at 500ms interval ************************
//************************************************************************************************************************************************************************
unsigned long RXbattTime = 0;

void RX_batt_check()
{
  if (payload.RXbatt <= RX_monitored_voltage)
  {
    if (millis() >= RXbattTime + 1000) //1000 (1second)
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
  
  if (payload.RXbatt >= RX_monitored_voltage)
  {
    RXbattstate = 0;
  }
}

//************************************************************************************************************************************************************************
// send and receive data *************************************************************************************************************************************************
//************************************************************************************************************************************************************************
void send_and_receive_data()
{
  rc_data.steering = ppm[0]; //A0
  rc_data.throttle = ppm[1]; //A1
  rc_data.ch3      = ppm[2]; //A2
  rc_data.ch4      = ppm[3]; //A3
  
  if (radio.write(&rc_data, sizeof(packet)))
  {
    if (radio.isAckPayloadAvailable())
    {
      radio.read(&payload, sizeof(ackPayload));
      
      lastRxTime = millis(); //at this moment we have received the data 
      RFstate = 0;                                
    }                              
  } 
}
 
