#include	"AT_Driver.h"
#include	"DataProces.h"
#include	"LXU_AtRISC.h"
#include	"stm32f10x_Init.h"
#include	"EEPFlash.h"


extern _TYPE_EEPROM_PACK SYS_Cfg;


/***************************************************************
Ӧ��淶���壺
���صı�ʾ�ֽ�ֻ�У�
OK��ָ��ִ��OK��ֻ��OK������п��ܴ�����
ER�� ����Ĳ��������糬����Χ�������޲���
FA�� �ڲ��������󣬺����޲���

���صĲ��������ʽ��ÿ�������á������ŷָ�����
���ز����������ݸ��ַ����������ַ������ϡ� ���� ������ʾ
�����������Ϣ�ĸ�ʽΪ��

OK "CC3200_WiFi","00:87:36:11:7D:67","172.27.35.3","172.27.35.1"
���ز����У����õ���ֵ�жϣ�δ�ر������£�0��ʾ���١���1��ʾ���桱

�������ݵĽ�βȫ���� �س����У�0x0d,0x0a������ʾ������
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

void AT_Send_String_Plus(const char *p)  //���ϡ������ַ������
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


			AT���ú�������(1,��������)
				

********************************************************/



void GetDeviceID(void)
{

AT_Send_String("OK ");
AT_Send_String_Plus(_DEV_ID);
AT_Send_End();

}



void Set_MotSPDPID(uchar type,long P,long I,long D)
	
{

	if(type)	 //==1,ֻ��ѯ
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



