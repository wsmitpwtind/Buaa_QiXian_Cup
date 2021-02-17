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


uchar JudNewsLenBuf[4]={0};  //消息体的4个字节的长
usint JudRxHBCnt=0;

void AutoParCLR(void)
{
	JudRxHBCnt=0;
}

/*
==0：正在接受数据
>0:  已经收到数据包了，返回值即为长度
<0： 错误
*/
int AutoRx(uchar iD,uchar *RxHB)
{
static ulong JudNewsLength=0;
static uchar RDSta=0;
int i=0;

	if(JudRxHBCnt==0)  //检测头
	{
		if((iD&0x0f)==0)
		{
			RxHB[JudRxHBCnt++]=iD;
			RDSta=0; //准备开始接收长度字节数据
		}
		else 
		{
			AutoParCLR();
			return -1; //非头
		}
	}
	else
	{
		if(RDSta<4) //收长度字节(1~4字节不等)
		{
			RxHB[JudRxHBCnt++]=iD;
			JudNewsLenBuf[RDSta]=iD&0x7f; //去掉BIT7
			if(iD&0x80) //长度需要连续
			{
				RDSta++;
				if(RDSta==4)
				{
					AutoParCLR();
					return -2; //长度错误
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
				RDSta=5; //不接受消息长度字节了
				
				if(JudNewsLength>(ulong)WIFIRxMaxNum) //长度最长
				{
					AutoParCLR();
					return -3;	
				}
				
				if(JudNewsLength==0) //后面的长度为0
				{
					i=JudRxHBCnt;
					AutoParCLR();
					return i;
				}
			}
		}
		else
		{
			//再接收JudNewsLength长度的数据即可
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

接收AT指令字串的处理

****************************************************************************/
char JudStr[_JudStrlen]={0};	//检测的目标字串
char JudSop[_JudStrlen]={0};	//保存结束的字串
char JudSave[_JudSavelen]={0};	//检测到目标字串后，保存后续的字节流


//下面是字串接收，判断的处理参数
uchar JudRxStrStep=0;
usint JudgeCnt=0;
usint JudgeCntSave=0;
usint JudgeLenS=0;	 //开始字串的长度
usint JudgeLenP=0; 	 //结束字串的长度
uchar JudgeSTA=0;	 //1:检测到匹配的字符串后为1
					 //2:检测到匹配的字符串后，且保存到了指定字节数后为1
					 
uchar JudgeGoonEn=0; //=1检测完前面的字串后，需要继续检测后面的字串，并保存中间的字串
uchar TRAutoModeMark=0;  //已经进入透传模式的标志位


/* 
P:在流中需要检测的字符串
STP:检测到字符串后到STP字串之间的数据将被保存
	 ==NULL，时将不保存
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
		JudgeGoonEn=1; //继续检测结束的字串
		JudgeLenP=strlen(STP);
		strcpy(JudSop,STP);	
		
	}
	JudgeCnt=0;
	JudgeSTA=0;
	JudRxStrStep=1; //开启检测字符串0
}


/*
在中断数据流中检测，是否有匹配的字串
返回0： 找到匹配的字串，
*/
uchar JudgeState(usint *Plen)
{
	if(Plen!=NULL) *Plen=JudgeCntSave;
	return (JudgeSTA);
}


//状态清0
void JudgeStaCLR(void)
{
	JudgeSTA=0;
	JudgeLenS=0;
	JudgeLenP=0;
	JudgeCnt=0;
	
	JudRxStrStep=0;
}



/****************************************************************************

中断实体

****************************************************************************/
void ATDataStream(uchar iD)
{

if(TRAutoModeMark) //开启了透传模式，接收云数据
{
	
	// MQTT_RxIRQ(iD);
	//这里是测试，收到的数据通过串口打印出来
	USART1_TxDat(iD);
}
else
{
		if(JudRxStrStep==1) //检测字符串匹配的阶段
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
		else if(JudRxStrStep==2) //保存后续数据的阶段
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
				if(JudgeCntSave>=_JudStrlen)  //保存的数据过长
				{
					JudgeSTA=0;
					JudRxStrStep=0;		
				}
			}
		}
}


}



/********************************************************************

					AT指令实体

********************************************************************/


/*
模块复位后等待"ready"字串
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



//发送AT指令
int ATorg_OK(void)
{
	if(TRAutoModeMark) return -1; //已经进入透传
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


//设置STA模式
int ATorg_STAMode(void)
{
	if(TRAutoModeMark) return -1; //已经进入透传
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

//设置AP模式
int ATorg_APMode(void)
{
	if(TRAutoModeMark) return -1; //已经进入透传
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

//开启smartconfig连接：ESPTOUCH+微信同时
//将自动换为STA模式
int ATorg_SmartCfgStart(void)
{
ulong Time=0;
ulong Cnt=1;

	if(TRAutoModeMark) return -1; //已经进入透传
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
			LoopTime(60000);  //60秒超时
			while(LoopSTA())
			{
				if(JudgeState(NULL))
				{
					JudgeStaCLR();
					return 0;
				}
				
				/*用于提示，可不要*/
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
	if(TRAutoModeMark) return -1; //已经进入透传
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
ShowChinese("<进入智能联网>",8,0);
ShowChinese("请用微信或专用APP联网",0,2);
ShowChinese("等待连接...",0,6);
*/
	
ShowString("<EspTouch>",1,24,1);
	
printf("WIFI 进入智能联网\r\n");
printf("请用专用APP联网\r\n");
printf("等待连接...\r\n");
	


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


if(i<0) printf("联网失败\r\n"); else printf("联网成功\r\n");

printf("退出智能联网\r\n");

ShowString("Exit",1,0,0);
delay(3000);
ShowClear();
}




//0://已经进入透传,查不到
//1; 已经获取IP，联网了
//2; TCP或UDP已连接
//3; TCP或UDP断开连接 
//4; 未连接到 WiFi

int ATorg_ConnStatus(void)
{
	if(TRAutoModeMark) return 0; //已经进入透传
	WSString("AT+CIPSTATUS\r\n");
	JudgeString("STATUS:","\r\n");
	/*
	该指令返回：
	后续还会返回连接的目标IP,端口等信息
	*/
	LoopTime(1000);
	while(LoopSTA())
	{
			if(JudgeState(NULL)==2)
			{
				JudgeStaCLR();
				if(JudSave[0]=='2') return 1; //已经获取IP，联网了
				if(JudSave[0]=='3') return 2; //TCP或UDP已连接
				if(JudSave[0]=='4') return 3; //TCP或UDP断开连接 
				if(JudSave[0]=='5') return 4; //未连接到 WiFi
				
				return -1;  //未知
			}
	}
	return -2;
}


int ATorg_ConnectTCPServer(const char *IP,usint port)
{
	char cBuf[100];
	int re=0;

	if(TRAutoModeMark) return 0; //已经进入透传
	
	
	/*
		if(JudSave[0]=='2') return 1; //已经获取IP，联网了
		if(JudSave[0]=='3') return 2; //TCP或UDP已连接
		if(JudSave[0]=='4') return 3; //TCP或UDP断开连接 
		if(JudSave[0]=='5') return 4; //未连接到 WiFi	
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
0:连接OK
<0：连接失败
*/
int ATorg_ConnectCloud(void)
{
	if(TRAutoModeMark) return 0; //已经进入透传
	return ATorg_ConnectTCPServer("183.230.40.39",876);
	//return ATorg_ConnectTCPServer("192.168.1.109",876);
}


/*
临时进入透传模式，复位或‘+++’退出透传
进入透传模式前，需要已经连接上TCP服务器
返回 0:已经进入了透传
<0 :错误
*/
int ATorg_TRAutoMode(void)
{
	if(TRAutoModeMark) return 0; //已经进入透传
	delay(300);
	WSString("AT+CIPMODE=1\r\n"); //1,透传模式
	JudgeString("OK",NULL);
	LoopTime(2000);
	while(LoopSTA())
	{
		if(JudgeState(NULL))
		{
			JudgeStaCLR();
			delay(300);
			WSString("AT+CIPSEND\r\n"); //1,透传模式
			JudgeString(">",NULL);
			LoopTime(2000);
			while(LoopSTA())
			{
				if(JudgeState(NULL))
				{
					JudgeStaCLR();
					TRAutoModeMark=1; //已经进入透传模式的标志位
					return 0;
				}
			}	
			return -2;
		}
	}
	return -1;
}


/*
透传模式下发送数据
*/
int ATorg_Send(uchar *buf,usint len)
{
	if(TRAutoModeMark) //已经进入透传
	{
		WSData(buf,len);
	}
	return 0;
}








