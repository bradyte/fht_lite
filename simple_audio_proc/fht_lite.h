///////////////////////////////////////////////////////////////////////////////
/* References
 * http://www.elektronika.kvalitne.cz/ATMEL/necoteorie/transformation/AVRFHT/AVRFHT.html
 * http://www.fourwalledcubicle.com/AVRArticles.php 
 *  
 *  
 *  
 *  
 */

#ifndef _FHT_LITE_H_ // include guard
#define _FHT_LITE_H_

#define NFHT 256
#define LOG_N 8
#define ADC_BIT_RES 10
#define ADCSRA_CONFIG (1<<ADEN) | (1<<ADSC) | (1<<ADATE) | (1<<ADPS2) | (1<<ADPS0)
#define ADCSRA_CONFIG_F (1<<ADEN) | (1<<ADSC) | (1<<ADATE) | (1<<ADIF) |  (1<<ADPS2) | (1<<ADPS0)
#define ADMUX_CONFIG 0x00
#define DIDR0_CONFIG 0x01

#define STRINGIFY_(a) #a
#define STRINGIFY(a) STRINGIFY_(a)

#define _R_V 8 // reorder value - used for reorder list

int16_t __attribute__((used)) fht_input[(NFHT)]; // FHT input data buffer
uint8_t __attribute__((used)) fht_log_out[(NFHT/2)];
//int16_t sinetable[NFHT] ={0};





extern const int16_t __attribute__((used)) _window_func[] PROGMEM   = { 
    0,    5,   20,   44,   79,  123,  177,  241,  315,  398,  491,  593,  705,  827,  958, 1098,
 1247, 1406, 1573, 1749, 1935, 2128, 2331, 2542, 2761, 2989, 3224, 3468, 3719, 3978, 4244, 4518,
 4799, 5087, 5381, 5682, 5990, 6304, 6624, 6950, 7282, 7619, 7961, 8308, 8661, 9018, 9379, 9745,
10114,10487,10864,11245,11628,12014,12403,12794,13188,13583,13980,14378,14778,15179,15580,15982,
16384,16786,17188,17589,17990,18390,18788,19185,19580,19974,20365,20754,21140,21523,21904,22281,
22654,23023,23389,23750,24107,24460,24807,25149,25486,25818,26144,26464,26778,27086,27387,27681,
27969,28250,28524,28790,29049,29300,29544,29779,30007,30226,30437,30640,30833,31019,31195,31362,
31521,31670,31810,31941,32063,32175,32277,32370,32453,32527,32591,32645,32689,32724,32748,32763,
32767,32763,32748,32724,32689,32645,32591,32527,32453,32370,32277,32175,32063,31941,31810,31670,
31521,31362,31195,31019,30833,30640,30437,30226,30007,29779,29544,29300,29049,28790,28524,28250,
27969,27681,27387,27086,26778,26464,26144,25818,25486,25149,24807,24460,24107,23750,23389,23023,
22654,22281,21904,21523,21140,20754,20365,19974,19580,19185,18788,18390,17990,17589,17188,16786,
16384,15982,15580,15179,14778,14378,13980,13583,13188,12794,12403,12014,11628,11245,10864,10487,
10114, 9745, 9379, 9018, 8661, 8308, 7961, 7619, 7282, 6950, 6624, 6304, 5990, 5682, 5381, 5087,
 4799, 4518, 4244, 3978, 3719, 3468, 3224, 2989, 2761, 2542, 2331, 2128, 1935, 1749, 1573, 1406,
 1247, 1098,  958,  827,  705,  593,  491,  398,  315,  241,  177,  123,   79,   44,   20,    5,
};

