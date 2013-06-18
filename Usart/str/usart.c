#include "usart.h"
 /******************************************************************************
  * 文件路径  ：   Hardware/usart/usart.c 
  * 作者      ：   农晓明   QQ：382421307
  * 固件库版本：   V1.0.0
  * 日期      ：   2012-03-28
  * 文件描述  ：   提供对板上串口的配置
  ******************************************************************************/

#define USART1_RX_CNT 100   // 定义环形数组大小，根据实际情况相应增减
INT8U USART1_RX_FIFO[USART1_RX_CNT];  //  定义环形数据
INT8U *PPut_USART1_RX_FIFO = USART1_RX_FIFO; //  定义环形数组放数指针
INT8U *PGet_USART1_RX_FIFO = USART1_RX_FIFO; //  定义环形数组取数指针

/*****************************************************************

          USART1 接收数据入队，中断接收调用
          功能：将temp放入环形数组

*****************************************************************/
void USART1_RXLoop(INT8U temp)
{
    *PPut_USART1_RX_FIFO = temp;
    if(PPut_USART1_RX_FIFO == &USART1_RX_FIFO[USART1_RX_CNT - 1]){
        PPut_USART1_RX_FIFO = &USART1_RX_FIFO[0];	
    }else{
        PPut_USART1_RX_FIFO++;
    }
}
/*******************************************************************************************************************************
 * 函数名：USART1_Config()
 * 参数：无
 *返回值：无
 * 功能：配置USART1，并初始化
********************************************************************************************************************************/																		
void USART1_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;                                          //定义GPIO初始化结构体
	USART_InitTypeDef USART_InitStructure;                                         //定义USART初始化结构体

	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	 RCC_AHB2PeriphClockCmd(RCC_APB2Periph_USART1 , ENABLE);
	/*配置复用管脚TXD(PA9)和RXD(PA10)连接到USART1*/
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);						             //配置PA9复用连接到USART1
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);					           //配置PA10复用连接到USART1
	/*配置USART1相应的时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);                          //使能GPIOA的AHB1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);                        //使能USART1的APB2时钟
	/*配置USART1的发送管脚TXD(PA9)为复用推挽输出*/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;                                       //设置初始化GPIO为PIN9
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;                               //设置GPIO的速度为50MHz
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;                                    //设置GPIO模式为复用模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								                //设置GPIO输出类型为推挽输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;						                    //设置GPIO为连接上拉电阻
	GPIO_Init(GPIOA,&GPIO_InitStructure);                                         //初始化GPIOA的PIN9
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;                                      //设置初始化GPIO为PIN10
	/*配置USART1的接收管脚RXD(PA10)为浮空输入*/
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;                                    //设置GPIO的模式为复用模式
	GPIO_Init(GPIOA,&GPIO_InitStructure);                                         //初始化GPIOA的PIN10

	/*配置USART1的模式*/
    USART_InitStructure.USART_BaudRate=115200;                                    //设置USART的波特率为115200
    USART_InitStructure.USART_Parity=USART_Parity_No;                             //设置USART的校验位为None
    USART_InitStructure.USART_WordLength=USART_WordLength_8b;                     //设置USART的数据位为8bit
    USART_InitStructure.USART_StopBits=USART_StopBits_1;                          //设置USART的停止位为1
    USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None; //失能硬件流控制
    USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;                   //设置USART的模式为发送接收模式
    USART_Init(USART1,&USART_InitStructure);    
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                                  //初始化USART1
    USART_Cmd(USART1,ENABLE);                                                     //使能USART1
}


/********************************************************************************************************************************
*函数名：fputc()
* 参数：int ch，FILE *f
* 返回值：int
* 功能：重新定义stdio.h中的fputc()函数，使printf()输出到USART1
*********************************************************************************************************************************/
int fputc(int ch,FILE *f)
{
  USART_SendData(USART1,(u8)ch);	                                                   //将ch从USART1发送出去
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)				               //等待发送完毕
  {
  }											  
  return ch;																	   //返回 ch
}	

INT8U USART1_GetByte(INT8U *temp)
{
    if(PGet_USART1_RX_FIFO == PPut_USART1_RX_FIFO){
        return(0);
    }

    *temp = *PGet_USART1_RX_FIFO;
    if(PGet_USART1_RX_FIFO == &USART1_RX_FIFO[USART1_RX_CNT - 1]){
        PGet_USART1_RX_FIFO = &USART1_RX_FIFO[0];
    }else{
        PGet_USART1_RX_FIFO++;
    }
    return(1);
}

/*****************************************************************

          从 USART1 发送temp

*****************************************************************/
void USART1_SendByte(INT8U temp)
{
    USART_SendData(USART1, temp);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
}

/*****************************************************************

          从 USART1 发送字符串

*****************************************************************/
void USART1_Printf(INT8U *pch)
{
    while(*pch != '\0'){
        USART1_SendByte(*pch);
        pch++;
    }
}
																	           








