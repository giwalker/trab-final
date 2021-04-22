#include "Temperature_Control.h"

//====================================================================//
//Constructor
Temperature_Control::Temperature_Control() {
	P1 = 0, P2 = 0, P3 = 1, P4 = 1, P5 = 21, P6 = 254, P7 = 0;
	actual_screen = 1, actual_parameter = 1, new_parameter_value = 66;
	//id = 254;
}
//Deconstructor
Temperature_Control::~Temperature_Control() {}
//Return de Parameters values
int Temperature_Control::getParameter(int p) {
	int aux_retorno = 0;
	if (p == 1) aux_retorno = P1;
	if (p == 2) aux_retorno = P2;
	if (p == 3) aux_retorno = P3;
	if (p == 4) aux_retorno = P4;
	if (p == 5) aux_retorno = P5;
	if (p == 6) aux_retorno = P6;
	if (p == 7) aux_retorno = P7;

	return aux_retorno;
}
//Set the Parameters values
void Temperature_Control::setParameter(int p, int v) {
	if (p == 1) P1 = v;
	if (p == 2) P2 = v;
	if (p == 3) P3 = v;
	if (p == 4) P4 = v;
	if (p == 5) P5 = v;
	if (p == 6) P6 = v;
	if (p == 7) P7 = v;
}
//Get the id
int Temperature_Control::getId() {
	return P6;
}
//Set the id
void Temperature_Control::setId(int ident) {
	P6 = ident;
}
//====================================================================//