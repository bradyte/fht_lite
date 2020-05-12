#include <avr/pgmspace.h>
#include <Adafruit_NeoPixel.h>
#include "fht_lite.h"
#include "fht_run.h"
#include "fht_mag_log.h"
#include "rgb_lite.h"

// Which pin on the Arduino is connected to the NeoPixels?
//#define TIMEIT
#define PIN       7
#define NUMPIXELS 8 // Popular NeoPixel ring size
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
///////////////////////////////////////////////////////////////////////////////
static inline void _fht_window(void) {
  // this applies a window to the data for better frequency resolution
  // save registers that are getting clobbered
  // avr-gcc requires r2:r17,r28:r29, and r1 cleared
  asm volatile (
  "push r2 \n"    // save R2 on to the stack
  "push r3 \n"    // save R3 on to the stack
  "push r4 \n"    // save R4 on to the stack
  "push r5 \n"    // save R5 on to the stack
  "push r15 \n"   // save R15 on to the stack
  "push r16 \n"   // save R16 on to the stack
  "push r17 \n"   // save R17 on to the stack
  "push r28 \n"   // save R28 on to the stack
  "push r29 \n"   // save R29 on to the stack
  );

  asm volatile (
  "ldi r28, lo8(fht_input) \n"    // R28 (YL) = fht_input & 0x00FF
  "ldi r29, hi8(fht_input) \n"    // R29 (YH) = fht_input & 0xFF00
  "ldi r30, lo8(_window_func) \n" // R30 (ZL) = _window_func & 0x00FF
  "ldi r31, hi8(_window_func) \n" // R31 (ZH) = _window_func & 0xFF00
  "ldi r20, " STRINGIFY(((NFHT)&(0xff))) " \n"               

  "1: \n"
  "lpm r22,z+ \n"     // R22 = Z; Z = Z+1 load data from SRAM
  "lpm r23,z+ \n"     // R23 = Z; Z = Z+1
  "ld r16,y \n"       // R16 = Y
  "ldd r17,y+1 \n"    // R17 = Y+1

  // Signed fractional multiply of two 16-bit numbers with 32-bit result.
  // r5:r4:r3:r2 = ( r23:r22 * r21:r20 ) << 1
  "clr r15 \n"        // R15    = 0
  "muls r17,r23 \n"  // R1:R0  = ((signed)R17 * (signed)R23) << 1, C is set if bit 15 is set before shift
  "movw r4,r0 \n"     // R5:R4  = R1:R0
  "mul r16,r22 \n"   // R1:R0  = ((unsigned)R16 * (unsigned)R22) << 1
  "adc r4,r15 \n"     // R4     = R4 + R15 + C
  "movw r2,r0 \n"     // R3:R2  = R1:R0 
  "mulsu r17,r22 \n" // R1:R0  = ((signed)R17 * (unsigned)R22) << 1
  "sbc r5,r15 \n"     // R5     = R5 - R15 - C
  "add r3,r0 \n"      // R3     = R3 + R0
  "adc r4,r1 \n"      // R4     = R4 + R1
  "adc r5,r15 \n"     // R5     = R5 + R15 + C
  "mulsu r23,r16 \n" // R1:R0  = ((signed)R23 * (unsigned)R16) << 1
  "sbc r5,r15 \n"     // R5     = R5 - R15 - C
  "add r3,r0 \n"      // R3     = R3 + R0
  "adc r4,r1 \n"      // R4     = R4 + R1
  "adc r5,r15 \n"     // R5     = R5 + R15 + C

  "st y+,r4 \n"       // Y = R4, then Y+1
  "st y+,r5 \n"       // Y = R5, then Y+1
  "dec r20 \n"        // R20 = R20 - 1
  "brne 1b \n"        // Branch to 1: if Z flag is true
  : :                 // no input or output
  : "r0", "r20", "r30", "r31", "r22", "r23" // clobber list
  );
                      // get the clobbers off the stack
  asm volatile (
  "pop r29 \n"        // restore R29 from the stack
  "pop r28 \n"        // restore R28 from the stack
  "pop r17 \n"        // restore R17 from the stack
  "pop r16 \n"        // restore R16 from the stack
  "pop r15 \n"        // restore R15 from the stack
  "pop r5 \n"         // restore R5 from the stack
  "pop r4 \n"         // restore R4 from the stack
  "pop r3 \n"         // restore R3 from the stack
  "pop r2 \n"         // restore R2 from the stack
  "clr r1 \n"         // R1 = 0
  );
}

