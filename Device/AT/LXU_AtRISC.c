#include "LXU_AtRISC.h"
#include "DataProces.h"



/*
���ж���ֻ���ã�AT_Flow_ReceByte(UD);
����ʱ���٣����������жϣ���������ִ��
�ж�ʵ����
void USART1_IRQHandler(void)
{
	uchar re;
	uchar UD=USART1_DR;    //���жϱ�־λ

	re=AT_Flow_ReceByte(UD);

	if( re==0x41)         //�Ѿ����յ���ȷ��ͷ,��ʼ����
	AT_TimeOut_Enable(1);
	else if(re==0)
	{
	AT_Order_Finish=1;
	AT_TimeOut_Enable(0);  //��ȷ�յ�һ��ָ��ؼ���
	}
}

����������ʵ����
void AT_Order_Running(void)
{
	if(AT_Order_Finish)
	{
		AT_Order_Finish=0;
		if( AT_APP(&_OutFormP) ==0)
		{
		AT_Order_handler(_OutFormP);  
		}
	}
}

�磺
AT+MYTEST 125,18945,"192.168.0.1",-1,"255.255.255.0","MyTest_OK",-14896


���ڴ��������Ľ��գ�
1�������ȷ��� �����������յ�����ָ�������һ���Ǵ��С����ȡ��ģ����磺AT+SOKTX 1024 \r\n
2: �����ȴ�ģ�鷵��OK���������ڿ��Է���ָ�����ȵ�������
3����������͸������
   ��Ҫע�⣺���յ�����OK���ڶ���������һ�������ֽڷ��͵�ʱ�����������Ƶġ�ģ�齫һֱ�ȴ���һ���ֽڵĵ���
   �������յ���һ�������ֽں��������ݵļ�϶ʱ�䲻����200ms,��������ʱ��������ER
4�������ȴ�ģ�鷵�ؽ������ݵ�״̬��OK��ʾ�ղŵ������Ѿ�����OK

*/

#define AT_Flow_RX_MAX_LEN  (((usint)_PARA_DAT_MAX_LEN*_NP_DAT_MAX_NUM)+((usint)_PARA_STR_MAX_LEN*_NP_STR_MAX_NUM)+_NAME_MAX_LEN+5) //����AT�� ָ��+��������,


uchar AT_ParLen=0;
uchar AT_RunBuf[AT_Flow_RX_MAX_LEN];
uchar FlowStep=0;



uchar *DataFlowBuf;		//���������Ļ���
usint  DataFlowLen;		//���մ��������ĳ���
uchar  DataFlowEn=0;	//Ϊ1ʹ�ܽ��մ�������

usint  DataFlowCnt=0;	//����


void DataFlow_SET(uchar en,usint Len,uchar *buf)
{
	DataFlowEn=1;
	DataFlowLen=Len;
	DataFlowBuf=buf;
	DataFlowCnt=0;
}


usint Data_Flow_RxLen(void)  //�������ڽ����˶����ֽ�
{
	return DataFlowCnt;
}

void Data_Flow_BUFF_Clear(void)
{
	DataFlowEn=0;
	DataFlowLen=0;
	DataFlowCnt=0;	
}



void AT_Flow_BUFF_Clear(void)
{
FlowStep=0;
AT_ParLen=0;
}

/********************************
ATָ���ֽڽ�����...

���أ�
0x00�� ����OK
0x8x�� x����������
0x4x�� x������һ�ν������ݽ������ĸ�����

0x21:  ��ʾ���ڽ��մ�������
0x2f:  ��ʾ���մ����������

���ڳ�ʱ��
����ָ��ʱ�䲻�ô������ʱ��
���մ��������У������ֽڵļ��ʱ�䲻�ô������ʱ��

********************************/ 

uchar AT_Flow_ReceByte(uchar RxB)
{
static uchar Step_Last=0;
static uchar RxCNT=0;

//-----------���������մ�����----------------
if(DataFlowEn)
{
	if(DataFlowLen--)
	{
		DataFlowBuf[DataFlowCnt++]=RxB;
		if(DataFlowLen==0)
		{
			DataFlowEn=0;
			return 0x2f;
		}
		else
		{
			return 0x21;
		}
	}
}
//-----------���������մ�����-----------------



if( RxB=='\r') {Step_Last=FlowStep; FlowStep=0x0f; return(FlowStep+0x40); } //�´α������'\n'��

switch(FlowStep)
{
case 0x0f:
FlowStep=0;
if( RxB == '\n')
{
	if(Step_Last<2)
	{
	FlowStep=0; 
	return(0x80);
	}
	AT_ParLen=RxCNT;
	return(0);
}
else
{
	FlowStep=0; 
	return(0x81);
}

case 0:  if(RxB=='A')  {FlowStep++; return(FlowStep+0x40); } else {FlowStep=0; return(0x82); }
case 1:  if(RxB=='T')  {RxCNT=0; FlowStep++; return(FlowStep+0x40); } else {FlowStep=0; return(0x83); }


case 2:  
//if( (RxB<' ') || (RxB>'z') ) 
//{
//	FlowStep=0;
//	return(0x84); 
//}
//else
{
	if(RxCNT >= AT_Flow_RX_MAX_LEN)
	{
	FlowStep=0;
	return(0x85); 
	}
	else
	{
	AT_RunBuf[RxCNT++]=RxB;
	return(FlowStep+0x40); 
	}
}

default: return 0xff;
}

}




