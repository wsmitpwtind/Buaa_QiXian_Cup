#include	"PID.h"
#include	"MotorDrive.h"
#include	"EEPFlash.h"
#include	<stdio.h>

extern _TYPE_EEPROM_PACK SYS_Cfg;



struct PID 
{
	int  P;         //  比例常数 Proportional Const 
	int  I;         //  积分常数 Integral Const 
	int  D;         //  微分常数 Derivative Const 
	
	int  SetGoal;   //  设定目标 Desired Value 
	int  LastError;  //  Error[-1] 
	int  PrevError;  //  Error[-2] 
	int  SumError;	
	
	int  Enable;
};


struct PID PIDParSPD_A;
struct PID PIDParSPD_B;
struct PID PIDParSPD_C;
struct PID PIDParSPD_D;

/*
PID参数更新，从SYS_Cfg更新最新数据到PIDParSPD或PIDParCT中
*/
void PIDParSPDUpdate(void)
{
//-----------------------------------
			printf("|-----------PIDParSPDUpdate-----------|\r\n");
PIDParSPD_A.P=SYS_Cfg.P;
PIDParSPD_A.I=SYS_Cfg.I;
PIDParSPD_A.D=SYS_Cfg.D;

PIDParSPD_B.P=SYS_Cfg.P;
PIDParSPD_B.I=SYS_Cfg.I;
PIDParSPD_B.D=SYS_Cfg.D;

PIDParSPD_C.P=SYS_Cfg.P;
PIDParSPD_C.I=SYS_Cfg.I;
PIDParSPD_C.D=SYS_Cfg.D;

PIDParSPD_D.P=SYS_Cfg.P;
PIDParSPD_D.I=SYS_Cfg.I;
PIDParSPD_D.D=SYS_Cfg.D;
}





//PID初始化部分
void PID_Init(void)
{
				printf("|-----------PID_Init-----------|\r\n");
PIDParSPDUpdate();

PIDParSPD_A.Enable=0;
PIDParSPD_A.SetGoal=0;
PIDParSPD_A.LastError=0;
PIDParSPD_A.PrevError=0;
PIDParSPD_A.SumError=0;

PIDParSPD_B.Enable=0;
PIDParSPD_B.SetGoal=0;
PIDParSPD_B.LastError=0;
PIDParSPD_B.PrevError=0;
PIDParSPD_B.SumError=0;
	
PIDParSPD_C.Enable=0;
PIDParSPD_C.SetGoal=0;
PIDParSPD_C.LastError=0;
PIDParSPD_C.PrevError=0;
PIDParSPD_C.SumError=0;

PIDParSPD_D.Enable=0;
PIDParSPD_D.SetGoal=0;
PIDParSPD_D.LastError=0;
PIDParSPD_D.PrevError=0;
PIDParSPD_D.SumError=0;

PIDSpeed_SetEnable_A(1);
PIDSpeed_SetEnable_B(1);
PIDSpeed_SetEnable_C(1);
PIDSpeed_SetEnable_D(1);
}



/**********************************************
PID计算部分

NextPoint:当前输入的值
PID：		  PID结构体
**********************************************/ 
int PIDCalc(int NextPoint,struct PID *P) 
{
	printf("|-----------PIDCalc-----------|\r\n");
	int dError=0,Error=0; 
	long tempP=0;
	long tempI=0;
	long tempD=0;
	
    Error = P->SetGoal- NextPoint;		//偏差 
	tempP=(long)P->P*Error;				//偏差*P

    P->SumError += Error;				//积分 
	tempI=(long)P->I*P->SumError;		//积分*I
	
	//这里三个P I D 值都放大了100倍，方便整数化PID值
	if(tempI>100000)   //超出范围，不再积分
	{
		P->SumError = 100000/P->I;	
	}
	if(tempI<-100000)
	{
		P->SumError = -100000/P->I;
	}
    dError = P->LastError - P->PrevError;	// 微分 
	tempD=(long)P->D*dError;				//微分*D
	
    P->PrevError = P->LastError; 
    P->LastError = Error; 
	
	tempP=(tempP+tempI+tempD)/100;
	if(tempP>1000) tempP=1000;
	if(tempP<-1000) tempP=-1000;
	
    return (int)(tempP);
}









long PulseSpeed_A=0;
long PulseSpeed_B=0;
long PulseSpeed_C=0;
long PulseSpeed_D=0;
long GetPulseSpeed_A(void)
{
		printf("|-----------GetPulseSpeed_A-----------|\r\n");
	return PulseSpeed_A;
}

