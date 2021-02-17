#include "STM32_I2C.H"

ulong I2C_Dtime=0;


/*
功能：I2C专属延时。入口为I2C_Dtime
*/
void I2C_delay(ulong DLAtime)
{
ulong j;
uchar i;
for(i=0;i<5;i++)
for(j=0;j<DLAtime;j++);

}


/*
IIC初始化函数
入口：freq,频率，单位为KHZ
出口：无
*/
void I2C_Init(usint freq)
{
//-------计算延时因子-------
I2C_Dtime=4000/freq;  //为半周期延时时间，单位0.125US
I2C_SCL_outmode;
I2C_SDA_outmode;
I2C_SCL_setb;
I2C_delay(10);
I2C_SDA_setb;
I2C_delay(10);
}


/*
功能：发出起始信号！
返回：
0：结束信号发送OK
1：结束信号发送失败.一般是由从机拉低了SDA线造成
*/
uchar I2C_start(void)
{
uchar k=0;
ulong j=0;
I2C_SCL_clr; 
I2C_SDA_setb; I2C_delay(I2C_Dtime/2);
I2C_SCL_setb; I2C_delay(I2C_Dtime/2);
do{k=I2C_SCL_input;if(j++>200000) return(1);}while(k==0); //起始信号失败，超时！
I2C_delay(I2C_Dtime/2);
I2C_SDA_clr; I2C_delay(I2C_Dtime);
return(0);
}
	 
/*
功能：发出结束信号！
返回：
0：结束信号发送OK
1：结束信号发送失败.一般是由从机拉低了SDA线造成
*/
uchar I2C_stop(void)
{
uchar k=0;
ulong j=0;
I2C_SCL_clr; I2C_delay(I2C_Dtime/2);
I2C_SDA_clr; I2C_delay(I2C_Dtime/2);
I2C_SCL_setb; I2C_delay(I2C_Dtime/2);
do{k=I2C_SCL_input;if(j++>200000) return(1);}while(k==0); //停止信号失败，超时！
I2C_delay(I2C_Dtime);
I2C_SDA_setb; I2C_delay(I2C_Dtime);
do{k=I2C_SDA_input;if(j++>200000) return(1);}while(k==0); //停止信号失败，超时！
return(0);
}

/*
功能：读ACK信号，带滤波
返回：
0：ACK=0
1：ACK=1
0xff//读ACK信号失败，超时！
*/
uchar I2C_RACK(void)
{
uchar buf[5];
uchar i=0,k=0;
ulong j=0;
I2C_SCL_clr; I2C_delay(I2C_Dtime);
I2C_SDA_setb; I2C_delay(I2C_Dtime/2); //释放总线
I2C_SCL_setb; I2C_delay(I2C_Dtime/3);
do{k=I2C_SCL_input;if(j++>200000) return(0xff);}while(k==0); //读ACK信号失败，超时！
I2C_delay(I2C_Dtime/3);

for(i=0;i<5;i++) buf[i]=I2C_SDA_input; 
k=0;
for(i=0;i<5;i++) {if(buf[i]!=0)k++; if(k>3) return(1);}

I2C_delay(I2C_Dtime/3);
return(0);
}

