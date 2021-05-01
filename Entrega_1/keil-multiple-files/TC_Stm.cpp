#include "TC_Stm.h"

//====================================================================//
//Constructor
TC_Stm::TC_Stm() {
	D1 = 0, D2 = 0, D3 = 0, D4 = 0, D1P = 0, D2P = 0, D3P = 0, D4P = 0;
	FLAG_B_Pressed = false;
	FLAG_AC_Block = false;
	FLAG_Smth_Open = false;
	FLAG_Updating_RTC = false;
}
//Deconstructor
TC_Stm::~TC_Stm() {}

//Initialization of the STM32 peripherals
void TC_Stm::Init() {
	__disable_irq(); 	//DISABLE Interruptions 

	//GPIO Config
	RCC->APB2ENR |= (1 << 4);										//IO port C clock enable
	RCC->APB2ENR |= (1 << 3);										//IO port B clock enable
	RCC->APB2ENR |= (1 << 2);										//IO port A clock enable

	//Obs: I'm reseting the bits, even though they are 0 by default, just to make sure that they are what they need to be, because I face some problems
	//in the past not doing this...
	GPIOC->CRH |= ((1 << 20) | (1 << 21));				//PORTC, PIN 13, MODE = 11 - Output mode, max speed 50 MHz.
	GPIOC->CRH &= ~((1 << 22) | (1 << 23));			//PORTC , PIN 13, CNF = 00 - General purpose output push-pull.

	GPIOA->CRL &= ~(0xFFFFFFFF); 						//Clean the register; Just in case...
	GPIOA->CRH &= ~(0xFFFF);									//Clean the register; Just in case...
	GPIOA->CRL |= 0x33333333;  							//Configure the PORT A (A0 - A7) as Output mode, max speed 50 MHz.
	GPIOA->CRH |= 0x3333;										//Configure the PORT A (A8 - A11) as Output mode, max speed 50 MHz.

	//GPIOB-> CRL &= ~(0xFFF);										//PORTB , PIN 4-6, CNF = 10 - Input with pull-up / pull-down
	GPIOB->CRL |= ((1 << 27) | (1 << 23)); 					//PORTB, PIN 4-6, MODE = 00 - Input Mode
	GPIOB->CRL &= ~((1 << 26) | (1 << 25) | (1 << 24) | (1 << 22) | (1 << 21) | (1 << 20));

	GPIOB->CRL |= (1 << 31);												//PORTB , PIN 7, CNF = 10 - Input with pull-up / pull-down
	GPIOB->CRL &= ~((1 << 30) | (1 << 29) | (1 << 28));//PORTB, PIN 7, MODE = 00 - Input Mode

	GPIOB->CRH &= ~(0xFF0000);									//Clean the register; Just in case...
	GPIOB->CRH |= ((1 << 23) | (1 << 19));   			//Configure the PORT B (B12 - B13) as Input with pull-up / pull-down


	GPIOB->CRH &= ~(0xFF);										//Clean the register; Just in case...
	GPIOB->CRH |= ((1 << 5) | (1 << 4) | (1 << 1) | (1 << 0));  				//Configure the PORT B9-8 as Output mode, max speed 50 MHz.


	//TMR2 Config
	RCC->APB1ENR |= 0x01;											//TIM2 clock enable
	TIM2->PSC = (720 - 1);										//Creating a 1000 Hz Update Event 1/2 
	TIM2->ARR = (100 - 1);										//Creating a 1000 Hz Update Event 2/2
	TIM2->CNT = 0;														//Set the timer counter to zero
	TIM2->CR1 = 0x01; 												//Enable the timer
	TIM2->DIER |= 0x01;												//Update interrupt enabled.
	NVIC_EnableIRQ(TIM2_IRQn);								//Enable TIM2 Interrupt in the NVIC

	__enable_irq(); 													//ENABLE Interruptions

	//A7 - A; A6 - B; A5 - C; A4 - D; A3 - E; A2 - F; A1 - G; A0 - Point (Initializing all ports as 0 (ON) - common anode)	
	GPIOA->BSRR = ((1 << (7 + 16)) | (1 << (6 + 16)) | (1 << (5 + 16)) | (1 << (4 + 16)) | (1 << (3 + 16)) | (1 << (2 + 16)) | (1 << (1 + 16)) | (1 << (0 + 16)));
	//Initializing LEDS
	GPIOB->BSRR = (1 << 9);   				//User Led ON
	//GPIOB->BSRR = (1<<(12+16));   				//Yellow LED ON
//	GPIOB->BSRR = (1<<13);   				//Blue LED ON
}  //End void TC_Stm::Init()

