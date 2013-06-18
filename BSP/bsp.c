#include "bsp.h"


//驱动程序初始化
void  BSP_Init (void)
{
	GPIO_Configuration();
	EXTI_Config();
	NVIC_Configuration();
}

//管脚配置
void GPIO_Configuration(void)
{
	STM_EVAL_LEDInitAll();
	USART1_Config();
	STM324xG_LCD_Init();
}

//配置中断引脚
void EXTI_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Configure the NVIC Preemption Priority Bits */  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    /* Enable the USART1 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

//中断配置
void NVIC_Configuration(void)
{

}
/*********************************************************************************
*描述：读取CPU的寄存器来确定CPU时钟频率的芯片
*
*返回值：CPU的时钟频率，单位为Hz。
**********************************************************************************/
INT32U  BSP_CPU_ClkFreq (void)
{
    RCC_ClocksTypeDef  rcc_clocks;


    RCC_GetClocksFreq(&rcc_clocks);

    return ((INT32U)rcc_clocks.HCLK_Frequency);
}

/*********************************************************************************
*描述：系统滴答的时钟频率
*
*返回值：时钟频率（系统时钟周期）。
**********************************************************************************/
INT32U  OS_CPU_SysTickClkFreq (void)
{
    INT32U  freq;


    freq = BSP_CPU_ClkFreq();
    return (freq);
}
