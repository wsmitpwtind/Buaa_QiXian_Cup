#ifndef _BoardLib_
#define _BoardLib_


#include		"stm32f10x_Reg.h"
#include		"stm32f10x_type.h"
#include		"stm32f10x_Init.h"
#include		"delay.h"   
#include		"DataProces.h"
#include 		"KeyFI.h"
#include		"MotorDrive.h"
#include 		"stdio.h"
#include 		"STM32_I2C.H"
#include		"MotorDrive.h"
#include		"Servo.H"
#include		"WIFIDevice.h"
#include		"ATOrg.h" 
#include		"MPU6050.h"
#include		"syn500r.h"
#include		"EEPFlash.h"
#include 		"stm32_flash.h"
#include 		"PS2.H"
#include		"AT_Driver.h"
#include 		"AT_Runing.h"
#include		"PID.h"
#include	  "EEPFlash.h"


#define LED_OutMode   Port_In_Out(PortC,BIT3,GE_TW_2M)
#define LED_ON        (PCout(3)=1)	
#define LED_OFF       (PCout(3)=0)


#define BEEP_OutMode   Port_In_Out(PortA,BIT8,GE_TW_2M)
#define BEEP_ON        (PAout(8)=1)	
#define BEEP_OFF       (PAout(8)=0)


#define Btt0_InMode   Port_In_Out(PortC,BIT15,Updown_IN)
#define Btt0_UP       (PCout(15)=1)
#define Btt0_InD      PCin(15)

#define Btt1_InMode   Port_In_Out(PortC,BIT14,Updown_IN)
#define Btt1_UP       (PCout(14)=1)
#define Btt1_InD      PCin(14)



void LedFlashSET(usint Keep,usint CY);
void LedFlash(void);
int BoardInit(void);
void BeatHandle_Key(void);

void BeatHandle_Beep(void);
void Beep(usint ms);


#endif








