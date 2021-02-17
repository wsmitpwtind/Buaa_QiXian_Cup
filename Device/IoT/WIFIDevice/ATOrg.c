#include	"stm32f10x_Reg.h"
#include	"stm32f10x_Init.h"
#include	"WIFIDevice.h"
#include	"delay.h" 
#include	"ATOrg.h" 
#include 	<string.h>
#include	<stdio.h>
#include	"OLED_I2C.h"

#define _JudStrlen	50
#define _JudSavelen	50

#define WIFIRxMaxNum 1024


uchar JudNewsLenBuf[4]={0};  //��Ϣ���4���ֽڵĳ�
usint JudRxHBCnt=0;

void AutoParCLR(void)
{
	JudRxHBCnt=0;
}

/*
==0�����ڽ�������
>0:  �Ѿ��յ����ݰ��ˣ�����ֵ��Ϊ����
<0�� ����
*/
int AutoRx(uchar iD,uchar *RxHB)
{
static ulong JudNewsLength=0;
static uchar RDSta=0;
int i=0;

	if(JudRxHBCnt==0)  //���ͷ
	{
		if((iD&0x0f)==0)
		{
			RxHB[JudRxHBCnt++]=iD;
			RDSta=0; //׼����ʼ���ճ����ֽ�����
		}
		else 
		{
			AutoParCLR();
			return -1; //��ͷ
		}
	}
	else
	{
		if(RDSta<4) //�ճ����ֽ�(1~4�ֽڲ���)
		{
			RxHB[JudRxHBCnt++]=iD;
			JudNewsLenBuf[RDSta]=iD&0x7f; //ȥ��BIT7
			if(iD&0x80) //������Ҫ����
			{
				RDSta++;
				if(RDSta==4)
				{
					AutoParCLR();
					return -2; //���ȴ���
				}
			}
			else
			{
				JudNewsLength=0;
				switch(RDSta)
				{
					case 3: JudNewsLength+=(ulong)JudNewsLenBuf[3]*2097152; 
					case 2: JudNewsLength+=(ulong)JudNewsLenBuf[2]*16384;
					case 1: JudNewsLength+=(ulong)JudNewsLenBuf[1]*128;
					case 0: JudNewsLength+=(ulong)JudNewsLenBuf[0];
				}
				RDSta=5; //��������Ϣ�����ֽ���
				
				if(JudNewsLength>(ulong)WIFIRxMaxNum) //�����
				{
					AutoParCLR();
					return -3;	
				}
				
				if(JudNewsLength==0) //����ĳ���Ϊ0
				{
					i=JudRxHBCnt;
					AutoParCLR();
					return i;
				}
			}
		}
		else
		{
			//�ٽ���JudNewsLength���ȵ����ݼ���
			if(JudNewsLength--)
			{
				RxHB[JudRxHBCnt++]=iD;
				if(JudNewsLength==0)
				{
					i=JudRxHBCnt;
					AutoParCLR();
					return i;
				}
			}
		}
	}
	return 0;
}





/****************************************************************************

����ATָ���ִ��Ĵ���

****************************************************************************/
char JudStr[_JudStrlen]={0};	//����Ŀ���ִ�
char JudSop[_JudStrlen]={0};	//����������ִ�
char JudSave[_JudSavelen]={0};	//��⵽Ŀ���ִ��󣬱���������ֽ���


//�������ִ����գ��жϵĴ������
uchar JudRxStrStep=0;
usint JudgeCnt=0;
usint JudgeCntSave=0;
usint JudgeLenS=0;	 //��ʼ�ִ��ĳ���
usint JudgeLenP=0; 	 //�����ִ��ĳ���
uchar JudgeSTA=0;	 //1:��⵽ƥ����ַ�����Ϊ1
					 //2:��⵽ƥ����ַ������ұ��浽��ָ���ֽ�����Ϊ1
					 
uchar JudgeGoonEn=0; //=1�����ǰ����ִ�����Ҫ������������ִ����������м���ִ�
uchar TRAutoModeMark=0;  //�Ѿ�����͸��ģʽ�ı�־λ


/* 
P:��������Ҫ�����ַ���
STP:��⵽�ַ�����STP�ִ�֮������ݽ�������
	 ==NULL��ʱ��������
*/
void JudgeString(const char * P,const char * STP)
{
	JudgeLenS=strlen(P);
	if(JudgeLenS>_JudStrlen)
	{
		JudgeLenS=_JudStrlen-1;
	}
	memcpy(JudStr,P,JudgeLenS);
	if(STP==NULL)
	{
		JudgeGoonEn=0;
		JudgeLenP=0;
	}
	else
	{
		JudgeGoonEn=1; //�������������ִ�
		JudgeLenP=strlen(STP);
		strcpy(JudSop,STP);	
		
	}
	JudgeCnt=0;
	JudgeSTA=0;
	JudRxStrStep=1; //��������ַ���0
}