//Prepare the digits values according to the current display beeing show
void TC_Stm::setDisplay(int as) {
	if (as == 1) {
		//In the screen 1, show the Hour and Minutes
		//D1 = Mh; D2 = Ml; D3 = Sh; D4 =Sl;
		int h = getParameter(1);
		int m = getParameter(2);
		D1 = (h / 10);
		D2 = (h - (D1 * 10));
		D3 = (m / 10);
		D4 = (m - (D3 * 10));
		D1P = 0; D2P = 1; D3P = 0; D4P = 0;
	}
	if (as == 2) {
		//In the screen 2, show the Parameter and his value
		int Aux_P = 0;
		Aux_P = getParameter(actual_parameter);
		D1 = 16;
		D2 = actual_parameter;
		D1P = 0; D2P = 1; D3P = 0; D4P = 0;
		if (actual_parameter == 6) {
			D3 = (Aux_P / 16);
			D4 = (Aux_P - (D3 * 16));
		}
		else {
			D3 = (Aux_P / 10);
			D4 = (Aux_P - (10 * D3));
		}
	}
	if (as == 3) {
		//In the screen 3, show the temporary value
		D1 = 20; D4 = 20;		//Default value (all leds turned off)
		D1P = 1; D2P = 0; D3P = 1; D4P = 0;
		if (actual_parameter == 6) {
			D2 = (new_parameter_value / 16);
			D3 = (new_parameter_value - (D2 * 16));
		}
		else {
			D2 = (new_parameter_value / 10);
			D3 = (new_parameter_value - (D2 * 10));
		}
	}
}			//End void TC_Stm::setDisplay

