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

int main(void)
{
	// Allow the power-supply to settle down, and initialize the CPU
	clock_prescale_set(clock_div_128);
	_delay_ms(10);
	clock_prescale_set(clock_div_1);
	
	// Initialize our subsystems
	serial_init();

	_delay_ms(100);

    while(1)
    {
        //TODO:: Please write your application code 
    }
}
