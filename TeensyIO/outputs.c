/*
 * outputs.c
 */ 

#include "teensy.h"

/*
 * Handle an output-control message.
 *
 * Note that we have a very simple protocol here. The first byte is the
 * message type, and we only have one ('O' - Output). Commands are
 * LF-terminated to make them easier to debug in a terminal application.
 *
 * An output command looks like:
 *     O0000000011
 *
 * where 'r' corresponds to a relay channel, and 'd' corresponds to an
 * LED. '0' means turn off the device. '1' means turn it on. Any other
 * value means "leave it alone".
 */
void outputs_process(void) {
	// We are expecting exactly 12 bytes ('O', RELAYx8, DIODEx2).
	if (serial_bytes_in < 11)
		return;
	
	// We don't use any clever logic here - it's really not necessary

	// Relay 1: PB0
	if (serial_buffer_in[1] == '0') PORTB &= ~_BV(PORTB0);
	else if (serial_buffer_in[1] == '1') PORTB |= _BV(PORTB0);

	// Relay 2: PB1
	if (serial_buffer_in[2] == '0') PORTB &= ~_BV(PORTB1);
	else if (serial_buffer_in[2] == '1') PORTB |= _BV(PORTB1);

	// Relay 3: PB2
	if (serial_buffer_in[3] == '0') PORTB &= ~_BV(PORTB2);
	else if (serial_buffer_in[3] == '1') PORTB |= _BV(PORTB2);

	// Relay 4: PB3
	if (serial_buffer_in[4] == '0') PORTB &= ~_BV(PORTB3);
	else if (serial_buffer_in[4] == '1') PORTB |= _BV(PORTB3);

	// Relay 5: PB7
	if (serial_buffer_in[5] == '0') PORTB &= ~_BV(PORTB7);
	else if (serial_buffer_in[5] == '1') PORTB |= _BV(PORTB7);

	// Relay 6: PD0
	if (serial_buffer_in[6] == '0') PORTD &= ~_BV(PORTD0);
	else if (serial_buffer_in[6] == '1') PORTD |= _BV(PORTD0);

	// Relay 7: PD1
	if (serial_buffer_in[7] == '0') PORTD &= ~_BV(PORTD1);
	else if (serial_buffer_in[7] == '1') PORTD |= _BV(PORTD1);

	// Relay 8: PD5
	if (serial_buffer_in[8] == '0') PORTD &= ~_BV(PORTD5);
	else if (serial_buffer_in[8] == '1') PORTD |= _BV(PORTD5);

	// Diode 0: PB6
	if (serial_buffer_in[9] == '0') PORTB &= ~_BV(PORTB6);
	else if (serial_buffer_in[9] == '1') PORTB |= _BV(PORTB6);

	// Diode 1: PE6
	if (serial_buffer_in[10] == '0') PORTE &= ~_BV(PORTE6);
	else if (serial_buffer_in[10] == '1') PORTE |= _BV(PORTE6);
	
	serial_bytes_in = 0;
}

/* Configure output settings */
void outputs_init(void) {
	// Set up the pins we want configured as outputs
	DDRB |= _BV(DDB0) |
	        _BV(DDB1) |
	        _BV(DDB2) |
		    _BV(DDB3) |
			_BV(DDB7);
	DDRD |= _BV(DDD0) |
	        _BV(DDD1) |
			_BV(DDD5);
	DDRE |= _BV(DDE6);
	
	// Set our initial outputs to OFF for safety
    PORTB = 0;
	PORTD = 0;
	PORTE = 0;
}
