#ifndef _DELAY_H_
#define _DELAY_H_

#include   "stm32f10x_type.h"

/******************************

�жϽ������ã��������磺�¼�����ʱ��

*******************************/

#define  SysBeat_Cycle 5  //���������ĵ���������5ms�ж�һ��



typedef struct
{
	ulong ms;
	ulong us;
	ulong cnt;
}TypeTimerPar;


struct Str_TaskLine
{
	void(*TaskLineFP[10])(); //����һ������ָ�� ��ָ��
	ulong *CyTime;  //ÿ��������Ӧ�Ľ���ʱ��㣬����ÿ2ms����һ��
	uchar TLSetp;
	ulong WaitTrueLTime; //������ѵ���ģ���ÿ2ms����һ��
	ulong RunTim;
};




//�������ö�ʱ��ʵ�ֵ���ʱ
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

