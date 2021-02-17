#include "stm32f10x_Reg.h"
#include "stm32f10x_Init.h"
#include "DataProces.h"




/*
����0��OK
����1��	  ֵ�����⣬�����붼Ϊ'0'~'9'
*/
uchar  AsciiToHEX(uchar len,uchar *buf,long *OutP)
{
uchar i=0,k=0;
ulong oD=0;
	
if(len==0) {*OutP=0; return 0;}

if(buf[0]=='-')
{
len--;
i++;
k=1;
}
else if(buf[0]=='+')
{
len--;
i++;
k=0;
}

if(len==0) {*OutP=0; return 0;}

switch(len)
{

case 10: if( (buf[i]<'0')||(buf[i]>'9') ) return 1; else{ oD+=(1000000000*(buf[i]-'0')); i++; }
case 9: if( (buf[i]<'0')||(buf[i]>'9') ) return 1; else { oD+=(100000000*(buf[i]-'0')); i++; }
case 8: if( (buf[i]<'0')||(buf[i]>'9') ) return 1; else { oD+=(10000000*(buf[i]-'0')); i++; }
case 7: if( (buf[i]<'0')||(buf[i]>'9') ) return 1; else { oD+=(1000000*(buf[i]-'0')); i++; }
case 6: if( (buf[i]<'0')||(buf[i]>'9') ) return 1; else { oD+=(100000*(buf[i]-'0')); i++; }
case 5: if( (buf[i]<'0')||(buf[i]>'9') ) return 1; else { oD+=(10000*(buf[i]-'0')); i++; }
case 4: if( (buf[i]<'0')||(buf[i]>'9') ) return 1; else { oD+=(1000*(buf[i]-'0')); i++; }
case 3: if( (buf[i]<'0')||(buf[i]>'9') ) return 1; else { oD+=(100*(buf[i]-'0')); i++; }
case 2: if( (buf[i]<'0')||(buf[i]>'9') ) return 1; else { oD+=(10*(buf[i]-'0')); i++; }
case 1: if( (buf[i]<'0')||(buf[i]>'9') ) return 1; else { oD+=((buf[i]-'0')); i++; } break;

default : return 1;
}

if(oD>2147483647) return 1;//������Χ

if(k)
{
*OutP= (~oD) +1 ;
}
else
{
*OutP=oD;
}

return 0;
}



void HEXtoBCD(ulong hexdata,uchar *BCD_dat)   //���ڣ�0x12345...
{
usint hexdat_usint;

  	BCD_dat[0]=0;
	
	while(hexdata>999999999)   //10
	{
	hexdata-=1000000000;
	BCD_dat[0]+=0x10;
	} 	

	while(hexdata>99999999)	   //9
	{
	hexdata-=100000000;
	BCD_dat[0]++;
	} 
		
  	BCD_dat[1]=0;
	while(hexdata>9999999)	   // 8
	{
	hexdata-=10000000;
	BCD_dat[1]+=0x10;
	}		
				  
	while(hexdata>999999)     // 7
	{
	hexdata-=1000000;
	BCD_dat[1]++;
	}

	BCD_dat[2]=0;
	while(hexdata>99999)	   // 6
	{
	hexdata-=100000;
	BCD_dat[2]+=0x10;;
	}


	while(hexdata>9999)	   //5
	{
	hexdata-=10000;
	BCD_dat[2]++;
	}

	hexdat_usint= hexdata;
    BCD_dat[3] = (hexdat_usint/1000)<<4;  //4
	hexdat_usint= hexdat_usint%1000;

	BCD_dat[3] += (hexdat_usint/100);  //3
 	hexdat_usint= hexdat_usint%100;

	BCD_dat[4] = (hexdat_usint/10)<<4;    //2
    BCD_dat[4]+= hexdat_usint%10;     //1
}




/*
�԰��ֽ�(4BIT)Ϊ��λ����ʾ��ַpath����ǰ4BIT��halfSTA=0�����4BIT��halfSTA=1����ʼ����
�ķ�0��num�����֣�

�� 00123 �򱣴�123

������Ҫ�ķ�0�ĸ���
OutP����ת�������ķ�0����
*/

