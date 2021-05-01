#pragma once

//====================================================================//
class Temperature_Control {
	int P1, P2, P3, P4, P5, P6, P7;
public:
	Temperature_Control();
	~Temperature_Control();
	int actual_screen, actual_parameter, new_parameter_value;
	int getParameter(int);
	void setParameter(int, int);
	int getId();
	void setId(int);
	virtual void Init() = 0;
	virtual void setDisplay(int) = 0;
	virtual void updateDisplay() = 0;
	virtual void getButton() = 0;
};		//Enc class Temperature_Control
//====================================================================//