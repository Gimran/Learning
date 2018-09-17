/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdint.h"

/* Private variables ---------------------------------------------------------*/
	long timer_tic=0;
	long BtnPeriod=0;
	volatile uint8_t front=0 ;	//0-rising, 1-falling edge
	volatile long SysTickDelay=0;
	volatile uint8_t RF_bufer[66];
	uint8_t volatile HCS_bit_counter;           // счетчик считанных бит данных
	volatile uint32_t read_ID, button_code;
	uint32_t last_ID;
	uint8_t channel_set=0;
	uint32_t flash_adr = 0;

  uint8_t red_state, green_state, ch1_state, ch2_state;
  uint32_t green_time, red_time, ch1_time, ch2_time;  

	uint8_t colour;
  #define GREEN 0
  #define RED 1
  #define CH1 2
  #define CH2 3

 struct keeloqStr keeloq;

 void out_handler(void)
 {

   //////////////green
   if (green_state == 1)
   {
     green_state = 2;
     GREEN_ON
   }
   if (green_state == 2 && !green_time)
   {
     green_state = 3;
     green_time = 200;
     GREEN_OFF
   }
   if (green_state == 3 && !green_time)
     green_state = 0;
   //////////////green

   //////////////red
   if (red_state == 1)
   {
     red_state = 2;
     RED_ON
   }
   if (red_state == 2 && !red_time)
   {
     red_state = 3;
     red_time = 200;
     RED_OFF
   }
   if (red_state == 3 && !red_time)
     red_state = 0;
   //////////////red

   //////////////ch1
   if (ch1_state == 1)
   {
     ch1_state = 2;
     OUT1_ON
   }
   if (ch1_state == 2 && !ch1_time)
   {
     ch1_state = 3;
     ch1_time = 200;
     GREEN_OFF
   }
   if (ch1_state == 3 && !ch1_time)
     ch1_state = 0;
   //////////////ch1

   //////////////ch2
   if (ch2_state == 1)
   {
     ch2_state = 2;
     OUT2_ON
   }
   if (ch2_state == 2 && !ch2_time)
   {
     ch2_state = 3;
     ch2_time = 200;
     GREEN_OFF
   }
   if (ch2_state == 3 && !ch2_time)
     ch2_state = 0;
   //////////////ch1
 }


 void out_blink(uint8_t colour, uint16_t delay)
 {
   switch (colour)
   {
   case GREEN:
     if (green_state != 3)
     {
       green_time = delay;
       green_state = 1;       
     }
	 break;

   case RED:
     if (red_state != 3)
     {
       red_time = delay;
       red_state = 1;       
     }
	 break;

   case CH1:
     if (ch1_state != 3)
     {
       ch1_time = delay;
       ch1_state = 1;       
     }
	 break;

   case CH2:
     if (ch2_state != 3)
     {
       ch2_time = delay;
       ch2_state = 1;      
     }
	 break;
   }
}

void channel_init(void){
	
	channel_set=0;
	
	channel_set|=((LL_GPIO_IsInputPinSet(GPIOB, chs1))^1);
	channel_set|=((LL_GPIO_IsInputPinSet(GPIOB, chs2))^1)<<1;
	channel_set|=((LL_GPIO_IsInputPinSet(GPIOB, chs3))^1)<<2;
	channel_set|=((LL_GPIO_IsInputPinSet(GPIOB, chs4))^1)<<3;
}

void Delay(unsigned int Val)
{
SysTickDelay = Val;
while (SysTickDelay != 0);} 





int main(void)
{
  LL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
	LL_TIM_DisableIT_TRIG(TIM4);
	LL_TIM_EnableCounter(TIM4);
	LL_SYSTICK_EnableIT();
	
	LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_9);

	LL_GPIO_TogglePin(GPIOB, redLed_Pin|greenLed_Pin);
	LL_mDelay(100);
	LL_GPIO_TogglePin(GPIOB, redLed_Pin|greenLed_Pin);
	LL_mDelay(100);
	LL_GPIO_TogglePin(GPIOB, redLed_Pin|greenLed_Pin);
	LL_mDelay(100);
	LL_GPIO_TogglePin(GPIOB, redLed_Pin|greenLed_Pin);
	LL_mDelay(100);
	
  
	  


 flash_unlock();
 flash_erase_page(ADDRESS);

 flash_write(ADDRESS, 0x26b41b);
 flash_write(ADDRESS+0x4, 0xa40490);

 LL_mDelay(2000);

 flash_lock();
 
  while (1)
  {
	level_check();
  out_handler();
  channel_init();
flash_adr=flash_read(ADDRESS);

  if(keeloq.state==100)
  {
    //считывание серийника и кода кнопки из буфера
		button_code=keeloq.data[7]>>4;
		read_ID|=keeloq.data[6]<<16;
		read_ID|=keeloq.data[5]<<8;
		read_ID|=keeloq.data[4];
    keeloq.state=0;

		if(read_ID == 0xa40455||read_ID == 0x26b41b || read_ID == 0xa40490 || read_ID == 0xa40440	||	read_ID == 0xeaf0f3)
		{
      if(button_code&channel_set){
		  //LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_8);
			out_blink(GREEN, 200);

		  //LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_8);
      }
			/* switch(button_code){
				case 0x1:
				out_blink(CH1, 200);
				break;
				
				case 0x2:
				out_blink(GREEN, 200);
				out_blink(CH1, 200);
				break;
				
				case 0x4:
				out_blink(RED, 500);
				out_blink(CH2, 200);
				break;				
				
				case 0x8:
				out_blink(CH1, 200);
				break;				
			} */

    	}
    last_ID=read_ID;
    read_ID=0;
	}


	//state=LL_GPIO_IsInputPinSet(GPIOB, Btn_Pin);

  }
}

