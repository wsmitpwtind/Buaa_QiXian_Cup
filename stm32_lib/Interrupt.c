#include	"stm32f10x_Reg.h"
#include	"stm32f10x_Init.h"
#include	"delay.h"
#include  	"BoardLib.h"


/*********************************************
STM32滴答计数器实现的 中断函数，比如5ms中断一次,

（由配置文件中SysBeat_Cycle决定）

用于事件，程序运行，计时等
**********************************************/


void SysTick_Handler(void)
{	
	BeatHandle_Servo();
	BeatHandle_Key();
	KFSynchronous();
	BeatHandle_Beep();
	
	BeatHandle_SYN500RPin();
	PIDSPD_SinglePulse();

	
}






/*********************************************

用于精确的 延时，系统运行时间，时间计数等时基

**********************************************/
void TIM3_IRQHandler(void)   
{
	if(TIM3_SR&0x0001) 
	{BeatHandle_delay(); TIM3_SR&=~0x0001; }//溢出中断
	else
	{BeatHandle_SinglePulse();}

	//TIM3_SR=0;
}




/*********************************************

用于wifi,0.1秒中断一次，优先级最低

**********************************************/
void TIM5_IRQHandler(void)   
{
	TIM5_SR=0;

}




/*********************************************

x线外部中断函数

**********************************************/
void EXTI1_IRQHandler(void)
{
	EXTI_MarkCLR(1);	
	SYN500RPin_IRQHandler();
}



/*********************************************

x线外部中断函数

**********************************************/
void EXTI3_IRQHandler(void)
{
	EncoderA_IRQ();
}


void EXTI4_IRQHandler(void)
{
	EncoderB_IRQ();
}



void EXTI9_5_IRQHandler(void)
{
	
	EncoderC_IRQ();
}


void EXTI15_10_IRQHandler(void)
{
	EncoderD_IRQ();
}

/*********************************************


串口中断

**********************************************/
void USART1_IRQHandler(void)
{
	uchar UD=USART1_DR; 
	ATUART_RQHandler(UD);
	
}

/*********************************************


串口中断

**********************************************/
void USART2_IRQHandler(void)
{
	uchar UD=USART2_DR; 
	
}


/*********************************************


串口中断

**********************************************/
void USART3_IRQHandler(void)
{
	uchar UD=USART3_DR;
	
}


/*********************************************


串口中断

**********************************************/
void UART4_IRQHandler(void)
{
	uchar UD=USART4_DR;

}


/*********************************************


串口中断

**********************************************/
void UART5_IRQHandler(void)
{
	uchar UD=USART5_DR;
	//WIFIUART_IRQ(UD);
	
}











