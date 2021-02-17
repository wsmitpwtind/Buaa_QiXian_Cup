#include	"MotorDrive.h"
#include 	"DataProces.h"
#include	"PID.h"
#include	<stdio.h>

#define  MotorRedRatio			30  //电机减速比
#define  MotorWheelPerimeter	201 //轮子周长，单位mm
#define  MotMagNumber  		    11  //霍尔极对数（转动一圈输出的脉冲数）


void MotorDriveInit(void)
{
MotAB_Sleep_OutMode;
MotAB_Ds;
MotCD_Sleep_OutMode;
MotCD_Ds;

/*A,B两个马达*/
PWM_Init(2,1);
PWM_Init(2,2);
PWM_Init(2,3);
PWM_Init(2,4);

TIM2_PSC=1; //f=7272/M/PSC+1 =36Mhz
TIM2_ARR=2000;//PWM最大值2000 ，频率=36Mhz/2000
TIM2_CCR1=0;
TIM2_CCR2=0;
TIM2_CCR3=0;
TIM2_CCR4=0;


/*C,D两个马达*/
PWM_Init(4,1);
PWM_Init(4,2);
PWM_Init(4,3);
PWM_Init(4,4);

TIM4_PSC=1; //f=7272/M/PSC+1 =36Mhz
TIM4_ARR=2000;//PWM最大值2000 ，频率=36Mhz/2000
TIM4_CCR1=0;
TIM4_CCR2=0;
TIM4_CCR3=0;
TIM4_CCR4=0;

EncoderInit(); //编码器初始化
MotAB_En;
MotCD_En;
}





/*
控制电机方向和PWM值，取值-1000~1000；
*/
void MotA(int val)
{
	printf("|-----------MotA-----------|\r\n");
	if(val>20)
	{
		TIM2_CCR1=val+1000;	TIM2_CCR2=0;
		
	}
	else if(val<-20)
	{
		val=-val;
		TIM2_CCR1=0;		TIM2_CCR2=val+1000;  //50%的启动值
	}
	else
	{
		TIM2_CCR1=0;		TIM2_CCR2=0;
	}
}

void MotB(int val)
{
	if(val>20)
	{
		TIM2_CCR3=0;		TIM2_CCR4=val+1000;  //50%的启动值
	}
	else if(val<-20)
	{
		val=-val;
		TIM2_CCR3=val+1000;	TIM2_CCR4=0;
	}
	else
	{
		TIM2_CCR3=0;		TIM2_CCR4=0;
	}
}


void MotC(int val)
{
	if(val>20)
	{
		TIM4_CCR1=0;		TIM4_CCR2=val+1000;  //50%的启动值
	}
	else if(val<-20)
	{
		val=-val;
		TIM4_CCR1=val+1000;	TIM4_CCR2=0;
	}
	else
	{
		TIM4_CCR1=0;		TIM4_CCR2=0;
	}
}

void MotD(int val)
{
	if(val>20)
	{
		TIM4_CCR3=val+1000;	TIM4_CCR4=0;
	}
	else if(val<-20)
	{
		val=-val;
		TIM4_CCR3=0;		TIM4_CCR4=val+1000;  //50%的启动值
		
		
	}
	else
	{
		TIM4_CCR3=0;		TIM4_CCR4=0;
	}
}





/***********************************************************

编码器相关

***********************************************************/
#define HA_IRQBit 3
#define HB_IRQBit 4
#define HC_IRQBit 5
#define HD_IRQBit 11


