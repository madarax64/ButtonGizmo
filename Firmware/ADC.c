
#include <avr/io.h>
#include "ADC.h"
#include <inttypes.h>
#define F_CPU 12000000UL
#include <util/delay.h>

void ADC_Init() {
	ADCSRA = (_BV(ADPS1) | _BV(ADPS2)); //187.5kHZ ADC clock
	ADMUX = (_BV(REFS0));//AVcc with cap at AREF pin
	ADCSRA |= (_BV(ADEN)); //finally, enable the ADC
	ADC_Read(0); //dummy conversion
}

uint16_t ADC_Read_Raw(int channel) {
	ADMUX = ((ADMUX & 0xF0) | (channel & 0x0F)); //retain the upper nibble of ADMUX, overwrite the lower nibble
	ADCSRA |= (_BV(ADSC)); //start the conversion
	while ((ADCSRA & _BV(ADIF)) == 0); //wait till the conversion ends
	ADCSRA |= _BV(ADIF); //reset the conversion flag
	return ADC; //return the 10-bit result
}

uint16_t ADC_Read(int channel) {
	uint16_t result = 0;
	for (int i = 0; i < 5;i++) {
		result += ADC_Read_Raw(channel);
		_delay_ms(2);
	}
	
	return (result / 5);
}