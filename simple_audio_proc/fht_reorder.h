#ifndef _FHT_REORDER_H_
#define _FHT_REORDER_H_


extern const uint8_t __attribute__((used)) _reorder_table[] PROGMEM = { 
240 , 15 ,
248 , 31 ,
244 , 47 ,
252 , 63 ,
242 , 79 ,
250 , 95 ,
246 , 111 ,
254 , 127 ,
241 , 143 ,
249 , 159 ,
245 , 175 ,
253 , 191 ,
243 , 207 ,
251 , 223 ,
247 , 239 ,
224 , 7 ,
232 , 23 ,
228 , 39 ,
236 , 55 ,
226 , 71 ,
234 , 87 ,
230 , 103 ,
238 , 119 ,
225 , 135 ,
233 , 151 ,
229 , 167 ,
237 , 183 ,
227 , 199 ,
235 , 215 ,
208 , 11 ,
216 , 27 ,
212 , 43 ,
220 , 59 ,
210 , 75 ,
218 , 91 ,
214 , 107 ,
222 , 123 ,
209 , 139 ,
217 , 155 ,
213 , 171 ,
221 , 187 ,
211 , 203 ,
192 , 3 ,
200 , 19 ,
196 , 35 ,
204 , 51 ,
194 , 67 ,
202 , 83 ,
198 , 99 ,
206 , 115 ,
193 , 131 ,
201 , 147 ,
197 , 163 ,
205 , 179 ,
176 , 13 ,
184 , 29 ,
180 , 45 ,
188 , 61 ,
178 , 77 ,
186 , 93 ,
182 , 109 ,
190 , 125 ,
177 , 141 ,
185 , 157 ,
181 , 173 ,
160 , 5 ,
168 , 21 ,
164 , 37 ,
172 , 53 ,
162 , 69 ,
170 , 85 ,
166 , 101 ,
174 , 117 ,
161 , 133 ,
169 , 149 ,
144 , 9 ,
152 , 25 ,
148 , 41 ,
156 , 57 ,
146 , 73 ,
154 , 89 ,
150 , 105 ,
158 , 121 ,
145 , 137 ,
128 , 1 ,
136 , 17 ,
132 , 33 ,
140 , 49 ,
130 , 65 ,
138 , 81 ,
134 , 97 ,
142 , 113 ,
112 , 14 ,
120 , 30 ,
116 , 46 ,
124 , 62 ,
114 , 78 ,
122 , 94 ,
118 , 110 ,
96 , 6 ,
104 , 22 ,
100 , 38 ,
108 , 54 ,
98 , 70 ,
106 , 86 ,
80 , 10 ,
88 , 26 ,
84 , 42 ,
92 , 58 ,
82 , 74 ,
64 , 2 ,
72 , 18 ,
68 , 34 ,
76 , 50 ,
48 , 12 ,
56 , 28 ,
52 , 44 ,
32 , 4 ,
40 , 20 ,
16 , 8 ,};


static inline void fht_reorder(void) {
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
  // these next 3 lines could be optimized out by chaging the lookup table only works for FHT_N <= 128
  "clr r27 \n"    // R27 = 0
  "lsl r26 \n"    // R26 = R26 << 1 ; C = bit 7 of R26
  "rol r27 \n"    // R27 = R27 << 1 + C ; C = bit 7 of R27
  "subi r26, lo8(-(fht_input)) \n" // R26 (XL) = fht_input & 0x00FF pointer
  "sbci r27, hi8(-(fht_input)) \n" // R27 (XH) = fht_input & 0xFF00 pointer
  "ld r2,x+ \n" // fetch source
  "ld r3,x \n"

  // find destination
  "lpm r28,z+ \n" // R28 = ZH; Z = Z+1 ; load data from SRAM
  // these next 3 lines could be optimized out by chaging the lookup table only works for FHT_N <= 128
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


#endif