void EncoderInit(void)
{
	HallA_InMode;
	HallB_InMode;
	HallC_InMode;
	HallD_InMode;
	
	Nvic_Init(2,1,0,EXTI3_IRQChannel);
	EXTI_Config(PortB,HA_IRQBit,1);
	EXTI_Enable(HA_IRQBit,1);
	EXTI_MarkCLR(HA_IRQBit);
	
	Nvic_Init(2,1,1,EXTI4_IRQChannel);
	EXTI_Config(PortB,HB_IRQBit,1);
	EXTI_Enable(HB_IRQBit,1);
	EXTI_MarkCLR(HB_IRQBit);

	Nvic_Init(2,1,2,EXTI9_5_IRQChannel);
	EXTI_Config(PortB,HC_IRQBit,1);
	EXTI_Enable(HC_IRQBit,1);
	EXTI_MarkCLR(HC_IRQBit);
	
	Nvic_Init(2,1,3,EXTI15_10_IRQChannel);
	EXTI_Config(PortA,HD_IRQBit,1);
	EXTI_Enable(HD_IRQBit,1);	
	EXTI_MarkCLR(HD_IRQBit);
}





ulong Odometer[4]={0,0,0,0};		//里程值（编码器累加）
ulong Odometer_Keep[4]={0,0,0,0};		//里程值（编码器累加）

long HallSpeed[4][5]={{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}}; //霍尔转速
long MotSpeed[4]={0,0,0,0};

int HallDir[4]={0,0,0,0};

void EncoderA_IRQ(void)
{
			printf("|-----------EncoderA_IRQ-----------|\r\n");
	static uchar trun=0;
	long i=0;
	
	EXTI_MarkCLR(HA_IRQBit);
	Odometer[0]++; //里程
	
	trun=!trun;
	if(trun)
	{TimerStart(0);i=TimerStop(1);}
	else
	{TimerStart(1);i=TimerStop(0);}

	FIFO_long(i,&HallSpeed[0][0],5);
	HallDir[0]=(HallA_InD)?(1):(0); //方向保存
	

}


void EncoderB_IRQ(void)
{
	static uchar trun=0;
	long i=0;
	
	EXTI_MarkCLR(HB_IRQBit);
	Odometer[1]++; //里程
	
	trun=!trun;
	if(trun)
	{TimerStart(2);i=TimerStop(3);}
	else
	{TimerStart(3);i=TimerStop(2);}

	FIFO_long(i,&HallSpeed[1][0],5);
	HallDir[1]=(HallB_InD)?(1):(0); //方向保存
}

void EncoderC_IRQ(void)
{
	static uchar trun=0;
	long i=0;
	
	EXTI_MarkCLR(HC_IRQBit);
	Odometer[2]++; //里程
	
	trun=!trun;
	if(trun)
	{TimerStart(4);i=TimerStop(5);}
	else
	{TimerStart(5);i=TimerStop(4);}

	FIFO_long(i,&HallSpeed[2][0],5);
	HallDir[2]=(HallC_InD)?(1):(0); //方向保存
}

void EncoderD_IRQ(void)
{
	static uchar trun=0;
	long i=0;
	
	EXTI_MarkCLR(HD_IRQBit);
	Odometer[3]++; //里程
	
	trun=!trun;
	if(trun)
	{TimerStart(6);i=TimerStop(7);}
	else
	{TimerStart(7);i=TimerStop(6);}

	FIFO_long(i,&HallSpeed[3][0],5);
	HallDir[3]=(HallD_InD)?(1):(0); //方向保存
}


/*
25m一次，用于电机测速计算
*/

void BeatHandle_Encoder(void)
{
		printf("|-----------BeatHandle_Encoder-----------|\r\n");
	static uchar Cnt=0;
	long temp=0;
	uchar i=0,k=0;
	
	Cnt++;
	for(i=0;i<4;i++)
	{

		bubbling_long(&HallSpeed[i][0],5);  //冒泡
		temp=(HallSpeed[i][1]+HallSpeed[i][2]+HallSpeed[i][3])/3; 
		
		//去最大最小，求平均
		temp=temp*MotMagNumber;

		temp=10000000/temp;//10倍的
		
		if(( i==0 ) ||(i==3)) //根据实际情况调整方向
		{temp=(HallDir[i])?(temp):(-temp);}
		else
		{temp=(HallDir[i])?(-temp):(temp);}
		
		MotSpeed[i]=temp;
		
		if(Cnt>=4) // 0.1秒钟之内没转到一0.1圈认为停转
		{
			if( (Odometer[i]-Odometer_Keep[i]) <= (ulong)MotMagNumber/10)
			{
				MotSpeed[i]=0;
				for(k=0;k<5;k++) HallSpeed[i][k]=0;
				
			}
			Odometer_Keep[i]=Odometer[i];
		}
	}
	if(Cnt>=4) Cnt=0;
}



