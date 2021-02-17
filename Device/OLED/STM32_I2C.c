#include "STM32_I2C.H"

ulong I2C_Dtime=0;


/*
���ܣ�I2Cר����ʱ�����ΪI2C_Dtime
*/
void I2C_delay(ulong DLAtime)
{
ulong j;
uchar i;
for(i=0;i<5;i++)
for(j=0;j<DLAtime;j++);

}


/*
IIC��ʼ������
��ڣ�freq,Ƶ�ʣ���λΪKHZ
���ڣ���
*/
void I2C_Init(usint freq)
{
//-------������ʱ����-------
I2C_Dtime=4000/freq;  //Ϊ��������ʱʱ�䣬��λ0.125US
I2C_SCL_outmode;
I2C_SDA_outmode;
I2C_SCL_setb;
I2C_delay(10);
I2C_SDA_setb;
I2C_delay(10);
}


/*
���ܣ�������ʼ�źţ�
���أ�
0�������źŷ���OK
1�������źŷ���ʧ��.һ�����ɴӻ�������SDA�����
*/
uchar I2C_start(void)
{
uchar k=0;
ulong j=0;
I2C_SCL_clr; 
I2C_SDA_setb; I2C_delay(I2C_Dtime/2);
I2C_SCL_setb; I2C_delay(I2C_Dtime/2);
do{k=I2C_SCL_input;if(j++>200000) return(1);}while(k==0); //��ʼ�ź�ʧ�ܣ���ʱ��
I2C_delay(I2C_Dtime/2);
I2C_SDA_clr; I2C_delay(I2C_Dtime);
return(0);
}
	 
/*
���ܣ����������źţ�
���أ�
0�������źŷ���OK
1�������źŷ���ʧ��.һ�����ɴӻ�������SDA�����
*/
uchar I2C_stop(void)
{
uchar k=0;
ulong j=0;
I2C_SCL_clr; I2C_delay(I2C_Dtime/2);
I2C_SDA_clr; I2C_delay(I2C_Dtime/2);
I2C_SCL_setb; I2C_delay(I2C_Dtime/2);
do{k=I2C_SCL_input;if(j++>200000) return(1);}while(k==0); //ֹͣ�ź�ʧ�ܣ���ʱ��
I2C_delay(I2C_Dtime);
I2C_SDA_setb; I2C_delay(I2C_Dtime);
do{k=I2C_SDA_input;if(j++>200000) return(1);}while(k==0); //ֹͣ�ź�ʧ�ܣ���ʱ��
return(0);
}

/*
���ܣ���ACK�źţ����˲�
���أ�
0��ACK=0
1��ACK=1
0xff//��ACK�ź�ʧ�ܣ���ʱ��
*/
uchar I2C_RACK(void)
{
uchar buf[5];
uchar i=0,k=0;
ulong j=0;
I2C_SCL_clr; I2C_delay(I2C_Dtime);
I2C_SDA_setb; I2C_delay(I2C_Dtime/2); //�ͷ�����
I2C_SCL_setb; I2C_delay(I2C_Dtime/3);
do{k=I2C_SCL_input;if(j++>200000) return(0xff);}while(k==0); //��ACK�ź�ʧ�ܣ���ʱ��
I2C_delay(I2C_Dtime/3);

for(i=0;i<5;i++) buf[i]=I2C_SDA_input; 
k=0;
for(i=0;i<5;i++) {if(buf[i]!=0)k++; if(k>3) return(1);}

I2C_delay(I2C_Dtime/3);
return(0);
}

/*
���ܣ�дACK�ź�
��ڣ�
0��ACK=0
1��ACK=1
*/
uchar I2C_WACK(uchar ACK)
{
uchar k=0;
ulong j=0;
I2C_SCL_clr;   I2C_delay(I2C_Dtime/2);

if(ACK)
I2C_SDA_setb;
else
I2C_SDA_clr;	
	
I2C_delay(I2C_Dtime/2);
I2C_SCL_setb;   I2C_delay(I2C_Dtime/2);
do{k=I2C_SCL_input;if(j++>1000000) return(1);}while(k==0);
I2C_delay(I2C_Dtime);

//-----------------  
I2C_SCL_clr;   I2C_delay(I2C_Dtime/4);
I2C_SDA_setb; 
return(0);
}

