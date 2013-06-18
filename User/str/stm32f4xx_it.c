#include "stm32f4xx_it.h"
#include <ucos_ii.h>

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
/*void PendSV_Handler(void)
{
}	*/

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
extern void TimeTick_Decrement(void);	    //�����ⲿ�жϼ�������
void SysTick_Handler(void)
{
  //TimingDelay_Decrement();
    OS_CPU_SR  cpu_sr;
	TimeTick_Decrement();
    OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR          */
    OSIntNesting++;
    OS_EXIT_CRITICAL();
    OSTimeTick();                                /* Call uC/OS-II's OSTimeTick()                       */
    OSIntExit();   //�����жϼ�������
}

void USART1_IRQHandler(void)
{
    OS_CPU_SR  cpu_sr;

    OS_ENTER_CRITICAL();  //����ȫ���жϱ�־,�����ж� Tell uC/OS-II that we are starting an ISR
    OSIntNesting++;
    OS_EXIT_CRITICAL();	  //�ָ�ȫ���жϱ�־*/

    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)	  //����Ǵ��ڽ����ж�
	{	
		USART1_SendByte(USART_ReceiveData(USART1));	
        //USART1_RXLoop(USART_ReceiveData(USART1));	          //�����ݷ��뻷�ζ���
		OSTaskResume(APP_TASK_Printf_PRIO);
    }

    OSIntExit();	
}
/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
