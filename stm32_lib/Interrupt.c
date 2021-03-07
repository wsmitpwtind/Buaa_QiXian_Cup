#include	"stm32f10x_Reg.h"
#include	"stm32f10x_Init.h"
#include	"delay.h"
#include  	"BoardLib.h"


/*********************************************
STM32�δ������ʵ�ֵ� �жϺ���������5ms�ж�һ��,

���������ļ���SysBeat_Cycle������

�����¼����������У���ʱ��
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

���ھ�ȷ�� ��ʱ��ϵͳ����ʱ�䣬ʱ�������ʱ��

**********************************************/
void TIM3_IRQHandler(void)   
{
	if(TIM3_SR&0x0001) 
	{BeatHandle_delay(); TIM3_SR&=~0x0001; }//����ж�
	else
	{BeatHandle_SinglePulse();}

	//TIM3_SR=0;
}




/*********************************************

����wifi,0.1���ж�һ�Σ����ȼ����

**********************************************/
void TIM5_IRQHandler(void)   
{
	TIM5_SR=0;

}




/*********************************************

x���ⲿ�жϺ���

**********************************************/
void EXTI1_IRQHandler(void)
{
	EXTI_MarkCLR(1);	
	SYN500RPin_IRQHandler();
}



/*********************************************

x���ⲿ�жϺ���

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


�����ж�

**********************************************/
void USART1_IRQHandler(void)
{
	uchar UD=USART1_DR; 
	ATUART_RQHandler(UD);
	
}

/*********************************************


�����ж�

**********************************************/
void USART2_IRQHandler(void)
{
	uchar UD=USART2_DR; 
	
}


/*********************************************


�����ж�

**********************************************/
void USART3_IRQHandler(void)
{
	uchar UD=USART3_DR;
	
}


/*********************************************


�����ж�

**********************************************/
void UART4_IRQHandler(void)
{
	uchar UD=USART4_DR;

}


/*********************************************


�����ж�

**********************************************/
void UART5_IRQHandler(void)
{
	uchar UD=USART5_DR;
	//WIFIUART_IRQ(UD);
	
}