/*
���ܣ���I2C����д8λ���ݣ���λ�ȷ���SCL���ڼ�ı����ݣ� 
��ڣ�Ҫд���ֽ�
����
0�� дOK��������Ӧ��
1�� �ӻ�����SCL�ź���ϣ����ʱ������ʱ�ˣ�
*/
uchar I2C_Wbyte(uchar Wdat)
{
ulong j=0;
uchar k=0;
uchar i=0;
	for(i=0;i<8;i++) 
	{
	I2C_SCL_clr;
	I2C_delay(I2C_Dtime/2);
		
	if((Wdat&(1<<(7-i)))?(1):(0))
	I2C_SDA_setb;
	else
	I2C_SDA_clr;		

	
	I2C_delay(I2C_Dtime/2);
	I2C_SCL_setb;  I2C_delay(I2C_Dtime/2);
	//��ʱ�ӻ�Ӧ�ͷ�ʱ�����ߡ������ͷ���ȴ����ͷ�
	do{k=I2C_SCL_input;if(j++>1000000) return(1);}while(k==0);
	I2C_delay(I2C_Dtime/2);
	}
	return(0);
}
/*
���ܣ���I2C���߶�8λ���ݣ���λ�ȶ���SCL���ڼ�����ݣ� 
��ڣ���
���ڣ������������ݣ�
*/
uchar I2C_Rbyte(void)
{
uchar i;
uchar temp=0;
uchar k=0;
ulong j=0;

I2C_SDA_setb;  //�ͷ�����
	for(i=0;i<8;i++)
	{	
	I2C_SCL_clr;  I2C_delay(I2C_Dtime);
	I2C_SCL_setb;  I2C_delay(I2C_Dtime/2);
	do{k=I2C_SCL_input;if(j++>1000000) return(0xee);}while(k==0);
	I2C_delay(I2C_Dtime/2);
	k=I2C_SDA_input;
	temp|=((k)?(1):(0))<<(7-i);
	I2C_delay(I2C_Dtime/2);
	}
return(temp);	
}


/*----------------------------------
���ܣ�
��һ���ֽڵ�*Rdat��ע�⣺ACK��ʱӦ��Ϊ0��
������Ϊ��
0��START�ź�
1��дChip��ַ+д�ź�
2��дҪ���Ĵ����ĵ�ַ
3���ظ�START�ź�
4��дChip��ַ+���ź�
5����������
6��STOP�ź�
----------------------------------
��ڣ�
ACK�����������ݺ��͵�Ӧ����ʱӦ��Ϊ0��
Chip_Adds��Ƭѡ��ַ
Reg_Adds��Ҫ���Ĵ����ĵ�ַ
*Rdat�� ���ݴ�ŵ�ָ��

���ڣ�
0�� OK
1��6:           Start�ź���Ч
2��4��7���ӻ�����SCL�ź�������ʱ������ʱ��
3��5��8���ӻ����͵�Ӧ����Ч��������0��
9:WACK;	ʧ��
10��             STOP������Ч��
----------------------------------*/
uchar I2C_ReceByte(uchar ACK,uchar Chip_Adds,uchar Reg_Adds,uchar *Rdat)
{
if(I2C_start())  					 return(1);
if(I2C_Wbyte(Chip_Adds<<1))          return(2); //�ӻ�����SCL�ź�������ʱ������ʱ��
if(I2C_RACK())				         return(3); //Ӧ����Ч
if(I2C_Wbyte(Reg_Adds))	 		     return(4); //�ӻ�����SCL�ź�������ʱ������ʱ��
if(I2C_RACK())		         	   	 return(5); //Ӧ����Ч

if(I2C_start())  					 return(6);							
if(I2C_Wbyte((Chip_Adds<<1)+1))      return(7); //�ӻ�����SCL�ź�������ʱ������ʱ��
if(I2C_RACK())				         return(8); //Ӧ����Ч
*Rdat=I2C_Rbyte();
if(I2C_WACK(ACK))					 return(9);
if(I2C_stop())				         return(10); //SOTP�źŷ�����Ч
return(0);
}

