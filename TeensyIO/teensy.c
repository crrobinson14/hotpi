/**
 * @file
 * Teensy Off-board Driver
 */ 

#include "teensy.h"

/*
uint16_t timer_read() {
	uint16_t i;
	
	i = TCNT0;
	return i;
}

uint16_t get_timer(uint8_t channel) {
	if (channel == 0) return TCNT1;
	return TCNT3;
}

void init_lcd(void) {
	PORTD = _BV(PD0) | _BV(PD1);
	DDRD = 0;
	TWBR = (F_CPU / 100000UL - 16) / 2;
	//TWBR = 46;
	
	_delay_ms(500);
}
*/

/* Main entry point */
int main(void)
{
	uint8_t slow = 0;
	
	// Allow the power supply to settle down
	clock_prescale_set(clock_div_128);
	_delay_ms(10);
	clock_prescale_set(clock_div_1);
	
	// Initialize our subsystems
	outputs_init();
	adc_init();
	serial_init();
	
	// Settle the CPU
	_delay_ms(100);
	
    while(1) {
		_delay_ms(50);

		// We read the buttons on a fast timer so we can get fast
		// reaction times. We debounce them in the reader.
		inputs_read();

		// The rest of the routines we do less often.
		if (slow++ >= 36) {
			slow = 0;		
		
			// Read the next ADC channel, in rotation
			adc_read_next();
		
			// Send our status string
			serial_send();
		
			// Process any incoming commands
			if (serial_read()) {
				switch (serial_buffer_in[0]) {
					case 'O': outputs_process(); break;
					default: break;
				}

				serial_bytes_in = 0;
			}
		}
    }
}
