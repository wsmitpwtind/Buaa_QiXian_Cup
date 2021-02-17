#ifndef _KeyFI_
#define _KeyFI_


#include		"stm32f10x_Reg.h"
#include		"stm32f10x_type.h"
#include		"stm32f10x_Init.h"
#include		"delay.h"   




void  KeyFiltIn(uchar Group,uchar Vol);
uchar  KeyFiltOut(uchar Group);
uchar  KeyFiltOut_UD(uchar Group);
uchar  KeyFiltUD_CLR(uchar Group);
void  KFSynchronous(void);
uchar KeyKeepTime(uchar Group);




#endif