uchar I2C_ReceByte_16ADD(uchar ACK,uchar Chip_Adds,usint Reg_Adds,uchar *Rdat)
{
if(I2C_start())  					 return(1);
if(I2C_Wbyte(Chip_Adds<<1))          return(2); //�ӻ�����SCL�ź�������ʱ������ʱ��
if(I2C_RACK())				         return(3); //Ӧ����Ч
if(I2C_Wbyte(Reg_Adds>>8))	 		 return(4); //�ӻ�����SCL�ź�������ʱ������ʱ��
if(I2C_RACK()!=0)		     	   	 return(5); //Ӧ����Ч
if(I2C_Wbyte(Reg_Adds))   			 return(6); //�ӻ�����SCL�ź�������ʱ������ʱ��
if(I2C_RACK()!=0)		     		 return(7); //Ӧ����Ч

if(I2C_start())  					 return(8);							
if(I2C_Wbyte((Chip_Adds<<1)+1))      return(9); //�ӻ�����SCL�ź�������ʱ������ʱ��
if(I2C_RACK())				         return(10); //Ӧ����Ч
*Rdat=I2C_Rbyte();
if(I2C_WACK(ACK))					 return(11);
if(I2C_stop())				         return(12); //SOTP�źŷ�����Ч
return(0);
}

/*----------------------------------
���ܣ�
�����������ݵ�*BUF����ַ��Χ��ע�ⷢ��ACK�ı仯�����һ���ֽڵ�ACK������1!

������Ϊ��
0��START�ź�
1��дChip��ַ+д�ź�
2��дҪ���Ĵ����ĵ�ַ
3���ظ�START�ź�
4��дChip��ַ+���ź�
5����������+ADC=0;  ѭ��R_LEN-1��
6����������+ADC=1; ���һ���ֽ���Ӧ"NO ACK"
7��STOP�ź�
----------------------------------
��ڣ�
Chip_Adds :Ƭѡ��ַ
Reg_Adds:  Ҫ���Ĵ����ĵ�ַ
R_LEN��     ������
*buf��        ���ݴ�ŵ�ָ��

���ڣ�
0�� OK
1��6:           Start�ź���Ч
2��4��7���ӻ�����SCL�ź�������ʱ������ʱ��
3��5��8���ӻ����͵�Ӧ����Ч��������0��
9: WACKʧ��
10��             STOP������Ч��
----------------------------------*/
uchar I2C_ReceString(uchar Chip_Adds,uchar Reg_Adds,usint R_LEN,uchar *buf)
{
usint i=0;

if(I2C_start())  					  return(1); //Start�ź���
if(I2C_Wbyte(Chip_Adds<<1))     return(2); //�ӻ�����SCL�ź�������ʱ������ʱ��
if(I2C_RACK())				          return(3); //Ӧ����Ч
if(I2C_Wbyte(Reg_Adds)) 	      return(4); //�ӻ�����SCL�ź�������ʱ������ʱ��
if(I2C_RACK()!=0) 				  return(5); //Ӧ����Ч
if(I2C_start())  					  return(6);	 //Start�ź���					
if(I2C_Wbyte((Chip_Adds<<1)+1))return(7); //�ӻ�����SCL�ź�������ʱ������ʱ��
if(I2C_RACK())				          return(8); //Ӧ����Ч
R_LEN=R_LEN-1;
for(i=0;i<R_LEN;i++)
{*(buf+i)=I2C_Rbyte();if(I2C_WACK(0)) return(9);}

*(buf+R_LEN)=I2C_Rbyte();
if(I2C_WACK(1)) return(9);

if(I2C_stop())	return(10); //SOTP�źŷ�����Ч
return(0);
}

