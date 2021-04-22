#pragma once

//====================================================================//
class USART {
	int BaudRate;
public:
	USART();
	~USART();
	void USART_Init(void);
	void USART_Write(int ch);
	char USART_Read(void);
};
//====================================================================//