//Update the display with the values previously set
void TC_Stm::updateDisplay() {
	int disp_value = 0, disp_point = 0;

	//Get the variable x from the 1 ms interrupt, and turn on just the respective display
	//Also, change the variables disp_values and disp_point values with the current display values
	if (x == 0) {
		GPIOA->BSRR = ((1 << 11) | (1 << (10 + 16)) | (1 << (9 + 16)) | (1 << (8 + 16))); 		//Display 4
		disp_value = D4;
		disp_point = D4P;
	}
	if (x == 1) {
		GPIOA->BSRR = ((1 << (11 + 16)) | (1 << 10) | (1 << (9 + 16)) | (1 << (8 + 16)));		//Display 3
		disp_value = D3;
		disp_point = D3P;
	}
	if (x == 2) {
		GPIOA->BSRR = ((1 << (11 + 16)) | (1 << (10 + 16)) | (1 << 9) | (1 << (8 + 16)));		//Display 2
		disp_value = D2;
		disp_point = D2P;
	}
	if (x == 3) {
		GPIOA->BSRR = ((1 << (11 + 16)) | (1 << (10 + 16)) | (1 << (9 + 16)) | (1 << 8));	      	//Display 1
		disp_value = D1;
		disp_point = D1P;
	}
	//Update the value of the display
	switch (disp_value) {
	case 0:
		GPIOA->BSRR = ((1 << (7 + 16)) | (1 << (6 + 16)) | (1 << (5 + 16)) | (1 << (4 + 16)) | (1 << (3 + 16)) | (1 << (2 + 16)) | (1 << 1));	//0
		break;
	case 1:
		GPIOA->BSRR = ((1 << 7) | (1 << (6 + 16)) | (1 << (5 + 16)) | (1 << 4) | (1 << 3) | (1 << 2) | (1 << 1)); 	//1
		break;
	case 2:
		GPIOA->BSRR = ((1 << (7 + 16)) | (1 << (6 + 16)) | (1 << 5) | (1 << (4 + 16)) | (1 << (3 + 16)) | (1 << 2) | (1 << (1 + 16))); 	//2	
		break;
	case 3:
		GPIOA->BSRR = ((1 << (7 + 16)) | (1 << (6 + 16)) | (1 << (5 + 16)) | (1 << (4 + 16)) | (1 << 3) | (1 << 2) | (1 << (1 + 16)));	//3	
		break;
	case 4:
		GPIOA->BSRR = ((1 << 7) | (1 << (6 + 16)) | (1 << (5 + 16)) | (1 << 4) | (1 << 3) | (1 << (2 + 16)) | (1 << (1 + 16)));	//4	
		break;
	case 5:
		GPIOA->BSRR = ((1 << (7 + 16)) | (1 << 6) | (1 << (5 + 16)) | (1 << (4 + 16)) | (1 << 3) | (1 << (2 + 16)) | (1 << (1 + 16)));	//5	
		break;
	case 6:
		GPIOA->BSRR = ((1 << (7 + 16)) | (1 << 6) | (1 << (5 + 16)) | (1 << (4 + 16)) | (1 << (3 + 16)) | (1 << (2 + 16)) | (1 << (1 + 16)));	//6	
		break;
	case 7:
		GPIOA->BSRR = ((1 << (7 + 16)) | (1 << (6 + 16)) | (1 << (5 + 16)) | (1 << 4) | (1 << 3) | (1 << 2) | (1 << 1));	//7	
		break;
	case 8:
		GPIOA->BSRR = ((1 << (7 + 16)) | (1 << (6 + 16)) | (1 << (5 + 16)) | (1 << (4 + 16)) | (1 << (3 + 16)) | (1 << (2 + 16)) | (1 << (1 + 16)));	//8	
		break;
	case 9:
		GPIOA->BSRR = ((1 << (7 + 16)) | (1 << (6 + 16)) | (1 << (5 + 16)) | (1 << 4) | (1 << 3) | (1 << (2 + 16)) | (1 << (1 + 16)));	//9	
		break;
	case 16:
		GPIOA->BSRR = ((1 << (7 + 16)) | (1 << (6 + 16)) | (1 << 5) | (1 << 4) | (1 << (3 + 16)) | (1 << (2 + 16)) | (1 << (1 + 16)));	//P	
		break;
	case 10:
		GPIOA->BSRR = ((1 << (7 + 16)) | (1 << (6 + 16)) | (1 << (5 + 16)) | (1 << 4) | (1 << (3 + 16)) | (1 << (2 + 16)) | (1 << (1 + 16)));	//A
		break;
	case 11:
		GPIOA->BSRR = ((1 << 7) | (1 << 6) | (1 << (5 + 16)) | (1 << (4 + 16)) | (1 << (3 + 16)) | (1 << (2 + 16)) | (1 << (1 + 16)));	//b
		break;
	case 12:
		GPIOA->BSRR = ((1 << (7 + 16)) | (1 << 6) | (1 << 5) | (1 << (4 + 16)) | (1 << (3 + 16)) | (1 << (2 + 16)) | (1 << 1));	//C
		break;
	case 13:
		GPIOA->BSRR = ((1 << 7) | (1 << (6 + 16)) | (1 << (5 + 16)) | (1 << (4 + 16)) | (1 << (3 + 16)) | (1 << 2) | (1 << (1 + 16)));	//d6
		break;
	case 14:
		GPIOA->BSRR = ((1 << (7 + 16)) | (1 << 6) | (1 << 5) | (1 << (4 + 16)) | (1 << (3 + 16)) | (1 << (2 + 16)) | (1 << (1 + 16)));	//E
		break;
	case 15:
		GPIOA->BSRR = ((1 << (7 + 16)) | (1 << 6) | (1 << 5) | (1 << 4) | (1 << (3 + 16)) | (1 << (2 + 16)) | (1 << (1 + 16)));	//F
		break;
	default:
		GPIOA->BSRR = ((1 << 7) | (1 << 6) | (1 << 5) | (1 << 4) | (1 << 3) | (1 << 2) | (1 << 1));	//All leds turned OFF	
		break;
	}
	if (disp_point == 1) GPIOA->BSRR |= (1 << (0 + 16));
	else GPIOA->BSRR |= (1 << 0);
}		//End void TC_Stm::updateDisplay()


