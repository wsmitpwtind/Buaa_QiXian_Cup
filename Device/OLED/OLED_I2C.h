#ifndef _OLED_I2C_H_
#define	_OLED_I2C_H_

#include		"stm32f10x_Reg.h"
#include		"stm32f10x_type.h"
#include		"stm32f10x_Init.h"
#include		"delay.h"   

#define LED_IMAGE_WHITE       1
#define LED_IMAGE_BLACK       0

#define LED_MAX_ROW_NUM      64
#define LED_MAX_COLUMN_NUM  128

void OLED_Init(void);
void OLED_SetPos(uchar SX, uchar SY);
void OLED_Fill(uchar ucData);
void OLED_FillRam(uchar *ucData);
void OLED_PrintLong_6x8(long sData,uchar SX, uchar SY);
void OLED_PrintLong_8x16(long sData,uchar SX, uchar SY);

void ShowData(long sData,uchar Size,uchar SX, uchar SY);
void ShowString(uchar * ucDataStr,uchar Size,uchar SX, uchar SY);

void ShowClear(void);
void OLED_PrintBMP(const uchar *pucTable,uchar wide,uchar high, uchar x, uchar y);


#endif
