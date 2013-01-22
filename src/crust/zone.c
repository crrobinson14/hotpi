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

uint8_t zone_force[8] = { '0', '0', '0', '0', '0', '0', '0', '0' };
uint8_t zone_demand[8] = { '0', '0', '0', '0', '0', '0', '0', '0' };

/**
 * Read the demand status flags for the zone inputs.
 */
void zone_read(void) {
	zone_demand[0] = (PINB & PINB0) ? '1' : '0';
	zone_demand[1] = (PINB & PINB1) ? '1' : '0';
	zone_demand[2] = (PINB & PINB2) ? '1' : '0';
	zone_demand[3] = (PINB & PINB3) ? '1' : '0';
	zone_demand[4] = (PINB & PINB4) ? '1' : '0';
	zone_demand[5] = (PINB & PINB5) ? '1' : '0';
	zone_demand[6] = (PINB & PINB6) ? '1' : '0';
	zone_demand[7] = (PINB & PINB7) ? '1' : '0';
}

/**
 * Force a given zone on or off.
 */
void zone_force(uint8_t zone, uint8_t state) {
	uint8_t reg, pin;

	switch (zone) {
		case 1: reg = PORTF; pin = PORTF0; break;
		case 2: reg = PORTF; pin = PORTF1; break;
		case 3: reg = PORTF; pin = PORTF4; break;
		case 4: reg = PORTF; pin = PORTF5; break;
		case 5: reg = PORTF; pin = PORTF6; break;
		case 6: reg = PORTF; pin = PORTF7; break;
		case 7: reg = PORTC; pin = PORTC6; break;
		case 8: reg = PORTC; pin = PORTC7; break;
		default:
			return;
	}

	if (state) {
		reg |= _BV(pin);
	} else {
		reg &= ~(_BV(pin));
	}
}

/**
 * Set the zone output pins based on serial input, if we have a full message.
 */
void zone_set() {
	uint8_t i;

	// We only have one type of message that we care about, but we do a little
	// basic logic to make expansion easier later.
	if (serial_bytes_in < 1) {
		return;
	}

	switch (serial_buffer_in[0]) {
		case 'F':
			// F........\r\n
			if (serial_bytes_in < 11) {
				return;
			}

			// Set the zones as demanded
			for (i = 1; i <= 8; i++) {
				if (serial_buffer_in[i] == '0') {
					zone_force(i, 0);
				} else if (serial_buffer_in[i] == '0') {
					zone_force(i, 1);
				}
			}

			// We could be smarter about this...
			serial_bytes_in = 0;
			break;
	}
}

/**
 * Initialize the zone ports.
 */
void zone_init(void) {
	// PortB is all inputs - Z1OPEN .. Z8OPEN
	DDRB = 0x00;
	PORTB = 0xFF;

	// Set outputs as required
	DDRF = _BV(PORTF0) | _BV(PORTF1) | _BV(PORTF4) | _BV(PORTF5) | _BV(PORTF6) | _BV(PORTF7);
	PORTF &= ~(_BV(PORTF0) | _BV(PORTF1) | _BV(PORTF4) | _BV(PORTF5) | _BV(PORTF6) | _BV(PORTF7));

	DDRC = _BV(PORTC6) | _BV(PORTC7);
	PORTC &= ~(_BV(PORTC6) | _BV(PORTC7));
}
