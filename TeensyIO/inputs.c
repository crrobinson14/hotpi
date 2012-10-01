/*
 * inputs.c
 */ 

#include "teensy.h"

uint8_t input_values[INPUT_COUNT];
static uint8_t input_debounce[INPUT_COUNT];

void inputs_read() {
	 // TODO: Review the debounce intervals
	if (input_debounce[0] > 0) {
		input_debounce[0]--;
	} else if (!input_values[0] && (bit_is_set(PORTC, PORTC6))) {
		input_values[0] = 1;
		input_debounce[0] = 25;
	}
	
	if (input_debounce[1] > 0) {
		input_debounce[1]--;
	} else if (!input_values[1] && bit_is_set(PORTC, PORTC7)) {
		input_values[1] = 1;
		input_debounce[1] = 25;
	}

	if (input_debounce[2] > 0) {
		input_debounce[2]--;
	} else if (!input_values[2] && bit_is_set(PORTB, PORTB2)) {
		input_values[2] = 1;
		input_debounce[2] = 25;
	}
}

void inputs_init(void) {
	// Set the three pins as inputs	
	DDRC &= ~(_BV(PORTC6));
	DDRC &= ~(_BV(PORTC7));
	DDRB &= ~(_BV(PORTB2));
	
	// Enable the pullups on the pins
	PORTC |= _BV(PORTC6);
	PORTC |= _BV(PORTC7);
	PORTB |= _BV(PORTB2);
}