uchar lcd_hide0(uchar *path,uchar halfSTA,uchar num,uchar *OutP)
{
uchar turn=0;
uchar CNT=0;
uchar add=0;
	
if(halfSTA==0)	turn=0;else turn=1;
//if(1==0)

CNT=path[0];
CNT=path[1];
CNT=path[2];
CNT=path[3];
CNT=path[4];
CNT=0;


for(;num>0;num--)  //����һ����0������
{
	if(turn==0)
	{
	if((path[add]>>4)!=0) break;
	turn=1;
	}
	else
	{
	if( (path[add]&0x0f) !=0) break;
	turn=0; 	add++;
	}
}

	for(;num>0;num--)
	{
		if(turn==0)
		{
		OutP[CNT]=(path[add]>>4)+0x30;
		CNT++; 
		turn=1;
		}
		else
		{
		OutP[CNT]=(path[add]&0x0f)+0x30;
		CNT++;
		turn=0;	add++;
		}		
		
		
	}

if(CNT==0)
{
CNT=1;
OutP[0]	='0';
}
return CNT;
}




//-------------------------------���Ƿָ��ߣ������׵�---------------------------

/*
ƽ���˲�
*/
uchar Mean_Filt_uchar(uchar d,uchar * Filt_FIFO,uchar Mean_Num)
{
uchar i;
usint sum=0;

for(i=0;i<(Mean_Num-1);i++)
Filt_FIFO[i]=Filt_FIFO[i+1];

Filt_FIFO[Mean_Num-1]=d;//���µ����ݷ��õ� ���ݵ������

for(i=0;i<Mean_Num;i++)	sum+=Filt_FIFO[i];	  
return (sum/Mean_Num);
}

usint Mean_Filt_usint(usint d,usint * Filt_FIFO,uchar Mean_Num)
{
uchar i;
ulong sum=0;

for(i=0;i<(Mean_Num-1);i++)
Filt_FIFO[i]=Filt_FIFO[i+1];

Filt_FIFO[Mean_Num-1]=d;//���µ����ݷ��õ� ���ݵ������

for(i=0;i<Mean_Num;i++)	sum+=Filt_FIFO[i];	  
return (sum/Mean_Num);
}

sint Mean_Filt_int(sint d,sint * Filt_FIFO,uchar Mean_Num)
{
uchar i;
long sum=0;

for(i=0;i<(Mean_Num-1);i++)
Filt_FIFO[i]=Filt_FIFO[i+1];

Filt_FIFO[Mean_Num-1]=d;//���µ����ݷ��õ� ���ݵ������

for(i=0;i<Mean_Num;i++)	sum+=Filt_FIFO[i];	  
return (sum/Mean_Num);
}


/*----ð���㷨-------
path:����ָ��
LEN: �������е�ǰLEN��ð������
���ڣ� ���к÷���ԭ��������,path[0]��С��path[LEN-1]���
----------------------*/
void bubbling(uchar *path,uchar LEN)
{
uchar i,k,j;
uchar temp;
k=LEN-1;
for(j=0;j<k;j++)
{
	LEN--;
	for(i=0;i<LEN;i++)
	{
		if(path[i]>path[i+1])   //���������λ��
		{
		temp=path[i];
		path[i]=path[i+1];
		path[i+1]=temp;
		}
	}

}
}

void bubbling_long(long *path,uchar LEN)
{
uchar i,k,j;
long temp=0;
k=LEN-1;
for(j=0;j<k;j++)
{
	LEN--;
	for(i=0;i<LEN;i++)
	{
		if(path[i]>path[i+1])   //���������λ��
		{
		temp=path[i];
		path[i]=path[i+1];
		path[i+1]=temp;
		}
	}
}
}

void bubbling_int(int *path,uchar LEN)
{
uchar i,k,j;
int temp=0;
k=LEN-1;
for(j=0;j<k;j++)
{
	LEN--;
	for(i=0;i<LEN;i++)
	{
		if(path[i]>path[i+1])   //���������λ��
		{
		temp=path[i];
		path[i]=path[i+1];
		path[i+1]=temp;
		}
	}
}
}

/*
FOFO���ܺ���--
buf[LEN-1] ���Ǳ������µ�����
*/
void FIFO_uchar(uchar FD,uchar *buf,uchar LEN)
{
uchar i;
for(i=0;i<(LEN-1);i++)
buf[i]=buf[i+1];
buf[i]=FD;
}


void FIFO_int(int FD,int *buf,uchar LEN)
{
uchar i;
for(i=0;i<(LEN-1);i++)
buf[i]=buf[i+1];
buf[i]=FD;
}

void FIFO_usint(usint FD,usint *buf,uchar LEN)
{
uchar i;
for(i=0;i<(LEN-1);i++)
buf[i]=buf[i+1];
buf[i]=FD;
}



svoid FIFO_long(long FD,long *buf,uchar LEN)
{
uchar i;
for(i=0;i<(LEN-1);i++)
buf[i]=buf[i+1];
buf[i]=FD;
}












