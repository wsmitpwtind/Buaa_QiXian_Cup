#include	"syn500r.h"
#include	"EEPFlash.h"
#include 	"stdio.h"

extern _TYPE_EEPROM_PACK SYS_Cfg;



typedef struct
{
usint HeMax;
usint HeMin;

usint HeDIVMax;
usint HeDIVMin;

usint DaMax;
usint DaDIVMax;
usint DaDIVMin;

uchar Step;
uchar RxBIT;
uchar RxBCnt;
uchar SynBIT;
uchar SynBITSave;

uchar SBTemp[32];
}SynInfo;

uchar ReKeep=0;
uchar ReKeepAny=0;
usint ReVal=0;

static SynInfo De10k;
static SynInfo DeAny;

static uchar ReKey[8];
static uchar KeyStudyMark=0;

void SYN500RxInit_10K(void)
{
De10k.HeMax=14000;
De10k.HeMin=10000;
De10k.HeDIVMax=70;
De10k.HeDIVMin=7;
De10k.DaMax=2000;
De10k.DaDIVMax=100;
De10k.DaDIVMin=15;

De10k.Step=0;
De10k.Step=0;
De10k.RxBIT=0;
De10k.RxBCnt=0;
De10k.SynBIT=0;
De10k.SynBITSave=0;
}


void SYN500RxInit_Any(void)
{
DeAny.HeMax=9000;
DeAny.HeMin=6000;
DeAny.HeDIVMax=32;
DeAny.HeDIVMin=12;
DeAny.DaMax=1000;
DeAny.DaDIVMax=80;
DeAny.DaDIVMin=10;

DeAny.Step=0;
DeAny.Step=0;
DeAny.RxBIT=0;
DeAny.RxBCnt=0;
DeAny.SynBIT=0;
DeAny.SynBITSave=0;
}


void SYN500RxCLR(SynInfo * SP)
{
	SP->Step=0;
	SP->RxBCnt=0;
	SP->SynBITSave=0;
}







void SYN500RInit(void)
{
uchar i;
Nvic_Init(2,1,2,EXTI1_IRQChannel);
EXTI_Config(PortC,1,3);
EXTI_Enable(1,1);
SYN500RPin_InMode;

for(i=0;i<8;i++) ReKey[i]=SYS_Cfg.ReKEYD[i];

SYN500RxInit_10K();
SYN500RxInit_Any();
}





uchar SYN500RRxData(void);
/***************************************

CE���жϺ���

***************************************/

static ulong HTime=0;
static ulong LTime=0;
static usint IRQGapTime=0;
static usint IRQGapTimeEn=0;  //�Ƿ����������

void SYN500RPin_IRQHandler(void)
{
	
	if( !(SYN500RPin_InDat) ) //�½���
	{
		TimerStart(8);
		HTime=TimerStop(9);
	}
	else
	{
		TimerStart(9);
		LTime=TimerStop(8);
		SYN500RRxData();
	}
}



//50*5ms ���޹ܽ��жϣ���Ϊ���·ſ������
void BeatHandle_SYN500RPin(void)
{
	if(IRQGapTimeEn)
	{
		if(IRQGapTime++>50) //50*5ms ���޹ܽ��жϣ���Ϊ���·ſ������
		{
			IRQGapTimeEn=0;
			ReKeep=0;
			ReKeepAny=0;
			IRQGapTime=0;
			SYN500RxCLR(&De10k);
			SYN500RxCLR(&DeAny);
		}
	}
}



static uchar StudySTA=0;
static uchar StudyCnt=0;
static uchar StudyBuf[3][8];

/*
����ң�ص�ַ���ѧϰ
*/
void ReKeyStudyEnable(void)
{
	if(KeyStudyMark==0) //==1��˵���Ѿ�����ѧϰ��
	{
		KeyStudyMark=1;
		StudyCnt=0; 
		StudySTA=0;	

	}
}


/*
ƥ��ɹ��󱣴��ַ��Ϣ��FLASH
*/
void ReKeyStudy_Save(void)
{
	if(StudySTA) //ѧϰ״̬
	{
		
		StudySTA=0;
		EFile_Inport(); //������
		
		printf("��ַƥ��ɹ�\r\n");
	}

}








