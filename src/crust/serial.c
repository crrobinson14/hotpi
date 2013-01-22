/**
 * Crust - HotPi zone controller client-side bridge (talks to Filling via RS-485)
 * Copyright (c) 2012-2013 Chad Robinson
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "crust.h"

#define SERIAL_BUFFER_SIZE 32

// There's really no sense passing these around as pointers on a microcontroller
uint8_t serial_bytes_out;
uint8_t serial_buffer_out[SERIAL_BUFFER_SIZE];

uint8_t serial_bytes_in;
uint8_t serial_buffer_in[SERIAL_BUFFER_SIZE];

/**
 * Flush the serial port.
 */
void serial_flush( void )
{
	volatile uint8_t x;

	while (UCSR1A & _BV(RXC1)) {
		x = UDR1;
	}
}

/**
 * Send the serial port buffer.
 */
void serial_write(void) {
	uint8_t i;

	for (i = 0; i < SERIAL_BUFFER_SIZE && serial_buffer_out[i] != 0x00; i++) {
		loop_until_bit_is_set(UCSR1A, UDRE1);

		UDR1 = serial_buffer_out[i];
	}
}

/**
 * Transmit a status message on the serial port.
 */
void serial_send(void) {
	uint8_t i;

	// We only write to the serial port when the timer overflows.
	if (!(TIFR1 | _BV(TOV1))) {
		return;
	}

	// Reset the overflow flag for next time.
	TIFR1 |= _BV(TOV1);

	// Yeah, we could memcpy this but we're playing with some of this in debug...
	serial_buffer_out[0] = 'D';
	serial_buffer_out[1] = zone_demand[0];
	serial_buffer_out[2] = zone_demand[1];
	serial_buffer_out[3] = zone_demand[2];
	serial_buffer_out[4] = zone_demand[3];
	serial_buffer_out[5] = zone_demand[4];
	serial_buffer_out[6] = zone_demand[5];
	serial_buffer_out[7] = zone_demand[6];
	serial_buffer_out[8] = zone_demand[7];
	serial_buffer_out[9] = '\r';
	serial_buffer_out[10] = '\n';

	serial_buffer_out[11] = 'F';
	serial_buffer_out[12] = zone_force[0];
	serial_buffer_out[13] = zone_force[1];
	serial_buffer_out[14] = zone_force[2];
	serial_buffer_out[15] = zone_force[3];
	serial_buffer_out[16] = zone_force[4];
	serial_buffer_out[17] = zone_force[5];
	serial_buffer_out[18] = zone_force[6];
	serial_buffer_out[19] = zone_force[7];
	serial_buffer_out[20] = '\r';
	serial_buffer_out[21] = '\n';

	serial_buffer_out[22] = 0x00;
	serial_write();
}

/**
 * Read a line from the serial port. We deal with buffer overflows here, but
 * it's up to the caller to determine whether the buffer is actually valid.
 */
void serial_read(void) {
	uint8_t count;

	// Read at most 16 characters in one go, just in case we're getting saturated.
	for (count = 0; count < 16 && (UCSR1A & _BV(RXC1)); count++) {
		// Store the incoming character.
		serial_buffer_in[serial_bytes_in] = UDR1;
		serial_bytes_in++;

		// If our buffer overflows, truncate it so we can sync back up.
		if (serial_bytes_in >= SERIAL_BUFFER_SIZE) {
			serial_bytes_in = 0;
		}
	}
}

/**
 * Initialize the serial port and timer.
 */
void serial_init(void) {
	// Set up for 9600 baud, 8N1.
	UCSR1B |= _BV(RXEN1) | _BV(TXEN1);
	UCSR1C |= _BV(UCSZ11) | _BV(UCSZ10);

	UBRR1H = 0;
	UBRR1L = 103;

	// Set up the timer with a 256-bit prescaler. At 16MHz this means it will
	// "tick" 62500 times per second, and "overflow" every 1.05sec. And yes,
	// we're setting the entire register to 0 except this bit.
	TCCR1B = _BV(CS12);

	// Start the timer fresh.
	TIFR1 |= _BV(TOV1);
}