uchar I2C_ReceString_16ADD(uchar Chip_Adds,usint Reg_Adds,usint R_LEN,uchar *buf)
{
usint i=0;

if(I2C_start())  				return(1); //Start�ź���
if(I2C_Wbyte(Chip_Adds<<1))     return(2); //�ӻ�����SCL�ź�������ʱ������ʱ��
if(I2C_RACK())				    return(3); //Ӧ����Ч
if(I2C_Wbyte(Reg_Adds>>8)) 	      return(4); //�ӻ�����SCL�ź�������ʱ������ʱ��
if(I2C_RACK()!=0) 				  return(5); //Ӧ����Ч
if(I2C_Wbyte(Reg_Adds)) 	      return(4); //�ӻ�����SCL�ź�������ʱ������ʱ��
if(I2C_RACK()!=0) 				  return(5); //Ӧ����Ч

if(I2C_start())  					  return(6);	 //Start�ź���					
if(I2C_Wbyte((Chip_Adds<<1)+1))       return(7); //�ӻ�����SCL�ź�������ʱ������ʱ��
if(I2C_RACK())				          return(8); //Ӧ����Ч
R_LEN--;
for(i=0;i<R_LEN;i++)
{buf[i]=I2C_Rbyte();if(I2C_WACK(0)) return(9);}

*(buf+R_LEN)=I2C_Rbyte();
if(I2C_WACK(1)) return(9);

if(I2C_stop())	return(10); //SOTP�źŷ�����Ч
return(0);
}
/*
���ܣ�����Wdat��I2C
д����Ϊ��
0��START�ź�
1��дChip��ַ+д�ź�
2��дҪд�Ĵ����ĵ�ַ
3��дҪд�Ĵ���������
4��STOP�ź�
----------------------------------
��ڣ�
Chip_Adds :Ƭѡ��ַ
Reg_Adds:  Ҫд�Ĵ����ĵ�ַ
Wdat��      Ҫд�Ĵ���������

���ڣ�
0�� OK
1:          Start�ź���Ч
2��4��6���ӻ�����SCL�ź�������ʱ������ʱ��
3��5��7���ӻ����͵�Ӧ����Ч��������0��
8��         STOP������Ч��
----------------------------------*/
uchar I2C_SendByte(uchar Chip_Adds,uchar Reg_Adds,uchar Wdat)
{
if(I2C_start())				 return(1);  //Start�ź���
if(I2C_Wbyte(Chip_Adds<<1))  return(2); //�ӻ�����SCL�ź�������ʱ������ʱ��
if(I2C_RACK()!=0)		 	 return(3); //Ӧ����Ч
if(I2C_Wbyte(Reg_Adds))	   	 return(4); //�ӻ�����SCL�ź�������ʱ������ʱ��
if(I2C_RACK()!=0)		     return(5); //Ӧ����Ч
if(I2C_Wbyte(Wdat))		  	 return(6); //�ӻ�����SCL�ź�������ʱ������ʱ��
if(I2C_RACK()!=0) 		  	 return(7); //Ӧ����Ч
if(I2C_stop())				 return(8); //Stop�ź���
return(0);
}