extern const uint8_t __attribute__((used)) _reorder_table[] PROGMEM = { 
  240,   15,  248,   31,  244,   47,  252,   63,  242,   79,  250,   95,  246,  111,  254,  127,
  241,  143,  249,  159,  245,  175,  253,  191,  243,  207,  251,  223,  247,  239,  224,    7,
  232,   23,  228,   39,  236,   55,  226,   71,  234,   87,  230,  103,  238,  119,  225,  135,
  233,  151,  229,  167,  237,  183,  227,  199,  235,  215,  208,   11,  216,   27,  212,   43,
  220,   59,  210,   75,  218,   91,  214,  107,  222,  123,  209,  139,  217,  155,  213,  171,
  221,  187,  211,  203,  192,    3,  200,   19,  196,   35,  204,   51,  194,   67,  202,   83,
  198,   99,  206,  115,  193,  131,  201,  147,  197,  163,  205,  179,  176,   13,  184,   29,
  180,   45,  188,   61,  178,   77,  186,   93,  182,  109,  190,  125,  177,  141,  185,  157,
  181,  173,  160,    5,  168,   21,  164,   37,  172,   53,  162,   69,  170,   85,  166,  101,
  174,  117,  161,  133,  169,  149,  144,    9,  152,   25,  148,   41,  156,   57,  146,   73,
  154,   89,  150,  105,  158,  121,  145,  137,  128,    1,  136,   17,  132,   33,  140,   49,
  130,   65,  138,   81,  134,   97,  142,  113,  112,   14,  120,   30,  116,   46,  124,   62,
  114,   78,  122,   94,  118,  110,   96,    6,  104,   22,  100,   38,  108,   54,   98,   70,
  106,   86,   80,   10,   88,   26,   84,   42,   92,   58,   82,   74,   64,    2,   72,   18,
   68,   34,   76,   50,   48,   12,   56,   28,   52,   44,   32,    4,   40,   20,   16,    8,
};

 extern const uint8_t __attribute__((used)) _log_table[] PROGMEM = {
    0,    0,    8,   13,   16,   19,   21,   22,   24,   25,   27,   28,   29,   30,   30,   31,
   32,   33,   33,   34,   35,   35,   36,   36,   37,   37,   38,   38,   38,   39,   39,   40,
   40,   40,   41,   41,   41,   42,   42,   42,   43,   43,   43,   43,   44,   44,   44,   44,
   45,   45,   45,   45,   46,   46,   46,   46,   46,   47,   47,   47,   47,   47,   48,   48,
   48,   48,   48,   49,   49,   49,   49,   49,   49,   50,   50,   50,   50,   50,   50,   50,
   51,   51,   51,   51,   51,   51,   51,   52,   52,   52,   52,   52,   52,   52,   52,   53,
   53,   53,   53,   53,   53,   53,   53,   53,   54,   54,   54,   54,   54,   54,   54,   54,
   54,   55,   55,   55,   55,   55,   55,   55,   55,   55,   55,   56,   56,   56,   56,   56,
   56,   56,   56,   56,   56,   56,   57,   57,   57,   57,   57,   57,   57,   57,   57,   57,
   57,   57,   58,   58,   58,   58,   58,   58,   58,   58,   58,   58,   58,   58,   58,   59,
   59,   59,   59,   59,   59,   59,   59,   59,   59,   59,   59,   59,   59,   59,   60,   60,
   60,   60,   60,   60,   60,   60,   60,   60,   60,   60,   60,   60,   60,   60,   61,   61,
   61,   61,   61,   61,   61,   61,   61,   61,   61,   61,   61,   61,   61,   61,   61,   62,
   62,   62,   62,   62,   62,   62,   62,   62,   62,   62,   62,   62,   62,   62,   62,   62,
   62,   63,   63,   63,   63,   63,   63,   63,   63,   63,   63,   63,   63,   63,   63,   63,
   63,   63,   63,   63,   63,   63,   64,   64,   64,   64,   64,   64,   64,   64,   64,   64,
};

extern const int16_t __attribute__((used)) _cas_constants[] PROGMEM  = {
30274,12540,
23170,23170,
12540,30274,
32138,6393,
30274,12540,
27246,18205,
23170,23170,
18205,27246,
12540,30274,
6393,32138,
32610,3212,
32138,6393,
31357,9512,
30274,12540,
28899,15447,
27246,18205,
25330,20788,
23170,23170,
20788,25330,
18205,27246,
15447,28899,
12540,30274,
9512,31357,
6393,32138,
3212,32610,
32729,1608,
32610,3212,
32413,4808,
32138,6393,
31786,7962,
31357,9512,
30853,11039,
30274,12540,
29622,14010,
28899,15447,
28106,16846,
27246,18205,
26320,19520,
25330,20788,
24279,22006,
23170,23170,
22006,24279,
20788,25330,
19520,26320,
18205,27246,
16846,28106,
15447,28899,
14010,29622,
12540,30274,
11039,30853,
9512,31357,
7962,31786,
6393,32138,
4808,32413,
3212,32610,
1608,32729,
32758,804,
32729,1608,
32679,2411,
32610,3212,
32522,4011,
32413,4808,
32286,5602,
32138,6393,
31972,7180,
31786,7962,
31581,8740,
31357,9512,
31114,10279,
30853,11039,
30572,11793,
30274,12540,
29957,13279,
29622,14010,
29269,14733,
28899,15447,
28511,16151,
28106,16846,
27684,17531,
27246,18205,
26791,18868,
26320,19520,
25833,20160,
25330,20788,
24812,21403,
24279,22006,
23732,22595,
23170,23170,
22595,23732,
22006,24279,
21403,24812,
20788,25330,
20160,25833,
19520,26320,
18868,26791,
18205,27246,
17531,27684,
16846,28106,
16151,28511,
15447,28899,
14733,29269,
14010,29622,
13279,29957,
12540,30274,
11793,30572,
11039,30853,
10279,31114,
9512,31357,
8740,31581,
7962,31786,
7180,31972,
6393,32138,
5602,32286,
4808,32413,
4011,32522,
3212,32610,
2411,32679,
1608,32729,
804,32758,};



/*
void initialize_sine(int16_t fc)
{
  // generate generic perfect sine table
  // add offset and scaling to emulate 10-bit Arduino ADC read
  int16_t f_samp = 38461; // generic f_samp which gives a cleanly periodic signal
  for(int i = 0; i < NFHT; i++)
  {
    sinetable[i] = (int16_t)(511 * sin(2 * PI * fc * i / f_samp) + 512);
  }
}*/

#endif
