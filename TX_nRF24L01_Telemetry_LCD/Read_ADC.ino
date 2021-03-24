//************************************************************************************************************************************************************************
// Input ADC sampling rate for reading analog input pots (1000 KHz is maximum for stable reading)
//************************************************************************************************************************************************************************
void read_adc_setup() {

  ADCSRA &= ~((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0));   // Remove bit settings from Arduino library

#if F_CPU == 16000000L
  ADCSRA |= (1 << ADPS2);                                  // Set ADC prescaler to 16 (1000 KHz)
#else
  ADCSRA |= (1 << ADPS1) | (1 << ADPS0);                     // Set ADC prescaler to 8 (1000 KHz)
#endif


  ADMUX |= (1 << REFS0);
  ADMUX &= ~(1 << REFS1);                                  // Avcc(+5v) as voltage reference

  ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0)); // ADC in free-running mode

  ADCSRA |= (1 << ADEN);                                   // Turn on ADC
  ADCSRA |= (1 << ADSC);                                   // Do an initial conversion this one is the slowest to ensure that everything is up and running

  DIDR0 = 0x3F;                                            // Digital circuit on analog ports disabled
  DIDR1 = 0x03;                                            // Disable digital input on AIN0/1 
}

//************************************************************************************************************************************************************************
// Macro for directly read ADC value on all analog ports
//************************************************************************************************************************************************************************
unsigned short int read_adc(unsigned char channel) {
  
  ADMUX &= 0xF0;                        //Clear the older channel that was read
  ADMUX |= channel;                     //Defines the new ADC channel to be read
  ADCSRA |= (1 << ADSC);                //Starts a new conversion
  while (ADCSRA & (1 << ADSC));         //Wait until the conversion is done
  return ADCW;                          //Returns the ADC value of the chosen channel
}
 
