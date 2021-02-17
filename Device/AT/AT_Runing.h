#ifndef  _AT_Runing_h_
#define  _AT_Runing_h_


#include   "stm32f10x_type.h"
#include   "stm32f10x_Init.h"
#include   "LXU_AtRISC.h"



void ATCommInit(void);
void AT_TimeOut_Handler(void);
void AT_SendImageDatas(void);
void  AT_Datas_Receive(usint Len,uchar *buf);
uchar AT_Datas_RxWait(usint *len);

void ATUART_RQHandler(uchar UD);
void AT_Datas_Receive(usint Len,uchar *buf);
uchar AT_Datas_RxWait(usint *len);

void ATCommunication_Init(void);
void AT_CommRunning(void);
uchar ATOrgEnableMark(void);


void ConntPhoneMarkSET(uchar on);
void ATPingRuning(void);
void AT_Order_handler(_Type_OutForm FInfo);


#endif

