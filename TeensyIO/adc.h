/*
 * adc.h
 */ 


#ifndef ADC_H_
#define ADC_H_

extern void adc_init(void);
extern void adc_read_all(void);

#define ADC_COUNT 11
extern uint16_t adc_values[ADC_COUNT];

#endif /* ADC_H_ */