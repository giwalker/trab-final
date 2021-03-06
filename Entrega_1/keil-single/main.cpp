
#include "main.h"
//#include "stm32f10x.h"				//Already included by HAL

RTC_HandleTypeDef hrtc;
RTC_TimeTypeDef sTime;
RTC_DateTypeDef sDate;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);

#define BR 9600

	//Global Variables
	int i = 0, x = 0, z = 0;
	int count_ms = 0;
	bool FLAG_Timer_Pause = false;
	bool FLAG_Change_Date = false;
	int size_g = 0;
	int aux_serial = 0;
	int h_1 = 10, m_1 = 0, d_1 = 7, me_1 = 4, y_1 = 21, cont_log = 25;

	
//====================================================================//
	class Temperature_Control{
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
		class Timer{
		protected:
			bool FLAG_OFF_Delay, FLAG_ON_Delay;
			int Mh, Ml, Sh, Sl, Mt, St, total_time_ms, remaining_time_ms, initial_time_ms_OFF;
		public:
			Timer();
			~Timer();
			virtual void setRTC(int, int, int, int, int) = 0;
			virtual void updateRTC() = 0;
	};		//End class Timer
//====================================================================//
	class USART{
			int BaudRate;
		public:		
			USART();
			~USART();
			void USART_Init(void);
			void USART_Write(int ch);
			char USART_Read(void);
	};
//====================================================================//
	class TC_Stm : public Timer, public Temperature_Control, public USART{
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
	class Node{
			int nmr, id, evt, hours, mins, day, month, year;
		protected:
			Node* next;
		public:
			Node(int d1, int d2, int d3, int d4, int d5, int d6, int d7, int d8, Node* nxt);
			int getVal(int sel);
			Node* getNext();
			void setVal(int d1, int d2, int d3, int d4, int d5, int d6, int d7, int d8);
			void setNext(Node* nxt);
};
//====================================================================//
	class List{
		public:
			Node* head;
			List();
			~List();
			void insertAfterLast(int d1, int d2, int d3, int d4, int d5, int d6, int d7, int d8);
			int removeFirst();
			int listSize();
			void sendList();
};
//====================================================================//
	Temperature_Control* p;
	TC_Stm EMB;
	List Log_1;

	List* l;
	Node* n;
	TC_Stm* t;

int main(void){
  
  HAL_Init();

  SystemClock_Config();

  //MX_GPIO_Init();
  MX_RTC_Init();
  
	p = &EMB;
	p-> Init();  										//Initialize the choosen platform
	
	EMB.USART_Init();
	
	l = &Log_1;
	
  while (1){
		EMB.updateRTC();											//Update the RTC value			
		p-> setDisplay(p->actual_screen); 		//Update the variables that will be used to update the display
		p-> updateDisplay();									//Update the display
		p-> getButton();											//Get the buttons, when pressed
		EMB.doStuff();												//Responsible for the funcions needed to control the AC (Read sensors and actuating the relay)
		//If P7 is set to 1 by the user, start the USART comunication and send the list to PC
		if (p->getParameter(7) == 1){				
			Log_1.sendList();
			p->setParameter(7, 0);		//Zerar P - Comunica??o Realizada
		}
		//If the yeart has been update, update all the RTC parameters set by the user
		if (FLAG_Change_Date){
			EMB.setRTC(EMB.getParameter(1), EMB.getParameter(2), EMB.getParameter(3), EMB.getParameter(4), EMB.getParameter(5));
			FLAG_Change_Date = false;
		}
	}
}

//====================================================================//
extern "C" void TIM2_IRQHandler(void){		//Routine responsible for the interruption
		TIM2->SR &=~1;		//Reset Interruption FLAG
			
		//Function responsible of toggling a LED in the board to show the interruptions in a better way
		//Not Used
//		if (z == 1000){			
//			GPIOC->ODR ^= 0x2000;
//			z = 0;
//		}
//		z++;
			
		x++;		//Display multiplexer counter
		if (x == 4) x = 0;
			
}		//End extern "C" void TIM2_IRQHandler(void)

//====================================================================//
//Constructor
Temperature_Control::Temperature_Control(){
	P1 = 0, P2 = 0, P3 = 1, P4 = 1, P5 = 21, P6 = 254, P7 = 0; 
	actual_screen = 1, actual_parameter = 1, new_parameter_value = 66;
	//id = 254;
}
//Deconstructor
Temperature_Control::~Temperature_Control(){}
//Return de Parameters values
int Temperature_Control::getParameter(int p){
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
void Temperature_Control::setParameter(int p, int v){
	if (p == 1) P1 = v;
	if (p == 2) P2 = v;
	if (p == 3) P3 = v;
	if (p == 4) P4 = v;
	if (p == 5) P5 = v;
	if (p == 6) P6 = v;
	if (p == 7) P7 = v;
}		
//Get the id
int Temperature_Control::getId(){
	return P6;
}
//Set the id
void Temperature_Control::setId(int ident){
	P6 = ident;
}
//====================================================================//
//Constructor
Timer::Timer(){
		FLAG_OFF_Delay = false, FLAG_ON_Delay = true;	
		Mh = 0, Ml = 0, Sh = 0, Sl = 0, Mt = 0, St = 0, total_time_ms = 0, remaining_time_ms = 0, initial_time_ms_OFF = 0;
}
//Deconstructor
Timer::~Timer(){}
//====================================================================//
//Constructor
USART::USART(){
}
USART::~USART(){
	BaudRate = 0; 
	USART3->CR1 &= ~((1<<3) | (1<<2));		//Disable Tx and Rx
	USART3->CR1 &= ~(1<<13);							//Disable Usart
}

void USART::USART_Init(){
	//USART3 -> TX PB10
	//					RX PB11
	RCC->APB1ENR |= (1<<18);						//USART 3 clock enable
	RCC->APB2ENR |= (1<<3);							//I/O port B clock enable
	
	GPIOB->CRH |= ((1<<9) | (1<<8));		//PB10 - TX
	GPIOB->CRH |= (1<<11);							//Output mode, max speed 50 MHz.
	GPIOB->CRH &= ~(1<<10);							//Alternate function output Push-pull
	
	GPIOB->CRH &= ~((1<<13) | (1<<12));	//PB11 - RX
	GPIOB->CRH &= ~(1<<15);							//Input mode (reset state)
	GPIOB->CRH |= (1<<14);							//Floating input (reset state)
	
	USART3->BRR = 0x0EA6;								//9600 Baudrate
	USART3->CR1 |= ((1<<3) | (1<<2));		//Enable Tx and Rx
	USART3->CR1 |= (1<<13);							//Enable Usart
}

//Send data by the USART
void USART::USART_Write(int ch){
	//Wait while Tx Buffer is empty
	while(!(USART3->SR & 0x0080)){}
	USART3->DR |= (ch);
	//USART3->DR = (ch & 0xFF);
}
//Read data by the USART
char USART::USART_Read(){
	while (!(USART3->SR & 0x20)){}
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
//Constructor
TC_Stm::TC_Stm(){
	D1 = 0, D2 = 0, D3 = 0, D4 = 0, D1P = 0, D2P = 0, D3P = 0, D4P = 0;
	FLAG_B_Pressed = false;
	FLAG_AC_Block = false;
	FLAG_Smth_Open = false;
	FLAG_Updating_RTC = false;
}
//Deconstructor
TC_Stm::~TC_Stm(){}
//====================================================================//
//Constructor
Node::Node(int d1, int d2, int d3, int d4, int d5, int d6, int d7, int d8, Node* nxt) {
	id = d1; nmr = d2; evt = d3; hours = d4; mins = d5; day = d6; month = d7; year = d8;
	next = nxt;
}

//Get the value needed from the list
int Node::getVal(int sel) {
    int val = 0;
    if (sel == 1) val = id;
    if (sel == 2) val = nmr;
    if (sel == 3) val = evt;
    if (sel == 4) val = hours;
    if (sel == 5) val = mins;
    if (sel == 6) val = day;
    if (sel == 7) val = month;
    if (sel == 8) val = year;
    return val;
}

//Returns a pointer for the next node in the list
Node* Node::getNext() {
    return next;
}

//Stores the values inside the node
void Node::setVal(int d1, int d2, int d3, int d4, int d5, int d6, int d7, int d8) {
    if (d1 >= 0) id = d1; 
    if (d2 >= 0) nmr = d2;
    if (d3 >= 0) evt = d3;
    if (d4 >= 0) hours = d4;
    if (d5 >= 0) mins = d5;
    if (d6 >= 0) day = d6;
    if (d7 >= 0) month = d7;
    if (d8 >= 0) year = d8;
}

//Stores the pointer to the next node in the list
void Node::setNext(Node* nxt) {
    next = nxt;
}
//====================================================================//
//Constructor
List::List() {
	head = 0;
}
List::~List() {
	Node* cursor = head;
	while (head) {
		cursor = cursor->getNext();
		delete head;
		head = cursor;
	}
	head = 0; 
}

//Return the size of the list
int List::listSize(){
	int aux_listSize = 0;
	Node* a = head;
	while (a != 0){
		aux_listSize++;
		a = a->getNext();
	}
	return aux_listSize;
}
//Send the Linked List via USART and delete the list
void List::sendList(){
	Node* q = head;
	
	int size = 0;
	size_g = Log_1.listSize();
	
	EMB.USART_Write(size_g);
	EMB.USART_Write(q->getVal(1));
	
	//int usartOK = 0x41;

	//if (usartOK == 0x41){
		EMB.USART_Write(0xAA);		//Cabe?alho
		while (q != 0){
			//EMB.USART_Write(q->getVal(1));
			EMB.USART_Write(q->getVal(2));
			EMB.USART_Write(q->getVal(3));
			EMB.USART_Write(q->getVal(4));
			EMB.USART_Write(q->getVal(5));
			EMB.USART_Write(q->getVal(6));
			EMB.USART_Write(q->getVal(7));
			EMB.USART_Write(q->getVal(8));
			q = q->getNext();
			removeFirst();
		}
		EMB.USART_Write(0x55);		//Rodap?
	//}
	//usartOK = 0;
}

//Insert Node after last Node
void List::insertAfterLast(int d1, int d2, int d3, int d4, int d5, int d6, int d7, int d8) {
	Node* p = head;
	Node* q = head;

	if (head == 0)
		head = new Node(d1, d2, d3, d4, d5, d6, d7, d8, head);
	else {
		while (q != 0) {
			p = q;
			q = p->getNext();
		}
		p->setNext(new Node(d1, d2, d3, d4, d5, d6, d7, d8, 0));
	}
}

//Remove the first Node
int List::removeFirst() {
	int retval = 0;
	if (head != 0) {
		retval = head->getVal(1);
		Node* oldHead = head;
		head = head->getNext();
		delete oldHead;
	}
	return retval;
}
//====================================================================//
//Initialization of the STM32 peripherals
void TC_Stm::Init(){
	__disable_irq(); 	//DISABLE Interruptions 
	
	//GPIO Config
	RCC->APB2ENR |= (1<<4);										//IO port C clock enable
	RCC->APB2ENR |= (1<<3);										//IO port B clock enable
	RCC->APB2ENR |= (1<<2);										//IO port A clock enable
	
	//Obs: I'm reseting the bits, even though they are 0 by default, just to make sure that they are what they need to be, because I face some problems
	//in the past not doing this...
	GPIOC-> CRH |= ((1<<20) | (1<<21));				//PORTC, PIN 13, MODE = 11 - Output mode, max speed 50 MHz.
	GPIOC-> CRH &= ~((1<<22) | (1<<23));			//PORTC , PIN 13, CNF = 00 - General purpose output push-pull.
	
	GPIOA-> CRL &= ~(0xFFFFFFFF); 						//Clean the register; Just in case...
	GPIOA-> CRH &= ~(0xFFFF);									//Clean the register; Just in case...
	GPIOA-> CRL |= 0x33333333;  							//Configure the PORT A (A0 - A7) as Output mode, max speed 50 MHz.
	GPIOA-> CRH |= 0x3333;										//Configure the PORT A (A8 - A11) as Output mode, max speed 50 MHz.
	
	//GPIOB-> CRL &= ~(0xFFF);										//PORTB , PIN 4-6, CNF = 10 - Input with pull-up / pull-down
	GPIOB-> CRL |= ((1<<27) | (1<<23)); 					//PORTB, PIN 4-6, MODE = 00 - Input Mode
	GPIOB-> CRL &= ~((1<<26) | (1<<25) | (1<<24) | (1<<22) | (1<<21) | (1<<20));	
	
	GPIOB-> CRL |= (1<<31);												//PORTB , PIN 7, CNF = 10 - Input with pull-up / pull-down
	GPIOB-> CRL &= ~((1<<30) | (1<<29) | (1<<28));//PORTB, PIN 7, MODE = 00 - Input Mode
	
	GPIOB-> CRH &= ~(0xFF0000);									//Clean the register; Just in case...
	GPIOB-> CRH |= ((1<<23) | (1<<19));   			//Configure the PORT B (B12 - B13) as Input with pull-up / pull-down


	GPIOB-> CRH &= ~(0xFF);										//Clean the register; Just in case...
	GPIOB-> CRH |= ((1<<5) | (1<<4) | (1<<1) | (1<<0));  				//Configure the PORT B9-8 as Output mode, max speed 50 MHz.
	
	
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
	GPIOA->BSRR = ((1<<(7+16)) | (1<<(6+16)) | (1<<(5+16)) | (1<<(4+16)) | (1<<(3+16)) | (1<<(2+16)) | (1<<(1+16)) | (1<<(0+16)));
	//Initializing LEDS
	GPIOB->BSRR = (1<<9);   				//User Led ON
	//GPIOB->BSRR = (1<<(12+16));   				//Yellow LED ON
//	GPIOB->BSRR = (1<<13);   				//Blue LED ON
}  //End void TC_Stm::Init()

//Prepare the digits values according to the current display beeing show
void TC_Stm::setDisplay(int as){
	if (as == 1){
			//In the screen 1, show the Hour and Minutes
			//D1 = Mh; D2 = Ml; D3 = Sh; D4 =Sl;
			int h = getParameter(1);
			int m = getParameter(2);
			D1 = (h/10);
			D2 = (h - (D1*10));
			D3 = (m/10);
			D4 = (m - (D3*10));
			D1P = 0; D2P = 1; D3P = 0; D4P = 0;
	}
	if (as == 2){
			//In the screen 2, show the Parameter and his value
			int Aux_P = 0; 
			Aux_P = getParameter(actual_parameter);
			D1 = 16;
			D2 = actual_parameter;
			D1P = 0; D2P = 1; D3P = 0; D4P = 0;
			if(actual_parameter == 6){
				D3 = (Aux_P / 16);
				D4 = (Aux_P - (D3*16));
			}
			else{
				D3 = (Aux_P/10);
				D4 = (Aux_P-(10*D3));
			}
	}
	if (as == 3){
			//In the screen 3, show the temporary value
			D1 = 20; D4 = 20;		//Default value (all leds turned off)
			D1P = 1; D2P = 0; D3P = 1; D4P = 0;
			if (actual_parameter == 6){
				D2 = (new_parameter_value / 16);
				D3 = (new_parameter_value - (D2*16));
			}
			else {
				D2 = (new_parameter_value/10);
				D3 = (new_parameter_value-(D2*10));
			}
	}	
}			//End void TC_Stm::setDisplay

//Update the display with the values previously set
void TC_Stm::updateDisplay(){
		int disp_value = 0, disp_point = 0;
		
		//Get the variable x from the 1 ms interrupt, and turn on just the respective display
		//Also, change the variables disp_values and disp_point values with the current display values
		if (x == 0){
				GPIOA->BSRR = ((1<<11)       | (1<<(10+16))  | (1<<(9+16)) | (1<<(8+16))); 		//Display 4
				disp_value = D4;
				disp_point = D4P;
		}
		if (x == 1){
				GPIOA->BSRR = ((1<<(11+16))  | (1<<10)       | (1<<(9+16)) | (1<<(8+16)));		//Display 3
				disp_value = D3;
				disp_point = D3P;
		}
		if (x == 2){
				GPIOA->BSRR = ((1<<(11+16))  | (1<<(10+16))  | (1<<9)      | (1<<(8+16)));		//Display 2
				disp_value = D2;
				disp_point = D2P;
		}
		if (x == 3){
				GPIOA->BSRR = ((1<<(11+16))  | (1<<(10+16))  | (1<<(9+16)) | (1<<8));	      	//Display 1
				disp_value = D1;
				disp_point = D1P;
		}
		//Update the value of the display
		switch (disp_value){
			case 0:
				GPIOA->BSRR =  ((1<<(7+16)) | (1<<(6+16)) | (1<<(5+16)) | (1<<(4+16)) | (1<<(3+16)) | (1<<(2+16)) | (1<<1));	//0
			break;
			case 1:
				GPIOA->BSRR =  ((1<<7)      | (1<<(6+16)) | (1<<(5+16)) | (1<<4)      | (1<<3)      | (1<<2)      | (1<<1)); 	//1
			break;
			case 2:
				GPIOA->BSRR =  ((1<<(7+16)) | (1<<(6+16)) | (1<<5)      | (1<<(4+16)) | (1<<(3+16)) | (1<<2)      | (1<<(1+16))); 	//2	
			break;
			case 3:
				GPIOA->BSRR =  ((1<<(7+16)) | (1<<(6+16)) | (1<<(5+16)) | (1<<(4+16)) | (1<<3)      | (1<<2)      | (1<<(1+16)));	//3	
			break;
			case 4:
				GPIOA->BSRR = ((1<<7)      | (1<<(6+16)) | (1<<(5+16)) | (1<<4)      | (1<<3)      | (1<<(2+16)) | (1<<(1+16)));	//4	
			break;
			case 5:
				GPIOA->BSRR = ((1<<(7+16)) | (1<<6)      | (1<<(5+16)) | (1<<(4+16)) | (1<<3)      | (1<<(2+16)) | (1<<(1+16)));	//5	
			break;
			case 6:
				GPIOA->BSRR = ((1<<(7+16)) | (1<<6)      | (1<<(5+16)) | (1<<(4+16)) | (1<<(3+16)) | (1<<(2+16)) | (1<<(1+16)));	//6	
			break;
			case 7:
				GPIOA->BSRR = ((1<<(7+16)) | (1<<(6+16)) | (1<<(5+16)) | (1<<4)      | (1<<3)      | (1<<2)      | (1<<1));	//7	
			break;
			case 8:
				GPIOA->BSRR = ((1<<(7+16)) | (1<<(6+16)) | (1<<(5+16)) | (1<<(4+16)) | (1<<(3+16)) | (1<<(2+16)) | (1<<(1+16)));	//8	
			break;
			case 9:
				GPIOA->BSRR = ((1<<(7+16)) | (1<<(6+16)) | (1<<(5+16)) | (1<<4)      | (1<<3)      | (1<<(2+16)) | (1<<(1+16)));	//9	
			break;
			case 16:
				GPIOA->BSRR = ((1<<(7+16)) | (1<<(6+16)) | (1<<5)      | (1<<4)      | (1<<(3+16)) | (1<<(2+16)) | (1<<(1+16)));	//P	
			break;
			case 10:
				GPIOA->BSRR = ((1<<(7+16)) | (1<<(6+16)) | (1<<(5+16)) | (1<<4) | (1<<(3+16)) | (1<<(2+16)) | (1<<(1+16)));	//A
			break;
			case 11:
				GPIOA->BSRR = ((1<<7) | (1<<6) | (1<<(5+16)) | (1<<(4+16)) | (1<<(3+16)) | (1<<(2+16)) | (1<<(1+16)));	//b
			break;
			case 12:
				GPIOA->BSRR = ((1<<(7+16)) | (1<<6) | (1<<5) | (1<<(4+16)) | (1<<(3+16)) | (1<<(2+16)) | (1<<1));	//C
			break;
			case 13:
				GPIOA->BSRR = ((1<<7) | (1<<(6+16)) | (1<<(5+16)) | (1<<(4+16)) | (1<<(3+16)) | (1<<2) | (1<<(1+16)));	//d6
			break;
			case 14:
				GPIOA->BSRR = ((1<<(7+16)) | (1<<6) | (1<<5) | (1<<(4+16)) | (1<<(3+16)) | (1<<(2+16)) | (1<<(1+16)));	//E
			break;
			case 15:
				GPIOA->BSRR = ((1<<(7+16)) | (1<<6) | (1<<5) | (1<<4) | (1<<(3+16)) | (1<<(2+16)) | (1<<(1+16)));	//F
			break;
			default:
				GPIOA->BSRR = ((1<<7) | (1<<6) | (1<<5) | (1<<4) | (1<<3) | (1<<2) | (1<<1));	//All leds turned OFF	
			break;
		}
			if (disp_point == 1) GPIOA->BSRR |= (1<<(0+16));
			else GPIOA->BSRR |= (1<<0);
}		//End void TC_Stm::updateDisplay()


//Function to processing the buttons, changing the current screens and updating the variables values
void TC_Stm::getButton(){
		if (GPIOB-> IDR & (1<<5) && !FLAG_B_Pressed) { //B1 Pressed
			FLAG_B_Pressed = true;
			switch(actual_screen){
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
					actual_screen=1;
				break;
				
				default:
					actual_screen=1;
				break;
			}			//End switch(actual_screen)
		} 		//End B1 Pressed
		
		if (GPIOB-> IDR & (1<<6) && !FLAG_B_Pressed) {	//B2 Pressed
			FLAG_B_Pressed = true;
			
			//If B2 (Increment) is pressed in the screen 1, nothing happens
			if (actual_screen == 1){
					//Nothing Happens
			} 
			//If B2 (Increment) is pressed in the screen 2, the actual paramenter shown on the display is incremented.
			if (actual_screen == 2){
				actual_parameter++;
				if (actual_parameter >= 8) actual_parameter = 1;		//End of range, return to parameter 1
			}
			//If B2 (Increment) is pressed in the screen 3, the parameter is incremented in a temporary variable, and if this variable
			//reach the maximum value, returns to 0. To really change the actual parameter value, press B1, as show in the start of this function
			if (actual_screen == 3){
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
		
		if (GPIOB-> IDR & (1<<7) && !FLAG_B_Pressed) {		//B3 Pressed
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
		if (!(GPIOB-> IDR & (1<<5)) && !(GPIOB-> IDR & (1<<6)) && !(GPIOB-> IDR & (1<<7)) && FLAG_B_Pressed){
			FLAG_B_Pressed = false;
		}
} 		//End void TC_Stm::getButton()

void TC_Stm::doStuff(){

		//Get windows and doors sensors, and update the relay
		if (!(GPIOB-> IDR & (1<<13)) || !(GPIOB-> IDR & (1<<12))) {	//If one sensor its open
			if (!FLAG_AC_Block){
				Log_1.insertAfterLast(p->getId(), cont_log, 1, h_1, m_1, d_1, me_1, y_1);
				cont_log++;
			}
			GPIOB->BSRR = (1<<9);   				//User Led ON (Something is Open)
			GPIOB->BSRR = (1<<8);   				//Relay ON (Open - Cuts energy)
			FLAG_AC_Block = true;
		}
		else GPIOB->BSRR = (1<<(9+16));   		//User Led OFF (Everything Closed)
	
		if (!FLAG_AC_Block) GPIOB->BSRR = (1<<(8+16));   								//Relay OFF (Closed - Restablish energy)
	
}

//Set the RTC value
void TC_Stm::setRTC(int h, int m, int d, int me, int y){
		
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
void TC_Stm::updateRTC(){
	
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
	if (!FLAG_Updating_RTC){
		p->setParameter(1, h_1);
		p->setParameter(2, m_1);
		p->setParameter(3, d_1);
		p->setParameter(4, me_1);
		p->setParameter(5, y_1);
	}
}
//Clock tree configuration by HAL Library
void SystemClock_Config(void){
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}


//RTC configuration by HAL Library
static void MX_RTC_Init(void){

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef DateToUpdate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 8;
  sTime.Minutes = 0;
  sTime.Seconds = 0;

  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK){
    Error_Handler();
  }
  DateToUpdate.WeekDay = 0x00;
  DateToUpdate.Month = 0x04;
  DateToUpdate.Date = 11;
  DateToUpdate.Year = 21;

  if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN) != HAL_OK){
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */
		//HAL_RTC_GetDate(RTC_HandleTypeDef *hrtc, RTC_DateTypeDef *sDate, uint32_t Format)
		//HAL_RTC_GetTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, uint32_t Format)
	
		//HAL_RTC_SetDate(RTC_HandleTypeDef *hrtc, RTC_DateTypeDef *sDate, uint32_t Format)
		//HAL_RTC_SetTime(RTC_HandleTypeDef *hrtc, RTC_TimeTypeDef *sTime, uint32_t Format)
  /* USER CODE END RTC_Init 2 */

}


//GPIO configuration by HAL Library (NOT USED)
static void MX_GPIO_Init(void){

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

}


//Error handling function by HAL Library (Hopefully, NOT USING)
void Error_Handler(void){
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}