/*
���ж��������м�⣬�Ƿ���ƥ����ִ�
����0�� �ҵ�ƥ����ִ���
*/
uchar JudgeState(usint *Plen)
{
	if(Plen!=NULL) *Plen=JudgeCntSave;
	return (JudgeSTA);
}


//״̬��0
void JudgeStaCLR(void)
{
	JudgeSTA=0;
	JudgeLenS=0;
	JudgeLenP=0;
	JudgeCnt=0;
	
	JudRxStrStep=0;
}



/****************************************************************************

�ж�ʵ��

****************************************************************************/
void ATDataStream(uchar iD)
{

if(TRAutoModeMark) //������͸��ģʽ������������
{
	
	// MQTT_RxIRQ(iD);
	//�����ǲ��ԣ��յ�������ͨ�����ڴ�ӡ����
	USART1_TxDat(iD);
}
else
{
		if(JudRxStrStep==1) //����ַ���ƥ��Ľ׶�
		{
			if(iD==JudStr[JudgeCnt])
			{
				JudgeCnt++;
				if(JudgeCnt>=JudgeLenS)
				{
					JudgeSTA=1;
					JudgeCnt=0;
					if(JudgeGoonEn) 
					{
					JudRxStrStep=2; 
					JudgeCntSave=0;
					}
					else 
					JudRxStrStep=0;
				}
			}
			else
			{
				JudgeCnt=0;
			}
		}
		else if(JudRxStrStep==2) //����������ݵĽ׶�
		{
			if(iD==JudSop[JudgeCnt])
			{
				JudgeCnt++;
				if(JudgeCnt>=JudgeLenP)
				{
					JudgeSTA=2;
					JudgeCnt=0;
					JudRxStrStep=0;
				}
			}
			else
			{
				JudgeCnt=0;
				
				JudSave[JudgeCntSave++]=iD;
				if(JudgeCntSave>=_JudStrlen)  //��������ݹ���
				{
					JudgeSTA=0;
					JudRxStrStep=0;		
				}
			}
		}
}


}



/********************************************************************

					ATָ��ʵ��

********************************************************************/


/*
ģ�鸴λ��ȴ�"ready"�ִ�
*/
int WIFIDevReady(void)
{
	//if(WIFIDRelayGET()) return 0; //ok
	
	WIFIDevRST();

	JudgeString("ready",NULL);
	LoopTime(3000);
	while(LoopSTA())
	{
		if(JudgeState(NULL))
		{
			JudgeStaCLR();
			return 0;
		}
	}
	return -1;
}



//����ATָ��
int ATorg_OK(void)
{
	if(TRAutoModeMark) return -1; //�Ѿ�����͸��
	WSString("AT\r\n");
	JudgeString("OK",NULL);
	LoopTime(1000);
	while(LoopSTA())
	{
		if(JudgeState(NULL))
		{
			JudgeStaCLR();
			return 0;
		}
	}
	return -2;
}


//����STAģʽ
int ATorg_STAMode(void)
{
	if(TRAutoModeMark) return -1; //�Ѿ�����͸��
	WSString("AT+CWMODE=1\r\n");
	JudgeString("OK",NULL);
	LoopTime(2000);
	while(LoopSTA())
	{
		if(JudgeState(NULL))
		{
			JudgeStaCLR();
			return 0;
		}
	}
	return -2;
}

//����APģʽ
int ATorg_APMode(void)
{
	if(TRAutoModeMark) return -1; //�Ѿ�����͸��
	WSString("AT+CWMODE=2\r\n");
	JudgeString("OK",NULL);
	LoopTime(2000);
	while(LoopSTA())
	{
		if(JudgeState(NULL))
		{
			JudgeStaCLR();
			return 0;
		}
	}
	return -2;
}

//����smartconfig���ӣ�ESPTOUCH+΢��ͬʱ
//���Զ���ΪSTAģʽ
int ATorg_SmartCfgStart(void)
{
ulong Time=0;
ulong Cnt=1;

	if(TRAutoModeMark) return -1; //�Ѿ�����͸��
	if(ATorg_STAMode()<0)
	return -2;
	
	delay(200);
	
	WSString("AT+CWSTARTSMART=3\r\n");
	JudgeString("OK",NULL);
	LoopTime(2000);
	while(LoopSTA())
	{
		if(JudgeState(NULL))
		{
			JudgeStaCLR();
			JudgeString("connected wifi",NULL);
			LoopTime(60000);  //60�볬ʱ
			while(LoopSTA())
			{
				if(JudgeState(NULL))
				{
					JudgeStaCLR();
					return 0;
				}
				
				/*������ʾ���ɲ�Ҫ*/
				if(SysRunTime()>Time+1000)
				{
					Time=SysRunTime();
					ShowData(Cnt++,1,58,4);
					printf(".");
				}
				//--------------------------------
				
			}
			return -3;
		}
	}
	return -4;
}


int ATorg_SmartCfgStop(void)
{
	if(TRAutoModeMark) return -1; //�Ѿ�����͸��
	WSString("AT+CWSTOPSMART\r\n");
	JudgeString("OK",NULL);
	LoopTime(2000);
	while(LoopSTA())
	{
		if(JudgeState(NULL))
		{
			JudgeStaCLR();
			return 0;
		}
	}
	return -2;
}




