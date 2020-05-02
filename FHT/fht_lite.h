// http://www.fourwalledcubicle.com/AVRArticles.php

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
  "ldi r20, $FF \n"               // R20 = 0xFF

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