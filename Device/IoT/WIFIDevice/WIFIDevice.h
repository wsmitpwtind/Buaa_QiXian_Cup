#ifndef _WIFIDevice_
#define _WIFIDevice_

#include   "stm32f10x_type.h"


#define WIFIRST_OutMode  (Port_In_Out(PortA,BIT12,GE_TW_10M))	//输出模式50M
#define WIFIRST_H      	 (PAout(12)=1)	 					    //输出的数据
#define WIFIRST_L        (PAout(12)=0)	 					    //输出的数据



void WIFIDevRST(void);
void WIFIDevInit(void);
void WSData(uchar * buf, usint rL);
void WSString(uchar * str);

void  WIFIDRelaySET(uchar Enable);
uchar WIFIDRelayGET(void);
void  WIFIUART_IRQ(uchar iD);

#endif
