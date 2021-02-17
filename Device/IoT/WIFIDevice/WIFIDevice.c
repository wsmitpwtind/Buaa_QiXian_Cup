#include	"stm32f10x_Reg.h"
#include	"stm32f10x_Init.h"
#include	"WIFIDevice.h"
#include 	<string.h>
#include	<stdio.h>
#include	"delay.h" 
#include	"ATOrg.h" 
#include	"string.h" 
#include	"stdlib.h" 
#include	"OLED_I2C.h"


static uchar WIFIDRelayEnable=0;  //����Ϊ1��ʱ�򣬿���ͨ������1 �������TypeC�ӿڣ� ֱ���շ�ATָ�����WIFIģ��

extern uchar TRAutoModeMark;  //�Ѿ�����͸��ģʽ�ı�־λ

//��λwifi�豸
void WIFIDevRST(void)
{
WIFIRST_L;
delay(200);
WIFIRST_H;
delay(100);

TRAutoModeMark=0;  //�Ѿ�����͸��ģʽ�ı�־λ
}

//��ʼ��wifi�豸
void WIFIDevInit(void)
{

WIFIRST_OutMode;
WIFIRST_L;
	
USART_Init(5,115200);	//����3��ʼ��
USART5_CR1|=(1<<5);		//ʹ��USART�����ж�
Nvic_Init(2,1,1,UART5_IRQChannel); 

TRAutoModeMark=0; 

}

/*
wifiģ������ת��������������ֱ��͸����USB���ڣ�usrt1���ϣ�һ�������Ͳ���WIFIģ����
En=1:�����ù���
*/
void WIFIDRelaySET(uchar Enable)
{
	WIFIDRelayEnable=Enable;
}

uchar WIFIDRelayGET(void)
{
	return WIFIDRelayEnable;
}



void WIFIUART_IRQ(uchar iD)
{
	if(WIFIDRelayGET())
	{
		if(USART1_SR&0x80) USART1_DR=iD;  //UART1��������
	}
	else
	{
		ATDataStream(iD);
	}
}


	
	
void WSData(uchar * buf, usint rL)
{
	UART5_Send_Length(buf,rL);
}


void WSString(uchar * str)
{
	UART5_Send_String(str);
}



