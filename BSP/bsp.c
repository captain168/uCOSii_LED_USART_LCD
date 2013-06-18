#include "bsp.h"


//���������ʼ��
void  BSP_Init (void)
{
	GPIO_Configuration();
	EXTI_Config();
	NVIC_Configuration();
}

//�ܽ�����
void GPIO_Configuration(void)
{
	STM_EVAL_LEDInitAll();
	USART1_Config();
	STM324xG_LCD_Init();
}

//�����ж�����
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

//�ж�����
void NVIC_Configuration(void)
{

}
/*********************************************************************************
*��������ȡCPU�ļĴ�����ȷ��CPUʱ��Ƶ�ʵ�оƬ
*
*����ֵ��CPU��ʱ��Ƶ�ʣ���λΪHz��
**********************************************************************************/
INT32U  BSP_CPU_ClkFreq (void)
{
    RCC_ClocksTypeDef  rcc_clocks;


    RCC_GetClocksFreq(&rcc_clocks);

    return ((INT32U)rcc_clocks.HCLK_Frequency);
}

/*********************************************************************************
*������ϵͳ�δ��ʱ��Ƶ��
*
*����ֵ��ʱ��Ƶ�ʣ�ϵͳʱ�����ڣ���
**********************************************************************************/
INT32U  OS_CPU_SysTickClkFreq (void)
{
    INT32U  freq;


    freq = BSP_CPU_ClkFreq();
    return (freq);
}
