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
	uint8_t counter = 0;
	
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
	
	PORTB = 0;
	PORTD = 0;
//	PORTB |= _BV(PORTB0);
//	PORTB |= _BV(PORTB1);
//	PORTB |= _BV(PORTB3);
//	PORTB |= _BV(PORTB7);

    while(1) {
		_delay_ms(50);

		// We read the buttons on a fast timer so we can get fast
		// reaction times. We debounce them in the reader.
		inputs_read();

		// Process any incoming commands
		serial_read();
		if (serial_bytes_in) {
			switch (serial_buffer_in[0]) {
				case 'O': outputs_process(); break;
				default: serial_bytes_in = 0; break;
			}
		}

		// Medium speed: read ADC channels in rotation
		if (counter % 3 == 0) {
			// Read the next ADC channel, in rotation
			adc_read_next();
		}

		// The rest of the routines we do less often.
		if (counter >= 36) {
			counter = 0;		
		
			// Send our status string
			serial_send();
		}
		
		counter++;
    }
}
