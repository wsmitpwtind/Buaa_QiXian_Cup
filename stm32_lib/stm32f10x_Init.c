#include 	"stm32f10x_Init.h"
#include	"stm32f10x_Reg.h"
#include	"stm32f10x_type.h"
#include 	"stdio.h"



struct SYSCLK_DAT  SYSCLK_DAT_reg;   //各个总线时钟信息寄存器，全局！！
struct SYSCLK_DAT *SYSCLK_DAT_pat=&SYSCLK_DAT_reg;




//重定义fputc函数 ,用于printf
int fputc(int ch, FILE *f)
{      
	while((USART1_SR&0x40)==0);//循环发送,直到发送完毕   
	USART1_DR=(u8)ch;      
	return ch;
}




//系统默认的函数启动文件中所缺少的时间定义函数--------
int SystemInit(void)
{
return(0);
}



/*****************************************************
注意：只用于外部时钟下PLL的情况
由外时钟，倍频系数，分频系数计算各个设备的时钟
以及对应到相关寄存器的分频系数值！
*****************************************************/
void SYSCLK_Count(void)
{
SYSCLK_DAT_reg.SYSCLK_DAT_SYS=1000000*((ulong)Sys_HSE*(ulong)Sys_PLL);//---系统时钟
//----AHB相关计算---

switch(Sys_DIV_AHB)
{
case 1:	 SYSCLK_DAT_reg.SYSCLK_DAT_R_AHB=0 ;break;
case 2:	 SYSCLK_DAT_reg.SYSCLK_DAT_R_AHB=8 ;break;
case 4:	 SYSCLK_DAT_reg.SYSCLK_DAT_R_AHB=9 ;break;
case 8:	 SYSCLK_DAT_reg.SYSCLK_DAT_R_AHB=10 ;break;
case 16: SYSCLK_DAT_reg.SYSCLK_DAT_R_AHB=11 ;break;
case 64: SYSCLK_DAT_reg.SYSCLK_DAT_R_AHB=12 ;break;
case 128:SYSCLK_DAT_reg.SYSCLK_DAT_R_AHB=13 ;break;
case 256:SYSCLK_DAT_reg.SYSCLK_DAT_R_AHB=14 ;break;
case 512:SYSCLK_DAT_reg.SYSCLK_DAT_R_AHB=15 ;break;
default: 
return; 
}

SYSCLK_DAT_reg.SYSCLK_DAT_AHB=SYSCLK_DAT_reg.SYSCLK_DAT_SYS/Sys_DIV_AHB;                //AHB真实时钟

//---APB2相关计算---
switch(Sys_DIV_APB2)
{
case 1:	 SYSCLK_DAT_reg.SYSCLK_DAT_R_APB2=0	 ;break;
case 2:	 SYSCLK_DAT_reg.SYSCLK_DAT_R_APB2=4  ;break;
case 4:	 SYSCLK_DAT_reg.SYSCLK_DAT_R_APB2=5	 ;break;
case 8:	 SYSCLK_DAT_reg.SYSCLK_DAT_R_APB2=6	 ;break;
case 16: SYSCLK_DAT_reg.SYSCLK_DAT_R_APB2=7	 ;break;

default: 
return;
}
SYSCLK_DAT_reg.SYSCLK_DAT_APB2=SYSCLK_DAT_reg.SYSCLK_DAT_AHB/Sys_DIV_APB2;	  //APB2真实时钟

//---APB1相关计算---
switch(Sys_DIV_APB1)
{
case 1:	 SYSCLK_DAT_reg.SYSCLK_DAT_R_APB1=0	 ;break;
case 2:	 SYSCLK_DAT_reg.SYSCLK_DAT_R_APB1=4	 ;break;
case 4:	 SYSCLK_DAT_reg.SYSCLK_DAT_R_APB1=5	 ;break;
case 8:	 SYSCLK_DAT_reg.SYSCLK_DAT_R_APB1=6	 ;break;
case 16: SYSCLK_DAT_reg.SYSCLK_DAT_R_APB1=7	 ;break;

default: 
return;
}
SYSCLK_DAT_reg.SYSCLK_DAT_APB1=SYSCLK_DAT_reg.SYSCLK_DAT_APB2/Sys_DIV_APB1;	  //APB1真实时钟
}






/************************************************************************
系统初始化的项目有：
1：默认是PLL9倍频
2: 选择HSE为PLL输入，且PLL为系统时钟，APB1为HCLK/4,其他总线时钟都为最高
3: 8888888次后PLL和HSE时钟还未就绪则系统初始化失败！ 返回1
4: 默认只开启了PA~G端口的时钟！！其他片内外设时钟都未开启!!
5：默认为输出1，且为开漏输出，最大10M
************************************************************************/


/*----------------------------------------

mode 的D3,D2值意义：

在输入模式(mode[1:0]=00)： 
00：模拟输入模式 
01：浮空输入模式(复位后的状态) 
10：上拉/下拉输入模式 
11：保留 

在输出模式(mode[1:0]>00)： 
00：通用推挽输出模式 
01：通用开漏输出模式 
10：复用功能推挽输出模式
11：复用功能开漏输出模式 

//---------------------------------------

mode 的D1，D0值意义：
00：输入模式  (复位后的状态) 
01：输出模式，最大速度10MHz
10：输出模式，最大速度2MHz 
11：输出模式，最大速度50MHz 
------------------------------------------*/
void Port_In_Out(uchar port,uchar p_bit,uchar mode)
{
uchar u8_temp;
ulong u32_temp1,u32_temp2;

if((p_bit>15)||(port>6)||(mode>16))
return;                  //输入错误，返回

mode&=0x0f;

if(p_bit>7)
u8_temp=p_bit-8;
else
u8_temp=p_bit;



switch(u8_temp)
{
case 0: u32_temp1=0xfffffff0; u32_temp2=(ulong)mode;     break;
case 1: u32_temp1=0xffffff0f; u32_temp2=(ulong)mode<<4;  break;
case 2: u32_temp1=0xfffff0ff; u32_temp2=(ulong)mode<<8;  break;
case 3: u32_temp1=0xffff0fff; u32_temp2=(ulong)mode<<12; break;
case 4: u32_temp1=0xfff0ffff; u32_temp2=(ulong)mode<<16; break;
case 5: u32_temp1=0xff0fffff; u32_temp2=(ulong)mode<<20; break;
case 6: u32_temp1=0xf0ffffff; u32_temp2=(ulong)mode<<24; break;
case 7: u32_temp1=0x0fffffff; u32_temp2=(ulong)mode<<28; break;

default :   return; 
}



switch(port)
{
case 0:
if(p_bit>7)
GPIOA_CRH=(GPIOA_CRH&u32_temp1)|(u32_temp2);
else
GPIOA_CRL=(GPIOA_CRL&u32_temp1)|(u32_temp2);
return;


case 1:
if(p_bit>7)
GPIOB_CRH=(GPIOB_CRH&u32_temp1)|(u32_temp2);
else
GPIOB_CRL=(GPIOB_CRL&u32_temp1)|(u32_temp2);
return;


case 2:
if(p_bit>7)
GPIOC_CRH=(GPIOC_CRH&u32_temp1)|(u32_temp2);
else
GPIOC_CRL=(GPIOC_CRL&u32_temp1)|(u32_temp2);
return;



case 3:
if(p_bit>7)
GPIOD_CRH=(GPIOD_CRH&u32_temp1)|(u32_temp2);
else
GPIOD_CRL=(GPIOD_CRL&u32_temp1)|(u32_temp2);
return;

case 4:
if(p_bit>7)
GPIOE_CRH=(GPIOE_CRH&u32_temp1)|(u32_temp2);
else
GPIOE_CRL=(GPIOE_CRL&u32_temp1)|(u32_temp2);
return;


case 5:
if(p_bit>7)
GPIOF_CRH=(GPIOF_CRH&u32_temp1)|(u32_temp2);
else
GPIOF_CRL=(GPIOF_CRL&u32_temp1)|(u32_temp2);
return;


case 6:
if(p_bit>7)
GPIOG_CRH=(GPIOG_CRH&u32_temp1)|(u32_temp2);
else
GPIOG_CRL=(GPIOG_CRL&u32_temp1)|(u32_temp2);
return;

default :   return;  

}
}