//Function to processing the buttons, changing the current screens and updating the variables values
void TC_Stm::getButton() {
	if (GPIOB->IDR & (1 << 5) && !FLAG_B_Pressed) { //B1 Pressed
		FLAG_B_Pressed = true;
		switch (actual_screen) {
		case 1:
			//if (!FLAG_OFF_Delay){
			actual_screen = 2;
			actual_parameter = 1;
			//}
			break;

		case 2:
			actual_screen = 3;
			new_parameter_value = getParameter(actual_parameter);
			break;

		case 3:
			setParameter(actual_parameter, new_parameter_value);
			//To avoid the system to update the RTC parameters with the current time and date while changing the values (1/2)
			//If the RTC parametes are changed, or the cancel button is pressed, the system can update the RTC parameters normally
			if (actual_parameter <= 4) FLAG_Updating_RTC = true;
			if (actual_parameter == 5) {
				FLAG_Change_Date = true;
				FLAG_Updating_RTC = false;
			}
			actual_screen = 1;
			break;

		default:
			actual_screen = 1;
			break;
		}			//End switch(actual_screen)
	} 		//End B1 Pressed

	if (GPIOB->IDR & (1 << 6) && !FLAG_B_Pressed) {	//B2 Pressed
		FLAG_B_Pressed = true;

		//If B2 (Increment) is pressed in the screen 1, nothing happens
		if (actual_screen == 1) {
			//Nothing Happens
		}
		//If B2 (Increment) is pressed in the screen 2, the actual paramenter shown on the display is incremented.
		if (actual_screen == 2) {
			actual_parameter++;
			if (actual_parameter >= 8) actual_parameter = 1;		//End of range, return to parameter 1
		}
		//If B2 (Increment) is pressed in the screen 3, the parameter is incremented in a temporary variable, and if this variable
		//reach the maximum value, returns to 0. To really change the actual parameter value, press B1, as show in the start of this function
		if (actual_screen == 3) {
			int max_value = 23;
			if (actual_parameter == 2) max_value = 59;
			if (actual_parameter == 3) max_value = 31;
			if (actual_parameter == 4) max_value = 12;
			if (actual_parameter == 5) max_value = 30;
			if (actual_parameter == 6) max_value = 255;
			if (actual_parameter == 7) max_value = 1;
			new_parameter_value++;
			if (new_parameter_value > max_value) new_parameter_value = 0;
		}
	}		//End B2 Pressed

	if (GPIOB->IDR & (1 << 7) && !FLAG_B_Pressed) {		//B3 Pressed
		FLAG_B_Pressed = true;
		//If B3 (Cancel) is pressed in screen 1, and all the doors and windows are closed, the AC is Unblocked
		if (actual_screen == 1) {
			if (FLAG_AC_Block) {
				FLAG_AC_Block = false;
				Log_1.insertAfterLast(p->getId(), cont_log, 2, h_1, m_1, d_1, me_1, y_1);
				cont_log++;
			}
		}

		//If B3 (Cancel) is pressed in the screens 2 and 3, the screen returns to the previus one
		if (actual_screen == 2) {
			actual_screen = 1;
			//To avoid the system to update the RTC parameters with the current time and date while changing the values
			//If the RTC parametes are changed, or the cancel button is pressed, the system can update the RTC parameters normally
			FLAG_Updating_RTC = false;
		}
		if (actual_screen == 3) {
			actual_screen = 2;
			//To avoid the system to update the RTC parameters with the current time and date while changing the values
			//If the RTC parametes are changed, or the cancel button is pressed, the system can update the RTC parameters normally
			FLAG_Updating_RTC = false;
		}
	}		//End B3 Pressed
	//When all the buttons are unpressed, clear the FLAG that avoid overclick
	if (!(GPIOB->IDR & (1 << 5)) && !(GPIOB->IDR & (1 << 6)) && !(GPIOB->IDR & (1 << 7)) && FLAG_B_Pressed) {
		FLAG_B_Pressed = false;
	}
} 		//End void TC_Stm::getButton()

void TC_Stm::doStuff() {

	//Get windows and doors sensors, and update the relay
	if (!(GPIOB->IDR & (1 << 13)) || !(GPIOB->IDR & (1 << 12))) {	//If one sensor its open
		if (!FLAG_AC_Block) {
			Log_1.insertAfterLast(p->getId(), cont_log, 1, h_1, m_1, d_1, me_1, y_1);
			cont_log++;
		}
		GPIOB->BSRR = (1 << 9);   				//User Led ON (Something is Open)
		GPIOB->BSRR = (1 << 8);   				//Relay ON (Open - Cuts energy)
		FLAG_AC_Block = true;
	}
	else GPIOB->BSRR = (1 << (9 + 16));   		//User Led OFF (Everything Closed)

	if (!FLAG_AC_Block) GPIOB->BSRR = (1 << (8 + 16));   								//Relay OFF (Closed - Restablish energy)

}

//Set the RTC value
void TC_Stm::setRTC(int h, int m, int d, int me, int y) {

	sTime.Hours = h;
	sTime.Minutes = m;
	//sTime.Seconds = 0;
	if (me >= 10) me += 6;
	sDate.Month = me;
	sDate.Date = d;
	sDate.Year = y;
	HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	p->setParameter(1, h_1);
	p->setParameter(2, m_1);
	p->setParameter(3, d_1);
	p->setParameter(4, me_1);
	p->setParameter(5, y_1);

}

//Get the RTC values to use in the code
void TC_Stm::updateRTC() {

	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

	h_1 = sTime.Hours;
	m_1 = sTime.Minutes;
	d_1 = sDate.Date;
	int aux_me = 0;
	if (sDate.Month >= 16) aux_me = (sDate.Month - 6);
	else aux_me = sDate.Month;
	me_1 = aux_me;
	y_1 = sDate.Year;
	if (!FLAG_Updating_RTC) {
		p->setParameter(1, h_1);
		p->setParameter(2, m_1);
		p->setParameter(3, d_1);
		p->setParameter(4, me_1);
		p->setParameter(5, y_1);
	}
}
//====================================================================//