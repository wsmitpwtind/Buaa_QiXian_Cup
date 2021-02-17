#ifndef __PS2_H__
#define __PS2_H__

#include		"stm32f10x_Reg.h"
#include		"stm32f10x_type.h"
#include		"stm32f10x_Init.h"
#include		"delay.h"   


#define PS2_CS_outmode		Port_In_Out(PortC,BIT2,GE_TW_10M) //输出模式 10M 
#define PS2_CS				PCout(2)

#define PS2_sck_outmode		Port_In_Out(PortA,BIT5,GE_TW_10M) //输出模式 10M 
#define PS2_sck				PAout(5)

#define PS2_miso_inmode		Port_In_Out(PortA,BIT6,Updown_IN)	//输出模式 10M 
#define PS2_miso_indat		PAin(6)	
#define PS2_miso			PAout(6)

#define PS2_mosi_outmode	Port_In_Out(PortA,BIT7,GE_TW_10M) //输出模式 10M 
#define PS2_mosi			PAout(7)


//-------------按键值定义-------------

#define _PS2_Key_Dir_U  0x0010
#define _PS2_Key_Dir_D  0x0040
#define _PS2_Key_Dir_L  0x0080
#define _PS2_Key_Dir_R  0x0020

#define _PS2_Key_Butt_U  0x1000
#define _PS2_Key_Butt_D  0x4000
#define _PS2_Key_Butt_L  0x8000
#define _PS2_Key_Butt_R  0x2000

#define _PS2_Key_SideL_1 0x0400
#define _PS2_Key_SideL_2 0x0100

#define _PS2_Key_SideR_1 0x0800
#define _PS2_Key_SideR_2 0x0200

#define _PS2_Key_SELECT  0x0001
#define _PS2_Key_MODE    0x0008

void   PS2_Init(void);
uchar  Get_PS2Dat(uchar *buf);
usint  PS2_ChangeKey(uchar *inBuf);

#endif


