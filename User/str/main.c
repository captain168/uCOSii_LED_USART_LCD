/*************************************************************************************
  * 文件描述：uCOS的主文件，这是一个最简单的uCOS的应用模型，简单的利用uCOS点亮Led灯，
  * 看完之后可以自己手动移植看看。
  * 板子的主芯片：STM32F407ZGT。
  * 创建日期：2012-09-29
**************************************************************************************/
#include "stm32f4xx.h"
#include "ucos_ii.h"
#include "app_cfg.h"
#include "delay.h"
#include "bsp.h"
#include "stm324xg_eval.h"
#include "stm324xg_eval_lcd.h"
#include "usart.h"




static  OS_STK App_TaskStartStk[APP_TASK_START_STK_SIZE];	            //开始任务对应的堆栈
static  OS_STK App_Task1Stk[APP_TASK_1_STK_SIZE];			            //第一个任务对应的堆栈
static  OS_STK App_Task_Printf_Stk[APP_TASK_Printf_STK_SIZE];			//第二个任务对应的堆栈


static  void  App_TaskStart(void *p_arg);                               //开始任务
static  void  App_TaskCreate(void);							            //创建子任务函数
static  void  App_Task1(void *p_arg);						            //第一个任务
static  void  App_Task_Printf(void *p_arg);						        //第二个任务

OS_EVENT *Mutex_USART1;	// 定义互斥型事件

/*************************************************************************************
  * 函数名称：main()
  * 参数    ：void
  * 返回值  ：void
  * 描述    ：程序主入口main函数
**************************************************************************************/
int main(void)
{  
    OSInit();                                                           //初始化OS内核
    OSTaskCreate((void (*)(void *)) App_TaskStart,     				    //建立开始任务
                             (void          * ) 0,
                             (OS_STK        * )&App_TaskStartStk[APP_TASK_START_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_START_PRIO);
#if (OS_TASK_NAME_SIZE >= 11)
    OSTaskNameSet(APP_TASK_START_PRIO, (CPU_INT08U *)"Start Task", &os_err);
#endif
    OSStart();                                                  //开始任务调度                                             
    return (0);
}
//开始任务函数
static  void  App_TaskStart (void *p_arg)
{	 
    (void)p_arg;                                                //Initialize BSP functions. 
	//初始化系统节拍时钟                     
    OS_CPU_SysTickInit();                                        
	//统计任务                              
#if (OS_TASK_STAT_EN > 0)
    OSStatInit();                                               // Determine CPU capacity.                          
#endif

	/* 在此可创建其他任务 */
    App_TaskCreate();	                                        //建立第一个任务
	BSP_Init();	                                                //外设的初始化
    while (1) 
	{ 	
		OSTaskSuspend(OS_PRIO_SELF);  // 挂起任务
    }
}

/*************************************************************************************
  * 函数名称：App_TaskCreate()
  * 参数    ：void
  * 返回值  ：void
  * 描述    ：创建任务
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
  * 函数名称：App_Task1()
  * 参数    ：void
  * 返回值  ：void
  * 描述    ：第一个任务执行程序
***************************************************************************************/
static  void  App_Task1(void *p_arg)
{
	INT8U  err;
	(void)p_arg;
	Mutex_USART1 = OSMutexCreate(USART1_MUTEX_PRIO, &err);  // 建立USART1互斥型信号量
	 while(1)
	{
		STM_EVAL_LEDOffAll();
		OSTimeDly(300);
		STM_EVAL_LEDOnAll();
		OSTimeDly(300);	
		OSMutexPend(Mutex_USART1, 0, &err);	 // 等待信号量
	    USART1_Printf("**串口输出任务**\r\n");		
		OSMutexPost(Mutex_USART1);			 // 释放信号量	
	}	
}

static void App_Task_Printf(void *p_arg)
{
	INT8U err;
	(void)p_arg;
	OSTaskSuspend(OS_PRIO_SELF);	         // 先将任务挂起，在串口接收中断中恢复任务
	OSMutexPend(Mutex_USART1, 0, &err);	 // 等待信号量
	/*while(1)
	{
		STM_EVAL_LEDOff(LED1);;
		OSTimeDly(300);
		STM_EVAL_LEDOn(LED1);
		OSTimeDly(300);	
	} */
	USART1_Printf("**串口接收任务**\r\n");
	OSMutexPost(Mutex_USART1);			 // 释放信号� 
	LCD_SetBackColor(Red);
	LCD_SetTextColor(White);
	LCD_DisplayStringLine(LINE(0), "zdd is a   good man!");
	LCD_DisplayStringLine(LINE(1), "12345678912345678912");
	LCD_DisplayStringLine(LINE(9), "xianshi Over!");
}

