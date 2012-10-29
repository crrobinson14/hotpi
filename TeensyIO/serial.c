/*
 * serial.c
 */ 

#include "teensy.h"

#define SERIAL_BUFFER_SIZE 32

// There's really no sense passing these around as pointers on a microcontroller
uint8_t serial_buffer_out[SERIAL_BUFFER_SIZE];

uint8_t serial_bytes_in;
uint8_t serial_buffer_in[SERIAL_BUFFER_SIZE];

/* Flush the serial port */
void serial_flush( void )
{
	uint8_t x;
	
	while (UCSR1A & _BV(RXC1)) {
		x = UDR1;
	}		
}

/* Send the serial port buffer */
void serial_write(void) {
	uint8_t i;
	
	for (i = 0; i < SERIAL_BUFFER_SIZE && serial_buffer_out[i] != 0x00; i++) {
		loop_until_bit_is_set(UCSR1A, UDRE1);

		UDR1 = serial_buffer_out[i];
	}
}

/* Transmit a status message on the serial port */
void serial_send(void) {
	uint8_t i;
	
	serial_buffer_out[0] = 'I';
	serial_buffer_out[1] = 0x00;
	serial_write();
	
	for (i = 0; i < ADC_COUNT; i++) {
		serial_buffer_out[0] = ',';
		itoa(adc_values[i], &serial_buffer_out[1], 10);
		serial_write();
	}
	
	for (i = 0; i < INPUT_COUNT; i++) {
		serial_buffer_out[0] = ',';
		itoa(input_values[i], &serial_buffer_out[1], 10);
		input_values[i] = 0;
		serial_write();
	}
	
	serial_buffer_out[0] = '\r';
	serial_buffer_out[1] = '\n';
	serial_buffer_out[2] = 0x00;
	serial_write();
}

/*
 * Read a line from the serial port. We deal with buffer overflows here, but
 * it's up to the caller to determine whether the buffer is actually valid.
 */
void serial_read(void) {
	uint8_t c;
	
	while (UCSR1A & _BV(RXC1)) {
		c = UDR1;
		/*
		serial_buffer_out[0] = c;
		serial_buffer_out[1] = '\r';
		serial_buffer_out[2] = '\n';
		serial_buffer_out[3] = 0x00;
		serial_write();
*/		
		if (c == '\r' || c == '\n') return;
		
		// Save the character, and see if it completes our string
		serial_buffer_in[serial_bytes_in] = c;
		serial_bytes_in++;

		// Otherwise, keep reading characters while we can.
		if (serial_bytes_in >= SERIAL_BUFFER_SIZE) {
			serial_bytes_in = 0;
		}
	}
}

void serial_init(void) {
	//UDR1 is the data register
	
	// Enable the serial port, 9600 baud, 8N1
	UCSR1B |= _BV(RXEN1) | _BV(TXEN1);
	UCSR1C |= _BV(UCSZ11) | _BV(UCSZ10);
	UBRR1H = 0;
	UBRR1L = 103;
}
