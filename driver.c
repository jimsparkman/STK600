#include "atmega2560_reg.h"
#include "atmega2560_port.h"

#define ALL_LEDS_ON 0x00
#define ALL_LEDS_OFF 0xFF
#define ALL_OUTPUT 0xFF
#define ALL_INPUT 0x00
#define MAX_COUNT 15624
#define MAX_LED 7

int main(void)
{
	unsigned char leds = 0, switches, timer_high, timer_low, data, record = 0;
	unsigned int timer_count, address = 0;
	Write(DDRB,ALL_OUTPUT);
	Write(DDRD,ALL_INPUT);
	Write(PORTB,ALL_LEDS_ON);
	*((volatile unsigned char *)TCCR1B) |= ((1 << CS10) | (1 << CS11));
	USART_Init();

	while(1) {	
		Read(PIND,switches);
		Read(TCNT1H,timer_high);
		Read(TCNT1L,timer_low);
		timer_count = (timer_high << 8) | timer_low;
		
		if (UCSR0A & (1<<RXC0)) {
			data = USART_Receive();
			if (data == 0x0D) {
				USART_Transmit(0x0D);
				USART_Transmit(0x0A);
			}
			else if (data == 0x1A) {
				if (record == 0)
					record = 1;
				else
					record = 0;	
			}
			else if (data == 0x19) {
				record = 0;
				for (unsigned int i = 0;i<address;i++) {
					USART_Transmit(EEPROM_read(i));
				}
				address = 0;
			}
			else if (record == 1) {
				USART_Transmit(data);
				EEPROM_write(address,data);
				address++;
			}
			else
				USART_Transmit(data);
		}

		if (timer_count >= MAX_COUNT) {
			Write(TCNT1H,0x00);
			Write(TCNT1L,0x00);
			leds++;
			
			if (leds > MAX_LED)
				leds = 0;
							
			if ((unsigned char)(~switches) > 0x00)
				if ((unsigned char)(1<<leds) >= (unsigned char)(~switches))
					leds = 0;
					
			Write(PORTB,~(1<<leds));
		}
	}

	return(0);
}