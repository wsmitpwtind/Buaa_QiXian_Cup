#include "AT_Runing.h"
#include <string.h>
#include "LXU_AtRISC.h"
#include "AT_Driver.h"
#include "stm32f10x_Reg.h"
#include "stm32f10x_type.h"
#include "stm32f10x_Init.h"
#include "MotorDrive.h"
#include "Servo.H"
#include	<stdio.h>

#define _AT_TimeOutSET 20  //5ms计数20次为溢出

static uchar AT_Order_TimeOutEN=0;
static usint AT_TimeOutCNT=0;
static uchar AT_Order_Finish=0;
//static uchar AT_TimOutMark=0; 		//AT指令(也包括数据)接收超时标志位


void AT_TimeOut_Handler(void) ;

void AT_TimeOut_Enable(uchar En)
{
	if(En)
	{
	AT_Order_TimeOutEN=1;
	AT_TimeOutCNT=0;
	}
	else
	{
	AT_Order_TimeOutEN=0;
	}
}


void AT_TimeOut_Handler(void)   
{

	
if(AT_Order_TimeOutEN)
{
	if(AT_TimeOutCNT++>_AT_TimeOutSET)  //溢出
	{
		AT_TimeOutCNT=0;	
		AT_Order_TimeOutEN=0;

//		AT_TimOutMark=1;
		AT_Flow_BUFF_Clear();
		Data_Flow_BUFF_Clear();
	}
}


}



void ATCommInit(void)
{
	
UART1_Clear_RL_Mark();
USART_Init(1,115200);	//串口2初始化
USART1_CR1|=(1<<5);		//使能USART接收中断
Nvic_Init(2,1,1,USART1_IRQChannel); 
UART1_Clear_RL_Mark();

}



/*************串口1接收中断********/
void ATUART_RQHandler(uchar UD)
{
	printf("|-----------ATUART_RQHandler-----------|\r\n");
	uchar re=0;
	re=AT_Flow_ReceByte(UD);
	if( re==0x41)         //已经接收到正确的头,开始计数，整个指令时间不得大于溢出时间
	{
		AT_TimeOut_Enable(1);
	}
	else if(re==0)
	{
		AT_Order_Finish=1;
		AT_TimeOut_Enable(0);	//正确收到一条指令，关计数
	}
	else if(re==0x21) 			//正在接收大数据流中，两两字节的间隔时间不得大于溢出时间
	{
		AT_TimeOut_Enable(1);	
	}
	else if(re==0x2f)			//大数据流接收完成
	{
//		AT_Datas_Finish=1;
		AT_TimeOut_Enable(0);
	}
}

//----对应的Case值，顺序可以打乱！！-----


#define _onlyAT 0x80

typedef struct 
{
const uchar ParaNum;  //参数的总数量，包括字符串，不包括特别的查询参数
const uchar ParaType; //参数为数据/字符串标记，BIT=1.为字符串。
const uchar EnumNum;  //输出对应的枚举号
const char  *str;

}_Type_NamGroup;

	

enum
{
_BKDat=0,
_DevID,
_VER,


_Mot,	
_Servo,
_PID,
	
_LenByte,
};



//------顺序可以打乱----
_Type_NamGroup NamGroupConst[_LenByte+1]=
{

{0,0x00,_BKDat,"BKDat"},
{0,0x00,_DevID,"DevID"},
{0,0x00,_VER,"VER"},


{4,0x00,_Mot,"Mot"},
{2,0x00,_Servo,"Servo"},
{3,0x00,_PID,"PID"},

};

int my_stricmp(char *inP1,const char *inP2)
{
int i,len=0;
int gap='a'-'A';

	if( strlen(inP1)!=strlen(inP2) ) return -1;
	len=strlen(inP1);

	for(i=0;i<len;i++)
	{
	if( !( ( inP1[i]==inP2[i] ) ||( inP1[i]==(inP2[i]+gap) )||( (inP1[i]+gap)==inP2[i] ) ) ) return -2;
	}

 return 0;
}



uchar AT_CompareChar(uchar Mode,uchar Plen,uchar Type,char* Cp)
{
				printf("|-----------AT_CompareChar-----------|\r\n");
uchar k;

if(strlen(Cp)==0) return _onlyAT;  //为AT指令
	
for(k=0;k<(_LenByte+1);k++)
{
	if(my_stricmp(Cp,NamGroupConst[k].str)==0)  //stricmp	不分大小写
	{
		if(Mode)  //==1,为特殊的查询模式，不需要检测参数数量，和参数类型的匹配。
		{
			return NamGroupConst[k].EnumNum;
		}
		else
		{
			if( (Plen==NamGroupConst[k].ParaNum) && (Type==NamGroupConst[k].ParaType) )
			return NamGroupConst[k].EnumNum;
			else
			return 0xff;
		}
	}
}
return 0xff;
}





void AT_Order_handler(_Type_OutForm FInfo)
{
	uchar i;
  printf("|-----------AT_Order_handler-----------|\r\n");
	i=AT_CompareChar(FInfo.Type,FInfo.PaNum+FInfo.StrNum,FInfo.DS_Flag,FInfo.NaBuf);
	switch(i)
	{
	case _onlyAT:	AT_Send_String("OK");	break;
	case _DevID:	GetDeviceID();  		break;
	case _Mot:	
			set_walkingmotor_speed(FInfo.Para[0],FInfo.Para[1],FInfo.Para[2],FInfo.Para[3]); //指定线速度mm/s 
			AT_Send_String("OK\r\n");
			break;
		
	case _Servo:	SSenvo(FInfo.Para[0],FInfo.Para[1]);AT_Send_String("OK\r\n"); 			break;
	case _PID:		Set_MotSPDPID(FInfo.Type,FInfo.Para[0],FInfo.Para[1],FInfo.Para[2]); 	break;
	
	}
}




/***************************************************



****************************************************/


_Type_OutForm _OutFormP;

void AT_CommRunning(void)
{
	uchar i=0;
	if(AT_Order_Finish)
	{
		AT_Order_Finish=0;
				printf("|-----------AT_ParamCount-----------|\r\n");
		i=AT_ParamCount(&_OutFormP);
		if(i==0)//参数解算
		{
						printf("|-----------AT_Order_handler-----------|\r\n");
			AT_Order_handler(_OutFormP);
			printf("||||||||||||||Recived|||||||||||||\r\n");
		}
		
	}
}










