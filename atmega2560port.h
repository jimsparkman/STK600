#define F_CPU 8000000UL
#define USART_BAUDRATE 9600
#define BaudScale (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

void USART_Init();
void USART_Transmit(unsigned char data);
unsigned char USART_Receive(void);
void EEPROM_write(unsigned int uiAddress, unsigned char ucData);
unsigned char EEPROM_read(unsigned int uiAddress);