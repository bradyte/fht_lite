#ifndef _FHT_WINDOW_H_
#define _FHT_WINDOW_H_


extern const int16_t __attribute__((used)) _window_func[] PROGMEM 	= { 
  // w_hann = round(2^14 - 2^14*cos(2*pi*n/256))
0,
5,
20,
44,
79,
123,
177,
241,
315,
398,
491,
593,
705,
827,
958,
1098,
1247,
1406,
1573,
1749,
1935,
2128,
2331,
2542,
2761,
2989,
3224,
3468,
3719,
3978,
4244,
4518,
4799,
5087,
5381,
5682,
5990,
6304,
6624,
6950,
7282,
7619,
7961,
8308,
8661,
9018,
9379,
9745,
10114,
10487,
10864,
11245,
11628,
12014,
12403,
12794,
13188,
13583,
13980,
14378,
14778,
15179,
15580,
15982,
16384,
16786,
17188,
17589,
17990,
18390,
18788,
19185,
19580,
19974,
20365,
20754,
21140,
21523,
21904,
22281,
22654,
23023,
23389,
23750,
24107,
24460,
24807,
25149,
25486,
25818,
26144,
26464,
26778,
27086,
27387,
27681,
27969,
28250,
28524,
28790,
29049,
29300,
29544,
29779,
30007,
30226,
30437,
30640,
30833,
31019,
31195,
31362,
31521,
31670,
31810,
31941,
32063,
32175,
32277,
32370,
32453,
32527,
32591,
32645,
32689,
32724,
32748,
32763,
32768,
32763,
32748,
32724,
32689,
32645,
32591,
32527,
32453,
32370,
32277,
32175,
32063,
31941,
31810,
31670,
31521,
31362,
31195,
31019,
30833,
30640,
30437,
30226,
30007,
29779,
29544,
29300,
29049,
28790,
28524,
28250,
27969,
27681,
27387,
27086,
26778,
26464,
26144,
25818,
25486,
25149,
24807,
24460,
24107,
23750,
23389,
23023,
22654,
22281,
21904,
21523,
21140,
20754,
20365,
19974,
19580,
19185,
18788,
18390,
17990,
17589,
17188,
16786,
16384,
15982,
15580,
15179,
14778,
14378,
13980,
13583,
13188,
12794,
12403,
12014,
11628,
11245,
10864,
10487,
10114,
9745,
9379,
9018,
8661,
8308,
7961,
7619,
7282,
6950,
6624,
6304,
5990,
5682,
5381,
5087,
4799,
4518,
4244,
3978,
3719,
3468,
3224,
2989,
2761,
2542,
2331,
2128,
1935,
1749,
1573,
1406,
1247,
1098,
958,
827,
705,
593,
491,
398,
315,
241,
177,
123,
79,
44,
20,
5,};

static inline void fht_window(void) {
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
  "ldi r20, " STRINGIFY(((NFHT)&(0xff))) " \n"               // R20 = 0xFF

  "1: \n"
  "lpm r22,z+ \n"     // R22 = Z; Z = Z+1 load data from SRAM
  "lpm r23,z+ \n"     // R23 = Z; Z = Z+1
  "ld r16,y \n"       // R16 = Y
  "ldd r17,y+1 \n"    // R17 = Y+1

  // Signed fractional multiply of two 16-bit numbers with 32-bit result.
  // r5:r4:r3:r2 = ( r23:r22 * r21:r20 ) << 1
  "clr r15 \n"        // R15    = 0
  "fmuls r17,r23 \n"  // R1:R0  = ((signed)R17 * (signed)R23) << 1, C is set if bit 15 is set before shift
  "movw r4,r0 \n"     // R5:R4  = R1:R0
  "fmul r16,r22 \n"   // R1:R0  = ((unsigned)R16 * (unsigned)R22) << 1
  "adc r4,r15 \n"     // R4     = R4 + R15 + C
  "movw r2,r0 \n"     // R3:R2  = R1:R0 
  "fmulsu r17,r22 \n" // R1:R0  = ((signed)R17 * (unsigned)R22) << 1
  "sbc r5,r15 \n"     // R5     = R5 - R15 - C
  "add r3,r0 \n"      // R3     = R3 + R0
  "adc r4,r1 \n"      // R4     = R4 + R1
  "adc r5,r15 \n"     // R5     = R5 + R15 + C
  "fmulsu r23,r16 \n" // R1:R0  = ((signed)R23 * (unsigned)R16) << 1
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

#endif
