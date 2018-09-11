/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx.h"
#include "stm32f1xx_it.h"

extern volatile uint32_t duration, durationL;
extern volatile uint8_t front, catcher2, preamble_flag;

extern volatile long SysTickDelay;
volatile long count4;
extern volatile uint32_t preamble_count, LOW_count, HI_count;
extern volatile uint8_t HCS_bit_counter;                // счетчик считанных бит данных
extern volatile uint8_t RF_bufer[66];
extern volatile uint8_t reciver_full;
extern volatile uint32_t read_ID;
volatile uint32_t preDBG[12], dataDBG[66];

extern volatile uint8_t level;
extern volatile unsigned long len;

void NMI_Handler(void){}
void HardFault_Handler(void){while (1){}}
void MemManage_Handler(void){ while (1){}}
void BusFault_Handler(void){while (1){}}
void UsageFault_Handler(void){while (1){}}
void SVC_Handler(void){}
void DebugMon_Handler(void){}
void PendSV_Handler(void){}
	
void SysTick_Handler(void)
{
	SysTickDelay--;
}



void EXTI9_5_IRQHandler(void)
{
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_8) != RESET)
	{
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_8);
    
		if(level!=255) return;
		
    //front = 1 rising, 0 falling
    if(front)
    {
      //обработка переднего фронта
      //Enable Falling trigger line 8
      LL_EXTI_DisableRisingTrig_0_31(LL_EXTI_LINE_8);
      LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_8);	
      front=0;
			level=0;
      //durationL=LL_TIM_GetCounter(TIM4); //high level duration
			len=LL_TIM_GetCounter(TIM4);
      LL_TIM_SetCounter(TIM4, 0);
      //LL_GPIO_ResetOutputPin(GPIOB, redLed_Pin);    
    }						
    else
    {
      //обработка заднего фронта
      //Enable Rising trigger line 8
      LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_8);
      LL_EXTI_DisableFallingTrig_0_31(LL_EXTI_LINE_8); 
			front=1;
			level=1;
			len=TIM4->CNT;	//LL_TIM_GetCounter(TIM4);
      duration=LL_TIM_GetCounter(TIM4);		 //high level duration
			LL_TIM_SetCounter(TIM4, 0);      
      //LL_GPIO_SetOutputPin(GPIOB, redLed_Pin);      
    }		
	}
	
	
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_9) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_9);
		
		/*
		if(catcher2==0)
		{
			LL_TIM_SetCounter(TIM4, 0);
			LL_GPIO_ResetOutputPin(GPIOB, greenLed_Pin);

			catcher2=1;
		}
		
		else
		{
			duration=LL_TIM_GetCounter(TIM4);
			//LL_TIM_DisableCounter(TIM4);
			
			LL_GPIO_SetOutputPin(GPIOB, greenLed_Pin);
			
			//LL_GPIO_TogglePin(GPIOB, greenLed_Pin);
			catcher2=0;
		}
		*/
  }
}



void TIM4_IRQHandler(void)
{
	count4++;
}
