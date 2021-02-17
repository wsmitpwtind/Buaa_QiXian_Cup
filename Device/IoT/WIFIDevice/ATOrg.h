#ifndef _ATOrg_
#define _ATOrg_

#include   "stm32f10x_type.h"




void ATDataStream(uchar iD);
void JudgeString(const char * P,const char * STP);
uchar JudgeState(usint *);
void JudgeStaCLR(void);

int WIFIDevReady(void);
int ATorg_SmartCfgStart(void);
int ATorg_SmartCfgStop(void);
void ShowSmartConfig(void);
int ATorg_ConnectTCPServer(const char *IP,usint port);

int ATorg_ConnectCloud(void);
int ATorg_Send(uchar *buf,usint len);
long ATorg_Receive(uchar *rBuf,ulong ms);
int ATorg_TRAutoMode(void);




#endif