void Dev_Sck_En(uchar Sck_EnNO,uchar ON_OFF)
{
if(ON_OFF==0)   //关闭某设备的时钟
 {
		if(Sck_EnNO<32)        //说明是AHB总线上的设备
		{
		 RCC_AHBENR&=~((ulong)(1<<Sck_EnNO));		 
		 return; 
		}
		if((63<Sck_EnNO)&&(31<Sck_EnNO<96))	   //说明是APB2总线上的设备
		{
		 RCC_APB2ENR&=~((ulong)(1<<(Sck_EnNO-0x40)));
		 return;
		}
		if((127<Sck_EnNO)&&(31<Sck_EnNO<160))	  //说明是APB1总线上的设备
		{
		 RCC_APB1ENR&=~((ulong)(1<<(Sck_EnNO-0x80)));
		 return;
		}
    return;
   }
  else			   //往下是开启某设备的时钟
  {
		if(Sck_EnNO<32)        //说明是AHB总线上的设备
		{
		 RCC_AHBENR|=((ulong)(1<<Sck_EnNO));		 
		 return; 
		}
		if((63<Sck_EnNO)&&(Sck_EnNO<96))	   //说明是APB2总线上的设备
		{
		 RCC_APB2ENR|=((ulong)(1<<(Sck_EnNO-0x40)));
		 return;
		}
		if((127<Sck_EnNO)&&(Sck_EnNO<160))	  //说明是APB1总线上的设备
		{
		 RCC_APB1ENR|=((ulong)(1<<(Sck_EnNO-0x80)));
		 return;
		}
    return;
  }
}


uchar sys_Init (void)
{

    ulong  temp=0; 
	SYSCLK_Count();     	//由外时钟，倍频系数，分频系数计算各个总线设备的时钟                 
  	RCC_AHBENR = 0x00000014;        //睡眠模式闪存和SRAM时钟使能.其他关闭.	  
  	RCC_APB2ENR =0x00000000;        //外设时钟关闭.			   
  	RCC_APB1ENR =0x00000000; 
 
	RCC_CFGR=  0x00010000;        //9倍频，HSE为PLL输入，但此时PLL并未设置成系统时钟，因为PLL还未开启				    
	RCC_CFGR|= (SYSCLK_DAT_reg.SYSCLK_DAT_R_APB2)<<11;	  //APB2分频系数设定
	RCC_CFGR|= (SYSCLK_DAT_reg.SYSCLK_DAT_R_APB1)<<8;	  //APB1分频系数设定
	RCC_CFGR|= (SYSCLK_DAT_reg.SYSCLK_DAT_R_AHB)  <<4;	  //AHB分频系数设定
	
	RCC_CFGR|= 0x00000000 <<14;	  //ADC时钟，由APB2经过RCC_CFGR寄存器（15-14位）分频得到，这里设定为76/2=36MHZ
	
	RCC_CFGR|=(Sys_PLL-2)<<18;		 //倍频系数;
	RCC_CR  =0x00010001;             //开启外部震荡 
	while(!(RCC_CR&0x00020000));     //等待外部时钟就绪
	FLASH_ACR|=0x32;                 //FLASH延时2个单位
	RCC_CR|=0x01000000;              //开启PLL震荡	 
	while(!(RCC_CR&0x02000000));     //等待PLL锁定 
	  
	RCC_CFGR|=0x00000002;            //选择PLL为系统时钟
	temp=0;
	while(temp!=0x02)                //等待PLL作为系统时钟设置成功
	{   
	temp=RCC_CFGR>>2;
	temp&=0x03;
	}  
	RCC_CR&=0xfffffffe;              //关闭内部震荡  
    RCC_APB2ENR=0x000001fc;          //默认只开启了PA~G端口的时钟！！
//----------------------------下面是GPIO口的默认设置----------------------
GPIOA_ODR=0xffffffff;
GPIOB_ODR=0xffffffff;
GPIOC_ODR=0xffffffff;
GPIOD_ODR=0xffffffff;
GPIOE_ODR=0xffffffff;
GPIOF_ODR=0xffffffff;
GPIOG_ODR=0xffffffff;    //默认为输出1，且为开漏输出，最大10M

GPIOA_CRL=0x88888888;
GPIOA_CRH=0x88888888;

GPIOB_CRL=0x88888888;
GPIOB_CRH=0x88888888;

GPIOC_CRL=0x88888888;
GPIOC_CRH=0x88888888;

GPIOD_CRL=0x88888888;
GPIOD_CRH=0x88888888;

GPIOE_CRL=0x88888888;
GPIOE_CRH=0x88888888;

GPIOF_CRL=0x88888888;
GPIOF_CRH=0x88888888;

GPIOG_CRL=0x88888888;
GPIOG_CRH=0x88888888;

Dev_Sck_En(SckEn_GPIOA,1);
Dev_Sck_En(SckEn_GPIOB,1);
Dev_Sck_En(SckEn_GPIOC,1);
Dev_Sck_En(SckEn_GPIOD,1);
Dev_Sck_En(SckEn_GPIOE,1);

return(1);
}



/*
开启关闭内部RC震荡器
Flash编程中需要开启内部RC振荡器

当是待机，停机模式，或外部时钟失效的时候，HSI被硬件开启

*/
void Sys_OpenHSI(uchar ON_OFF)
{
if(ON_OFF)
{
RCC_CR|=0X00000001;
while( (RCC_CR&0xfffffffd) ==0); //等待稳定
}
else
{
RCC_CR&=0xfffffffe;    
while( (RCC_CR&0xfffffffd) ); //等待关闭，要6个时钟周期
}

}



void Soft_Reset(void)	    //软件复位
{

SCB_AIRCR=0x05fa0000|(ulong)0x04;
while(1);

}


uchar DMA1_STA_HTIF(uchar DMA1_NO)	//0：半传输完成标志
{if(DMA1_ISR&((ulong)1<<((DMA1_NO-1)*4+2))) return 0; else return 1; }

void DMA1_STA_CHTIF(uchar DMA1_NO)	//清除半传输完成标志
{DMA1_IFCR|=(ulong)1<<((DMA1_NO-1)*4+2);}