/*****************************************

ͨ�ý��պ���

******************************************/
uchar SYN500Rx(SynInfo * SP)
{

//uchar i=0;
ulong TeLTime=LTime;
ulong TeHTime=HTime;
usint DIV=0;

if( (TeLTime<SP->HeMax)&&(TeLTime>SP->HeMin) )
{
	DIV=TeLTime/TeHTime;
	if((DIV>SP->HeDIVMin)&&(DIV<SP->HeDIVMax))//ͬ��֡�͸߿�ȱ���4:124
	{
		
		SP->Step=1; //�յ�ͬ��֡
		SP->RxBCnt=0;
		SP->SynBITSave=0;
		//AT_Send_Byte('A');
		return SP->Step;
	}
	else
	{
		return SP->Step;
	}
}

if(SP->Step)
{
	if((TeLTime<SP->DaMax)&&(TeHTime<SP->DaMax)) //����ֻ�ж���10K������
	{
		if(TeLTime>TeHTime)
		{
			DIV=TeLTime*10/TeHTime;
			if((DIV>SP->DaDIVMin)&&(DIV<SP->DaDIVMax)) //��׼��4:12
			{
				SP->SynBIT='0'; //���յ�һ��խ����
				//AT_Send_Byte('B');
			}
			else
			{
				
				SP->RxBCnt=0;SP->Step=0; return SP->Step;
			}
		}
		else
		{
			DIV=TeHTime*10/TeLTime;
			if((DIV>SP->DaDIVMin)&&(DIV<SP->DaDIVMax)) //��׼��12:4
			{
				SP->SynBIT='1'; //���յ�һ��������
				//AT_Send_Byte('C');
				
			}
			else
			{
				//AT_Send_Byte('D');
				SP->RxBCnt=0; SP->Step=0; return SP->Step;
			}
		}
		
		
		if(SP->RxBCnt%2)
		{
			if( (SP->SynBITSave=='0')&&((SP->SynBIT=='0')) )
			{
				SP->RxBIT='0';IRQGapTime=0;
			}
			else if( (SP->SynBITSave=='1')&&((SP->SynBIT=='1')) )
			{
				SP->RxBIT='1';IRQGapTime=0;
			}
			else if( (SP->SynBITSave=='0')&&((SP->SynBIT=='1')) )
			{
				SP->RxBIT='2';IRQGapTime=0;
			}
			else
			{
				//AT_Send_Byte('D');
				SP->RxBCnt=0;SP->Step=0; return SP->Step;
			}
			SP->SBTemp[SP->RxBCnt/2]=SP->RxBIT;
			IRQGapTime=0; //����������
		}
		else
		{
			SP->SynBITSave=SP->SynBIT;
		}
		
		if(++SP->RxBCnt>=32)
		{
			//AT_Send_Byte('E');
			SP->RxBCnt=0;SP->Step=0; return SP->Step;
		}

		if(SP->RxBCnt==24)  return 0xff; //���յ�24�ֽڵ�����
	}
	else
	{
		SP->Step=0;
	}
}

return SP->Step;
}







ulong SynData=0;

/***********************************************
�ʺ�10K���ʵĽ���

***********************************************/
uchar SYN500RRxData(void)
{
uchar i=0;


i=SYN500Rx(&De10k);

if(i==0xff)
{
	if(KeyStudyMark)  //ѧϰģʽ
	{
		for(i=0;i<8;i++) StudyBuf[StudyCnt][i]=De10k.SBTemp[i];
		if(++StudyCnt>=3) 
		{
			StudyCnt=0; 
			for(i=0;i<8;i++) 
			{
				if((StudyBuf[0][i]!=StudyBuf[1][i])||(StudyBuf[0][i]!=StudyBuf[2][i]))
				{
					break;
				}
			}
					
			if(i==8)
			{
				
				for(i=0;i<8;i++) SYS_Cfg.ReKEYD[i]=StudyBuf[0][i];
				for(i=0;i<8;i++) ReKey[i]=StudyBuf[0][i];
				KeyStudyMark=0;
				StudySTA=1;  //ѧϰ�ɹ�
			}
			else
			{
				StudySTA=0;
				//AT_Send_String("ѧϰʧ��");
			}

		}
	}
	else
	{
		for(i=0;i<8;i++)
		{
			//AT_Send_Byte(De10k.SBTemp[i]);
			//return 0;
			
			if(ReKey[i]!=De10k.SBTemp[i]) //��ַ��ƥ��
			{
				SYN500RxCLR(&De10k);
				return 0;
			}
			
		}
		SynData=(De10k.SBTemp[8]-'0');
		SynData|=(De10k.SBTemp[9]-'0')<<4;
		SynData|=(De10k.SBTemp[10]-'0')<<8;
		SynData|=(De10k.SBTemp[11]-'0')<<12;
		ReVal=SynData;
			
		IRQGapTimeEn=1; //�����������,��ⰴ���ſ�
		ReKeep=1;
		IRQGapTime=0;
		SYN500RxCLR(&De10k);
	}
}




i=SYN500Rx(&DeAny);
if(i==0xff)
{
	IRQGapTimeEn=1; //�����������,��ⰴ���ſ�
	ReKeepAny=1;
	IRQGapTime=0;
	
}
return 0;
}

//-------------------------------------------------------

//������ʵ��Ӧ��

//------------------------------------------------------

uchar RemKey_Change(usint key)
{
	switch(key)
	{
	case  0x0110: return S_Key0;
	case  0x1110: return S_Key1;
	case  0x0011: return S_Key2;
	case  0x1001: return S_Key3;
	case  0x1100: return S_Key4;
	case  0x0111: return S_Key5;
	case  0x1011: return S_Key6;
	case  0x1111: return S_Key7;
	case  0x0001: return S_Key8;
	case  0x1010: return S_Key9;
	case  0x0000: return S_Key10;
	case  0x0002: return S_Key11;
	case  0x0020: return S_Key12;
	case  0x1000: return S_KeyUp;
	case  0x0100: return S_KeyDown;
	case  0x0010: return S_KeyL;
	case  0x0101: return S_KeyR;
	case  0x1101: return S_KeyOk;
	default: return S_KeyErr;
	}
}




//����Ϊ1��������
uchar GetSyn_SAT(void)
{
	return ReKeep; 
}

uchar GetSyn_Key(void)
{
	return RemKey_Change(ReVal); 
}















