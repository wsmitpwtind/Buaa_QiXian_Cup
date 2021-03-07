#include "BoardLib.h"




#define LedFlashLOWTim 700 //慢闪的半周期

uchar LEDTRUN=0;
ulong LEDTRUNTime=LedFlashLOWTim;
usint LEDTRUNCnt=0;
usint LEDTRUNNum=0;

_st_Mpu MPU6050;
_st_AngE Angle;


//keep，快闪的持续时间
//CY，  快闪的半周期
void LedFlashSET(usint Keep,usint CY)  //LED快闪 提示信息
{
	LEDTRUNNum=Keep/CY;
	LEDTRUNCnt=0;
	LEDTRUNTime=CY; 
}

ulong LEDLastTime=0;

void LedFlash(void)
{
	if( SysRunTime() > LEDLastTime+LEDTRUNTime ) //500ms ,一般是500，通信的时候60ms
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



//蜂鸣器响一定时间后自动关闭
void Beep(usint ms)
{
	BEEP_ON;
	BeatBeepNum=ms/SysBeat_Cycle;
}




int BoardInit(void)
{
sys_Init();
JTAG_ENable(1);
SysTimeBase_Init(); //用于精确计时，获取系统时间
	
LED_OutMode; 
LED_OFF;
Btt0_InMode;
Btt1_InMode; //按键
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
SysTick_Init(); //系统滴答时钟，用于系统节拍

KeyFiltUD_CLR(0);
KeyFiltUD_CLR(1);


return 0;
}






//5ms中断一次，按键输入滤波
void BeatHandle_Key(void)
{
	KeyFiltIn(0,Btt0_InD);
	KeyFiltIn(1,Btt1_InD);
}











