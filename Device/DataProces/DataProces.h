#ifndef _DataProces_h_
#define _DataProces_h_

#include "stm32f10x_type.h"


#include "stm32f10x_Reg.h"
#include "stm32f10x_Init.h"
#include "DataProces.h"




void HEXtoBCD(ulong hexdata,uchar *BCD_dat); //³ö¿Ú£º0x12345...
uchar lcd_hide0(uchar *path,uchar halfSTA,uchar num,uchar *OutP);
uchar Mean_Filt_uchar(uchar d,uchar * Filt_FIFO,uchar Mean_Num);
usint Mean_Filt_usint(usint d,usint * Filt_FIFO,uchar Mean_Num);
sint Mean_Filt_int(sint d,sint * Filt_FIFO,uchar Mean_Num);


uchar  AsciiToHEX(uchar len,uchar *buf,long *OutP);

void bubbling(uchar *path,uchar LEN);
void bubbling_long(long *path,uchar LEN);
void bubbling_int(int *path,uchar LEN);
void FIFO_uchar(uchar FD,uchar *buf,uchar LEN);
void FIFO_int(int FD,int *buf,uchar LEN);
void FIFO_usint(usint FD,usint *buf,uchar LEN);
void FIFO_long(long FD,long *buf,uchar LEN);



#endif



