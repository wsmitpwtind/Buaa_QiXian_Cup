#include "Servo.H"
#include "delay.h"



static usint SoVal[4];


void ServoFun0(void)
{
	ServoP0_L;
}

void ServoFun1(void)
{
	ServoP1_L;
}
void ServoFun2(void)
{
	ServoP2_L;
}
void ServoFun3(void)
{
	ServoP3_L;
}




void ServoInit(void)
{
ServoP0_OutMode;ServoP0_L;
ServoP1_OutMode;ServoP1_L;
ServoP2_OutMode;ServoP2_L;	

	
	
SoVal[0]=1500;
SoVal[1]=1500;
SoVal[2]=1500;
SoVal[3]=1500;
}


/*
20ms中断函数
*/
void BeatHandle_Servo(void)
{
	static uchar CNT=0;
	
	if(++CNT>=4)
	{
		CNT=0;
		ServoP0_H;
		ServoP1_H;
		ServoP2_H;
		ServoP3_H;
		SinglePulseStart(0,SoVal[0],ServoFun0);  //使用"delay.C"文件中的单脉冲发生器实现，不占CPU资源
		SinglePulseStart(1,SoVal[1],ServoFun1);
		SinglePulseStart(2,SoVal[2],ServoFun2);
		SinglePulseStart(3,SoVal[3],ServoFun3);
	}
}



/************************************
设置舵机脉冲
Num：舵机号，这里取值10~3
PWM：舵机信号高电平时间，一般舵机取值500~2500 us
*************************************/
void SSenvo(uchar Num,usint PWM)
{
	SoVal[Num]=PWM;
}