void DMA1_STA_CTCIF(uchar DMA1_NO)	//清除半传输完成标志
{DMA1_IFCR|=(ulong)1<<((DMA1_NO-1)*4+1);}

uchar DMA1_STA_TCIF(uchar DMA1_NO) 	//0：全传输完成标志
{if(DMA1_ISR&((ulong)1<<((DMA1_NO-1)*4+1))) return 0; else return 1;}

//UART MDA 传输配置函数
void USART_MDAEN (uchar usart_NO,uchar TxDMA_EN,uchar RxDMA_EN)
{
switch(usart_NO)
{
case 1:
if(TxDMA_EN==0)	USART1_CR3&=~((ulong)1<<7);	 else USART1_CR3|=(ulong)1<<7;
if(RxDMA_EN==0)	USART1_CR3&=~((ulong)1<<6);	 else USART1_CR3|=(ulong)1<<6;
break;
case 2:
if(TxDMA_EN==0)	USART2_CR3&=~((ulong)1<<7);	 else USART2_CR3|=(ulong)1<<7;
if(RxDMA_EN==0)	USART2_CR3&=~((ulong)1<<6);	 else USART2_CR3|=(ulong)1<<6;
break;
case 3:
if(TxDMA_EN==0)	USART3_CR3&=~((ulong)1<<7);	 else USART3_CR3|=(ulong)1<<7;
if(RxDMA_EN==0)	USART3_CR3&=~((ulong)1<<6);	 else USART3_CR3|=(ulong)1<<6;
break;
case 4:	 
if(TxDMA_EN==0)	USART4_CR3&=~((ulong)1<<7);	 else USART4_CR3|=(ulong)1<<7;
if(RxDMA_EN==0)	USART4_CR3&=~((ulong)1<<6);	 else USART4_CR3|=(ulong)1<<6;
break;
case 5:
if(TxDMA_EN==0)	USART5_CR3&=~((ulong)1<<7);	 else USART5_CR3|=(ulong)1<<7;
if(RxDMA_EN==0)	USART5_CR3&=~((ulong)1<<6);	 else USART5_CR3|=(ulong)1<<6;
break;
}
}

