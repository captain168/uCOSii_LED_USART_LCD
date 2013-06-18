/*************************************************************************************
  * ÎÄ¼şÃèÊö£ºuCOSµÄÖ÷ÎÄ¼ş£¬ÕâÊÇÒ»¸ö×î¼òµ¥µÄuCOSµÄÓ¦ÓÃÄ£ĞÍ£¬¼òµ¥µÄÀûÓÃuCOSµãÁÁLedµÆ£¬
  * ¿´ÍêÖ®ºó¿ÉÒÔ×Ô¼ºÊÖ¶¯ÒÆÖ²¿´¿´¡£
  * °å×ÓµÄÖ÷Ğ¾Æ¬£ºSTM32F407ZGT¡£
  * ´´½¨ÈÕÆÚ£º2012-09-29
**************************************************************************************/
#include "stm32f4xx.h"
#include "ucos_ii.h"
#include "app_cfg.h"
#include "delay.h"
#include "bsp.h"
#include "stm324xg_eval.h"
#include "stm324xg_eval_lcd.h"
#include "usart.h"




static  OS_STK App_TaskStartStk[APP_TASK_START_STK_SIZE];	            //¿ªÊ¼ÈÎÎñ¶ÔÓ¦µÄ¶ÑÕ»
static  OS_STK App_Task1Stk[APP_TASK_1_STK_SIZE];			            //µÚÒ»¸öÈÎÎñ¶ÔÓ¦µÄ¶ÑÕ»
static  OS_STK App_Task_Printf_Stk[APP_TASK_Printf_STK_SIZE];			//µÚ¶ş¸öÈÎÎñ¶ÔÓ¦µÄ¶ÑÕ»


static  void  App_TaskStart(void *p_arg);                               //¿ªÊ¼ÈÎÎñ
static  void  App_TaskCreate(void);							            //´´½¨×ÓÈÎÎñº¯Êı
static  void  App_Task1(void *p_arg);						            //µÚÒ»¸öÈÎÎñ
static  void  App_Task_Printf(void *p_arg);						        //µÚ¶ş¸öÈÎÎñ

OS_EVENT *Mutex_USART1;	// ¶¨Òå»¥³âĞÍÊÂ¼ş

/*************************************************************************************
  * º¯ÊıÃû³Æ£ºmain()
  * ²ÎÊı    £ºvoid
  * ·µ»ØÖµ  £ºvoid
  * ÃèÊö    £º³ÌĞòÖ÷Èë¿Úmainº¯Êı
**************************************************************************************/
int main(void)
{  
    OSInit();                                                           //³õÊ¼»¯OSÄÚºË
    OSTaskCreate((void (*)(void *)) App_TaskStart,     				    //½¨Á¢¿ªÊ¼ÈÎÎñ
                             (void          * ) 0,
                             (OS_STK        * )&App_TaskStartStk[APP_TASK_START_STK_SIZE - 1],
                             (INT8U           ) APP_TASK_START_PRIO);
#if (OS_TASK_NAME_SIZE >= 11)
    OSTaskNameSet(APP_TASK_START_PRIO, (CPU_INT08U *)"Start Task", &os_err);
#endif
    OSStart();                                                  //¿ªÊ¼ÈÎÎñµ÷¶È                                             
    return (0);
}
//¿ªÊ¼ÈÎÎñº¯Êı
static  void  App_TaskStart (void *p_arg)
{	 
    (void)p_arg;                                                //Initialize BSP functions. 
	//³õÊ¼»¯ÏµÍ³½ÚÅÄÊ±ÖÓ                     
    OS_CPU_SysTickInit();                                        
	//Í³¼ÆÈÎÎñ                              
#if (OS_TASK_STAT_EN > 0)
    OSStatInit();                                               // Determine CPU capacity.                          
#endif

	/* ÔÚ´Ë¿É´´½¨ÆäËûÈÎÎñ */
    App_TaskCreate();	                                        //½¨Á¢µÚÒ»¸öÈÎÎñ
	BSP_Init();	                                                //ÍâÉèµÄ³õÊ¼»¯
    while (1) 
	{ 	
		OSTaskSuspend(OS_PRIO_SELF);  // ¹ÒÆğÈÎÎñ
    }
}

/*************************************************************************************
  * º¯ÊıÃû³Æ£ºApp_TaskCreate()
  * ²ÎÊı    £ºvoid
  * ·µ»ØÖµ  £ºvoid
  * ÃèÊö    £º´´½¨ÈÎÎñ
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
  * º¯ÊıÃû³Æ£ºApp_Task1()
  * ²ÎÊı    £ºvoid
  * ·µ»ØÖµ  £ºvoid
  * ÃèÊö    £ºµÚÒ»¸öÈÎÎñÖ´ĞĞ³ÌĞò
***************************************************************************************/
static  void  App_Task1(void *p_arg)
{
	INT8U  err;
	(void)p_arg;
	Mutex_USART1 = OSMutexCreate(USART1_MUTEX_PRIO, &err);  // ½¨Á¢USART1»¥³âĞÍĞÅºÅÁ¿
	 while(1)
	{
		STM_EVAL_LEDOffAll();
		OSTimeDly(300);
		STM_EVAL_LEDOnAll();
		OSTimeDly(300);	
		OSMutexPend(Mutex_USART1, 0, &err);	 // µÈ´ıĞÅºÅÁ¿
	    USART1_Printf("**´®¿ÚÊä³öÈÎÎñ**\r\n");		
		OSMutexPost(Mutex_USART1);			 // ÊÍ·ÅĞÅºÅÁ¿	
	}	
}

static void App_Task_Printf(void *p_arg)
{
	INT8U err;
	(void)p_arg;
	OSTaskSuspend(OS_PRIO_SELF);	         // ÏÈ½«ÈÎÎñ¹ÒÆğ£¬ÔÚ´®¿Ú½ÓÊÕÖĞ¶ÏÖĞ»Ö¸´ÈÎÎñ
	OSMutexPend(Mutex_USART1, 0, &err);	 // µÈ´ıĞÅºÅÁ¿
	/*while(1)
	{
		STM_EVAL_LEDOff(LED1);;
		OSTimeDly(300);
		STM_EVAL_LEDOn(LED1);
		OSTimeDly(300);	
	} */
	USART1_Printf("**´®¿Ú½ÓÊÕÈÎÎñ**\r\n");
	OSMutexPost(Mutex_USART1);			 // ÊÍ·ÅĞÅºÅ¿ 
	LCD_SetBackColor(Red);
	LCD_SetTextColor(White);
	LCD_DisplayStringLine(LINE(0), "zdd is a   good man!");
	LCD_DisplayStringLine(LINE(1), "12345678912345678912");
	LCD_DisplayStringLine(LINE(9), "xianshi Over!");
}

