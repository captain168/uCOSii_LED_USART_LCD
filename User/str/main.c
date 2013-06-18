/*************************************************************************************
  * �ļ�������uCOS�����ļ�������һ����򵥵�uCOS��Ӧ��ģ�ͣ��򵥵�����uCOS����Led�ƣ�
  * ����֮������Լ��ֶ���ֲ������
  * ���ӵ���оƬ��STM32F407ZGT��
  * �������ڣ�2012-09-29
**************************************************************************************/
#include "stm32f4xx.h"
#include "ucos_ii.h"
#include "app_cfg.h"
#include "delay.h"
#include "bsp.h"
#include "stm324xg_eval.h"
#include "stm324xg_eval_lcd.h"
#include "usart.h"




static  OS_STK App_TaskStartStk[APP_TASK_START_STK_SIZE];	            //��ʼ�����Ӧ�Ķ�ջ
static  OS_STK App_Task1Stk[APP_TASK_1_STK_SIZE];			            //��һ�������Ӧ�Ķ�ջ
static  OS_STK App_Task_Printf_Stk[APP_TASK_Printf_STK_SIZE];			//�ڶ��������Ӧ�Ķ�ջ


static  void  App_TaskStart(void *p_arg);                               //��ʼ����
static  void  App_TaskCreate(void);							            //������������
static  void  App_Task1(void *p_arg);						            //��һ������
static  void  App_Task_Printf(void *p_arg);						        //�ڶ�������

OS_EVENT *Mutex_USART1;	// ���廥�����¼�

/*************************************************************************************
  * �������ƣ�main()
  * ����    ��void
  * ����ֵ  ��void
  * ����    �����������main����
**************************************************************************************/
int main(void)
{  
    OSInit();                                                           //��ʼ��OS�ں�
    OSTaskCreate((void (*)(void *)) App_TaskStart,     				    //������ʼ����
                             (void          * ) 0,
                             (OS_STK        * )&App_TaskStartStk[APP_TASK_START_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_START_PRIO);
#if (OS_TASK_NAME_SIZE >= 11)
    OSTaskNameSet(APP_TASK_START_PRIO, (CPU_INT08U *)"Start Task", &os_err);
#endif
    OSStart();                                                  //��ʼ�������                                             
    return (0);
}
//��ʼ������
static  void  App_TaskStart (void *p_arg)
{	 
    (void)p_arg;                                                //Initialize BSP functions. 
	//��ʼ��ϵͳ����ʱ��                     
    OS_CPU_SysTickInit();                                        
	//ͳ������                              
#if (OS_TASK_STAT_EN > 0)
    OSStatInit();                                               // Determine CPU capacity.                          
#endif

	/* �ڴ˿ɴ����������� */
    App_TaskCreate();	                                        //������һ������
	BSP_Init();	                                                //����ĳ�ʼ��
    while (1) 
	{ 	
		OSTaskSuspend(OS_PRIO_SELF);  // ��������
    }
}

/*************************************************************************************
  * �������ƣ�App_TaskCreate()
  * ����    ��void
  * ����ֵ  ��void
  * ����    ����������
***************************************************************************************/
static  void App_TaskCreate(void)
{
    OSTaskCreate((void (*)(void *)) App_Task1,         //Create the start task
                             (void          * ) 0,
                             (OS_STK        * )&App_Task1Stk[APP_TASK_1_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_1_PRIO);  
    OSTaskCreate((void (*)(void *)) App_Task_Printf,         //Create the start task
                             (void          * ) 0,
                             (OS_STK        * )&App_Task_Printf_Stk[APP_TASK_Printf_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_Printf_PRIO);
}
/*************************************************************************************
  * �������ƣ�App_Task1()
  * ����    ��void
  * ����ֵ  ��void
  * ����    ����һ������ִ�г���
***************************************************************************************/
static  void  App_Task1(void *p_arg)
{
	INT8U  err;
	(void)p_arg;
	Mutex_USART1 = OSMutexCreate(USART1_MUTEX_PRIO, &err);  // ����USART1�������ź���
	 while(1)
	{
		STM_EVAL_LEDOffAll();
		OSTimeDly(300);
		STM_EVAL_LEDOnAll();
		OSTimeDly(300);	
		OSMutexPend(Mutex_USART1, 0, &err);	 // �ȴ��ź���
	    USART1_Printf("**�����������**\r\n");		
		OSMutexPost(Mutex_USART1);			 // �ͷ��ź���	
	}	
}

static void App_Task_Printf(void *p_arg)
{
	INT8U err;
	(void)p_arg;
	OSTaskSuspend(OS_PRIO_SELF);	         // �Ƚ���������ڴ��ڽ����ж��лָ�����
	OSMutexPend(Mutex_USART1, 0, &err);	 // �ȴ��ź���
	/*while(1)
	{
		STM_EVAL_LEDOff(LED1);;
		OSTimeDly(300);
		STM_EVAL_LEDOn(LED1);
		OSTimeDly(300);	
	} */
	USART1_Printf("**���ڽ�������**\r\n");
	OSMutexPost(Mutex_USART1);			 // �ͷ��źſ 
	LCD_SetBackColor(Red);
	LCD_SetTextColor(White);
	LCD_DisplayStringLine(LINE(0), "zdd is a   good man!");
	LCD_DisplayStringLine(LINE(1), "12345678912345678912");
	LCD_DisplayStringLine(LINE(9), "xianshi Over!");
}

