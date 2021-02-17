#ifndef _syn500r_
#define _syn500r_

#include		"stm32f10x_Reg.h"
#include		"stm32f10x_type.h"
#include		"stm32f10x_Init.h"
#include		"delay.h"   


#define S_Key0 0
#define S_Key1 1
#define S_Key2 2
#define S_Key3 3
#define S_Key4 4
#define S_Key5 5
#define S_Key6 6
#define S_Key7 7
#define S_Key8 8
#define S_Key9 9
#define S_Key10 10
#define S_Key11 11
#define S_Key12 12
#define S_KeyUp 13
#define S_KeyDown 14
#define S_KeyL 	15
#define S_KeyR 	16
#define S_KeyOk 	17

#define S_KeyNull 254
#define S_KeyErr 	255


#define SYN500RPin_InMode   Port_In_Out(PortC,BIT1,Updown_IN)
#define SYN500RPin_InDat    PCin(1)



void SYN500RInit(void);
void SYN500RPin_IRQHandler(void);
void BeatHandle_SYN500RPin(void);
void SYN500StudyRuning(void);
void ReKeyStudyEnable(void);
void ReKeyStudy_Save(void);
uchar GetSyn_SAT(void);
uchar GetSyn_Key(void);

#endif

