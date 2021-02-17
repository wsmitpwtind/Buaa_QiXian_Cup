
#include   "stm32f10x_type.h"
#include   "stm32f10x_Init.h"
#include	"EEPFlash.h"
#include 	"stm32_flash.h"
#include 	"string.h"

#define _EPSADD  32

_TYPE_EEPROM_PACK SYS_Cfg;

const _TYPE_EEPROM_PACK EP_Default_PACK=
{
_EFile_Magic_String,	//EFile�е���ʼ�ļ���ʾ

	
90,//�ٶȻ���PID����
3,
0,	

	
{0,0,0,0,0,0,0,0,},
"0000"
};




/***************************************************

		EFile-EEPROM �������

**************************************************/

_TYPE_EEPROM_PACK SYS_Cfg_last;


int EFile_Read(usint Reg_Adds,uchar *buf,usint R_LEN)
{
		EEPFlash_Read(Reg_Adds,buf,R_LEN);
		return 0;
}


	

int EFile_Write(usint Reg_Adds,uchar *buf,usint R_LEN)
{
	
	EEPFlash_Write(Reg_Adds,buf,R_LEN);
	return 0;
}


/*
��EFile�е�����export�����õĲ�����P��

*/
int EFile_Export(_TYPE_EEPROM_PACK *P)
{
	EFile_Read(_EPSADD,(uchar *)(P),sizeof(_TYPE_EEPROM_PACK) );
	return (0);
}


/*
�ֱ�ȶ�Pnew��Pold��ÿһ��Ԫ�أ�
�в�ͬ�ģ�������EFile��д���ȡУ��ķ�����

���أ�=�������б䶯,��EFile�޸Ĵ���  ���޸�ʧ��
	   =0������,�ޱ䶯��EFile������޸ģ������޸ĵĸ���
*/
int EFile_Inport(void)
{
usint len=sizeof(_TYPE_EEPROM_PACK);
EFile_Write(_EPSADD,(uchar *)(&SYS_Cfg),len);

return (0);	
}



/*
�ظ�������Ĭ�ϵĳ���ֵ
�����������ص� P��
*/
int EFile_Default(_TYPE_EEPROM_PACK *P)
{
usint i=0;
uchar *Tem0,*Tem1;

EFile_Write(_EPSADD,(uchar *)(&EP_Default_PACK),sizeof(_TYPE_EEPROM_PACK));

if( EFile_Export(P)<0 ) return (-2);

//---------------�ṹ��Ƚ�-------------
Tem0=(uchar *)P;
Tem1=(uchar*)(&EP_Default_PACK);
for(i=0;i<sizeof(_TYPE_EEPROM_PACK);i++)
{
	if(Tem0[i]!=Tem1[i]) return -3;
}
//---------------------------------------

return 0;
}


/*
EEPROM �������س���
����
0��OK��
<0������ʧ��
*/
int EFile_DownLoad(void)
{
	int i=0;
	_TYPE_EEPROM_PACK Temp_Cfg;

	if( EFile_Read(_EPSADD,(uchar *)(&Temp_Cfg),sizeof(_TYPE_EEPROM_PACK) )<0 ) return (-1);

	if( strcmp(Temp_Cfg.EFMake,EP_Default_PACK.EFMake)!=0 ) //EFileͷ��ͬ
	{
		i=EFile_Default(&SYS_Cfg);
		if(i<0)  return (-2);
	}
	else
	{
		if(EFile_Export(&SYS_Cfg)<0)  return (-3);
	}
	SYS_Cfg_last=SYS_Cfg;	//����һ��
	return 0;
}






/*
���õĳ��Ȳ��ܳ���STM_SECTOR_SIZE/2����STM32F103C8����512�����ֽ�
*/
#define _EEPFlashLenght 100			//����EEPROM�ֽڴ�С100�ֽڣ�
#define _EEPFlashAdds    0x0807FC00

//const uchar Smile[_EEPFlashLenght*2] __attribute__((at(_EEPFlashAdds))) ; //ָ����ַ����һ����������

static usint RWBuff[_EEPFlashLenght];
/*
�����EEPROM��ʼ����
ע��ʵ����Ҫ��ռ��flash��_EEPFlashLenght*2���ֽ���
*/
void EEPFlashInit(void)
{
Sys_OpenHSI(1);  //Flash�����Ҫ����HSI
}


/*
�������EEPROMд��uchar����
Addr��ȡֵ0~512��STM32F103C8����512��
IP��uchar ���͵�ָ��
Num��д���uchar ���͵�����
*/
void EEPFlash_Write(usint Addr,uchar *IP,usint Num)
{
	usint i=0;
	for(i=0;i<Num;i++)
	{
		RWBuff[i]=IP[i];  //ת��Ϊusint
	}
	STMFLASH_Write(_EEPFlashAdds+Addr*2,RWBuff,Num);//д��Num������
}

/*
��ȡ��uchar����
*/
void EEPFlash_Read(usint Addr,uchar *IP,usint Num)
{
	usint i=0;
	STMFLASH_Read(_EEPFlashAdds+Addr*2,RWBuff,Num);
	for(i=0;i<Num;i++)
	{
		IP[i]=(uchar)RWBuff[i];  //ת��Ϊuchar
	}
}



