#ifndef  _LXU_AtRISC_h_
#define  _LXU_AtRISC_h_


#include   "stm32f10x_type.h"
#include   "stm32f10x_Init.h"


#define _NAME_MAX_LEN  10  //最大的AT指令名长度，不含+号

#define _PARA_DAT_MAX_LEN  11  //最大的单个AT指令参数(数据)长度包含+- 
#define _NP_DAT_MAX_NUM    5  //最大的AT指令参数(数据)个数,包含Name+Para

#define _PARA_STR_MAX_LEN  200  //最大的AT指令参数(字符串)长度包含""
#define _NP_STR_MAX_NUM    3   //最大的AT指令参数(字符串)个数


typedef struct 
{
char NaBuf[_NAME_MAX_LEN+1];	 //AT名缓存，内存中包含结束符："\0"

uchar PaNum;				 //参数数量，包含字符串的
long  Para[_NP_DAT_MAX_NUM];//数据参数，long型

uchar StrNum;				   //字符串参数数量
char Str[_NP_STR_MAX_NUM][_PARA_STR_MAX_LEN];  //字符串参数缓存,字符串中不含",但内存中包含结束符："\0"

uchar DS_Flag;//数据，字符串标记。标记着第几个参数是数据，BIT0为第一个参数，第几个参数是字符串。数据：0，字符串：1

uchar Type;  //==0,正常的设置，==1，为查询专用标志，特殊的查询模式，不需要检测参数数量，和参数类型的匹配

}_Type_OutForm;


uchar AT_Flow_ReceByte(uchar RxB);
uchar AT_ParaDncode(uchar Len,uchar * inP,_Type_OutForm * _OutF);
uchar AT_ParamCount(_Type_OutForm * _OutF);
void AT_Flow_BUFF_Clear(void);
void DataFlow_SET(uchar en,usint Len,uchar *buf);
void Data_Flow_BUFF_Clear(void);
usint Data_Flow_RxLen(void) ;


#endif


