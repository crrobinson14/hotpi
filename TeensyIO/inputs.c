/*
 * inputs.c
 */ 

#include "teensy.h"

uint8_t input_values[INPUT_COUNT];
static uint8_t input_debounce[INPUT_COUNT];

void inputs_read() {
	// TODO: PC7 is not reading reliably
	if (input_debounce[0] > 0) {
		input_debounce[0]--;
	} else if (!bit_is_set(PINC, PINC7)) {
		input_values[0]++;
		input_debounce[0] = 10;
	}
	
	if (input_debounce[1] > 0) {
		input_debounce[1]--;
	} else if (!bit_is_set(PINC, PINC6)) {
		input_values[1]++;
		input_debounce[1] = 10;
	}

	if (input_debounce[2] > 0) {
		input_debounce[2]--;
	} else if (!bit_is_set(PINB, PINB5)) {
		input_values[2]++;
		input_debounce[2] = 10;
	}
}

void inputs_init(void) {
	// Set the three pins as inputs	
	DDRC &= ~(_BV(PORTC6));
	DDRC &= ~(_BV(PORTC7));
	DDRB &= ~(_BV(PORTB5));
	
	// Enable the pullups on the pins
	PORTC |= _BV(PORTC6);
	PORTC |= _BV(PORTC7);
	PORTB |= _BV(PORTB5);
}
