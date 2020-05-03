///////////////////////////////////////////////////////////////////////////////
/* References
 * http://www.elektronika.kvalitne.cz/ATMEL/necoteorie/transformation/AVRFHT/AVRFHT.html
 * http://www.fourwalledcubicle.com/AVRArticles.php 
 *  
 *  
 *  
 *  
 */

#ifndef _fht_lite_h // include guard
#define _fht_lite_h

#define NFHT 256
#define LOG_N 8
#define ADC_BIT_RES 10
#define ADCSRA_CONFIG (1<<ADEN) | (1<<ADSC) | (1<<ADATE) |  (1<<ADPS2) | (1<<ADPS0)

#define STRINGIFY_(a) #a
#define STRINGIFY(a) STRINGIFY_(a)

#define _R_V 8 // reorder value - used for reorder list

int16_t __attribute__((used)) fht_input[(NFHT)]; // FHT input data buffer


#endif