uchar I2C_SendByte_16ADD(uchar Chip_Adds,usint Reg_Adds,uchar Wdat)
{
if(I2C_start())				 				return(1);  //Start�ź���
if(I2C_Wbyte(Chip_Adds<<1))  return(2); //�ӻ�����SCL�ź�������ʱ������ʱ��
if(I2C_RACK()!=0)		 	 				return(3); //Ӧ����Ч
if(I2C_Wbyte(Reg_Adds>>8))	 return(4); //�ӻ�����SCL�ź�������ʱ������ʱ��
if(I2C_RACK()!=0)		     			return(5); //Ӧ����Ч
if(I2C_Wbyte(Reg_Adds))   	 return(6); //�ӻ�����SCL�ź�������ʱ������ʱ��
if(I2C_RACK()!=0)		     			return(7); //Ӧ����Ч

if(I2C_Wbyte(Wdat))		  	 	return(8); //�ӻ�����SCL�ź�������ʱ������ʱ��
if(I2C_RACK()!=0) 		  	 	return(9); //Ӧ����Ч
if(I2C_stop())				 			return(10); //Stop�ź���
return(0);
}
/*--------------------------------
���ܣ�����Wdat��I2C
д����Ϊ��
0��START�ź�
1��дChip��ַ+д�ź�
2��дҪд�Ĵ����ĵ�ַ
3��дҪд�Ĵ���������
4��STOP�ź�
----------------------------------
��ڣ�
Chip_Adds :Ƭѡ��ַ
Reg_Adds:  Ҫд�Ĵ����ĵ�ַ
Wdat��      Ҫд�Ĵ���������
���ڣ�
0�� OK
1:        Start�ź���Ч
2��4��6���ӻ�����SCL�ź�������ʱ������ʱ��
3��5��   �ӻ����͵�Ӧ����Ч��������0��
7,8��    STOP������Ч��
----------------------------------*/
uchar I2C_SendString(uchar Chip_Adds,uchar Reg_Adds,uchar R_LEN,uchar *buf)
{
uchar i=0;
if(I2C_start())				 return(1);  //Start�ź���
if(I2C_Wbyte(Chip_Adds<<1))  return(2); //�ӻ�����SCL�ź�������ʱ������ʱ��
if(I2C_RACK()!=0)		 	 return(3); //Ӧ����Ч
if(I2C_Wbyte(Reg_Adds))	   	 return(4); //�ӻ�����SCL�ź�������ʱ������ʱ��
if(I2C_RACK()!=0)		     return(5); //Ӧ����Ч

for(i=0;i<R_LEN;i++)
{
	if(I2C_Wbyte(buf[i]))      return(6); //�ӻ�����SCL�ź�������ʱ������ʱ��
	if(I2C_RACK()!=0)		
	{if(I2C_stop()) return(7); return(0);}   //Stop�ź���							
}

if(I2C_stop())				 return(8); //Stop�ź���
return(0);
}


uchar I2C_SendString_full(uchar Chip_Adds,uchar Reg_Adds,uchar R_LEN,uchar iD)
{
uchar i=0;
if(I2C_start())				 return(1);  //Start�ź���
if(I2C_Wbyte(Chip_Adds<<1))  return(2); //�ӻ�����SCL�ź�������ʱ������ʱ��
if(I2C_RACK()!=0)		 	 return(3); //Ӧ����Ч
if(I2C_Wbyte(Reg_Adds))	   	 return(4); //�ӻ�����SCL�ź�������ʱ������ʱ��
if(I2C_RACK()!=0)		     return(5); //Ӧ����Ч

for(i=0;i<R_LEN;i++)
{
	if(I2C_Wbyte(iD))      return(6); //�ӻ�����SCL�ź�������ʱ������ʱ��
	if(I2C_RACK()!=0)		
	{if(I2C_stop()) return(7); return(0);}   //Stop�ź���							
}

if(I2C_stop())				 return(8); //Stop�ź���
return(0);
}

uchar I2C_CheckBusy(uchar Chip_Adds) //����0����У�1����д
{
if(I2C_start())				 return(1);  //Start�ź���
if(I2C_Wbyte(Chip_Adds<<1))  return(2); //�ӻ�����SCL�ź�������ʱ������ʱ��
if(I2C_RACK()!=0)		 	 return(3); //Ӧ����Ч
return(0);
}

uchar I2C_SendString_16ADD(uchar Chip_Adds,usint Reg_Adds,usint R_LEN,uchar *buf)
{
usint i=0;
if(I2C_start())				 return(1); //Start�ź���
if(I2C_Wbyte(Chip_Adds<<1))  return(2); //�ӻ�����SCL�ź�������ʱ������ʱ��
if(I2C_RACK()!=0)		 	 return(3); //Ӧ����Ч
if(I2C_Wbyte(Reg_Adds>>8))	 return(4); //�ӻ�����SCL�ź�������ʱ������ʱ��
if(I2C_RACK()!=0)		     return(5); //Ӧ����Ч
if(I2C_Wbyte(Reg_Adds))	   	 return(4); //�ӻ�����SCL�ź�������ʱ������ʱ��
if(I2C_RACK()!=0)		     return(5); //Ӧ����Ч

for(i=0;i<R_LEN;i++)
{
	if(I2C_Wbyte(buf[i]))      return(6); //�ӻ�����SCL�ź�������ʱ������ʱ��
	if(I2C_RACK()!=0)		
	{if(I2C_stop()) return(7); return(8);}   //Stop�ź���							
}

if(I2C_stop())				 return(9); //Stop�ź���
return(0);
}


