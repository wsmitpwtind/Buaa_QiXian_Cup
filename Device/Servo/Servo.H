#ifndef __Servo__
#define __Servo__

#include		"stm32f10x_Reg.h"
#include		"stm32f10x_type.h"
#include		"stm32f10x_Init.h"
#include		"delay.h"  

#define ServoP0_OutMode  Port_In_Out(PortB,BIT10,GE_TW_2M)
#define	ServoP0_H        PBout(10)=1	
#define ServoP0_L        PBout(10)=0

#define ServoP1_OutMode  Port_In_Out(PortB,BIT11,GE_TW_2M)
#define	ServoP1_H        PBout(11)=1	
#define ServoP1_L        PBout(11)=0

#define ServoP2_OutMode  Port_In_Out(PortB,BIT12,GE_TW_2M)
#define	ServoP2_H        PBout(12)=1	
#define ServoP2_L        PBout(12)=0

#define ServoP3_OutMode  Port_In_Out(PortB,BIT13,GE_TW_2M)
#define	ServoP3_H        PBout(13)=1	
#define ServoP3_L        PBout(13)=0



void ServoInit(void);
void BeatHandle_Servo(void);
void SSenvo(uchar Num,usint val);
void SetSenvo(uchar Num,usint val,uchar gap);

#endif