long GetPulseSpeed_B(void)
{
	return PulseSpeed_B;
}
long GetPulseSpeed_C(void)
{
	return PulseSpeed_C;
}
long GetPulseSpeed_D(void)
{
	return PulseSpeed_D;
}


/*
速度环，在5ms中断中执行
*/
void PIDSPD_SinglePulse(void)   
{
	int temp=0;
		printf("|-----------PIDSPD_SinglePulse-----------|\r\n");
	
	
	 BeatHandle_Encoder();	//用于计算转速
	//------------------------------------------------------
	if(PIDParSPD_A.Enable)
	{
		PulseSpeed_A=MotorRPM_A();
		if(PIDParSPD_A.SetGoal==0)
		{
			MotA(0);
		}
		else
		{
			temp=PIDCalc(PulseSpeed_A,&PIDParSPD_A); //PID运算
			MotA(temp);			
		}
	}
	//------------------------------------------------------
	if(PIDParSPD_B.Enable)
	{
		PulseSpeed_B=MotorRPM_B();
		if(PIDParSPD_B.SetGoal==0)
		{
			MotB(0);
		}
		else
		{
			temp=PIDCalc(PulseSpeed_B,&PIDParSPD_B); //PID运算
			MotB(temp);			
		}
	}
	//------------------------------------------------------
	if(PIDParSPD_C.Enable)
	{
		PulseSpeed_C=MotorRPM_C();
		if(PIDParSPD_C.SetGoal==0)
		{
			MotC(0);
		}
		else
		{
			temp=PIDCalc(PulseSpeed_C,&PIDParSPD_C); //PID运算
			MotC(temp);			
		}
	}
		//------------------------------------------------------
	if(PIDParSPD_D.Enable)
	{
		PulseSpeed_D=MotorRPM_D();
		if(PIDParSPD_D.SetGoal==0)
		{
			MotD(0);
		}
		else
		{
			temp=PIDCalc(PulseSpeed_D,&PIDParSPD_D); //PID运算
			MotD(temp);			
		}
	}
	
	
}

/*******************************************
PID目标值设定,PID速度调节
En:=1开启PID配置
*******************************************/
void PIDSpeed_SetGoal_A(int goal)
{
	  printf("|-----------PIDSpeed_SetGoal_A-----------|\r\n");
	PIDParSPD_A.SetGoal=goal;
	if(goal==0)
	{
		PIDParSPD_A.LastError=0;
		PIDParSPD_A.PrevError=0;
		PIDParSPD_A.SumError=0;
	}
}
void PIDSpeed_SetEnable_A(uchar En)
{
			printf("|-----------PIDSpeed_SetEnable_A-----------|\r\n");
	PIDParSPD_A.SetGoal=0;
	PIDParSPD_A.LastError=0;
	PIDParSPD_A.PrevError=0;
	PIDParSPD_A.SumError=0;
	PIDParSPD_A.Enable=En;
}



void PIDSpeed_SetGoal_B(int goal)
{
	PIDParSPD_B.SetGoal=goal;
	if(goal==0)
	{
		PIDParSPD_B.LastError=0;	
		PIDParSPD_B.PrevError=0;	
		PIDParSPD_B.SumError=0;		
	}
}
void PIDSpeed_SetEnable_B(uchar En)
{
	PIDParSPD_B.SetGoal=0;
	PIDParSPD_B.LastError=0;
	PIDParSPD_B.PrevError=0;
	PIDParSPD_B.SumError=0;
	PIDParSPD_B.Enable=En;
}



void PIDSpeed_SetGoal_C(int goal)
{
	PIDParSPD_C.SetGoal=goal;
	if(goal==0)
	{
		PIDParSPD_C.LastError=0;
		PIDParSPD_C.PrevError=0;
		PIDParSPD_C.SumError=0;
	}
}

void PIDSpeed_SetEnable_C(uchar En)
{
	PIDParSPD_C.SetGoal=0;
	PIDParSPD_C.LastError=0;
	PIDParSPD_C.PrevError=0;
	PIDParSPD_C.SumError=0;
	PIDParSPD_C.Enable=En;
}

void PIDSpeed_SetGoal_D(int goal)
{
	PIDParSPD_D.SetGoal=goal;
	if(goal==0)
	{
		PIDParSPD_D.LastError=0;
		PIDParSPD_D.PrevError=0;
		PIDParSPD_D.SumError=0;
	}
}
void PIDSpeed_SetEnable_D(uchar En)
{
	PIDParSPD_D.SetGoal=0;
	PIDParSPD_D.LastError=0;
	PIDParSPD_D.PrevError=0;
	PIDParSPD_D.SumError=0;
	PIDParSPD_D.Enable=En;
}

