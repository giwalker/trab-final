#include "USART.h"

//====================================================================//
//Constructor
USART::USART() {
}
USART::~USART() {
	BaudRate = 0;
	USART3->CR1 &= ~((1 << 3) | (1 << 2));		//Disable Tx and Rx
	USART3->CR1 &= ~(1 << 13);							//Disable Usart
}

void USART::USART_Init() {
	//USART3 -> TX PB10
	//					RX PB11
	RCC->APB1ENR |= (1 << 18);						//USART 3 clock enable
	RCC->APB2ENR |= (1 << 3);							//I/O port B clock enable

	GPIOB->CRH |= ((1 << 9) | (1 << 8));		//PB10 - TX
	GPIOB->CRH |= (1 << 11);							//Output mode, max speed 50 MHz.
	GPIOB->CRH &= ~(1 << 10);							//Alternate function output Push-pull

	GPIOB->CRH &= ~((1 << 13) | (1 << 12));	//PB11 - RX
	GPIOB->CRH &= ~(1 << 15);							//Input mode (reset state)
	GPIOB->CRH |= (1 << 14);							//Floating input (reset state)

	USART3->BRR = 0x0EA6;								//9600 Baudrate
	USART3->CR1 |= ((1 << 3) | (1 << 2));		//Enable Tx and Rx
	USART3->CR1 |= (1 << 13);							//Enable Usart
}

//Send data by the USART
void USART::USART_Write(int ch) {
	//Wait while Tx Buffer is empty
	while (!(USART3->SR & 0x0080)) {}
	USART3->DR |= (ch);
	//USART3->DR = (ch & 0xFF);
}
//Read data by the USART
char USART::USART_Read() {
	while (!(USART3->SR & 0x20)) {}
	int aux_read = USART3->DR;
	aux_serial = aux_read;
	//aux_serial = 15;
	return aux_read;

	//	char aux_read = 'a';
	//	if (USART3->SR & (1<<5)){
	//		aux_read = USART3->DR;
	//	}
	//	aux_serial = aux_read;
	//	return aux_read;
}
//====================================================================//