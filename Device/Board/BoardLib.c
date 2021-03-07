#include "BoardLib.h"




#define LedFlashLOWTim 700 //�����İ�����

uchar LEDTRUN=0;
ulong LEDTRUNTime=LedFlashLOWTim;
usint LEDTRUNCnt=0;
usint LEDTRUNNum=0;

_st_Mpu MPU6050;
_st_AngE Angle;


//keep�������ĳ���ʱ��
//CY��  �����İ�����
void LedFlashSET(usint Keep,usint CY)  //LED���� ��ʾ��Ϣ
{
	LEDTRUNNum=Keep/CY;
	LEDTRUNCnt=0;
	LEDTRUNTime=CY; 
}

ulong LEDLastTime=0;

void LedFlash(void)
{
	if( SysRunTime() > LEDLastTime+LEDTRUNTime ) //500ms ,һ����500��ͨ�ŵ�ʱ��60ms
	{
		LEDLastTime=SysRunTime();
		LEDTRUN=!LEDTRUN; if(LEDTRUN) LED_OFF; else LED_ON;
		
		if(LEDTRUNCnt++>LEDTRUNNum)
		{
			LEDTRUNCnt=LEDTRUNNum;
			LEDTRUNTime=LedFlashLOWTim;
		}
	}
}



static ulong BeatBeepNum=0;

void BeatHandle_Beep(void)
{
	if(BeatBeepNum)
	{
		BeatBeepNum--;
		if(BeatBeepNum==0)
		{
			BEEP_OFF;
		}
	}
}



//��������һ��ʱ����Զ��ر�
void Beep(usint ms)
{
	BEEP_ON;
	BeatBeepNum=ms/SysBeat_Cycle;
}




int BoardInit(void)
{
sys_Init();
JTAG_ENable(1);
SysTimeBase_Init(); //���ھ�ȷ��ʱ����ȡϵͳʱ��
	
LED_OutMode; 
LED_OFF;
Btt0_InMode;
Btt1_InMode; //����
BEEP_OutMode;
BEEP_OFF;
USART_Init(1,115200);
	
EEPFlashInit();
EFile_DownLoad();
ATCommInit();
	
I2C_Init(4000);

MotorDriveInit();
ServoInit();
PID_Init();
	
Beep(200);
SysTick_Init(); //ϵͳ�δ�ʱ�ӣ�����ϵͳ����

KeyFiltUD_CLR(0);
KeyFiltUD_CLR(1);


return 0;
}






//5ms�ж�һ�Σ����������˲�
void BeatHandle_Key(void)
{
	KeyFiltIn(0,Btt0_InD);
	KeyFiltIn(1,Btt1_InD);
}











