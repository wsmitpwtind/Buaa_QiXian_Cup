
#include   "stm32f10x_type.h"
#include   "stm32f10x_Init.h"
#include	"EEPFlash.h"
#include 	"stm32_flash.h"
#include 	"string.h"

#define _EPSADD  32

_TYPE_EEPROM_PACK SYS_Cfg;

const _TYPE_EEPROM_PACK EP_Default_PACK=
{
_EFile_Magic_String,	//EFile中的起始文件标示

	
90,//速度环的PID参数
3,
0,	

	
{0,0,0,0,0,0,0,0,},
"0000"
};




/***************************************************

		EFile-EEPROM 操作相关

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
从EFile中导出（export）配置的参数到P中

*/
int EFile_Export(_TYPE_EEPROM_PACK *P)
{
	EFile_Read(_EPSADD,(uchar *)(P),sizeof(_TYPE_EEPROM_PACK) );
	return (0);
}


/*
分别比对Pnew跟Pold的每一个元素，
有不同的，将更新EFile（写后读取校验的方法）

返回：=负数，有变动,但EFile修改错误  ，修改失败
	   =0或正数,无变动或EFile完成了修改，返回修改的个数
*/
int EFile_Inport(void)
{
usint len=sizeof(_TYPE_EEPROM_PACK);
EFile_Write(_EPSADD,(uchar *)(&SYS_Cfg),len);

return (0);	
}



/*
回复参数到默认的出厂值
并将参数下载到 P中
*/
int EFile_Default(_TYPE_EEPROM_PACK *P)
{
usint i=0;
uchar *Tem0,*Tem1;

EFile_Write(_EPSADD,(uchar *)(&EP_Default_PACK),sizeof(_TYPE_EEPROM_PACK));

if( EFile_Export(P)<0 ) return (-2);

//---------------结构体比较-------------
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
EEPROM 参数下载程序
返回
0：OK，
<0：下载失败
*/
int EFile_DownLoad(void)
{
	int i=0;
	_TYPE_EEPROM_PACK Temp_Cfg;

	if( EFile_Read(_EPSADD,(uchar *)(&Temp_Cfg),sizeof(_TYPE_EEPROM_PACK) )<0 ) return (-1);

	if( strcmp(Temp_Cfg.EFMake,EP_Default_PACK.EFMake)!=0 ) //EFile头不同
	{
		i=EFile_Default(&SYS_Cfg);
		if(i<0)  return (-2);
	}
	else
	{
		if(EFile_Export(&SYS_Cfg)<0)  return (-3);
	}
	SYS_Cfg_last=SYS_Cfg;	//备份一份
	return 0;
}






/*
设置的长度不能超过STM_SECTOR_SIZE/2，（STM32F103C8就是512）个字节
*/
#define _EEPFlashLenght 100			//虚拟EEPROM字节大小100字节）
#define _EEPFlashAdds    0x0807FC00

//const uchar Smile[_EEPFlashLenght*2] __attribute__((at(_EEPFlashAdds))) ; //指定地址开设一个常量数组

static usint RWBuff[_EEPFlashLenght];
/*
虚拟的EEPROM初始化，
注意实际上要求占用flash的_EEPFlashLenght*2的字节数
*/
void EEPFlashInit(void)
{
Sys_OpenHSI(1);  //Flash编程需要开启HSI
}


/*
向虚拟的EEPROM写入uchar数据
Addr：取值0~512（STM32F103C8就是512）
IP：uchar 类型的指针
Num：写入的uchar 类型的数量
*/
void EEPFlash_Write(usint Addr,uchar *IP,usint Num)
{
	usint i=0;
	for(i=0;i<Num;i++)
	{
		RWBuff[i]=IP[i];  //转化为usint
	}
	STMFLASH_Write(_EEPFlashAdds+Addr*2,RWBuff,Num);//写入Num个半字
}

/*
读取出uchar数据
*/
void EEPFlash_Read(usint Addr,uchar *IP,usint Num)
{
	usint i=0;
	STMFLASH_Read(_EEPFlashAdds+Addr*2,RWBuff,Num);
	for(i=0;i<Num;i++)
	{
		IP[i]=(uchar)RWBuff[i];  //转化为uchar
	}
}



