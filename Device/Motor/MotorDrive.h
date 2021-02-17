#ifndef  __MoorDrive__
#define  __MoorDrive__		    

#include   "stm32f10x_Reg.h"
#include   "stm32f10x_type.h"
#include   "stm32f10x_Init.h"
#include   "delay.h" 


//---------------------------------------------------------------

#define MotAB_Sleep_OutMode Port_In_Out(PortA,BIT15,GE_TW_2M)
#define MotAB_En        	PAout(15)=1
#define MotAB_Ds        	PAout(15)=0

#define MotCD_Sleep_OutMode Port_In_Out(PortC,BIT0,GE_TW_2M)
#define MotCD_En        	PCout(0)=1
#define MotCD_Ds        	PCout(0)=0

//---------------------------------------------------------------

#define HallA_InMode   Port_In_Out(PortC,BIT9,Updown_IN)
#define HallA_InD      PCin(9)

#define HallB_InMode   Port_In_Out(PortC,BIT8,Updown_IN)
#define HallB_InD      PCin(8)

#define HallC_InMode   Port_In_Out(PortC,BIT7,Updown_IN)
#define HallC_InD      PCin(7)

#define HallD_InMode   Port_In_Out(PortC,BIT6,Updown_IN)
#define HallD_InD      PCin(6)

//---------------------------------------------------------------

void MotorDriveInit(void);
void EncoderInit(void);

void MotA(int val);
void MotB(int val);
void MotC(int val);
void MotD(int val);


void EncoderInit(void);
void EncoderA_IRQ(void);
void EncoderB_IRQ(void);
void EncoderC_IRQ(void);
void EncoderD_IRQ(void);
void BeatHandle_Encoder(void);

long MotorRPM_A(void);
long MotorRPM_B(void);
long MotorRPM_C(void);
long MotorRPM_D(void);
int GetLinearVelocity_A(void);
int GetLinearVelocity_B(void);
int GetLinearVelocity_C(void);
int GetLinearVelocity_D(void);
void set_walkingmotor_speed(int Speed1, int Speed2,int Speed3, int Speed4);
void GetMileage(void);
long walkingmotor_cumulate_A_mm(void);
long walkingmotor_cumulate_B_mm(void);
long walkingmotor_cumulate_C_mm(void);
long walkingmotor_cumulate_D_mm(void);



#endif








