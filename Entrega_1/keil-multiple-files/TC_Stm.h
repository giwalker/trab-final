#pragma once

#include "Timer.h"
#include "Temperature_Control.h"
#include "USART.h"


//====================================================================//
class TC_Stm : public Timer, public Temperature_Control, public USART {
	int D1, D2, D3, D4, D1P, D2P, D3P, D4P;
	bool FLAG_B_Pressed;
	bool FLAG_AC_Block;
	bool FLAG_Smth_Open;
public:
	bool FLAG_Updating_RTC;
	TC_Stm();
	~TC_Stm();
	void doStuff();
	virtual void Init();
	virtual void setDisplay(int);
	virtual void updateDisplay();
	virtual void getButton();
	virtual void setRTC(int, int, int, int, int);
	virtual void updateRTC();
};		//End class TC_Stm
//====================================================================//