
#include "main.h"				//Needed by HAL
#include "TC_Stm.h"
#include "List.h"

//#include "stm32f10x.h"		//Already included by HAL

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

Temperature_Control* p;
TC_Stm EMB;
List Log_1;

List* l;
Node* n;
TC_Stm* t;

int main(void) {

	HAL_Init();

	SystemClock_Config();

	//MX_GPIO_Init();
	MX_RTC_Init();

	p = &EMB;
	p->Init();  										//Initialize the choosen platform

	EMB.USART_Init();

	l = &Log_1;

	while (1) {
		EMB.updateRTC();						//Update the RTC value			
		p->setDisplay(p->actual_screen); 		//Update the variables that will be used to update the display
		p->updateDisplay();						//Update the display
		p->getButton();							//Get the buttons, when pressed
		EMB.doStuff();							//Responsible for the funcions needed to control the AC (Read sensors and actuating the relay)
		//If P7 is set to 1 by the user, start the USART comunication and send the list to PC
		if (p->getParameter(7) == 1) {
			Log_1.sendList();
			p->setParameter(7, 0);				//Zerar P - Comunicação Realizada
		}
		//If the yeart has been update, update all the RTC parameters set by the user
		if (FLAG_Change_Date) {
			EMB.setRTC(EMB.getParameter(1), EMB.getParameter(2), EMB.getParameter(3), EMB.getParameter(4), EMB.getParameter(5));
			FLAG_Change_Date = false;
		}
	}
}
//====================================================================//

extern "C" void TIM2_IRQHandler(void) {		//Routine responsible for the interruption
	TIM2->SR &= ~1;		//Reset Interruption FLAG

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

//Clock tree configuration by HAL Library
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

	/** Initializes the RCC Oscillators according to the specified parameters
	* in the RCC_OscInitTypeDef structure.
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE;
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
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
		| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
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
static void MX_RTC_Init(void) {

	/* USER CODE BEGIN RTC_Init 0 */

	/* USER CODE END RTC_Init 0 */

	RTC_TimeTypeDef sTime = { 0 };
	RTC_DateTypeDef DateToUpdate = { 0 };

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

	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK) {
		Error_Handler();
	}
	DateToUpdate.WeekDay = 0x00;
	DateToUpdate.Month = 0x04;
	DateToUpdate.Date = 11;
	DateToUpdate.Year = 21;

	if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BIN) != HAL_OK) {
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
static void MX_GPIO_Init(void) {

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

}

//Error handling function by HAL Library (Hopefully, NOT USING)
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}



