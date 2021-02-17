#ifndef _EEPFlash_
#define _EEPFlash_

#include 	"stm32f10x_type.h"

#define _EFile_Magic_String	"VR2"  //EEPROM ��ʾ�ֽ�

typedef struct 
{
char  EFMake[sizeof(_EFile_Magic_String)];

int P;
int I;
int D;
	
char  ReKEYD[8];
char  null[4]; //����STM32F4�洢��4�ֽڵģ�����Ԥ��4��
	
}_TYPE_EEPROM_PACK;


void EEPFlashInit(void);
void EEPFlash_Write(usint Addr,uchar *IP,usint Num);
void EEPFlash_Read(usint Addr,uchar *IP,usint Num);



int EFile_Read(usint Reg_Adds,uchar *buf,usint R_LEN);
int EFile_Write(usint Reg_Adds,uchar *buf,usint R_LEN);
int EFile_Export(_TYPE_EEPROM_PACK *P);
int EFile_Inport(void);
int EFile_Default(_TYPE_EEPROM_PACK *P);
int EFile_DownLoad(void);




#endif


