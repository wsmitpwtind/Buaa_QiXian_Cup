#ifndef  _LXU_AtRISC_h_
#define  _LXU_AtRISC_h_


#include   "stm32f10x_type.h"
#include   "stm32f10x_Init.h"


#define _NAME_MAX_LEN  10  //����ATָ�������ȣ�����+��

#define _PARA_DAT_MAX_LEN  11  //���ĵ���ATָ�����(����)���Ȱ���+- 
#define _NP_DAT_MAX_NUM    5  //����ATָ�����(����)����,����Name+Para

#define _PARA_STR_MAX_LEN  200  //����ATָ�����(�ַ���)���Ȱ���""
#define _NP_STR_MAX_NUM    3   //����ATָ�����(�ַ���)����


typedef struct 
{
char NaBuf[_NAME_MAX_LEN+1];	 //AT�����棬�ڴ��а�����������"\0"

uchar PaNum;				 //���������������ַ�����
long  Para[_NP_DAT_MAX_NUM];//���ݲ�����long��

uchar StrNum;				   //�ַ�����������
char Str[_NP_STR_MAX_NUM][_PARA_STR_MAX_LEN];  //�ַ�����������,�ַ����в���",���ڴ��а�����������"\0"

uchar DS_Flag;//���ݣ��ַ�����ǡ�����ŵڼ������������ݣ�BIT0Ϊ��һ���������ڼ����������ַ��������ݣ�0���ַ�����1

uchar Type;  //==0,���������ã�==1��Ϊ��ѯר�ñ�־������Ĳ�ѯģʽ������Ҫ�������������Ͳ������͵�ƥ��

}_Type_OutForm;


uchar AT_Flow_ReceByte(uchar RxB);
uchar AT_ParaDncode(uchar Len,uchar * inP,_Type_OutForm * _OutF);
uchar AT_ParamCount(_Type_OutForm * _OutF);
void AT_Flow_BUFF_Clear(void);
void DataFlow_SET(uchar en,usint Len,uchar *buf);
void Data_Flow_BUFF_Clear(void);
usint Data_Flow_RxLen(void) ;


#endif


