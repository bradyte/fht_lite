#include <avr/pgmspace.h>
#include "fht_lite.h"
#include "fht_window.h"
#include "fht_reorder.h"
#include "fht_run.h"
#include "fht_mag_log.h"

///////////////////////////////////////////////////////////////////////////////
int16_t sinetable[NFHT] ={0};

void initialize_adc(void)
{
  //TIMSK0  = 0; // turn off timer0 for lower jitter, will not be able to use delay()
  ADCSRA  = ADCSRA_CONFIG;
  ADMUX   = 0; /*(1<<ADLAR)*/ // REFS[0:1] = 0 for AREF
  DIDR0   = 0x01;   // disable digital input of ADC0
}

void initialize_sine(int16_t fc)
{
  // generate generic perfect sine table
  // add offset and scaling to emulate 10-bit Arduino ADC read
  int16_t f_samp = 25600; // generic f_samp which gives a cleanly periodic signal
  for(int i = 0; i < NFHT; i++)
  {
    sinetable[i] = (int16_t)(511 * sin(2 * PI * fc * i / f_samp) + 512);
  }
}

void setup()
{
  Serial.begin(115200);
  initialize_adc();
  initialize_sine(1500);  
}

void c_fht_window(void)
{
  uint8_t r20, r28, r29, r30, r31;

  r28 = (uint8_t)((uint16_t)(&fht_input) & 0x00ff);
  r29 = (uint8_t)(((uint16_t)(&fht_input) >> 8) % 0x00ff);
  r30 = (uint8_t)((uint16_t)(&_window_func) & 0x00ff);
  r31 = (uint8_t)(((uint16_t)(&_window_func) >> 8) % 0x00ff);
  r20 = 256 & 0xff;
  
}


void loop()
{
  while(1)
  {
    //cli();
    //unsigned long tmr = micros();
    for (int i = 0 ; i < NFHT ; i++)  // Capture 256 samples of the signal
    {
      while(!(ADCSRA & (1<<ADIF)));   // wait for ADC complete flag ADIF
      ADCSRA = ADCSRA_CONFIG;     // restart adc
      //uint8_t j = ADCH;         // fetch high ADC byte
      //uint8_t m = ADCL;         // fetch low ADC byte
      //int16_t k = (j << 8) | m; // combine ADCH with ADCL to form int
      int16_t k = sinetable[i];   // inject artifical sine wave
      k -= 0x0200;                // k = k - 10-bit/2 to form signed int
      k <<= 6;                    // shift 10b int to form into a 16b signed int
      fht_input[i] = k;           // put real data into bins
    }
    fht_window();  // fht_input.*_window_func / 2^15
    fht_reorder(); // reorder the data before doing the fht
    //fht_run(); // process the data in the fht
    //fht_mag_log(); // take the output of the fht
    //sei();
    //tmr = micros() - tmr;
    //Serial.println(tmr);
    delay(1000);
    //tmr = 0;
    
    //for (int i = 0; i < NFHT/2; i++) {Serial.println(fht_input[i], DEC);}
    //for (int i = 0; i < 500-NFHT/2; i++) {Serial.println(0);} // zero pad plotter window
    //delay(5000);
  }
}
