/**
 * @file Teensy Off-board Driver
 */ 

#define F_CPU 16000000UL

/*

High-speed loop (every 10ms?):
1. Each Button Flag--.
2. Monitor buttons 1-3. If any button is down and its flag is 0, set its to 25.
3. Read and discard serial characters until we see a 'O'. Save the rest. If the
twelfth character is not a \n, flush the buffer.
4. ADCCheck++.
5. If ADCCheck >= 5:
a. ADCCheck = 0.
b. Call "Read ADC(n);"
c. ADCn++; If ADCn >= 8, ADCn = 0;
6. LowSpeed++.
7. If LowSpeed >= 100:
a. LowSpeed = 0;
b. Call LowSpeed().



LowSpeed:
1. Check serial buffer. If we have a valid command, set outputs as required.
2. Output "Ixxxx,xxxx,xxxx,xxxx,xxxx,xxxx,xxxx,xxxx,xxxx,xxxx,b,b\n". Each
'xxxx' is one ADC channel, 0-1023. (We output the raw values and let the
Raspberry Pi interpret them). Each 'b' is one of the two buttons, 0=not
pressed, 1=pressed.
*/

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <util/twi.h>
#include <stdint.h>

static uint8_t aref = (1 << REFS0); // Default to AREF = VCC

/* Read a channel from the ADC bank */
uint16_t adc_read(uint8_t channel) {
	uint8_t low;
	
	// Set the reference voltage to AVCC, and select the channel
	ADMUX = (1 << REFS0) | (channel & 0x1F);
	
	ADCSRA |= (1 << ADEN);                      // Enable the ADC
	ADCSRB = (1 << ADHSM) | (channel & 0x20);   // High speed mode
	ADCSRA |= (1 << ADSC);                      // Start the conversion
	
	low = ADCL;
	return (ADCH << 8) | low;
}

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

/* Main entry point */
int main(void)
{
	
	CPU_PRESCALE(CPU_125kHz);
	_delay_ms(1);
	CPU_PRESCALE(CPU_8MHz);
	
	// Set LED outputs
	DDRE |= (1 << 2);
	DDRD |= (1 << 0);
	
	// Set timer prescalers to 1024 (0x500)
	TCCR0A = 0;
	TCCR0B = 5;
	
	// Set LED1 on and LED2 off
	PORTE |= (1 << 2);
	PORTB &= ~(1 << 0);
	
	// Do the opposite
	PORTE &= ~(1 << 2);
	PORTB |= (1 << 0);
	
	// Set ADC prescaler to 128. Would 64 be better?
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	
	//DIDR0 = 0xFF;
	
	// Settle the CPU
	_delay_ms(100);
	
	
	
    while(1)
    {
        //TODO:: Please write your application code 
    }
}
