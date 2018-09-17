/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx.h"
#include "stm32f1xx_it.h"

extern volatile uint8_t front;
extern volatile long SysTickDelay;
extern volatile uint8_t level;
extern volatile unsigned long len;
extern uint32_t green_time, red_time, ch1_time, ch2_time;
volatile uint32_t temp_timer, filter_count;

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
	if(SysTickDelay)SysTickDelay--;
	if(green_time)green_time--;
	if(red_time)red_time--;
	if(ch1_time)ch1_time--;
	if(ch2_time)ch2_time--;
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
      //durationL=LL_TIM_GetCounter(TIM4); //low level duration
		len=LL_TIM_GetCounter(TIM4);
      LL_TIM_SetCounter(TIM4, 0);
      //LL_GPIO_ResetOutputPin(GPIOB, redLed_Pin);    
    }						
    else
    {/*
			temp_timer=TIM4->CNT;
			LL_TIM_DisableCounter(TIM4);
			WRITE_REG(TIM4->CNT, 0);			
			LL_TIM_SetAutoReload(TIM4, 200);
			*/
      //обработка заднего фронта
      //Enable Rising trigger line 8
      LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_8);
      LL_EXTI_DisableFallingTrig_0_31(LL_EXTI_LINE_8); 
			front=1;
			level=1;
			len=TIM4->CNT;	//LL_TIM_GetCounter(TIM4);
      //duration=LL_TIM_GetCounter(TIM4);		 //high level duration
			LL_TIM_SetCounter(TIM4, 0);      
      //LL_GPIO_SetOutputPin(GPIOB, redLed_Pin);      
    }		
	}
	
	
  if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_9) != RESET)
  {
    LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_9);
		
		/*
		button interrupt
		*/
  }
}



void TIM4_IRQHandler(void)
{
	//count4++;

}
