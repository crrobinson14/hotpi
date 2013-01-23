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

void zones_off(c
	zone_force_set(1, 0);
	zone_force_set(2, 0);
	zone_force_set(3, 0);
	zone_force_set(4, 0);
	zone_force_set(5, 0);
	zone_force_set(6, 0);
	zone_force_set(7, 0);
	zone_force_set(8, 0);
}

/**
 * _delay_ms() can only delay up to 262.14/F_CPU(MHz), or 16.38 ms.
 */
void delay_sec(void) {
	uint8_t counter;

	for (counter = 0; counter < 100; counter++) {
		_delay_ms(10);
	}
}

int main(void)
{
	// Allow the power-supply to settle down, and initialize the CPU
	clock_prescale_set(clock_div_128);
	_delay_ms(16);
	clock_prescale_set(clock_div_1);

	// Initialize our subsystems
	zone_init();
	serial_init();

	_delay_ms(16);

	// Debugging code
	while (1) {
		uint8_t counter;

		for (counter = 1; counter <= 8; counter++) {
			zones_off();
			zone_force_set(counter, 1);

			delay_sec();
			delay_sec();
			delay_sec();
			delay_sec();
			delay_sec();
		}
	}

	while(1) {
		// Check the zone pins
		zone_read();

		// serial_send() checks the timer to see if it should actually do its thing.
		serial_send();

		// Get whatever bytes we can from the serial port.
		serial_read();

		// See if we have a complete message.
		zone_set();
	}
}
