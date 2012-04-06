/*************************************************************************
Title:    testing output to a HD44780 based LCD display.
Author:   Peter Fleury  <pfleury@gmx.ch>  http://jump.to/fleury
File:     $Id: test_lcd.c,v 1.6 2004/12/10 13:53:59 peter Exp $
Software: AVR-GCC 3.3
Hardware: HD44780 compatible LCD text display
          ATS90S8515/ATmega if memory-mapped LCD interface is used
          any AVR with 7 free I/O pins if 4-bit IO port mode is used
**************************************************************************/
#include <stdlib.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h> 
#include <avr/interrupt.h>
#include "usart.h"
#include "onewire.h"
#include "ds18x20.h"



#include "uart_hex.c"

#define BAUD 9600                    //definiert die Bautrate für den USART.
#define USART0_RX_BUFFER_SIZE 32      //definiert die größe des Empfangsbuffers. Die Buffergröße kann 2, 4, 8, 16, 32, 64, 128 oder 256 Byte groß sein.
#define USART0_TX_BUFFER_SIZE 32      //definiert die größe des Sendebuffers. Die Buffergröße kann 2, 4, 8, 16, 32, 64, 128 oder 256 Byte groß sein.

void usart0_puts (const char *s)
{
    while (*s)
    {
        usart0_putc(*s);
        s++;
    }
}

uint8_t search_and_start_measurement(uint8_t diff)
{
	uint8_t sensor_id[OW_ROMCODE_SIZE];
#ifndef OW_ONE_BUS
	ow_set_bus(&PINB,&PORTB,&DDRB,PB0);
#endif
	ow_reset();
	DS18X20_find_sensor( &diff, &sensor_id[0] );

	if( diff == OW_PRESENCE_ERR || diff == OW_DATA_ERR ) {
		return diff;
	}
	DS18X20_start_meas(DS18X20_POWER_EXTERN, &sensor_id[0]);
	return diff;
}

uint8_t search_and_display(uint8_t diff)
{
	uint8_t sensor_id[OW_ROMCODE_SIZE];
	int16_t decicelsius;
	uint8_t puffer[20];
	double output;
#ifndef OW_ONE_BUS
	ow_set_bus(&PINB,&PORTB,&DDRB,PB0);
#endif
	ow_reset();
	DS18X20_find_sensor( &diff, &sensor_id[0] );
    
	if( diff == OW_PRESENCE_ERR || diff == OW_DATA_ERR ) {
		return diff;
	}
	if (DS18X20_read_decicelsius( &sensor_id[0], &decicelsius) == DS18X20_OK) {
		output = decicelsius / 10.0;
		sprintf( puffer, " %.2f ", output );
		uart_put_hex(sensor_id, OW_ROMCODE_SIZE);
		usart0_puts( puffer );
		usart0_puts("\r\n");
	}
		
	return diff;
}

void step_down() {
	// Set direction pin
	PORTD |= (1<<5);
	_delay_us(3);
	PORTD |= (1<<6);
	_delay_us(3);
	PORTD &= ~(1<<6);
}

void step_up() {
	// Set direction pin
	PORTD &= ~(1<<5);
	_delay_us(3);
	PORTD |= (1<<6);
	_delay_us(3);
	PORTD &= ~(1<<6);
}


void step_right() {
	// Set direction pin
	PORTD &= ~(1<<3);
	_delay_us(3);
	PORTD |= (1<<4);
	_delay_us(3);
	PORTD &= ~(1<<4);
}

void step_left() {
	// Set direction pin
	PORTD |= (1<<3);
	_delay_us(3);
	PORTD |= (1<<4);
	_delay_us(3);
	PORTD &= ~(1<<4);
}



int main(void) {
	
    usart0_init();
		
	sei();

	DDRD |= (1<<3);
	DDRD |= (1<<4);
	DDRD |= (1<<5);
	DDRD |= (1<<6);

	for (;;) {
		unsigned char command = usart0_getc();
		switch (command) {
			case 'd': step_down(); break;
			case 'u': step_up();  break;
			case 'l': step_left(); break;
			case 'r': step_right(); break;
			case '1': step_left(); step_up();  break;
			case '2': step_right(); step_up(); break;
			case '3': step_left(); step_down(); break;
			case '4': step_right(); step_down(); break;
		}
	}

	return 0;
}



