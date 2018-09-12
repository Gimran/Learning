/* Includes ------------------------------------------------------------------*/
#include "main.h"





/* Private variables ---------------------------------------------------------*/
	long timer_tic=0;
	long BtnPeriod=0;
	volatile uint8_t front=0, catcher2=0, preamble_flag=0;	//0-rising, 1-falling edge
	volatile uint32_t duration, durationL;
	volatile long SysTickDelay=0;
	
	volatile uint32_t preamble_count=0,HI_count, LOW_count;
	volatile uint8_t RF_bufer[66];
	uint16_t his_ICR1, new_ICR1, CycleCount;
	volatile uint8_t reciver_full;
	uint8_t volatile HCS_bit_counter;                // счетчик считанных бит данных
	volatile uint32_t read_ID, button_code;
	uint8_t detect_vect=0;
	volatile uint8_t state;
	uint8_t debug[8]={0};
  uint32_t last_ID;

volatile uint8_t level=255;
volatile unsigned long last, len;
uint8_t p_level;
unsigned long p_len, p_len_prev;
	

	
/* Private function prototypes -----------------------------------------------*/
static void LL_Init(void);
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM4_Init(void);

/* Private function prototypes -----------------------------------------------*/
	
typedef	struct
{
char Parameter1;		// 1 byte
uint8_t Parameter2;		// 1 byte
uint16_t Parameter3;	// 2 byte
uint32_t Parameter4;	// 4 byte

                      // 8 byte = 2  32-bits words.  It's - OK
                      // !!! Full size (bytes) must be a multiple of 4 !!!
} tpSettings;



void Delay( unsigned int Val)  
{  
  SysTickDelay = Val;  
  while (SysTickDelay != 0); 
} 


struct
{
  uint8_t state;
  unsigned long TE;
  uint8_t pre_count, data[9], dat_bit;
} keeloq;


void setbit(uint8_t *data, uint8_t n)
{
  data[n/8]|=1<<(n%8);
}

void process_keeloq()
{
  switch(keeloq.state)
  {
    case 0:
      if(p_level) break;
      keeloq.state=1;
      keeloq.pre_count=0;
      break;

    case 1: //pre+hdr
      if(p_len>=KL_MIN_TE && p_len<=KL_MAX_TE) keeloq.pre_count++;
      else if(!p_level && p_len>=KL_MIN_TE*10 && p_len<=KL_MAX_TE*10 && keeloq.pre_count>=KL_MIN_PRE_COUNT)
      {
        keeloq.TE=p_len/10;
        keeloq.state=2;
        keeloq.dat_bit=0;
        keeloq.data[0]=0x00;
        keeloq.data[1]=0x00;
        keeloq.data[2]=0x00;
        keeloq.data[3]=0x00;
        keeloq.data[4]=0x00;
        keeloq.data[5]=0x00;
        keeloq.data[6]=0x00;
        keeloq.data[7]=0x00;
				keeloq.data[8]=0x00;
				
				for(uint8_t i=0; i<66; i++)
				{
					RF_bufer[i]=0;
				}
      }
        else
      {
        keeloq.state=0;
        break;
      }
      break;

    case 2: //dat
      if(!p_level) break;

      if(p_len<keeloq.TE/2 || p_len>keeloq.TE*3)
      {
        keeloq.state=0;
        break;
      }

      if(p_len<=keeloq.TE+keeloq.TE/2) 
			{
				setbit(keeloq.data, keeloq.dat_bit);
				RF_bufer[keeloq.dat_bit]=1;
			}
      if(++keeloq.dat_bit==KL_MAX_BITS) keeloq.state=100;
      break;
  }
}

void dump_hex(uint8_t *buf, uint8_t bits)
{
  uint8_t a;
  
  for(a=0; a<(bits+7)/8; a++)
  {
    if(buf[a]<=0x0f) {}
  }
}


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
	Delay(100);
  LL_mDelay(1000);
	LL_GPIO_TogglePin(GPIOB, redLed_Pin|greenLed_Pin);
	Delay(100);
	LL_GPIO_TogglePin(GPIOB, redLed_Pin|greenLed_Pin);
	Delay(100);
	LL_GPIO_TogglePin(GPIOB, redLed_Pin|greenLed_Pin);
	Delay(100);
 // LL_GPIO_ResetOutputPin(GPIOB, redLed_Pin|greenLed_Pin);

  



  while (1)
  {
		if(level!=255)
  {
    LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_8);
    p_level=level;
    p_len=len;
    len=0;
    level=255;
    LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_8);
    
    process_keeloq();
    
    p_len_prev=p_len;
  }
  
  if(keeloq.state==100)
  {
    //Serial.print("KEELOQ: ");
    //dump_hex(keeloq.data, 64);
		button_code=keeloq.data[7]>>4;
		read_ID|=keeloq.data[6]<<16;
		read_ID|=keeloq.data[5]<<8;
		read_ID|=keeloq.data[4];
    keeloq.state=0;
		if(read_ID == FANTOM_ID)
		{
			//LL_GPIO_ResetOutputPin(GPIOB, greenLed_Pin);
			GREEN_ON
      RED_ON
      LL_GPIO_TogglePin(GPIOB, redLed_Pin);
      OUT_ON
			Delay(500);
			GREEN_OFF
      RED_OFF
      LL_GPIO_TogglePin(GPIOB, redLed_Pin);
			OUT_OFF
		}
    last_ID=read_ID;
    read_ID=0;
    //Serial.println("");
	}


	//state=LL_GPIO_IsInputPinSet(GPIOB, Btn_Pin);
		/*
	if(reciver_full)
		{
			LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_8);
			for(uint8_t i=0;i<8; i++)
			{
				debug[i]=0;
				
			}
			debug[0]=RF_bufer[2];
			debug[1]=RF_bufer[3];
			debug[2]=RF_bufer[4];
			debug[3]=RF_bufer[5];
			reciver_full=0;
			for(uint8_t i=0; i<66; i++)
			{
				RF_bufer[i]=0;
			}
			Delay(400);
			LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_8);

		  
		}
		*/
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
  GPIO_InitStruct.Pin = ch1|ch2|ch3|ch4;
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