void USART_Init (uchar usart_NO,ulong usart_BB)
{
//----其他默认，8位数据位，1个停止位，异步，DMA关闭 等

ulong  usart_Fclk;

float temp;
u16 mantissa;
u16 fraction;

SYSCLK_Count();     	//由外时钟,倍频系数,分频系数计算各个总线设备的时钟 
Dev_Sck_En(SckEn_AFIO, 1);

if(usart_NO==1)
usart_Fclk=SYSCLK_DAT_reg.SYSCLK_DAT_APB2;	   //APB2总线的时钟\\
else
usart_Fclk=SYSCLK_DAT_reg.SYSCLK_DAT_APB1;	   //APB2总线的时钟\\

   	if(usart_NO==1)
	temp=(float)(usart_Fclk)/(usart_BB*8);//得到USARTDIV
	else
	temp=(float)(usart_Fclk)/(usart_BB*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 



switch(usart_NO)
{
case 1:
Dev_Sck_En(SckEn_GPIOA,1);  //开启IO口时钟
Dev_Sck_En(SckEn_USART1,1); //开启USART时钟
Port_In_Out(PortA,BIT9, 0x0b);  // TX 
Port_In_Out(PortA,BIT10,0x08);  // RX   复用引脚功能
USART1_BRR=mantissa;       //波特率设置
USART1_CR1 =0x00;
USART1_CR1|=(1<<3);        //使能USARRT 发送
USART1_CR1|=(1<<2);        //使能USARRT	接收
USART1_CR1|=(1<<13);       //使能USARRT
USART1_SR|=0x00;
break;


case 2:
Dev_Sck_En(SckEn_GPIOA,1);  //开启IO口时钟
Dev_Sck_En(SckEn_USART2,1); //开启USART时钟
Port_In_Out(PortA,BIT2,0x0b);  // TX 
Port_In_Out(PortA,BIT3,0x08);  // RX   复用引脚功能

USART2_BRR=mantissa;      //波特率设置
USART2_CR1 =0x00;
USART2_CR1|=(1<<3);        //使能USARRT 发送
USART2_CR1|=(1<<2);        //使能USARRT	接收
USART2_CR1|=(1<<13);       //使能USARRT
USART2_SR|=0x00;
break;

case 3:
Dev_Sck_En(SckEn_GPIOB,1);  //开启IO口时钟
Dev_Sck_En(SckEn_USART3,1); //开启USART时钟
Port_In_Out(PortB,BIT10,0x0b);  // TX 
Port_In_Out(PortB,BIT11,0x08);  // RX   复用引脚功能

USART3_BRR=mantissa;      //波特率设置
USART3_CR1 =0x00;
USART3_CR1|=(1<<3);        //使能USARRT 发送
USART3_CR1|=(1<<2);        //使能USARRT	接收
USART3_CR1|=(1<<13);       //使能USARRT
USART3_SR|=0x00;
break;


case 4:
Dev_Sck_En(SckEn_GPIOC,1);  //开启IO口时钟
Dev_Sck_En(SckEn_USART4,1); //开启USART时钟
Port_In_Out(PortC,BIT10,0x0b);  // TX 
Port_In_Out(PortC,BIT11,0x08);  // RX   复用引脚功能

USART4_BRR=mantissa;      //波特率设置
USART4_CR1 =0x00;
USART4_CR1|=(1<<3);        //使能USARRT 发送
USART4_CR1|=(1<<2);        //使能USARRT	接收
USART4_CR1|=(1<<13);       //使能USARRT
USART4_SR|=0x00;
break;


case 5:
Dev_Sck_En(SckEn_GPIOC,1);  //开启IO口时钟
Dev_Sck_En(SckEn_GPIOD,1);  //开启IO口时钟
Dev_Sck_En(SckEn_USART5,1); //开启USART时钟
Port_In_Out(PortC,BIT12,0x0b);  // TX 
Port_In_Out(PortD,BIT2, 0x08);  // RX   复用引脚功能

USART5_BRR=mantissa;      //波特率设置
USART5_CR1 =0x00;
USART5_CR1|=(1<<3);        //使能USARRT 发送
USART5_CR1|=(1<<2);        //使能USARRT	接收
USART5_CR1|=(1<<13);       //使能USARRT
USART5_SR|=0x00;
break;

default:
return;
}

return;
} 




void USART1_TxDat(uchar d)
{
	
USART1_DR=d;
while((USART1_SR&0x40)==0);
}

void USART2_TxDat(uchar d)
{
	
USART2_DR=d;
while((USART2_SR&0x40)==0);
}
void USART3_TxDat(uchar d)
{
	
USART3_DR=d;
while((USART3_SR&0x40)==0);
}

void USART4_TxDat(uchar d)
{
USART4_DR=d;
while((USART4_SR&0x40)==0);
}

void USART5_TxDat(uchar d)
{
USART5_DR=d;
while((USART5_SR&0x40)==0);
}



void UART1_Clear_RL_Mark(void)
{
volatile uchar clr=0;
clr=USART1_DR;  //清除接收标志位
clr=USART1_SR;  //清除过载标志位1
clr=USART1_CR1; //清除过载标志位2
}

void UART2_Clear_RL_Mark(void)
{
volatile uchar clr=0;
clr=USART2_DR;  //清除接收标志位
clr=USART2_SR;  //清除过载标志位1
clr=USART2_CR1; //清除过载标志位2
}

void UART3_Clear_RL_Mark(void)
{
volatile uchar clr=0;
clr=USART3_DR;  //清除接收标志位
clr=USART3_SR;  //清除过载标志位1
clr=USART3_CR1; //清除过载标志位2
}

/*******************************

UART DMA

********************************/

ulong UART1_DMA_ConfigVal=0;

void USART1_Send_DMA_Init(void)
{
	
USART1_CR3|=(1<<7);      //使能USART DMA,发射
	
//------------------DMA设置----------------------------
Dev_Sck_En(SckEn_DMA1,1);      //开启DMA1时钟
DMA1_CCR4=0;
//DMA1_CCR4|=(1<<14);   //开启存储器到存储器模式
DMA1_CCR4|=0x01<<12;    //该通道优先级
//DMA1_CCR4|=0x01<<10;    //该通道存储器数据宽度,=0,8
//DMA1_CCR4|=0x01<<8;     //该通道外设数据宽度,=0,8

DMA1_CCR4|=1<<7;		//为1则执行存储器地址增量模式
//DMA1_CCR4|=1<<6;		//为1则执行外设地址增量模式
//DMA1_CCR4|=1<<5;		//为1则循环模式
DMA1_CCR4|=1<<4;		//数据方向，0：从外设读，1：从存储器读
//DMA1_CCR4|=1<<2;		//允许半传输中断
//DMA1_CCR4|=1<<1;		//允许传输中断
DMA1_CPAR4=(ulong)(&USART1_DR);	    //外设基址

UART1_DMA_ConfigVal=DMA1_CCR4;
}


void USART1_Send_DMA_Running(uchar *buf,uchar Len)
{
	
DMA1_CCR4=0;
DMA1_CNDTR4=Len;   				//一次传输的数量
DMA1_CMAR4=(ulong)(buf);	//寄存器基址
DMA1_CCR4=UART1_DMA_ConfigVal;
DMA1_CCR4|=1;	        //为1开启该通道传输
	
}

/*
返回0：传输未完成，1，已经完成
*/
uchar USART1_Send_DMA_WaitFinish(void)
{
	
if(DMA1_ISR & ((ulong)1<<13) )   //等待上次传输完成
{
DMA1_IFCR |= (1<<13);//清除

return 1;
}
return 0;

}


//----------------------------------------------------

ulong UART2_DMA_ConfigVal=0;
void USART2_Send_DMA_Init(void)
{
	
USART2_CR3|=(1<<7);      //使能USART DMA,发射
	
//------------------DMA设置----------------------------
Dev_Sck_En(SckEn_DMA1,1);      //开启DMA1时钟
	
DMA1_CCR7=0;
//DMA1_CCR7|=(1<<14);   //开启存储器到存储器模式
DMA1_CCR7|=0x01<<12;    //该通道优先级
//DMA1_CCR7|=0x01<<10;    //该通道存储器数据宽度,=0,8
//DMA1_CCR7|=0x01<<8;     //该通道外设数据宽度,=0,8

DMA1_CCR7|=1<<7;		//为1则执行存储器地址增量模式
//DMA1_CCR7|=1<<6;		//为1则执行外设地址增量模式
//DMA1_CCR7|=1<<5;		//为1则循环模式
DMA1_CCR7|=1<<4;		//数据方向，0：从外设读，1：从存储器读
//DMA1_CCR7|=1<<2;		//允许半传输中断
//DMA1_CCR7|=1<<1;		//允许传输中断
DMA1_CPAR7=(ulong)(&USART2_DR);	    //外设基址

UART2_DMA_ConfigVal=DMA1_CCR7;
}


void USART2_Send_DMA_Running(uchar *buf,uchar Len)
{
	
DMA1_CCR7=0;
DMA1_CNDTR7=Len;   				//一次传输的数量
DMA1_CMAR7=(ulong)(buf);	//寄存器基址
DMA1_CCR7=UART2_DMA_ConfigVal;
DMA1_CCR7|=1;	        //为1开启该通道传输
	
}

/*
返回0：传输未完成，1，已经完成
*/
uchar USART2_Send_DMA_WaitFinish(void)
{
	
if(DMA1_ISR & ((ulong)1<<25) )   //等待上次传输完成
{
DMA1_IFCR |= (1<<25);//清除

return 1;
}
return 0;

}


//----------------------------------------

ulong UART3_DMA_ConfigVal=0;

void USART3_Send_DMA_Init(void)
{
	
USART3_CR3|=(1<<7);      //使能USART DMA,发射
	
//------------------DMA设置----------------------------
Dev_Sck_En(SckEn_DMA1,1);      //开启DMA1时钟
DMA1_CCR2=0;
//DMA1_CCR2|=(1<<14);   //开启存储器到存储器模式
DMA1_CCR2|=0x01<<12;    //该通道优先级
//DMA1_CCR2|=0x01<<10;    //该通道存储器数据宽度,=0,8
//DMA1_CCR2|=0x01<<8;     //该通道外设数据宽度,=0,8

DMA1_CCR2|=1<<7;		//为1则执行存储器地址增量模式
//DMA1_CCR2|=1<<6;		//为1则执行外设地址增量模式
//DMA1_CCR2|=1<<5;		//为1则循环模式
DMA1_CCR2|=1<<4;		//数据方向，0：从外设读，1：从存储器读
//DMA1_CCR2|=1<<2;		//允许半传输中断
//DMA1_CCR2|=1<<1;		//允许传输中断
DMA1_CPAR2=(ulong)(&USART3_DR);	    //外设基址

UART3_DMA_ConfigVal=DMA1_CCR2;
}


void USART3_Send_DMA_Running(uchar *buf,uchar Len)
{
	
DMA1_CCR2=0;
DMA1_CNDTR2=Len;   				//一次传输的数量
DMA1_CMAR2=(ulong)(buf);	//寄存器基址
DMA1_CCR2=UART3_DMA_ConfigVal;
DMA1_CCR2|=1;	        //为1开启该通道传输
	
}

/*
返回0：传输未完成，1，已经完成
*/
uchar USART3_Send_DMA_WaitFinish(void)
{
	
if(DMA1_ISR & ((ulong)1<<5) )   //等待上次传输完成
{
DMA1_IFCR |= (1<<5);//清除

return 1;
}
return 0;

}
void UART1_Send_String(uchar *path)//串口发送，返回0完成
{
	while( (*path)!= '\0')
	{
		USART1_TxDat(*path);
		path++;	
	}
}

void UART2_Send_String(uchar *path)//串口发送，返回0完成
{
	while( (*path)!= '\0')
	{
		USART2_TxDat(*path);
		path++;	
	}
}



void UART3_Send_String(uchar *path)//串口发送，返回0完成
{
	while( (*path)!= '\0')
	{
		USART3_TxDat(*path);
		path++;	
	}
}

void UART4_Send_String(uchar *path)//串口发送，返回0完成
{
	while( (*path)!= '\0')
	{
		USART4_TxDat(*path);
		path++;	
	}
}


void UART5_Send_String(uchar *path)//串口发送，返回0完成
{
	while( (*path)!= '\0')
	{
		USART5_TxDat(*path);
		path++;	
	}
}








void UART1_Send_Length( uchar *TemBuf,usint len )
{
usint i=0;
while(len--)
{
USART1_TxDat(TemBuf[i++]);
}
}

void UART2_Send_Length( uchar *TemBuf,usint len )
{
usint i=0;
while(len--)
{
	
USART2_TxDat(TemBuf[i++]);

}

}



void UART3_Send_Length( uchar *TemBuf,usint len )
{
	usint i=0;
	while(len--)
	{
	USART3_TxDat(TemBuf[i++]);
	}
}



void UART4_Send_Length( uchar *TemBuf,usint len )
{
	usint i=0;
	while(len--)
	{
		
	USART4_TxDat(TemBuf[i++]);
	}
}


void UART5_Send_Length( uchar *TemBuf,usint len )
{
	usint i=0;
	while(len--)
	{
	USART5_TxDat(TemBuf[i++]);
	}
}



void USART_ChangeBAUD(uchar usart_NO,ulong usart_BB)
{
ulong  usart_Fclk;
float temp;
u16 mantissa;
u16 fraction;
	
if(usart_NO==1)
usart_Fclk=SYSCLK_DAT_reg.SYSCLK_DAT_APB2;	   //APB2总线的时钟\\
else
usart_Fclk=SYSCLK_DAT_reg.SYSCLK_DAT_APB1;	   //APB2总线的时钟\\

if(usart_NO==1)
temp=(float)(usart_Fclk)/(usart_BB*8);//得到USARTDIV
else
temp=(float)(usart_Fclk)/(usart_BB*16);//得到USARTDIV
	
mantissa=temp;				 //得到整数部分
fraction=(temp-mantissa)*16; //得到小数部分	 
mantissa<<=4;
mantissa+=fraction; 


switch(usart_NO)
{
case 1:USART1_BRR=mantissa; break;
case 2:USART2_BRR=mantissa; break;
case 3:USART3_BRR=mantissa; break;
case 4:USART4_BRR=mantissa; break;
case 5:USART5_BRR=mantissa; break;	

}

}









void UART_ALLMkCLR(void)
{
volatile uchar clr=0;
	
	clr=USART1_SR;
	clr=USART1_DR;
	
	clr=USART2_SR;
	clr=USART2_DR;
	
	clr=USART3_SR;
	clr=USART3_DR;	
	
	clr=USART4_SR;
	clr=USART4_DR;
	
	clr=USART5_SR;
	clr=USART5_DR;

	
}

//----------------------------------------


static uchar ADCChgNum=0;   //ADC转化的通道数目，由每调用一次函数增加一次！
static uchar ADCDMAEn=0;    //为1开启MDA传输

/***********************************
ADC初始化配置，对整个ADC有效
SCAN		扫描模式
DMA			开启DMA
Loop		循环转换
DatLay		数据对其格式
************************************/
void ADC1_Init(uchar SCAN,uchar DMA,uchar Loop,uchar DatLay)
{
Dev_Sck_En(SckEn_AFIO, 1);	   //开启复用时钟
Dev_Sck_En(SckEn_ADC1,1);      //开启ADC1时钟

ADCDMAEn=DMA;	
ADC1_CR2=0;
ADC1_CR1|=(SCAN<<8);        //扫描模式
ADC1_CR2|=(DMA <<8);        //DMA模式
ADC1_CR2|=(Loop<<1);        //连续转化模式设置
ADC1_CR2|=(DatLay<<11);    //数据左对齐

ADCChgNum=0;   //转化个数清0！
}










/*****************************************************
ADC各个通道配置，可多次重复调用
chann_val :通道值，取值0~15
chann_ord：此通道在规则转换下的顺序取值1~16
ADCChgNumrst： 是否清0转化个数为0 ，还是继续累加！ 为0则清0！
*****************************************************/
void  ADC1_ChannelSET(uchar chann_val,uchar chann_time,uchar chann_ord,uchar ADCChgNumrst)
{

if(chann_val>15) return;
if((chann_ord>16)||(chann_ord==0)) return;


//----写采样时间！
if(chann_val<10)    //0到通道9
ADC1_SMPR2|=((ulong)chann_time<<(chann_val*3));
else
ADC1_SMPR1|=((ulong)chann_time<<((chann_val-10)*3));

if(ADCChgNumrst==0)  	 //为0则清0！
ADCChgNum=0;

//转化个数设置----
ADC1_SQR1&=~((ulong)0X0F<<20);     //复位转化个数
ADC1_SQR1|=((ulong)ADCChgNum<<20); //更新个数,00：转化个数为1个

ADCChgNum++;   //转化通道数++

switch((chann_ord-1)/6)	 //转化顺序更新
{
case 0:
ADC1_SQR3&=   ~((ulong)0X1F<<((chann_ord-1)*5));     //复位
ADC1_SQR3|=((ulong)chann_val<<((chann_ord-1)*5)); //写新值
break;

case 1:
ADC1_SQR2&=   ~((ulong)0X1F<<((chann_ord-7)*5));  //复位
ADC1_SQR2|=((ulong)chann_val<<((chann_ord-7)*5)); //写新值
break;

case 2:
ADC1_SQR1&=    ~((ulong)0X1F<<((chann_ord-13)*5));     //复位
ADC1_SQR1|=((ulong)chann_val<<((chann_ord-13)*5)); //写新值
break;
default: 
break;
}

switch(chann_val)
{
case 0:
Dev_Sck_En(SckEn_GPIOA,1);
Port_In_Out(PortA,BIT0,0x00);  //ADC1_IN0 //复用引脚功能,模拟输入
break;
case 1:
Dev_Sck_En(SckEn_GPIOA,1);
Port_In_Out(PortA,BIT1,0x00);  //ADC1_IN1 //复用引脚功能,模拟输入
break;
case 2:
Dev_Sck_En(SckEn_GPIOA,1);
Port_In_Out(PortA,BIT2,0x00);  //ADC1_IN2 //复用引脚功能,模拟输入

case 3:
Dev_Sck_En(SckEn_GPIOA,1);
Port_In_Out(PortA,BIT3,0x00);  //ADC1_IN3 //复用引脚功能,模拟输入
break;
case 4:
Dev_Sck_En(SckEn_GPIOA,1);
Port_In_Out(PortA,BIT4,0x00);  //ADC1_IN4 //复用引脚功能,模拟输入
break;
case 5:
Dev_Sck_En(SckEn_GPIOA,1);
Port_In_Out(PortA,BIT5,0x00);  //ADC1_IN5 //复用引脚功能,模拟输入
break;
case 6:
Dev_Sck_En(SckEn_GPIOA,1);
Port_In_Out(PortA,BIT6,0x00);  //ADC1_IN6 //复用引脚功能,模拟输入
break;
case 7:
Dev_Sck_En(SckEn_GPIOA,1);
Port_In_Out(PortA,BIT7,0x00);  //ADC1_IN7 //复用引脚功能,模拟输入
break;

case 8:
Dev_Sck_En(SckEn_GPIOB,1);
Port_In_Out(PortB,BIT0,0x00);  //ADC1_IN8 //复用引脚功能,模拟输入
break;
case 9:
Dev_Sck_En(SckEn_GPIOB,1);
Port_In_Out(PortB,BIT1,0x00);  //ADC1_IN9 //复用引脚功能,模拟输入	
break;
case 10:
Dev_Sck_En(SckEn_GPIOC,1);
Port_In_Out(PortC,BIT0,0x00);  //ADC1_IN10//复用引脚功能,模拟输入
break;
case 11:
Dev_Sck_En(SckEn_GPIOC,1);
Port_In_Out(PortC,BIT1,0x00);  //ADC1_IN11//复用引脚功能,模拟输入
break;
case 12:
Dev_Sck_En(SckEn_GPIOC,1);
Port_In_Out(PortC,BIT2,0x00);  //ADC1_IN12//复用引脚功能,模拟输入
break;
case 13:
Dev_Sck_En(SckEn_GPIOC,1);
Port_In_Out(PortC,BIT3,0x00);  //ADC1_IN13//复用引脚功能,模拟输入
break;
case 14:
Dev_Sck_En(SckEn_GPIOC,1);
Port_In_Out(PortC,BIT4,0x00);  //ADC1_IN14//复用引脚功能,模拟输入
break;
case 15:
Dev_Sck_En(SckEn_GPIOC,1);
Port_In_Out(PortC,BIT5,0x00);  //ADC1_IN15//复用引脚功能,模拟输入
break;
default: 
break;
}


}





/*
经过ADC1_Init， ADC1_ChannelSET后，便可以开启ADC转化
该函数只能单次调用，否则DMA模式下的规则转化顺序会乱掉
ADCBuffer:DMA模式下 的ADC数据保存的地址，注意长度>=ADCChgNum;
*/
void ADC1_Start(usint * ADCBuffer)
{
usint i=0;
	

if(ADCDMAEn)
{
Dev_Sck_En(SckEn_DMA1,1);      //开启DMA1时钟

DMA1_CCR1=0;
//DMA1_CCR1|=(1<<14);   //开启存储器到存储器模式
DMA1_CCR1|=0x01<<12;    //该通道优先级
DMA1_CCR1|=0x01<<10;	//该通道存储器数据宽度16
DMA1_CCR1|=0x01<<8;		//该通道外设数据宽度16
DMA1_CCR1|=1<<7;		//为1则执行存储器地址增量模式
//DMA1_CCR1|=1<<6;		//为1则执行外设地址增量模式
DMA1_CCR1|=1<<5;		//为1则循环模式
//DMA1_CCR1|=1<<4;		//数据方向，0：从外设读，1：从存储器读

DMA1_CNDTR1=ADCChgNum;   		//一次传输的数量！

DMA1_CPAR1=(ulong)(&ADC1_DR);	  //外设基址
DMA1_CMAR1=(ulong)(ADCBuffer);	  //寄存器基址
DMA1_CCR1|=1<<0;	              //为1开启该通道传输
}	

ADC1_CR2|=(1<<0);   //开启ADC电源；
for(i=0;i<1000;i++);  //延时

ADC1_CR2|=(1<<3);
while(ADC1_CR2&(1<<3));  //等待校准完成
ADC1_CR2|=(1<<2);
while(ADC1_CR2&(1<<2));  //等待校准完成
ADC1_CR2|=(1<<0);        //开启ADC转化；

}





//--------------------------------------------------------------------------

void SPI_Init(uchar spi_NO,uchar spi_speed)
{
ulong REG_temp1=0;
ulong REG_temp2=0;

//REG_temp1|=1<<15;    //1：单线双向模式
//REG_temp1|=1<<14;    //1：单线双向模式下，只发，0：只收
//REG_temp1|=1<<13;    //1：CRC使能
//REG_temp1|=1<<12;    //1：CRC相关值
//REG_temp1|=1<<11;    //0：8位模式，1：16位
//REG_temp1|=1<<10;    //双线模式下，0：全双工，1：只能收
REG_temp1|=1<<9;     //0：禁止软件管理片选，1：软件管理，启用SII控制
REG_temp1|=1<<8;     //1：开启片选（1<<9=1时有效）
//REG_temp1|=1<<7;     //0：MSB先发，1：LSB先发
REG_temp1|=(ulong)spi_speed<<3;  //速度设置  Fplk/16
REG_temp1|=1<<2;     //0:配置为从模式，1：主模式
//REG_temp1|=1<<1;     //极性控制 0：空闲时SCK为0
//REG_temp1|=1<<0;     //相位控制 0：第一个时钟边沿采样第一个数据

//REG_temp2|=1<<7;   //1:发送缓存区空中断使能
//REG_temp2|=1<<6;   //1：接收区非空中断使能
//REG_temp2|=1<<5;   //1：错误中断使能
//REG_temp2|=1<<2;   //0：禁止主模式时SS输出
//REG_temp2|=1<<1;   //1：TX发射DMA使能
//REG_temp2|=1<<0;   //1：RX接收DMA使能

if(REG_temp2&0x03)
Dev_Sck_En(SckEn_DMA1,1);      //开启DMA1时钟


switch(spi_NO)
{
case 1:
Dev_Sck_En(SckEn_GPIOA,1);     //开启IO口时钟
Dev_Sck_En(SckEn_AFIO,1);	   //开启复用时钟
Dev_Sck_En(SckEn_SPI1,1);      //开启SPI1时钟

Port_In_Out(PortA,BIT5,0x0b);   //SPI1_SCK复用推挽输出， 主机模式
Port_In_Out(PortA,BIT6,0x08);	//SPI1_MISO复用上拉输入，主机模式
Port_In_Out(PortA,BIT7,0x0b);	//SPI1_MOSI复用推挽输出，主机模式

SPI1_CR1=REG_temp1;
SPI1_CR2=REG_temp2;
SPI1_CR1|=1<<6;      //开启SPI1
break;

case 2:

Port_In_Out(PortB,BIT13,0x0b);  //SPI2_SCK复用推挽输出， 主机模式
Port_In_Out(PortB,BIT14,0x08);	//SPI2_MISO复用上拉输入，主机模式
Port_In_Out(PortB,BIT15,0x0b);	//SPI2_MOSI复用推挽输出，主机模式

Dev_Sck_En(SckEn_GPIOB,1);     //开启IO口时钟
Dev_Sck_En(SckEn_AFIO, 1);	   //开启复用时钟
Dev_Sck_En(SckEn_SPI2,1);      //开启SPI2时钟

SPI2_CR1=REG_temp1;
SPI2_CR2=REG_temp2;
SPI2_CR1|=1<<6;      //开启SPI2
break;

case 3:
Dev_Sck_En(SckEn_GPIOB,1);     //开启IO口时钟
Dev_Sck_En(SckEn_AFIO, 1);	   //开启复用时钟
Dev_Sck_En(SckEn_SPI3, 1);      //开启SPI3时钟

Port_In_Out(PortB,BIT3,0x0b);   //SPI1_SCK复用推挽输出， 主机模式
Port_In_Out(PortB,BIT4,0x08);	//SPI1_MISO复用上拉输入，主机模式
Port_In_Out(PortB,BIT5,0x0b);	//SPI1_MOSI复用推挽输出，主机模式
SPI3_CR1=REG_temp1;
SPI3_CR2=REG_temp2;
SPI3_CR1|=1<<6;      //开启SPI3

break;

default: 
return;
}
}

//定时器相关设备驱动-----------
//PWM篇
//注意通用定时器在APCK1==1时是倍频的，72MHZ

void PWM_Init(uchar TIM_NO,uchar CH_NO)
{

switch(TIM_NO)
{
case 2:
Dev_Sck_En(SckEn_GPIOA,1);     //开启IO口时钟
Dev_Sck_En(SckEn_AFIO, 1);	   //开启复用时钟
Dev_Sck_En(SckEn_TIM2, 1);     //开启TIM2时钟

TIM2_CR1=0;
TIM2_CR2=0;
TIM2_SMCR=0;
TIM2_DIER=0;
TIM2_SR=0;
TIM2_CNT=0;
TIM2_PSC=0X05;    //f=72M/PSC+1 =12Mhz
TIM2_ARR=0X00;

switch(CH_NO)
{
case 1:
TIM2_CCMR1&=0xff00;
TIM2_CCMR1|=0x0068;		//PWM1	模式
TIM2_CCER|=0X0001;         //输出使能
TIM2_CCR1=0XFFFF;
Port_In_Out(PortA,BIT0,AF_TW_50M); 
break;
case 2:
TIM2_CCMR1&=0x00ff;
TIM2_CCMR1|=0x6800;		 //PWM1	模式
TIM2_CCER|=0X0010;         //输出使能
TIM2_CCR2=0XFFFF;
Port_In_Out(PortA,BIT1,AF_TW_50M); 
break;
case 3:
TIM2_CCMR2&=0xff00;
TIM2_CCMR2|=0x0068;		//PWM1	模式
TIM2_CCER|=0X0100;         //输出使能
TIM2_CCR3=0XFFFF;
Port_In_Out(PortA,BIT2,AF_TW_50M); 
break;

case 4:
TIM2_CCMR2&=0x00FF;
TIM2_CCMR2|=0x6800;		//PWM1	模式
TIM2_CCER|=0X1000;         //输出使能
TIM2_CCR4=0XFFFF;
Port_In_Out(PortA,BIT3,AF_TW_50M); 
break;
}

TIM2_CR1=0x01;	//开启时钟计数
TIM2_EGR=0X01;  //软件产生更新事件
break;

//----------------------------------------------
case 3:
Dev_Sck_En(SckEn_GPIOA,1);     //开启IO口时钟
Dev_Sck_En(SckEn_GPIOB,1);     //开启IO口时钟
Dev_Sck_En(SckEn_AFIO, 1);	   //开启复用时钟
Dev_Sck_En(SckEn_TIM3, 1);     //开启TIM2时钟

TIM3_CR1=0;
TIM3_CR2=0;
TIM3_SMCR=0;
TIM3_DIER=0;
TIM3_SR=0;
TIM3_CNT=0;
TIM3_PSC=0X05;   //f=Fck/PSC+1 =72M/6=12Mhz
TIM3_ARR=0X00;

switch(CH_NO)
{
case 1:
TIM3_CCMR1&=0xff00;
TIM3_CCMR1|=0x0068;		//PWM1	模式
TIM3_CCER|=0X0001;         //输出使能
TIM3_CCR1=0XFFFF;
Port_In_Out(PortA,BIT6,AF_TW_50M); 
break;
case 2:
TIM3_CCMR1&=0x00ff;
TIM3_CCMR1|=0x6800;		 //PWM1	模式
TIM3_CCER|=0X0010;         //输出使能
TIM3_CCR2=0XFFFF;
Port_In_Out(PortA,BIT7,AF_TW_50M); 
break;
case 3:
TIM3_CCMR2&=0xff00;
TIM3_CCMR2|=0x0068;		//PWM1	模式
TIM3_CCER|=0X0100;         //输出使能
TIM3_CCR3=0XFFFF;
Port_In_Out(PortB,BIT0,AF_TW_50M); 
break;

case 4:
TIM3_CCMR2&=0x00FF;
TIM3_CCMR2|=0x6800;	     	//PWM1	模式
TIM3_CCER|=0X1000;         //输出使能
TIM3_CCR4=0XFFFF;
Port_In_Out(PortB,BIT1,AF_TW_50M); 
break;
}

TIM3_CR1=0x01;	//开启时钟计数
TIM3_EGR=0X01;  //软件产生更新事件
break;
//--------------------------------------------------------
case 4:
Dev_Sck_En(SckEn_GPIOB,1);     //开启IO口时钟
Dev_Sck_En(SckEn_AFIO, 1);	   //开启复用时钟
Dev_Sck_En(SckEn_TIM4, 1);     //开启TIM2时钟

TIM4_CR1=0;
TIM4_CR2=0;
TIM4_SMCR=0;
TIM4_DIER=0;
TIM4_SR=0;
TIM4_CNT=0;
TIM4_PSC=0X05;   //f=Fck/PSC+1 =12Mhz
TIM4_ARR=0X00;

switch(CH_NO)
{
case 1:
TIM4_CCMR1&=0xff00;
TIM4_CCMR1|=0x0068;		//PWM1	模式
TIM4_CCER|=0X0001;         //输出使能
TIM4_CCR1=0XFFFF;
Port_In_Out(PortB,BIT6,AF_TW_50M); 
break;
case 2:
TIM4_CCMR1&=0x00ff;
TIM4_CCMR1|=0x6800;		 //PWM1	模式
TIM4_CCER|=0X0010;         //输出使能
TIM4_CCR2=0XFFFF;
Port_In_Out(PortB,BIT7,AF_TW_50M); 
break;
case 3:
TIM4_CCMR2&=0xff00;
TIM4_CCMR2|=0x0068;		//PWM1	模式
TIM4_CCER|=0X0100;         //输出使能
TIM4_CCR3=0XFFFF;
Port_In_Out(PortB,BIT8,AF_TW_50M); 
break;

case 4:
TIM4_CCMR2&=0x00FF;
TIM4_CCMR2|=0x6800;		//PWM1	模式
TIM4_CCER|=0X1000;         //输出使能
TIM4_CCR4=0XFFFF;
Port_In_Out(PortB,BIT9,AF_TW_50M); 
break;
}

TIM4_CR1=0x01;	//开启时钟计数
TIM4_EGR=0X01;  //软件产生更新事件
break;
//------------------------------------------------------------------
case 5:
Dev_Sck_En(SckEn_GPIOA,1);     //开启IO口时钟
Dev_Sck_En(SckEn_AFIO, 1);	   //开启复用时钟
Dev_Sck_En(SckEn_TIM5, 1);     //开启TIM2时钟

TIM5_CR1=0;
TIM5_CR2=0;
TIM5_SMCR=0;
TIM5_DIER=0;
TIM5_SR=0;
TIM5_CNT=0;
TIM5_PSC=0X05;   //f=Fck/PSC+1 =12Mhz
TIM5_ARR=0X00;

switch(CH_NO)
{
case 1:
TIM5_CCMR1&=0xff00;
TIM5_CCMR1|=0x0068;		//PWM1	模式
TIM5_CCER|=0X0001;         //输出使能
TIM5_CCR1=0XFFFF;
Port_In_Out(PortA,BIT0,AF_TW_50M); 
break;
case 2:
TIM5_CCMR1&=0x00ff;
TIM5_CCMR1|=0x6800;		 //PWM1	模式
TIM5_CCER|=0X0010;         //输出使能
TIM5_CCR2=0XFFFF;
Port_In_Out(PortA,BIT1,AF_TW_50M); 
break;
case 3:
TIM5_CCMR2&=0xff00;
TIM5_CCMR2|=0x0068;		//PWM1	模式
TIM5_CCER|=0X0100;         //输出使能
TIM5_CCR3=0XFFFF;
Port_In_Out(PortA,BIT2,AF_TW_50M); 
break;

case 4:
TIM5_CCMR2&=0x00FF;
TIM5_CCMR2|=0x6800;		//PWM1	模式
TIM5_CCER|=0X1000;         //输出使能
TIM5_CCR4=0XFFFF;
Port_In_Out(PortA,BIT3,AF_TW_50M); 
break;
}

TIM5_CR1=0x01;	//开启时钟计数
TIM5_EGR=0X01;  //软件产生更新事件
break;
//-------------------------------------------------------

}
}




//----------------------中断相关函数----------------------

/*
设置向量表偏移地址
NVIC_VectTab:基址
Offset:偏移量		

Flash的NVIC_VectTab：0x08000000
SRAM 的NVIC_VectTab：0x20000000
*/
void Nvic_SetVectorTable(ulong NVIC_VectTab, ulong Offset)	 
{ 	
SCB_VTOR = (ulong)0x00000000;
SCB_VTOR = NVIC_VectTab|(Offset & (ulong)0x1FFFFF80);//设置NVIC的向量表偏移寄存器
//用于标识向量表是在CODE区还是在RAM区
}



//设置NVIC分组
//NVIC_Group:NVIC分组 0~4 总共5组 
//注意优先级不能超过设定的组的范围!否则会有意想不到的错误
//组划分:
//组0:0位抢占优先级,4位响应优先级
//组1:1位抢占优先级,3位响应优先级
//组2:2位抢占优先级,2位响应优先级
//组3:3位抢占优先级,1位响应优先级
//组4:4位抢占优先级,0位响应优先级
//数值越小,越优先
void Nvic_GroupConfig(uchar NVIC_Group)	 
{ 
	ulong temp,temp1;	  
	temp1=(~NVIC_Group)&0x07;//取后三位
	temp1<<=8;
	temp=SCB_AIRCR;   //读取先前的设置
	temp&=0X0000F8FF; //清空先前分组
	temp|=0X05FA0000; //写入钥匙
	temp|=temp1;	   
	SCB_AIRCR=temp;   //设置分组	    	  				   
}

//设置各个中断 
//NVIC_PreemptionPriority:抢占优先级
//NVIC_SubPriority       :响应优先级
//NVIC_Channel           :中断编号
//NVIC_Group             :中断分组 0~4

//设置系统中断分组，相应中断号的优先级，开启全局中断使能
void Nvic_Init(uchar NVIC_Group,uchar NVIC_PreemptionPriority,uchar NVIC_SubPriority,uchar NVIC_Channel)	 
{ 
	ulong temp;	
	uchar IPRADDR=NVIC_Channel/4;  //每组只能存4个,得到组地址 
	uchar IPROFFSET=NVIC_Channel%4;//在组内的偏移
	IPROFFSET=IPROFFSET*8+4;    //得到偏移的确切位置
	Nvic_GroupConfig(NVIC_Group);//设置分组
	temp=NVIC_PreemptionPriority<<(4-NVIC_Group);	  
	temp|=NVIC_SubPriority&(0x0f>>NVIC_Group);
	temp&=0xf;//取低四位

	if(NVIC_Channel<32) 
	NVIC->ISER[0]|=1<<NVIC_Channel;//使能中断位(要清除的话,相反操作就OK)
	else		
	NVIC->ISER[1]|=1<<(NVIC_Channel-32);   
	
	NVIC->IPR[IPRADDR]|=temp<<IPROFFSET;//设置响应优先级和抢断优先级   	    	  				   
}




/******************************************
外部触发中断配置
RorFSR=0 --上升、下降沿都不允许
RorFSR=1 --上升沿触发
RorFSR=2 --下降沿触发
RorFSR=3 --上，下沿都触发
*******************************************/
void EXTI_Config(uchar GPIO_x,uchar BIT_x,uchar RorFSR)
{
Dev_Sck_En(SckEn_AFIO, 1);	        //开启复用时钟
Port_In_Out(GPIO_x,BIT_x,Updown_IN);
GPIO_x&=0xf;

switch(RorFSR)
{
case 0:
EXTI_RTSR&=~((ulong)0x01<<BIT_x);
EXTI_FTSR&=~((ulong)0x01<<BIT_x);
break;

case 1:
EXTI_RTSR|=(ulong)0x01<<BIT_x;
EXTI_FTSR&=~((ulong)0x01<<BIT_x);
break;

case 2:
EXTI_FTSR|=(ulong)0x01<<BIT_x;
EXTI_RTSR&=~((ulong)0x01<<BIT_x);
break;

case 3:
EXTI_RTSR|=(ulong)0x01<<BIT_x;
EXTI_FTSR|=(ulong)0x01<<BIT_x;
break;
}
switch(BIT_x/4)
{
case 0:
AFIO_EXTICR1&=(~((ulong)0x0f<<((BIT_x%4)*4)));
AFIO_EXTICR1|=((ulong)GPIO_x<<((BIT_x%4)*4));
break;
case 1:
AFIO_EXTICR2&=(~((ulong)0x0f<<((BIT_x%4)*4)));
AFIO_EXTICR2|=((ulong)GPIO_x<<((BIT_x%4)*4));
break;
case 2:
AFIO_EXTICR3&=(~((ulong)0x0f<<((BIT_x%4)*4)));
AFIO_EXTICR3|=((ulong)GPIO_x<<((BIT_x%4)*4));
break;
case 3:
AFIO_EXTICR4&=(~((ulong)0x0f<<((BIT_x%4)*4)));
AFIO_EXTICR4|=((ulong)GPIO_x<<((BIT_x%4)*4));
break;

default:
return;
}


}



//清除中断标志
void EXTI_MarkCLR(uchar Lin_x)  
{
EXTI_PR|=(ulong)1 << Lin_x;
}
/*
关闭后，中断标志挂起位将不会记录
=1：开启
线中断使能/关闭
*/
void EXTI_Enable(uchar Lin_x,uchar ENorC)
{
if(ENorC)
EXTI_IMR|=(ulong)0x01<<Lin_x;
else
EXTI_IMR&=~((ulong)0x01<<Lin_x);
}


//查询具体是哪路中断
ulong EXTI_LineINQ(void)
{
	return EXTI_PR&0x0007ffff;
}

//清除中断标志,入口是位的方式，可以一次清除多个标志
void EXTI_MarkCLR_BIT(ulong BitCLR)
{
	EXTI_PR=BitCLR;
}

/*
0：JTAG正常
1：JTAG做普通IO口,但SWD可用
*/
void JTAG_ENable(uchar JTAG_STA)
{
if(JTAG_STA==0)
{
AFIO_MAPR=0X00000000;
}
else
{
Dev_Sck_En(SckEn_AFIO, 1);
AFIO_MAPR=0X02000000;  //JTAG做普通IO口，但SWD可用
}
return;

}






