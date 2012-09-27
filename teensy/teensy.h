/*
 * teensy.h
 *
 * Created: 9/27/2012 11:19:31 AM
 *  Author: Administrator
 */ 


#ifndef TEENSY_H_
#define TEENSY_H_

uint16_t adc_read(uint8_t channel);

#define ADC_REF_POWER (1 << REFS0)
#define ADC_REF_INTERNAL ((1 << REFS1) | (1 << REFS0))
#define ADC_REF_EXTERNAL (0)

#if F_CPU == 16000000L
#define ADC_PRESCALER ((1 << ADPS2) | (1 << ADPS1))
#elif F_CPU == 8000000L
#define ADC_PRESCALER ((1 << ADPS2) | (1 << ADPS0))
#elif F_CPU == 4000000L
#define ADC_PRESCALER (1 << ADPS2)
#elif F_CPU == 2000000L
#define ADC_PRESCALER ((1 << ADPS1) | (1 << ADPS0))
#elif F_CPU == 1000000L
#define ADC_PRESCALER (1 << ADPS1)
#else
#define ADC_PRESCALER (1 << ADPS0)
#endif

#endif /* TEENSY_H_ */