long MotorRPM_A(void)
{
	return MotSpeed[0];
}

long MotorRPM_B(void)
{
	return MotSpeed[1];
}

long MotorRPM_C(void)
{
	return MotSpeed[2];
}

long MotorRPM_D(void)
{
	return MotSpeed[3];
}



/*
获取线速度，单位mm/s
*/
int GetLinearVelocity_A(void)
{
	long i=MotorRPM_A();
	i=(i*MotorWheelPerimeter)/(MotorRedRatio*10);
	return i;
}



/*
获取线速度，单位mm/s
*/
int GetLinearVelocity_B(void)
{
	long i=MotorRPM_B();
	i=(i*MotorWheelPerimeter)/(MotorRedRatio*10);
	return i;
}

/*
获取线速度，单位mm/s
*/
int GetLinearVelocity_C(void)
{
	long i=MotorRPM_C();
	i=(i*MotorWheelPerimeter)/(MotorRedRatio*10);
	return i;
}



/*
获取线速度，单位mm/s
*/
int GetLinearVelocity_D(void)
{
	long i=MotorRPM_D();
	i=(i*MotorWheelPerimeter)/(MotorRedRatio*10);
	return i;
}






/*****************************************************************


	激光导航部分，需要PID


*****************************************************************/

//设定左右电机速度，单位：mm/s，需要PID
void set_walkingmotor_speed(int Speed1, int Speed2,int Speed3, int Speed4)
{
	
	int SPD1=0;
	int SPD2=0;
	int SPD3=0;
	int SPD4=0;
  printf("|-----------set_walkingmotor_speed-----------|\r\n");
	SPD1=(long)Speed1*MotorRedRatio*10/MotorWheelPerimeter;
	SPD2=(long)Speed2*MotorRedRatio*10/MotorWheelPerimeter;   //放大10倍
	SPD3=(long)Speed3*MotorRedRatio*10/MotorWheelPerimeter;
	SPD4=(long)Speed4*MotorRedRatio*10/MotorWheelPerimeter; 
	
	PIDSpeed_SetGoal_A(SPD1);
	PIDSpeed_SetGoal_B(SPD2);
	PIDSpeed_SetGoal_C(SPD3);
	PIDSpeed_SetGoal_D(SPD4);	
	
}


/*
需要循环调用，比如5ms调用一次，根据轮子尺寸算里程
*/
long MileageA_mm=0;
long MileageB_mm=0;
long MileageC_mm=0;
long MileageD_mm=0;

void GetMileage(void)
{
MileageA_mm=( Odometer[0]*MotorWheelPerimeter)/(MotorRedRatio*MotMagNumber);
  printf("A\r\n");
MileageB_mm=( Odometer[1]*MotorWheelPerimeter)/(MotorRedRatio*MotMagNumber); 
	printf("B\r\n");
MileageC_mm=( Odometer[2]*MotorWheelPerimeter)/(MotorRedRatio*MotMagNumber);
	printf("C\r\n");
MileageD_mm=( Odometer[3]*MotorWheelPerimeter)/(MotorRedRatio*MotMagNumber);
	printf("D\r\n");
printf("|-----------GetMileage-----------|\r\n");
printf("||||||||||||||||||||||||||||||||||\r\n");

}



long walkingmotor_cumulate_A_mm(void)
{
return MileageA_mm;

}

long walkingmotor_cumulate_B_mm(void)
{

return MileageB_mm;

}
	
long walkingmotor_cumulate_C_mm(void)
{
return MileageC_mm;

}

long walkingmotor_cumulate_D_mm(void)
{

return MileageD_mm;

}


	
	

	

