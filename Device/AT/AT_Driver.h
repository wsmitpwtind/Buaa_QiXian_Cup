#ifndef  _AT_Driver_h_
#define  _AT_Driver_h_

#include   <string.h>
#include   "stm32f10x_Reg.h"
#include   "stm32f10x_type.h"
#include   "stm32f10x_Init.h"

#define _DEV_ID	"P2MDA-EM2" //…Ë±∏∫≈



void AT_Send_Byte(uchar id);
void AT_Send_String_Len(uchar *p,usint len);

void AT_Send_String(const char *p);
void AT_Send_String_Plus(const char *p);
void AT_Send_End(void);
void AT_Send_ER(void);
void AT_Send_FA(void);
void AT_Send_ToAscii_BCD(long iD);
void AT_Send_ToAscii_HEX(uchar hex);

void GetDeviceID(void);
void Set_MotSPDPID(uchar type,long P,long I,long D);

#endif





