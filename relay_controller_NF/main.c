#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "NeoControl.h"

#define BAUDRATE 9600
#define BAUD_PSC F_CPU/16/BAUDRATE-1
unsigned int baudPsc = BAUD_PSC;
/*
	Commands:
		10 - Spectrum analyzer relay
		11 - Vector Network Analyzer relay
		12 - Robot relay
		13 - Aux relay
		14 - Aux relay
		20 - Status
		30 - Shut down all
		
		NOTE: Status is sent every time
			
		LIGHTS:
		50 - Red
		51 - Yellow
		52 - Green
		60 - Off
		
*/

//Devices
#define SA 10
#define VNA 11
#define ROBOT 12
#define AUX 13
#define EUT 14
#define STATUS 20
#define SHUT_DOWN_ALL 30
#define SHUT_DOWN_WO_RS 31

//Lights
#define LED_RED 50
#define LED_YELLOW 51
#define LED_GREEN 52
#define LED_OFF 60


void USART0_init();
void USART0_write(uint8_t);

volatile uint8_t f_sa_on = 0;
volatile uint8_t f_vna_on = 0;
volatile uint8_t f_robot_on = 0;
volatile uint8_t f_aux_on = 0;
volatile uint8_t f_eut_on = 0;
volatile uint8_t shut_down_all_on = 0;
volatile uint8_t shut_down_wo_rs_on = 0;

ISR(USART_RX_vect)
{
	uint8_t rx_Val = UDR0;
	
	if(rx_Val == SA){
		f_sa_on = !f_sa_on;
	}else if(rx_Val == VNA){
		f_vna_on = !f_vna_on;
	}else if(rx_Val == ROBOT){
		f_robot_on = !f_robot_on;
	}else if(rx_Val == AUX){
		f_aux_on = !f_aux_on;
	}else if(rx_Val == EUT){
		f_eut_on = !f_eut_on;
    }else if(rx_Val == SHUT_DOWN_ALL){
		shut_down_all_on = 1;
	}else if(rx_Val == SHUT_DOWN_WO_RS){
		shut_down_wo_rs_on = 1;
	}else if(rx_Val == STATUS){

	}else if(rx_Val == LED_RED){
		red();
	}else if(rx_Val == LED_YELLOW){
		yellow();
	}else if(rx_Val == LED_GREEN){
		green();
	}else if(rx_Val == LED_OFF){
		off();
	}
	uint8_t stat = f_sa_on + 2*f_vna_on + 4*f_robot_on + 8*f_aux_on + 16*f_eut_on;
	USART0_write(stat);

}

int main(void)
{
	DDRD = 0xFF;
	PORTD = 0x00;
	
	USART0_init();
	sei();

	off();
	
	green();
	
    while (1) 
    {
		f_sa_on		? (PORTD |= (1<<PORTD2)) : (PORTD &= ~(1<<PORTD2));
		f_vna_on	? (PORTD |= (1<<PORTD3)) : (PORTD &= ~(1<<PORTD3));
		f_robot_on	? (PORTD |= (1<<PORTD4)) : (PORTD &= ~(1<<PORTD4));
		f_aux_on	? (PORTD |= (1<<PORTD5)) : (PORTD &= ~(1<<PORTD5));
		f_eut_on	? (PORTD |= (1<<PORTD6)) : (PORTD &= ~(1<<PORTD6));
		
		if(shut_down_all_on){
			PORTD &= ~(1<<PORTD2);
			PORTD &= ~(1<<PORTD3);
			PORTD &= ~(1<<PORTD4);
			PORTD &= ~(1<<PORTD5);
			PORTD &= ~(1<<PORTD6);
			shut_down_all_on = 0;
			f_sa_on = 0;
			f_vna_on = 0;
			f_robot_on = 0;
			f_aux_on = 0;
			f_eut_on = 0;
		}
		if(shut_down_wo_rs_on){
			PORTD &= ~(1<<PORTD4);
			PORTD &= ~(1<<PORTD5);
			PORTD &= ~(1<<PORTD6);
			shut_down_wo_rs_on = 0;
			f_robot_on = 0;
			f_aux_on = 0;
			f_eut_on = 0;
		}
		_delay_ms(20);
    }
}

void USART0_init()
{
	UBRR0L = (unsigned char)baudPsc;
	UBRR0H = (unsigned char)(baudPsc>>8);
	
	UCSR0B |= (1<<RXEN0)|(1<<RXCIE0)|(1<<TXEN0);
	UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
}

void USART0_write(uint8_t ch)
{
	while(!(UCSR0A & (1<<UDRE0))){};
	UDR0 = ch;
}