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


static uchar WIFIDRelayEnable=0;  //设置为1的时候，可以通过串口1 （主板的TypeC接口） 直接收发AT指令，调试WIFI模块

extern uchar TRAutoModeMark;  //已经进入透传模式的标志位

//复位wifi设备
void WIFIDevRST(void)
{
WIFIRST_L;
delay(200);
WIFIRST_H;
delay(100);

TRAutoModeMark=0;  //已经进入透传模式的标志位
}

//初始化wifi设备
void WIFIDevInit(void)
{

WIFIRST_OutMode;
WIFIRST_L;
	
USART_Init(5,115200);	//串口3初始化
USART5_CR1|=(1<<5);		//使能USART接收中断
Nvic_Init(2,1,1,UART5_IRQChannel); 

TRAutoModeMark=0; 

}

/*
wifi模块数据转发，将串口数据直接透传到USB串口（usrt1）上，一般升级和测试WIFI模块用
En=1:开启该功能
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
		if(USART1_SR&0x80) USART1_DR=iD;  //UART1发送数据
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



