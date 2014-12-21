void USART_Init(){
	//UBRR
	UBRR0H = (unsigned char)(BaudScale>>8);
	UBRR0L = (unsigned char)BaudScale;
	//RX and TX
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	//8 data bit 1 stop bit
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00)|(0<<UPM00)|(0<<UPM01)|(0<<USBS0)|(0<<UMSEL01)|(0<<UMSEL00);
}

void USART_Transmit(unsigned char data) {
	while (!(UCSR0A & (1<<UDRE0)) );
	UDR0 = data;
}

unsigned char USART_Receive(void) {
	while (!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

void EEPROM_write(unsigned int uiAddress, unsigned char ucData) {
	while(EECR & (1<<EEPE));
	EEAR = uiAddress;
	EEDR = ucData;
	EECR |= (1<<EEMPE);
	EECR |= (1<<EEPE);
}

unsigned char EEPROM_read(unsigned int uiAddress) {
	while(EECR & (1<<EEPE));
	EEAR = uiAddress;
	EECR |= (1<<EERE);
	return EEDR;
}