static inline void _fht_reorder(void) {
  // save registers that are getting clobbered
  // avr-gcc requires r2:r17,r28:r29, and r1 cleared
  asm volatile (
  "push r2 \n"    // save R2 on to the stack
  "push r3 \n"    // save R3 on to the stack
  "push r6 \n"    // save R6 on to the stack
  "push r7 \n"    // save R7 on to the stack  
  "push r28 \n"   // save R28 on to the stack
  "push r29 \n"   // save R29 on to the stack
  );

  // move values to bit reversed locations
  asm volatile (
  "ldi r30, lo8(_reorder_table) \n" // load ptr to address of _reorder_table[0] into R30 (ZL)
  "ldi r31, hi8(_reorder_table) \n" // load ptr to address of _reorder_table[1] into R30 (ZL)
  "ldi r20, " STRINGIFY((NFHT/2) - _R_V) " \n" // R20 = 128 - 8 = 120

  // get source sample
  "1: \n"
  "lpm r26,z+ \n" // load data from memory location in ZL into R26 and increment pointer in Z
  "clr r27 \n"                      // R27 = 0
  "lsl r26 \n"                      // R26 = R26 << 1 ; C = bit 7 of R26
  "rol r27 \n"                      // R27 = R27 << 1 + C ; C = bit 7 of R27
  "subi r26, lo8(-(fht_input)) \n"  // R26 = R26 - (-fht_input & 0x00FF)
  "sbci r27, hi8(-(fht_input)) \n"  // R27 = R27 - (-fht_input & 0xFF00)
  "ld r2,x+ \n" // load lower byte pointed to by Z
  "ld r3,x \n"  // load higher byte pointed to by Z

  // find destination
  "lpm r28,z+ \n" // R28 = ZH; Z = Z+1 ; load data from SRAM
  "clr r29 \n" // same here
  "lsl r28 \n" // multiply offset by 2
  "rol r29 \n"
  "subi r28, lo8(-(fht_input)) \n" // add pointer to offset
  "sbci r29, hi8(-(fht_input)) \n"
  "ld r6,y \n" // fetch destination
  "ldd r7,y+1 \n"

  // swap source and destination samples
  "st x,r7 \n"
  "st -x,r6 \n"
  "st y,r2 \n"
  "std y+1,r3 \n"

  // check if done
  "dec r20 \n" // go to next sample
  "brne 1b \n" // finish off if last sample
  : :
  : "r20", "r26", "r27", "r30", "r31" // clobber list
  );

  // get the clobbers off the stack
  asm volatile (
  "pop r29 \n"
  "pop r28 \n"
  "pop r7 \n"
  "pop r6 \n"
  "pop r3 \n"
  "pop r2 \n"
  );
}

void rgb_demo(void) {
  uint8_t rr, gg, bb;
  for(int idx = 0; idx < 256; idx++) {
    for(int i=0; i<NUMPIXELS; i++) {
      rr = pgm_read_byte(&_R[idx]);
      gg = pgm_read_byte(&_G[idx]);
      bb = pgm_read_byte(&_B[idx]);
      
      pixels.setPixelColor(i, pixels.Color(rr, gg, bb));
      pixels.show();
    }
    //Serial.println(rr, DEC);
    delay(10);
  }
}

void setup() {
  Serial.begin(115200);
  //TIMSK0  = 0; // turn off timer0 for lower jitter, will not be able to use delay()
  ADCSRA  = ADCSRA_CONFIG;
  ADMUX   = ADMUX_CONFIG;   // REFS[0:1] = 0 for AREF
  DIDR0   = DIDR0_CONFIG;   // disable digital input of ADC0

  pixels.begin();
  pixels.setBrightness(50);
}

void loop() {
  uint8_t rr, gg, bb;
  uint8_t f_max = 0;
  uint8_t idx = 0;
  //while(1) { rgb_demo();}
  while(1) {
    #ifdef TIMEIT
      unsigned long tmr = micros();
    #endif
    
    //cli();
    for (int i = 0 ; i < NFHT ; i++) {// Capture 256 samples of the signal
      while(!(ADCSRA & (1<<ADIF)));   // wait for ADC complete flag ADIF
      ADCSRA    = ADCSRA_CONFIG_F;            // restart adc
      uint8_t m = ADCL;         // fetch low ADC byte
      uint8_t j = ADCH;         // fetch high ADC byte
      int16_t k = (j << 8) | m; // combine ADCH with ADCL to form int
      //int16_t k = sinetable[i];   // inject artifical sine wave
      k = (k - 0x0200) << 6;      // shift 10b int to form into a 16b signed int
      fht_input[i] = k;           // put real data into bins
    }
    _fht_window();  // fht_input.*_window_func / 2^15
    _fht_reorder(); // reorder the data before doing the fht
    _fht_run(); // process the data in the fht
    _fht_mag_log(); // take the output of the fht
    
    #ifndef TIMEIT
      //Serial.write(255); // send a start byte
      //Serial.write(fht_log_out, NFHT/2); // send out the data
    #endif
    /*for(int i=0; i < NFHT; i++)
    {
      Serial.print(fht_log_out[i],DEC);
      Serial.print(" ");
    }
    Serial.println();
    delay(500);*/

    for(int i=0; i < NUMPIXELS; i++) {
      rr = pgm_read_byte(&_R[fht_log_out[i]]);
      gg = pgm_read_byte(&_G[fht_log_out[i]]);
      bb = pgm_read_byte(&_B[fht_log_out[i]]);
      pixels.setPixelColor(i, pixels.gamma32((pixels.Color(rr, gg, bb))));
      pixels.show();
    }
    delay(10);
    //sei();
    #ifdef TIMEIT
      tmr = micros() - tmr;
      Serial.println(tmr);
      delay(1000);
      tmr = 0;
    #endif

  }
}
