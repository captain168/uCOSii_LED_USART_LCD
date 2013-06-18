#ifndef  BSP_PRESENT
#define  BSP_PRESENT

#ifdef   BSP_MODULE
#define  BSP_EXT
#else
#define  BSP_EXT  extern
#endif


#include "ucos_ii.h"
#include "stm32f4xx.h"
#include "stm324xg_eval.h"
#include "stm324xg_eval_lcd.h"
#include "usart.h"


void         BSP_Init                    (void);

INT32U   BSP_CPU_ClkFreq             (void);

void GPIO_Configuration(void);
void EXTI_Config(void);
void NVIC_Configuration(void);

/*
*********************************************************************************************************
*                                           INTERRUPT SERVICES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of module include.                               */
