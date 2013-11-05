/*
 * adc.c
 */ 

#include "teensy.h"

uint16_t adc_values[ADC_COUNT];

/* Read a channel from the ADC bank */
static void adc_read(uint8_t channel) {
	// PF0/ADC0, PF1/ADC1, PF4/ADC4, PF5/ADC5, PF6/ADC6
	// PF7/ADC7, PD4/ADC8, PD6/ADC9, PD7/ADC10, PB4/ADC11

	// We have to do this with a switch statement because the
	// MUX has some wacky behaviors - MUX5 is in a different
	// register from MUX0..4. Also, we call channel 10 a fake
	// input from the temperature sensor in the MCU.
	switch (channel) {
		// The first six channels require MUX5 to be off
		case 0: ADMUX = _BV(REFS0) | 0; ADCSRB &= ~_BV(MUX5); break;
		case 1: ADMUX = _BV(REFS0) | 1; ADCSRB &= ~_BV(MUX5); break;
		case 2: ADMUX = _BV(REFS0) | 4; ADCSRB &= ~_BV(MUX5); break;
		case 3: ADMUX = _BV(REFS0) | 5; ADCSRB &= ~_BV(MUX5); break;
		case 4: ADMUX = _BV(REFS0) | 6; ADCSRB &= ~_BV(MUX5); break;
		case 5: ADMUX = _BV(REFS0) | 7; ADCSRB &= ~_BV(MUX5); break;
		
		// The others require MUX5 to be on
		case 6: ADMUX = _BV(REFS0) | 0; ADCSRB |= _BV(MUX5); break;
		case 7: ADMUX = _BV(REFS0) | 1; ADCSRB |= _BV(MUX5); break;
		case 8: ADMUX = _BV(REFS0) | 2; ADCSRB |= _BV(MUX5); break;
		case 9: ADMUX = _BV(REFS0) | 3; ADCSRB |= _BV(MUX5); break;
		
		// This is the temperature sensor. Effectively we have 11 possible inputs.
		case 10: ADMUX = _BV(REFS0) | 7; ADCSRB |= _BV(MUX5); break;
	}
	
	// Clear the conversion-complete interrupt flag. It will be set when
	// the conversion is complete. (Per the docs, we write a '1' bit to
	// _clear_ it.
	ADCSRA |= _BV(ADIF);
	
	// Start the conversion and wait for it to complete.
	ADCSRA |= _BV(ADSC);
	loop_until_bit_is_set(ADCSRA, ADIF);
	
	// We must read ADCL first "to ensure that the content
	//  of the registers belongs to the same conversion"
	adc_values[channel] = 0;
	adc_values[channel] = ADCL;
	adc_values[channel] |= (ADCH << 8);
}

/* Read the next ADC value, in rotation */
void adc_read_next(void) {
	static uint8_t next_read = 0;
	
	adc_read(next_read);
	
	next_read++;
	if (next_read >= ADC_COUNT) {
		next_read = 0;
	}
}

/* Initialize the ADC subsystem */
void adc_init(void) {
	// Set ADC pre-scaler to 128. We need the ADC to run slowly because it
	// gets much better accuracy that way. 16MHz/128 = 125kHz, which is
	// good (the docs recommend 50-200kHz.
	ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
	
	// Enable high speed conversions.
	ADCSRB = _BV(ADHSM);
	
	// Disable the digital input pins on our ADC channels. This reduces
	// power consumption and noise on the pins.
	DIDR0 |= _BV(ADC0D) | _BV(ADC1D) | _BV(ADC4D) | _BV(ADC5D) | _BV(ADC6D) | _BV(ADC7D);
	DIDR2 |= _BV(ADC8D) | _BV(ADC9D) | _BV(ADC10D) | _BV(ADC11D);
}
