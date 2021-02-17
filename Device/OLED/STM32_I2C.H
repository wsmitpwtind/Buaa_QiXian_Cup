#ifndef _STM32_I2C_H_
#define _STM32_I2C_H_

#include		"stm32f10x_Reg.h"
#include		"stm32f10x_type.h"
#include		"stm32f10x_Init.h"
#include		"delay.h"   



#define I2C_SCL_outmode  Port_In_Out(PortC,BIT5,GE_OC_10M)//输出模式 10M 
#define I2C_SCL_input    PCin(5)	
#define I2C_SCL_clr      PCout(5)=0 					
#define I2C_SCL_setb     PCout(5)=1

#define I2C_SDA_outmode  Port_In_Out(PortC,BIT4,GE_OC_10M)//输出模式 10M 
#define I2C_SDA_input    PCin(4)			      
#define I2C_SDA_clr		 PCout(4)=0 	 	
#define I2C_SDA_setb	 PCout(4)=1


//---------------------------------
uchar I2C_start(void);
uchar I2C_stop(void);
uchar I2C_RACK(void);
uchar I2C_WACK(uchar ACK);
uchar I2C_Wbyte(uchar Wdat);
uchar I2C_Rbyte(void);
//----------------------------------

void  I2C_Init(usint freq);
uchar I2C_SendByte(uchar Chip_Adds,uchar Reg_Adds,uchar Wdat);
uchar I2C_SendByte_16ADD(uchar Chip_Adds,usint Reg_Adds,uchar Wdat);
uchar I2C_SendString(uchar Chip_Adds,uchar Reg_Adds,uchar R_LEN,uchar *buf);
uchar I2C_SendString_16ADD(uchar Chip_Adds,usint Reg_Adds,usint R_LEN,uchar *buf);

uchar I2C_ReceByte(uchar ACK,uchar Chip_Adds,uchar Reg_Adds,uchar *Rdat);
uchar I2C_ReceByte_16ADD(uchar ACK,uchar Chip_Adds,usint Reg_Adds,uchar *Rdat);
uchar I2C_ReceString(uchar Chip_Adds,uchar Reg_Adds,usint R_LEN,uchar *buf);
uchar I2C_ReceString_16ADD(uchar Chip_Adds,usint Reg_Adds,usint R_LEN,uchar *buf);

uchar I2C_CheckBusy(uchar Chip_Adds); //返回0则空闲，1正在写

uchar I2C_SendString_full(uchar Chip_Adds,uchar Reg_Adds,uchar R_LEN,uchar iD);

#endif