uchar  AT_Packet(uchar iLen, uchar *inP,uchar * PackADD,uchar * PackLen);

uchar PackADD[_NP_DAT_MAX_NUM+_NP_STR_MAX_NUM+3];
uchar PackLen[_NP_DAT_MAX_NUM+_NP_STR_MAX_NUM+3];

uchar AT_ParaDncode(uchar Len,uchar * inP,_Type_OutForm * _OutF)
{

uchar i=0,k;
uchar Group=0;


long OutHexP=0;

if(Len==0) //ΪAT
{
	_OutF->NaBuf[0]='\0'; //�����ַ�����������\0
	return 0;
}
else  //ΪAT+
{
	if(inP[0]!='+')
	{
		return 0x80;
	}
	else
	{
		Group=AT_Packet(Len-1,&inP[1],PackADD,PackLen);
		{
		
		if( PackLen[0]>_NAME_MAX_LEN )		return 0x82;  //Name���ȹ���
			
		
		for(k=0;k<PackLen[0];k++)  		//����Name
		{
		i=inP[ 1+PackADD[0] +k ];
		if( ( i<'A') || (i >'z') ) 
		return 0x83;  					 //Nameֵ�����
		else
		_OutF->NaBuf[k]=i;
		}
		_OutF->NaBuf[PackLen[0]]='\0'; //�����ַ�����������\0
		
		_OutF->StrNum=0;  //�ַ�����������
		_OutF->PaNum=0;
		_OutF->DS_Flag=0;
		
		
		if(((Group-1)==1)&&(inP[ 1+PackADD[1] ]=='?'))
		{			
			_OutF->Type=1;  //==1,��ѯ��־��,����Ĳ�ѯģʽ������Ҫ�������������Ͳ������͵�ƥ�䡣
			return 0;
		}
		else
		{
			_OutF->Type=0;  //==0,�ǲ�ѯ��־��
		}
		
		for(k=0;k<(Group-1);k++)  		//����Param
		{
			if(inP[ 1+PackADD[1+k] ]=='"')  //�ַ�����������
			{
				if( PackLen[1+k]> _PARA_STR_MAX_LEN)  return 0x86;//�ַ����������ȹ���
				if(inP[ 1+PackADD[1+k] + (PackLen[1+k]-1) ]!='"')  return 0x87;  //û�жԳƵ�""
				
				if((_OutF->StrNum+1)>_NP_STR_MAX_NUM) return 0x88;//�ַ�������������
				
				for(i=0;i<(PackLen[1+k]-1);i++)
				_OutF->Str[_OutF->StrNum][i]=inP[1+PackADD[1+k] +i+1];
			
				_OutF->Str[_OutF->StrNum][PackLen[1+k]-2]='\0'; //�����ַ�����������\0
				
				_OutF->StrNum++;
				_OutF->DS_Flag|=(1<<k);//���ݣ��ַ�����ǡ�
			}
			else							//���ݴ���������
			{
				if( PackLen[1+k]> _PARA_DAT_MAX_LEN)    return 0x84;//Para���ȹ���
				if( (_OutF->PaNum+1)> _NP_DAT_MAX_NUM) 	return 0x81;  //���������
				
				i=AsciiToHEX(PackLen[1+k],&inP[ 1+PackADD[1+k] ],&OutHexP);
				
				if(i)  		 //Paramֵ�����
				{
				return 0x85;  	
				}				
				else
				{
				_OutF->Para[_OutF->PaNum]=OutHexP;
				_OutF->PaNum++;
				//_OutF->DS_Flag&=~(1<<k);//��Ϊ0�����ݣ��ַ�����ǡ�				
				}
				
			}
		}
		return 0;
		
	}
}
}

}


uchar AT_ParamCount(_Type_OutForm * _OutF)
{

//AT_Send_String_Len(AT_RunBuf,AT_ParLen);

return AT_ParaDncode(AT_ParLen,AT_RunBuf, _OutF);

}


/*
����ͳ�ƺ����������Զ���Ϊ�ָ������
�Լ�ÿ�������
*/
uchar AT_Packet(uchar iLen, uchar *inP,uchar * PackADD,uchar * PackLen)
{

uchar i;
uchar PackNum=1;
uchar CNT=0;

uchar JA=' ';
	
PackADD[0]=0;

for(i=0;i<iLen;i++)
{
	if( (inP[i]==',') || (inP[i]==JA) )
	{
		
	if(inP[i]==' ') JA=',';
	PackLen[PackNum-1]=CNT;
	PackADD[PackNum]=i+1;
	
		if( i!=(iLen-1) )//���һ����','�����
		{
		CNT=0;
		PackNum++;
		}
	}
	else
	{
	CNT++;
	}
}

//--���һ��
PackLen[PackNum-1]=CNT;
return PackNum;
}





