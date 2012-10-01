/**
 * @file
 * Teensy Off-board Driver
 */ 

#include "teensy.h"

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

uint8_t button1_debounce, button2_debounce, button3_debounce;
uint8_t button1_pressed, button2_pressed, button3_pressed;
uint8_t serial_count;
uint8_t serial_buffer[20];

/* Main entry point */
int main(void)
{
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
		_delay_ms(900); // Check against 9600 baud?

		// Read all ADC channels and buttons		
		adc_read_all();
		inputs_read();
		
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
