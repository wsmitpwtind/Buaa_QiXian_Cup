#ifndef _PID_h_
#define _PID_h_

#include   "stm32f10x_Reg.h"
#include   "stm32f10x_type.h"
#include   "stm32f10x_Init.h"
#include   "delay.h" 





void PID_Init(void);
long GetPulseSpeed_A(void);
long GetPulseSpeed_B(void);
long GetPulseSpeed_C(void);
long GetPulseSpeed_D(void);

void PIDParSPDUpdate(void);
void PIDSPD_SinglePulse(void) ;  
void PIDSpeed_SetGoal_A(int goal);
void PIDSpeed_SetEnable_A(uchar En);

void PIDSpeed_SetGoal_B(int goal);
void PIDSpeed_SetEnable_B(uchar En);

void PIDSpeed_SetGoal_C(int goal);
void PIDSpeed_SetEnable_C(uchar En);


void PIDSpeed_SetGoal_D(int goal);
void PIDSpeed_SetEnable_D(uchar En);


#endif