/*
功能：写ACK信号
入口：
0：ACK=0
1：ACK=1
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
功能：向I2C总线写8位数据，高位先发，SCL低期间改变数据！ 
入口：要写的字节
出口
0： 写OK，不包含应答！
1： 从机拉低SCL信号线希望延时，但超时了！
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
	//此时从机应释放时钟总线。若不释放则等待其释放
	do{k=I2C_SCL_input;if(j++>1000000) return(1);}while(k==0);
	I2C_delay(I2C_Dtime/2);
	}
	return(0);
}
/*
功能：从I2C总线读8位数据，高位先读，SCL高期间读数据！ 
入口：无
出口：所读到的数据！
*/
uchar I2C_Rbyte(void)
{
uchar i;
uchar temp=0;
uchar k=0;
ulong j=0;

I2C_SDA_setb;  //释放总线
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
功能：
读一个字节到*Rdat，注意：ACK有时应该为0！
读流程为：
0：START信号
1：写Chip地址+写信号
2：写要读寄存器的地址
3：重复START信号
4：写Chip地址+读信号
5：读出数据
6：STOP信号
----------------------------------
入口：
ACK：主机度数据后发送的应答，有时应该为0！
Chip_Adds：片选地址
Reg_Adds：要读寄存器的地址
*Rdat： 数据存放的指针

出口：
0： OK
1，6:           Start信号无效
2，4，7：从机拉低SCL信号线以延时，但超时！
3，5，8：从机发送的应答无效，即不是0！
9:WACK;	失败
10：             STOP发送无效！
----------------------------------*/
uchar I2C_ReceByte(uchar ACK,uchar Chip_Adds,uchar Reg_Adds,uchar *Rdat)
{
if(I2C_start())  					 return(1);
if(I2C_Wbyte(Chip_Adds<<1))          return(2); //从机拉低SCL信号线以延时，但超时！
if(I2C_RACK())				         return(3); //应答无效
if(I2C_Wbyte(Reg_Adds))	 		     return(4); //从机拉低SCL信号线以延时，但超时！
if(I2C_RACK())		         	   	 return(5); //应答无效

if(I2C_start())  					 return(6);							
if(I2C_Wbyte((Chip_Adds<<1)+1))      return(7); //从机拉低SCL信号线以延时，但超时！
if(I2C_RACK())				         return(8); //应答无效
*Rdat=I2C_Rbyte();
if(I2C_WACK(ACK))					 return(9);
if(I2C_stop())				         return(10); //SOTP信号发送无效
return(0);
}

uchar I2C_ReceByte_16ADD(uchar ACK,uchar Chip_Adds,usint Reg_Adds,uchar *Rdat)
{
if(I2C_start())  					 return(1);
if(I2C_Wbyte(Chip_Adds<<1))          return(2); //从机拉低SCL信号线以延时，但超时！
if(I2C_RACK())				         return(3); //应答无效
if(I2C_Wbyte(Reg_Adds>>8))	 		 return(4); //从机拉低SCL信号线以延时，但超时！
if(I2C_RACK()!=0)		     	   	 return(5); //应答无效
if(I2C_Wbyte(Reg_Adds))   			 return(6); //从机拉低SCL信号线以延时，但超时！
if(I2C_RACK()!=0)		     		 return(7); //应答无效

if(I2C_start())  					 return(8);							
if(I2C_Wbyte((Chip_Adds<<1)+1))      return(9); //从机拉低SCL信号线以延时，但超时！
if(I2C_RACK())				         return(10); //应答无效
*Rdat=I2C_Rbyte();
if(I2C_WACK(ACK))					 return(11);
if(I2C_stop())				         return(12); //SOTP信号发送无效
return(0);
}

/*----------------------------------
功能：
连续读出数据到*BUF，地址范围，注意发出ACK的变化！最后一个字节的ACK发送是1!

读流程为：
0：START信号
1：写Chip地址+写信号
2：写要读寄存器的地址
3：重复START信号
4：写Chip地址+读信号
5：读出数据+ADC=0;  循环R_LEN-1次
6：读出数据+ADC=1; 最后一个字节响应"NO ACK"
7：STOP信号
----------------------------------
入口：
Chip_Adds :片选地址
Reg_Adds:  要读寄存器的地址
R_LEN：     读长度
*buf：        数据存放的指针

出口：
0： OK
1，6:           Start信号无效
2，4，7：从机拉低SCL信号线以延时，但超时！
3，5，8：从机发送的应答无效，即不是0！
9: WACK失败
10：             STOP发送无效！
----------------------------------*/
uchar I2C_ReceString(uchar Chip_Adds,uchar Reg_Adds,usint R_LEN,uchar *buf)
{
usint i=0;

if(I2C_start())  					  return(1); //Start信号无
if(I2C_Wbyte(Chip_Adds<<1))     return(2); //从机拉低SCL信号线以延时，但超时！
if(I2C_RACK())				          return(3); //应答无效
if(I2C_Wbyte(Reg_Adds)) 	      return(4); //从机拉低SCL信号线以延时，但超时！
if(I2C_RACK()!=0) 				  return(5); //应答无效
if(I2C_start())  					  return(6);	 //Start信号无					
if(I2C_Wbyte((Chip_Adds<<1)+1))return(7); //从机拉低SCL信号线以延时，但超时！
if(I2C_RACK())				          return(8); //应答无效
R_LEN=R_LEN-1;
for(i=0;i<R_LEN;i++)
{*(buf+i)=I2C_Rbyte();if(I2C_WACK(0)) return(9);}

*(buf+R_LEN)=I2C_Rbyte();
if(I2C_WACK(1)) return(9);

if(I2C_stop())	return(10); //SOTP信号发送无效
return(0);
}

uchar I2C_ReceString_16ADD(uchar Chip_Adds,usint Reg_Adds,usint R_LEN,uchar *buf)
{
usint i=0;

if(I2C_start())  				return(1); //Start信号无
if(I2C_Wbyte(Chip_Adds<<1))     return(2); //从机拉低SCL信号线以延时，但超时！
if(I2C_RACK())				    return(3); //应答无效
if(I2C_Wbyte(Reg_Adds>>8)) 	      return(4); //从机拉低SCL信号线以延时，但超时！
if(I2C_RACK()!=0) 				  return(5); //应答无效
if(I2C_Wbyte(Reg_Adds)) 	      return(4); //从机拉低SCL信号线以延时，但超时！
if(I2C_RACK()!=0) 				  return(5); //应答无效

if(I2C_start())  					  return(6);	 //Start信号无					
if(I2C_Wbyte((Chip_Adds<<1)+1))       return(7); //从机拉低SCL信号线以延时，但超时！
if(I2C_RACK())				          return(8); //应答无效
R_LEN--;
for(i=0;i<R_LEN;i++)
{buf[i]=I2C_Rbyte();if(I2C_WACK(0)) return(9);}

*(buf+R_LEN)=I2C_Rbyte();
if(I2C_WACK(1)) return(9);

if(I2C_stop())	return(10); //SOTP信号发送无效
return(0);
}
/*
功能：发送Wdat到I2C
写流程为：
0：START信号
1：写Chip地址+写信号
2：写要写寄存器的地址
3：写要写寄存器的内容
4：STOP信号
----------------------------------
入口：
Chip_Adds :片选地址
Reg_Adds:  要写寄存器的地址
Wdat：      要写寄存器的内容

出口：
0： OK
1:          Start信号无效
2，4，6：从机拉低SCL信号线以延时，但超时！
3，5，7：从机发送的应答无效，即不是0！
8：         STOP发送无效！
----------------------------------*/
uchar I2C_SendByte(uchar Chip_Adds,uchar Reg_Adds,uchar Wdat)
{
if(I2C_start())				 return(1);  //Start信号无
if(I2C_Wbyte(Chip_Adds<<1))  return(2); //从机拉低SCL信号线以延时，但超时！
if(I2C_RACK()!=0)		 	 return(3); //应答无效
if(I2C_Wbyte(Reg_Adds))	   	 return(4); //从机拉低SCL信号线以延时，但超时！
if(I2C_RACK()!=0)		     return(5); //应答无效
if(I2C_Wbyte(Wdat))		  	 return(6); //从机拉低SCL信号线以延时，但超时！
if(I2C_RACK()!=0) 		  	 return(7); //应答无效
if(I2C_stop())				 return(8); //Stop信号无
return(0);
}

uchar I2C_SendByte_16ADD(uchar Chip_Adds,usint Reg_Adds,uchar Wdat)
{
if(I2C_start())				 				return(1);  //Start信号无
if(I2C_Wbyte(Chip_Adds<<1))  return(2); //从机拉低SCL信号线以延时，但超时！
if(I2C_RACK()!=0)		 	 				return(3); //应答无效
if(I2C_Wbyte(Reg_Adds>>8))	 return(4); //从机拉低SCL信号线以延时，但超时！
if(I2C_RACK()!=0)		     			return(5); //应答无效
if(I2C_Wbyte(Reg_Adds))   	 return(6); //从机拉低SCL信号线以延时，但超时！
if(I2C_RACK()!=0)		     			return(7); //应答无效

if(I2C_Wbyte(Wdat))		  	 	return(8); //从机拉低SCL信号线以延时，但超时！
if(I2C_RACK()!=0) 		  	 	return(9); //应答无效
if(I2C_stop())				 			return(10); //Stop信号无
return(0);
}
/*--------------------------------
功能：发送Wdat到I2C
写流程为：
0：START信号
1：写Chip地址+写信号
2：写要写寄存器的地址
3：写要写寄存器的内容
4：STOP信号
----------------------------------
入口：
Chip_Adds :片选地址
Reg_Adds:  要写寄存器的地址
Wdat：      要写寄存器的内容
出口：
0： OK
1:        Start信号无效
2，4，6：从机拉低SCL信号线以延时，但超时！
3，5：   从机发送的应答无效，即不是0！
7,8：    STOP发送无效！
----------------------------------*/
uchar I2C_SendString(uchar Chip_Adds,uchar Reg_Adds,uchar R_LEN,uchar *buf)
{
uchar i=0;
if(I2C_start())				 return(1);  //Start信号无
if(I2C_Wbyte(Chip_Adds<<1))  return(2); //从机拉低SCL信号线以延时，但超时！
if(I2C_RACK()!=0)		 	 return(3); //应答无效
if(I2C_Wbyte(Reg_Adds))	   	 return(4); //从机拉低SCL信号线以延时，但超时！
if(I2C_RACK()!=0)		     return(5); //应答无效

for(i=0;i<R_LEN;i++)
{
	if(I2C_Wbyte(buf[i]))      return(6); //从机拉低SCL信号线以延时，但超时！
	if(I2C_RACK()!=0)		
	{if(I2C_stop()) return(7); return(0);}   //Stop信号无							
}

if(I2C_stop())				 return(8); //Stop信号无
return(0);
}


uchar I2C_SendString_full(uchar Chip_Adds,uchar Reg_Adds,uchar R_LEN,uchar iD)
{
uchar i=0;
if(I2C_start())				 return(1);  //Start信号无
if(I2C_Wbyte(Chip_Adds<<1))  return(2); //从机拉低SCL信号线以延时，但超时！
if(I2C_RACK()!=0)		 	 return(3); //应答无效
if(I2C_Wbyte(Reg_Adds))	   	 return(4); //从机拉低SCL信号线以延时，但超时！
if(I2C_RACK()!=0)		     return(5); //应答无效

for(i=0;i<R_LEN;i++)
{
	if(I2C_Wbyte(iD))      return(6); //从机拉低SCL信号线以延时，但超时！
	if(I2C_RACK()!=0)		
	{if(I2C_stop()) return(7); return(0);}   //Stop信号无							
}

if(I2C_stop())				 return(8); //Stop信号无
return(0);
}

uchar I2C_CheckBusy(uchar Chip_Adds) //返回0则空闲，1正在写
{
if(I2C_start())				 return(1);  //Start信号无
if(I2C_Wbyte(Chip_Adds<<1))  return(2); //从机拉低SCL信号线以延时，但超时！
if(I2C_RACK()!=0)		 	 return(3); //应答无效
return(0);
}

uchar I2C_SendString_16ADD(uchar Chip_Adds,usint Reg_Adds,usint R_LEN,uchar *buf)
{
usint i=0;
if(I2C_start())				 return(1); //Start信号无
if(I2C_Wbyte(Chip_Adds<<1))  return(2); //从机拉低SCL信号线以延时，但超时！
if(I2C_RACK()!=0)		 	 return(3); //应答无效
if(I2C_Wbyte(Reg_Adds>>8))	 return(4); //从机拉低SCL信号线以延时，但超时！
if(I2C_RACK()!=0)		     return(5); //应答无效
if(I2C_Wbyte(Reg_Adds))	   	 return(4); //从机拉低SCL信号线以延时，但超时！
if(I2C_RACK()!=0)		     return(5); //应答无效

for(i=0;i<R_LEN;i++)
{
	if(I2C_Wbyte(buf[i]))      return(6); //从机拉低SCL信号线以延时，但超时！
	if(I2C_RACK()!=0)		
	{if(I2C_stop()) return(7); return(8);}   //Stop信号无							
}

if(I2C_stop())				 return(9); //Stop信号无
return(0);
}


