#ifndef _DELAY_H_
#define _DELAY_H_

#include   "stm32f10x_type.h"

/******************************

中断节拍配置，可用于如：事件，计时等

*******************************/

#define  SysBeat_Cycle 5  //其他函数的调用周期是5ms中断一次



typedef struct
{
	ulong ms;
	ulong us;
	ulong cnt;
}TypeTimerPar;


struct Str_TaskLine
{
	void(*TaskLineFP[10])(); //定义一个函数指针 的指针
	ulong *CyTime;  //每个函数对应的进入时间点，比如每2ms运行一次
	uchar TLSetp;
	ulong WaitTrueLTime; //用于轮训检测的，如每2ms运行一次
	ulong RunTim;
};




//以下是用定时器实现的延时
void  SysTick_Init(void);
void SysTimeBase_Init(void) ;
void  BeatHandle_delay(void);
void  delay(ulong ms);
ulong SysRunTiygme(void);


void  TimerStart(uchar change);
ulong TimerStop(uchar change);
ulong TimerGet(uchar change);


void SinglePulseStart(uchar Channel,usint us, void (*UFun)());
void BeatHandle_SinglePulse(void);
void delayus(ulong us);

void LoopTime(ulong ms);
uchar LoopSTA(void);
uchar WaitTrue(uchar Num,ulong GapTime);

uchar WaitTrue_TL(uchar Num,ulong GapTime);
void TaskLineRunning(uchar Num);
ulong TaskLine_GetRunTim(uchar Num);
void TaskLineSetp_UP(uchar Num);
void TaskLineSetp_Down(uchar Num);
void TaskLineSetp_Start(uchar Num);
void TaskLineSetp_Stop(uchar Num);
uchar TaskLineSetp_Get(uchar Num);

void Init_time(void);
void TimeReload(void);
uchar TimeOn(ulong GapTime);
#endif