void ShowSmartConfig(void)
{
int i=0;
uchar k=0;

/*
ShowChinese("<������������>",8,0);
ShowChinese("����΢�Ż�ר��APP����",0,2);
ShowChinese("�ȴ�����...",0,6);
*/
	
ShowString("<EspTouch>",1,24,1);
	
printf("WIFI ������������\r\n");
printf("����ר��APP����\r\n");
printf("�ȴ�����...\r\n");
	


i=ATorg_SmartCfgStart();
delay(100);
ATorg_SmartCfgStop();

for(k=0;k<3;k++)
{
	ShowClear();
	delay(300);
	if(i<0)
	ShowString("Fail",1,0,0);
	else
	ShowString("Done",1,0,0);
	delay(500);
}


if(i<0) printf("����ʧ��\r\n"); else printf("�����ɹ�\r\n");

printf("�˳���������\r\n");

ShowString("Exit",1,0,0);
delay(3000);
ShowClear();
}




//0://�Ѿ�����͸��,�鲻��
//1; �Ѿ���ȡIP��������
//2; TCP��UDP������
//3; TCP��UDP�Ͽ����� 
//4; δ���ӵ� WiFi

int ATorg_ConnStatus(void)
{
	if(TRAutoModeMark) return 0; //�Ѿ�����͸��
	WSString("AT+CIPSTATUS\r\n");
	JudgeString("STATUS:","\r\n");
	/*
	��ָ��أ�
	�������᷵�����ӵ�Ŀ��IP,�˿ڵ���Ϣ
	*/
	LoopTime(1000);
	while(LoopSTA())
	{
			if(JudgeState(NULL)==2)
			{
				JudgeStaCLR();
				if(JudSave[0]=='2') return 1; //�Ѿ���ȡIP��������
				if(JudSave[0]=='3') return 2; //TCP��UDP������
				if(JudSave[0]=='4') return 3; //TCP��UDP�Ͽ����� 
				if(JudSave[0]=='5') return 4; //δ���ӵ� WiFi
				
				return -1;  //δ֪
			}
	}
	return -2;
}


int ATorg_ConnectTCPServer(const char *IP,usint port)
{
	char cBuf[100];
	int re=0;

	if(TRAutoModeMark) return 0; //�Ѿ�����͸��
	
	
	/*
		if(JudSave[0]=='2') return 1; //�Ѿ���ȡIP��������
		if(JudSave[0]=='3') return 2; //TCP��UDP������
		if(JudSave[0]=='4') return 3; //TCP��UDP�Ͽ����� 
		if(JudSave[0]=='5') return 4; //δ���ӵ� WiFi	
	*/
	
	re=ATorg_ConnStatus();

	if((re==1)||(re==2)||(re==3))
	{
		sprintf(cBuf,"AT+CIPSTART=\"TCP\",\"%s\",%d\r\n",IP,port); 
		WSString((uchar *)cBuf);
		JudgeString("CONNECT",NULL);
		LoopTime(2000);
		while(LoopSTA())
		{
			if(JudgeState(NULL))
			{
				JudgeStaCLR();
				return 0;
			}
		}
		return -2;
	
	}
	else
	{
		return -1;
	}
	
	
	
}



/*
0:����OK
<0������ʧ��
*/
int ATorg_ConnectCloud(void)
{
	if(TRAutoModeMark) return 0; //�Ѿ�����͸��
	return ATorg_ConnectTCPServer("183.230.40.39",876);
	//return ATorg_ConnectTCPServer("192.168.1.109",876);
}


/*
��ʱ����͸��ģʽ����λ��+++���˳�͸��
����͸��ģʽǰ����Ҫ�Ѿ�������TCP������
���� 0:�Ѿ�������͸��
<0 :����
*/
int ATorg_TRAutoMode(void)
{
	if(TRAutoModeMark) return 0; //�Ѿ�����͸��
	delay(300);
	WSString("AT+CIPMODE=1\r\n"); //1,͸��ģʽ
	JudgeString("OK",NULL);
	LoopTime(2000);
	while(LoopSTA())
	{
		if(JudgeState(NULL))
		{
			JudgeStaCLR();
			delay(300);
			WSString("AT+CIPSEND\r\n"); //1,͸��ģʽ
			JudgeString(">",NULL);
			LoopTime(2000);
			while(LoopSTA())
			{
				if(JudgeState(NULL))
				{
					JudgeStaCLR();
					TRAutoModeMark=1; //�Ѿ�����͸��ģʽ�ı�־λ
					return 0;
				}
			}	
			return -2;
		}
	}
	return -1;
}


/*
͸��ģʽ�·�������
*/
int ATorg_Send(uchar *buf,usint len)
{
	if(TRAutoModeMark) //�Ѿ�����͸��
	{
		WSData(buf,len);
	}
	return 0;
}








