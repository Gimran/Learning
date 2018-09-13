#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_exti.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_pwr.h"
#include "stm32f1xx_ll_dma.h"
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx.h"
#include "stm32f1xx_ll_gpio.h"

#include "flash.h"
#include "keeloq.h"


static void LL_Init(void);
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM4_Init(void);

void Delay( unsigned int Val);

struct keeloqStr
{
  uint8_t state;
  unsigned long TE;
  uint8_t pre_count, data[9], dat_bit;
};

extern struct keeloqStr keeloq;



#define GREEN_ON LL_GPIO_ResetOutputPin(GPIOB, greenLed_Pin);
#define GREEN_OFF LL_GPIO_SetOutputPin(GPIOB, greenLed_Pin);
#define RED_ON LL_GPIO_SetOutputPin(GPIOB, redLed_Pin);
#define RED_OFF LL_GPIO_ResetOutputPin(GPIOB, redLed_Pin);
#define OUT_ON LL_GPIO_SetOutputPin(out1_GPIO_Port, out1_Pin|out2_Pin);
#define OUT_OFF LL_GPIO_ResetOutputPin(out1_GPIO_Port, out1_Pin|out2_Pin);

#define FANTOM_ID 0x26b41b    //0xa40550

	




/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define out1_Pin LL_GPIO_PIN_0
#define out1_GPIO_Port GPIOB
#define out2_Pin LL_GPIO_PIN_1
#define out2_GPIO_Port GPIOB
#define redLed_Pin LL_GPIO_PIN_10
#define redLed_GPIO_Port GPIOB
#define greenLed_Pin LL_GPIO_PIN_11
#define greenLed_GPIO_Port GPIOB
#define rfData_Pin LL_GPIO_PIN_8
#define rfData_GPIO_Port GPIOB
#define rfData_EXTI_IRQn EXTI9_5_IRQn
#define Btn_Pin LL_GPIO_PIN_9
#define Btn_GPIO_Port GPIOB
#define Btn_EXTI_IRQn EXTI9_5_IRQn

#define ch1 LL_GPIO_PIN_3
#define ch2 LL_GPIO_PIN_4
#define ch3 LL_GPIO_PIN_5
#define ch4 LL_GPIO_PIN_6
#ifndef NVIC_PRIORITYGROUP_0
#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority,
                                                                 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bit  for pre-emption priority,
                                                                 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority,
                                                                 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority,
                                                                 1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority,
                                                                 0 bit  for subpriority */
#endif

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