static void LL_Init(void)
{
  

  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt init*/
  /* MemoryManagement_IRQn interrupt configuration */
  NVIC_SetPriority(MemoryManagement_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  /* BusFault_IRQn interrupt configuration */
  NVIC_SetPriority(BusFault_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  /* UsageFault_IRQn interrupt configuration */
  NVIC_SetPriority(UsageFault_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  /* SVCall_IRQn interrupt configuration */
  NVIC_SetPriority(SVCall_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  /* DebugMonitor_IRQn interrupt configuration */
  NVIC_SetPriority(DebugMonitor_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  /* PendSV_IRQn interrupt configuration */
  NVIC_SetPriority(PendSV_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  /* SysTick_IRQn interrupt configuration */
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));

    /**NOJTAG: JTAG-DP Disabled and SW-DP Enabled 
    */
  LL_GPIO_AF_Remap_SWJ_NOJTAG();

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

   if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_2)
  {
    Error_Handler();  
  }
  LL_RCC_HSI_SetCalibTrimming(16);

  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {
    
  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI_DIV_2, LL_RCC_PLL_MUL_16);

  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {
    
  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);

  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL); 

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  
  }
  LL_Init1msTick(64000000);

  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);

  LL_SetSystemCoreClock(64000000);

  /* SysTick_IRQn interrupt configuration */
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
}

/* TIM4 init function */
static void MX_TIM4_Init(void)
{

  LL_TIM_InitTypeDef TIM_InitStruct;

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);

  /* TIM4 interrupt Init */
  NVIC_SetPriority(TIM4_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(TIM4_IRQn);

  TIM_InitStruct.Prescaler = 63;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 30000;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM4, &TIM_InitStruct);

  LL_TIM_DisableARRPreload(TIM4);

  LL_TIM_SetClockSource(TIM4, LL_TIM_CLOCKSOURCE_INTERNAL);

  LL_TIM_SetTriggerOutput(TIM4, LL_TIM_TRGO_RESET);

  LL_TIM_DisableMasterSlaveMode(TIM4);

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  LL_EXTI_InitTypeDef EXTI_InitStruct;
  LL_GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);

  /**/
  LL_GPIO_ResetOutputPin(GPIOB, out1_Pin|out2_Pin|redLed_Pin|greenLed_Pin);

  /**/
  GPIO_InitStruct.Pin = out1_Pin|out2_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = redLed_Pin|greenLed_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*channel pin setup*/
  GPIO_InitStruct.Pin = chs1|chs2|chs3|chs4;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;  
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);


  /**/
  LL_GPIO_AF_SetEXTISource(LL_GPIO_AF_EXTI_PORTB, LL_GPIO_AF_EXTI_LINE8);

  /**/
  LL_GPIO_AF_SetEXTISource(LL_GPIO_AF_EXTI_PORTB, LL_GPIO_AF_EXTI_LINE9);

  /**/
  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_8;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
  LL_EXTI_Init(&EXTI_InitStruct);

  /**/
  EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_9;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
  LL_EXTI_Init(&EXTI_InitStruct);

  /**/
  LL_GPIO_SetPinMode(rfData_GPIO_Port, rfData_Pin, LL_GPIO_MODE_FLOATING);

  /**/
  LL_GPIO_SetPinMode(Btn_GPIO_Port, Btn_Pin, LL_GPIO_MODE_INPUT);

  /**/
  LL_GPIO_SetPinPull(Btn_GPIO_Port, Btn_Pin, LL_GPIO_PULL_UP);

  /* EXTI interrupt init*/
  NVIC_SetPriority(EXTI9_5_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
