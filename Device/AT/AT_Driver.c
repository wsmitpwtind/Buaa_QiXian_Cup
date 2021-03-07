#include	"AT_Driver.h"
#include	"DataProces.h"
#include	"LXU_AtRISC.h"
#include	"stm32f10x_Init.h"
#include	"EEPFlash.h"


extern _TYPE_EEPROM_PACK SYS_Cfg;


/***************************************************************
应答规范定义：
返回的标示字节只有：
OK：指令执行OK，只有OK后面才有可能带参数
ER： 输入的参数错误，如超出范围，后续无参数
FA： 内部函数错误，后续无参数

返回的参数输出格式，每个参数用‘，’号分隔开。
返回参数包含数据跟字符串，其中字符串加上‘ “” ’来标示
如输出连接信息的格式为：

OK "CC3200_WiFi","00:87:36:11:7D:67","172.27.35.3","172.27.35.1"
返回参数中，常用到真值判断，未特别声明下，0表示“假”，1表示“真”

返回数据的结尾全部用 回车换行（0x0d,0x0a）来表示结束符
****************************************************************/

void AT_Send_Byte(uchar id)
{
	USART1_TxDat(id);
}

void AT_Send_String_Len(uchar *p,usint len)
{
	usint cnt=0;
	while(len--)
	{
		AT_Send_Byte(p[cnt++]);
	}
}

void AT_Send_String(const char *p)
{
	uchar i=0;
	for(i=0;i<200;i++)
	{
		if( (*p)== '\0') return;
		AT_Send_Byte(*p);
		p++;
	}
}

void AT_Send_String_Plus(const char *p)  //加上“”的字符串输出
{
AT_Send_Byte('"');
AT_Send_String(p);
AT_Send_Byte('"');
}


void AT_Send_End(void)
{
AT_Send_Byte(0x0d);
AT_Send_Byte(0x0a);
}

void AT_Send_ER(void)
{

AT_Send_String("ER");
AT_Send_Byte(0x0d);
AT_Send_Byte(0x0a);

}

void AT_Send_FA(void)
{

AT_Send_String("FA");
AT_Send_Byte(0x0d);
AT_Send_Byte(0x0a);

}



void AT_Send_ToAscii_BCD(long iD)
{
uchar inB[10];
uchar outB[10];
uchar i;
	
	if(iD<0)
	{
	AT_Send_String("-");
	iD=-iD;
	}
	HEXtoBCD(iD,inB);
	i=lcd_hide0(inB,0,10,outB);
	AT_Send_String_Len(outB,i);
}


void AT_Send_ToAscii_HEX(uchar hex)
{
uchar i=0;

i=hex>>4;

if( i<10)
AT_Send_Byte(i+'0');
else
AT_Send_Byte((i-10)+'A');

i=hex&0x0f;
if( i<10)
AT_Send_Byte(i+'0');
else
AT_Send_Byte((i-10)+'A');
}




/*******************************************************


			AT设置函数集合(1,杂项设置)
				

********************************************************/



void GetDeviceID(void)
{

AT_Send_String("OK ");
AT_Send_String_Plus(_DEV_ID);
AT_Send_End();

}



void Set_MotSPDPID(uchar type,long P,long I,long D)
	
{

	if(type)	 //==1,只查询
	{
		AT_Send_String("OK ");
		AT_Send_ToAscii_BCD(SYS_Cfg.P);
		AT_Send_String(",");
		AT_Send_ToAscii_BCD(SYS_Cfg.I);
		AT_Send_String(",");
		AT_Send_ToAscii_BCD(SYS_Cfg.D);
		AT_Send_End();
	}
	else
	{
		SYS_Cfg.P=P;
		SYS_Cfg.I=I;
		SYS_Cfg.D=D;
		if(EFile_Inport()<0)  { AT_Send_FA(); return; }
		
		AT_Send_String("OK ");
		AT_Send_ToAscii_BCD(SYS_Cfg.P);
		AT_Send_String(",");
		AT_Send_ToAscii_BCD(SYS_Cfg.I);
		AT_Send_String(",");
		AT_Send_ToAscii_BCD(SYS_Cfg.D);
		AT_Send_End();
		
		//PIDParSPDUpdate();
	}
}



