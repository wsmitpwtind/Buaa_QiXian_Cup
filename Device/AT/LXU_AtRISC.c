#include "LXU_AtRISC.h"
#include "DataProces.h"



/*
在中断中只调用：AT_Flow_ReceByte(UD);
暂用时间少，而参数的判断，在主函数执行
中断实例：
void USART1_IRQHandler(void)
{
	uchar re;
	uchar UD=USART1_DR;    //清中断标志位

	re=AT_Flow_ReceByte(UD);

	if( re==0x41)         //已经接收到正确的头,开始计数
	AT_TimeOut_Enable(1);
	else if(re==0)
	{
	AT_Order_Finish=1;
	AT_TimeOut_Enable(0);  //正确收到一条指令，关计数
	}
}

主函数处理实例：
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

如：
AT+MYTEST 125,18945,"192.168.0.1",-1,"255.255.255.0","MyTest_OK",-14896


关于大数据流的接收：
1：主机先发送 大数据流接收的申请指令（参数中一定是带有“长度”的），如：AT+SOKTX 1024 \r\n
2: 主机等待模块返回OK，标明现在可以发送指定长度的数据了
3：主机发送透传数据
   需要注意：在收到返回OK（第二步）到第一个数据字节发送的时间间隔是无限制的。模块将一直等待第一个字节的到来
   但是在收到第一个数据字节后，两两数据的间隙时间不大于200ms,否则做超时处理，返回ER
4：主机等待模块返回接收数据的状态，OK表示刚才的数据已经接收OK

*/

#define AT_Flow_RX_MAX_LEN  (((usint)_PARA_DAT_MAX_LEN*_NP_DAT_MAX_NUM)+((usint)_PARA_STR_MAX_LEN*_NP_STR_MAX_NUM)+_NAME_MAX_LEN+5) //最大的AT的 指令+参数长度,


uchar AT_ParLen=0;
uchar AT_RunBuf[AT_Flow_RX_MAX_LEN];
uchar FlowStep=0;



uchar *DataFlowBuf;		//大数据流的缓存
usint  DataFlowLen;		//接收大数据流的长度
uchar  DataFlowEn=0;	//为1使能接收大数据流

usint  DataFlowCnt=0;	//计数


void DataFlow_SET(uchar en,usint Len,uchar *buf)
{
	DataFlowEn=1;
	DataFlowLen=Len;
	DataFlowBuf=buf;
	DataFlowCnt=0;
}


usint Data_Flow_RxLen(void)  //返回正在接收了多少字节
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
AT指令字节接收流...

返回：
0x00： 接收OK
0x8x： x代表错误代码
0x4x： x代表下一次接收数据将处于哪个步骤

0x21:  表示正在接收大数据流
0x2f:  表示接收大数据流完成

关于超时：
整个指令时间不得大于溢出时间
接收大数据流中，两两字节的间隔时间不得大于溢出时间

********************************/ 

uchar AT_Flow_ReceByte(uchar RxB)
{
static uchar Step_Last=0;
static uchar RxCNT=0;

//-----------数据流接收处理部分----------------
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
//-----------数据流接收处理部分-----------------



if( RxB=='\r') {Step_Last=FlowStep; FlowStep=0x0f; return(FlowStep+0x40); } //下次必须接收'\n'。

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

if(Len==0) //为AT
{
	_OutF->NaBuf[0]='\0'; //加上字符串结束符，\0
	return 0;
}
else  //为AT+
{
	if(inP[0]!='+')
	{
		return 0x80;
	}
	else
	{
		Group=AT_Packet(Len-1,&inP[1],PackADD,PackLen);
		{
		
		if( PackLen[0]>_NAME_MAX_LEN )		return 0x82;  //Name长度过长
			
		
		for(k=0;k<PackLen[0];k++)  		//保存Name
		{
		i=inP[ 1+PackADD[0] +k ];
		if( ( i<'A') || (i >'z') ) 
		return 0x83;  					 //Name值域出错
		else
		_OutF->NaBuf[k]=i;
		}
		_OutF->NaBuf[PackLen[0]]='\0'; //加上字符串结束符，\0
		
		_OutF->StrNum=0;  //字符串参数数量
		_OutF->PaNum=0;
		_OutF->DS_Flag=0;
		
		
		if(((Group-1)==1)&&(inP[ 1+PackADD[1] ]=='?'))
		{			
			_OutF->Type=1;  //==1,查询标志符,特殊的查询模式，不需要检测参数数量，和参数类型的匹配。
			return 0;
		}
		else
		{
			_OutF->Type=0;  //==0,非查询标志符
		}
		
		for(k=0;k<(Group-1);k++)  		//保存Param
		{
			if(inP[ 1+PackADD[1+k] ]=='"')  //字符串参数设置
			{
				if( PackLen[1+k]> _PARA_STR_MAX_LEN)  return 0x86;//字符串参数长度过长
				if(inP[ 1+PackADD[1+k] + (PackLen[1+k]-1) ]!='"')  return 0x87;  //没有对称的""
				
				if((_OutF->StrNum+1)>_NP_STR_MAX_NUM) return 0x88;//字符串参数量过长
				
				for(i=0;i<(PackLen[1+k]-1);i++)
				_OutF->Str[_OutF->StrNum][i]=inP[1+PackADD[1+k] +i+1];
			
				_OutF->Str[_OutF->StrNum][PackLen[1+k]-2]='\0'; //加上字符串结束符，\0
				
				_OutF->StrNum++;
				_OutF->DS_Flag|=(1<<k);//数据，字符串标记。
			}
			else							//数据串参数设置
			{
				if( PackLen[1+k]> _PARA_DAT_MAX_LEN)    return 0x84;//Para长度过长
				if( (_OutF->PaNum+1)> _NP_DAT_MAX_NUM) 	return 0x81;  //参数组过多
				
				i=AsciiToHEX(PackLen[1+k],&inP[ 1+PackADD[1+k] ],&OutHexP);
				
				if(i)  		 //Param值域出错
				{
				return 0x85;  	
				}				
				else
				{
				_OutF->Para[_OutF->PaNum]=OutHexP;
				_OutF->PaNum++;
				//_OutF->DS_Flag&=~(1<<k);//本为0，数据，字符串标记。				
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
分组统计函数，计算以逗号为分割的组数
以及每组的数量
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
	
		if( i!=(iLen-1) )//最后一个是','的情况
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

//--最后一组
PackLen[PackNum-1]=CNT;
return PackNum;
}





