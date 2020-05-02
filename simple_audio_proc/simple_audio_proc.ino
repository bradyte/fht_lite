///////////////////////////////////////////////////////////////////////////////
/* References
 * http://www.elektronika.kvalitne.cz/ATMEL/necoteorie/transformation/AVRFHT/AVRFHT.html
 * http://www.fourwalledcubicle.com/AVRArticles.php 
 *  
 *  
 *  
 *  
 */
//#include <avr/pgmspace.h>
#include "fht_lite.h"
#include "fht_window.h"
#include "fht_reorder.h"
#include "fht_run.h"
#include "fht_mag_log.h"

///////////////////////////////////////////////////////////////////////////////
/*const int16_t sine_3200[NFHT]  = { // 10 bit 3.2kHz sine wave
511,873,1023,873,511,150,1,150,511,873,1023,873,511,150,0,150,
511,873,1023,873,511,150,1,150,511,873,1023,873,511,150,0,150,
511,873,1023,873,511,150,1,150,511,873,1023,873,511,150,0,150,
511,873,1023,873,511,150,1,150,511,873,1023,873,511,150,0,150,
511,873,1023,873,511,150,1,150,511,873,1023,873,511,150,0,150,
511,873,1023,873,511,150,1,150,511,873,1023,873,511,150,0,150,
511,873,1023,873,511,150,1,150,511,873,1023,873,511,150,0,150,
511,873,1023,873,511,150,1,150,511,873,1023,873,511,150,0,150,
511,873,1023,873,511,150,1,150,511,873,1023,873,511,150,0,150,
511,873,1023,873,511,150,1,150,511,873,1023,873,511,150,0,150,
511,873,1023,873,511,150,1,150,511,873,1023,873,511,150,0,150,
511,873,1023,873,511,150,1,150,511,873,1023,873,511,150,0,150,
511,873,1023,873,511,150,1,150,511,873,1023,873,511,150,0,150,
511,873,1023,873,511,150,1,150,511,873,1023,873,511,150,0,150,
511,873,1023,873,511,150,1,150,511,873,1023,873,511,150,0,150,
511,873,1023,873,511,150,1,150,511,873,1023,873,511,150,0,150,};*/
int16_t sinetable[NFHT] ={0};

void initialize_adc(void)
{
  //TIMSK0  = 0; // turn off timer0 for lower jitter, will not be able to use delay()
  ADCSRA  = ADCSRA_CONFIG;
  ADMUX   = 0; /*(1<<ADLAR)*/ // REFS[0:1] = 0 for AREF
  DIDR0   = 0x01;   // disable digital input of ADC0
}

void setup()
{
  Serial.begin(115200);
  initialize_adc();
  int16_t fc = 1200;
  int16_t f_samp = 25600;

  // generate generic perfect sine table
  for(int i = 0; i < NFHT; i++)
  {
    sinetable[i] = (int16_t)(511*sin(2*PI*fc*i/f_samp)+512);
  }
}


void loop()
{
  while(1)
  {
    //cli();
    for (int i = 0 ; i < NFHT ; i++)  // Capture 256 samples of the signal
    {
      while(!(ADCSRA & (1<<ADIF)));   // wait for ADC complete flag ADIF
      ADCSRA = ADCSRA_CONFIG; // restart adc
      //uint8_t j = ADCH;         // fetch high ADC byte
      //uint8_t m = ADCL;         // fetch low ADC byte
      //int16_t k = (j << 8) | m; // combine ADCH with ADCL to form int
      int16_t k = sinetable[i];   // inject artifical sine wave
      k -= 0x0200;                // k = k - 10-bit/2 to form signed int
      k <<= 6;                    // form into a 16b signed int
      fht_input[i] = k; // put real data into bins
      //delay(1000);
    }
    fht_window();  // fht_input.*_window_func / 2^15
    fht_reorder(); // reorder the data before doing the fht
    fht_run(); // process the data in the fht
    //fht_mag_log(); // take the output of the fht
    //sei();

    
    for (int i = 0; i < NFHT; i++) {Serial.println(fht_input[i], DEC);}
    for (int i = 0; i < 500-NFHT; i++) {Serial.println(0);} // zero pad plotter window
    delay(5000);
  }
}
