#include 	"stm32f10x_Init.h"
#include	"stm32f10x_Reg.h"
#include	"stm32f10x_type.h"
#include 	"stdio.h"



struct SYSCLK_DAT  SYSCLK_DAT_reg;   //��������ʱ����Ϣ�Ĵ�����ȫ�֣���
struct SYSCLK_DAT *SYSCLK_DAT_pat=&SYSCLK_DAT_reg;




//�ض���fputc���� ,����printf
int fputc(int ch, FILE *f)
{      
	while((USART1_SR&0x40)==0);//ѭ������,ֱ���������   
	USART1_DR=(u8)ch;      
	return ch;
}




//ϵͳĬ�ϵĺ��������ļ�����ȱ�ٵ�ʱ�䶨�庯��--------
int SystemInit(void)
{
return(0);
}



/*****************************************************
ע�⣺ֻ�����ⲿʱ����PLL�����
����ʱ�ӣ���Ƶϵ������Ƶϵ����������豸��ʱ��
�Լ���Ӧ����ؼĴ����ķ�Ƶϵ��ֵ��
*****************************************************/
void SYSCLK_Count(void)
{
SYSCLK_DAT_reg.SYSCLK_DAT_SYS=1000000*((ulong)Sys_HSE*(ulong)Sys_PLL);//---ϵͳʱ��
//----AHB��ؼ���---

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

SYSCLK_DAT_reg.SYSCLK_DAT_AHB=SYSCLK_DAT_reg.SYSCLK_DAT_SYS/Sys_DIV_AHB;                //AHB��ʵʱ��

//---APB2��ؼ���---
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
SYSCLK_DAT_reg.SYSCLK_DAT_APB2=SYSCLK_DAT_reg.SYSCLK_DAT_AHB/Sys_DIV_APB2;	  //APB2��ʵʱ��

//---APB1��ؼ���---
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
SYSCLK_DAT_reg.SYSCLK_DAT_APB1=SYSCLK_DAT_reg.SYSCLK_DAT_APB2/Sys_DIV_APB1;	  //APB1��ʵʱ��
}






/************************************************************************
ϵͳ��ʼ������Ŀ�У�
1��Ĭ����PLL9��Ƶ
2: ѡ��HSEΪPLL���룬��PLLΪϵͳʱ�ӣ�APB1ΪHCLK/4,��������ʱ�Ӷ�Ϊ���
3: 8888888�κ�PLL��HSEʱ�ӻ�δ������ϵͳ��ʼ��ʧ�ܣ� ����1
4: Ĭ��ֻ������PA~G�˿ڵ�ʱ�ӣ�������Ƭ������ʱ�Ӷ�δ����!!
5��Ĭ��Ϊ���1����Ϊ��©��������10M
************************************************************************/


/*----------------------------------------

mode ��D3,D2ֵ���壺

������ģʽ(mode[1:0]=00)�� 
00��ģ������ģʽ 
01����������ģʽ(��λ���״̬) 
10������/��������ģʽ 
11������ 

�����ģʽ(mode[1:0]>00)�� 
00��ͨ���������ģʽ 
01��ͨ�ÿ�©���ģʽ 
10�����ù����������ģʽ
11�����ù��ܿ�©���ģʽ 

//---------------------------------------

mode ��D1��D0ֵ���壺
00������ģʽ  (��λ���״̬) 
01�����ģʽ������ٶ�10MHz
10�����ģʽ������ٶ�2MHz 
11�����ģʽ������ٶ�50MHz 
------------------------------------------*/
void Port_In_Out(uchar port,uchar p_bit,uchar mode)
{
uchar u8_temp;
ulong u32_temp1,u32_temp2;

if((p_bit>15)||(port>6)||(mode>16))
return;                  //������󣬷���

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
if(ON_OFF==0)   //�ر�ĳ�豸��ʱ��
 {
		if(Sck_EnNO<32)        //˵����AHB�����ϵ��豸
		{
		 RCC_AHBENR&=~((ulong)(1<<Sck_EnNO));		 
		 return; 
		}
		if((63<Sck_EnNO)&&(31<Sck_EnNO<96))	   //˵����APB2�����ϵ��豸
		{
		 RCC_APB2ENR&=~((ulong)(1<<(Sck_EnNO-0x40)));
		 return;
		}
		if((127<Sck_EnNO)&&(31<Sck_EnNO<160))	  //˵����APB1�����ϵ��豸
		{
		 RCC_APB1ENR&=~((ulong)(1<<(Sck_EnNO-0x80)));
		 return;
		}
    return;
   }
  else			   //�����ǿ���ĳ�豸��ʱ��
  {
		if(Sck_EnNO<32)        //˵����AHB�����ϵ��豸
		{
		 RCC_AHBENR|=((ulong)(1<<Sck_EnNO));		 
		 return; 
		}
		if((63<Sck_EnNO)&&(Sck_EnNO<96))	   //˵����APB2�����ϵ��豸
		{
		 RCC_APB2ENR|=((ulong)(1<<(Sck_EnNO-0x40)));
		 return;
		}
		if((127<Sck_EnNO)&&(Sck_EnNO<160))	  //˵����APB1�����ϵ��豸
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
	SYSCLK_Count();     	//����ʱ�ӣ���Ƶϵ������Ƶϵ��������������豸��ʱ��                 
  	RCC_AHBENR = 0x00000014;        //˯��ģʽ�����SRAMʱ��ʹ��.�����ر�.	  
  	RCC_APB2ENR =0x00000000;        //����ʱ�ӹر�.			   
  	RCC_APB1ENR =0x00000000; 
 
	RCC_CFGR=  0x00010000;        //9��Ƶ��HSEΪPLL���룬����ʱPLL��δ���ó�ϵͳʱ�ӣ���ΪPLL��δ����				    
	RCC_CFGR|= (SYSCLK_DAT_reg.SYSCLK_DAT_R_APB2)<<11;	  //APB2��Ƶϵ���趨
	RCC_CFGR|= (SYSCLK_DAT_reg.SYSCLK_DAT_R_APB1)<<8;	  //APB1��Ƶϵ���趨
	RCC_CFGR|= (SYSCLK_DAT_reg.SYSCLK_DAT_R_AHB)  <<4;	  //AHB��Ƶϵ���趨
	
	RCC_CFGR|= 0x00000000 <<14;	  //ADCʱ�ӣ���APB2����RCC_CFGR�Ĵ�����15-14λ����Ƶ�õ��������趨Ϊ76/2=36MHZ
	
	RCC_CFGR|=(Sys_PLL-2)<<18;		 //��Ƶϵ��;
	RCC_CR  =0x00010001;             //�����ⲿ�� 
	while(!(RCC_CR&0x00020000));     //�ȴ��ⲿʱ�Ӿ���
	FLASH_ACR|=0x32;                 //FLASH��ʱ2����λ
	RCC_CR|=0x01000000;              //����PLL��	 
	while(!(RCC_CR&0x02000000));     //�ȴ�PLL���� 
	  
	RCC_CFGR|=0x00000002;            //ѡ��PLLΪϵͳʱ��
	temp=0;
	while(temp!=0x02)                //�ȴ�PLL��Ϊϵͳʱ�����óɹ�
	{   
	temp=RCC_CFGR>>2;
	temp&=0x03;
	}  
	RCC_CR&=0xfffffffe;              //�ر��ڲ���  
    RCC_APB2ENR=0x000001fc;          //Ĭ��ֻ������PA~G�˿ڵ�ʱ�ӣ���
//----------------------------������GPIO�ڵ�Ĭ������----------------------
GPIOA_ODR=0xffffffff;
GPIOB_ODR=0xffffffff;
GPIOC_ODR=0xffffffff;
GPIOD_ODR=0xffffffff;
GPIOE_ODR=0xffffffff;
GPIOF_ODR=0xffffffff;
GPIOG_ODR=0xffffffff;    //Ĭ��Ϊ���1����Ϊ��©��������10M

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
�����ر��ڲ�RC����
Flash�������Ҫ�����ڲ�RC����

���Ǵ�����ͣ��ģʽ�����ⲿʱ��ʧЧ��ʱ��HSI��Ӳ������

*/
void Sys_OpenHSI(uchar ON_OFF)
{
if(ON_OFF)
{
RCC_CR|=0X00000001;
while( (RCC_CR&0xfffffffd) ==0); //�ȴ��ȶ�
}
else
{
RCC_CR&=0xfffffffe;    
while( (RCC_CR&0xfffffffd) ); //�ȴ��رգ�Ҫ6��ʱ������
}

}



void Soft_Reset(void)	    //�����λ
{

SCB_AIRCR=0x05fa0000|(ulong)0x04;
while(1);

}


uchar DMA1_STA_HTIF(uchar DMA1_NO)	//0���봫����ɱ�־
{if(DMA1_ISR&((ulong)1<<((DMA1_NO-1)*4+2))) return 0; else return 1; }

void DMA1_STA_CHTIF(uchar DMA1_NO)	//����봫����ɱ�־
{DMA1_IFCR|=(ulong)1<<((DMA1_NO-1)*4+2);}

void DMA1_STA_CTCIF(uchar DMA1_NO)	//����봫����ɱ�־
{DMA1_IFCR|=(ulong)1<<((DMA1_NO-1)*4+1);}

uchar DMA1_STA_TCIF(uchar DMA1_NO) 	//0��ȫ������ɱ�־
{if(DMA1_ISR&((ulong)1<<((DMA1_NO-1)*4+1))) return 0; else return 1;}

//UART MDA �������ú���
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
//----����Ĭ�ϣ�8λ����λ��1��ֹͣλ���첽��DMA�ر� ��

ulong  usart_Fclk;

float temp;
u16 mantissa;
u16 fraction;

SYSCLK_Count();     	//����ʱ��,��Ƶϵ��,��Ƶϵ��������������豸��ʱ�� 
Dev_Sck_En(SckEn_AFIO, 1);

if(usart_NO==1)
usart_Fclk=SYSCLK_DAT_reg.SYSCLK_DAT_APB2;	   //APB2���ߵ�ʱ��\\
else
usart_Fclk=SYSCLK_DAT_reg.SYSCLK_DAT_APB1;	   //APB2���ߵ�ʱ��\\

   	if(usart_NO==1)
	temp=(float)(usart_Fclk)/(usart_BB*8);//�õ�USARTDIV
	else
	temp=(float)(usart_Fclk)/(usart_BB*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 



switch(usart_NO)
{
case 1:
Dev_Sck_En(SckEn_GPIOA,1);  //����IO��ʱ��
Dev_Sck_En(SckEn_USART1,1); //����USARTʱ��
Port_In_Out(PortA,BIT9, 0x0b);  // TX 
Port_In_Out(PortA,BIT10,0x08);  // RX   �������Ź���
USART1_BRR=mantissa;       //����������
USART1_CR1 =0x00;
USART1_CR1|=(1<<3);        //ʹ��USARRT ����
USART1_CR1|=(1<<2);        //ʹ��USARRT	����
USART1_CR1|=(1<<13);       //ʹ��USARRT
USART1_SR|=0x00;
break;


case 2:
Dev_Sck_En(SckEn_GPIOA,1);  //����IO��ʱ��
Dev_Sck_En(SckEn_USART2,1); //����USARTʱ��
Port_In_Out(PortA,BIT2,0x0b);  // TX 
Port_In_Out(PortA,BIT3,0x08);  // RX   �������Ź���

USART2_BRR=mantissa;      //����������
USART2_CR1 =0x00;
USART2_CR1|=(1<<3);        //ʹ��USARRT ����
USART2_CR1|=(1<<2);        //ʹ��USARRT	����
USART2_CR1|=(1<<13);       //ʹ��USARRT
USART2_SR|=0x00;
break;

case 3:
Dev_Sck_En(SckEn_GPIOB,1);  //����IO��ʱ��
Dev_Sck_En(SckEn_USART3,1); //����USARTʱ��
Port_In_Out(PortB,BIT10,0x0b);  // TX 
Port_In_Out(PortB,BIT11,0x08);  // RX   �������Ź���

USART3_BRR=mantissa;      //����������
USART3_CR1 =0x00;
USART3_CR1|=(1<<3);        //ʹ��USARRT ����
USART3_CR1|=(1<<2);        //ʹ��USARRT	����
USART3_CR1|=(1<<13);       //ʹ��USARRT
USART3_SR|=0x00;
break;


case 4:
Dev_Sck_En(SckEn_GPIOC,1);  //����IO��ʱ��
Dev_Sck_En(SckEn_USART4,1); //����USARTʱ��
Port_In_Out(PortC,BIT10,0x0b);  // TX 
Port_In_Out(PortC,BIT11,0x08);  // RX   �������Ź���

USART4_BRR=mantissa;      //����������
USART4_CR1 =0x00;
USART4_CR1|=(1<<3);        //ʹ��USARRT ����
USART4_CR1|=(1<<2);        //ʹ��USARRT	����
USART4_CR1|=(1<<13);       //ʹ��USARRT
USART4_SR|=0x00;
break;


case 5:
Dev_Sck_En(SckEn_GPIOC,1);  //����IO��ʱ��
Dev_Sck_En(SckEn_GPIOD,1);  //����IO��ʱ��
Dev_Sck_En(SckEn_USART5,1); //����USARTʱ��
Port_In_Out(PortC,BIT12,0x0b);  // TX 
Port_In_Out(PortD,BIT2, 0x08);  // RX   �������Ź���

USART5_BRR=mantissa;      //����������
USART5_CR1 =0x00;
USART5_CR1|=(1<<3);        //ʹ��USARRT ����
USART5_CR1|=(1<<2);        //ʹ��USARRT	����
USART5_CR1|=(1<<13);       //ʹ��USARRT
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
clr=USART1_DR;  //������ձ�־λ
clr=USART1_SR;  //������ر�־λ1
clr=USART1_CR1; //������ر�־λ2
}

void UART2_Clear_RL_Mark(void)
{
volatile uchar clr=0;
clr=USART2_DR;  //������ձ�־λ
clr=USART2_SR;  //������ر�־λ1
clr=USART2_CR1; //������ر�־λ2
}

void UART3_Clear_RL_Mark(void)
{
volatile uchar clr=0;
clr=USART3_DR;  //������ձ�־λ
clr=USART3_SR;  //������ر�־λ1
clr=USART3_CR1; //������ر�־λ2
}

/*******************************

UART DMA

********************************/

ulong UART1_DMA_ConfigVal=0;

void USART1_Send_DMA_Init(void)
{
	
USART1_CR3|=(1<<7);      //ʹ��USART DMA,����
	
//------------------DMA����----------------------------
Dev_Sck_En(SckEn_DMA1,1);      //����DMA1ʱ��
DMA1_CCR4=0;
//DMA1_CCR4|=(1<<14);   //�����洢�����洢��ģʽ
DMA1_CCR4|=0x01<<12;    //��ͨ�����ȼ�
//DMA1_CCR4|=0x01<<10;    //��ͨ���洢�����ݿ��,=0,8
//DMA1_CCR4|=0x01<<8;     //��ͨ���������ݿ��,=0,8

DMA1_CCR4|=1<<7;		//Ϊ1��ִ�д洢����ַ����ģʽ
//DMA1_CCR4|=1<<6;		//Ϊ1��ִ�������ַ����ģʽ
//DMA1_CCR4|=1<<5;		//Ϊ1��ѭ��ģʽ
DMA1_CCR4|=1<<4;		//���ݷ���0�����������1���Ӵ洢����
//DMA1_CCR4|=1<<2;		//����봫���ж�
//DMA1_CCR4|=1<<1;		//�������ж�
DMA1_CPAR4=(ulong)(&USART1_DR);	    //�����ַ

UART1_DMA_ConfigVal=DMA1_CCR4;
}


void USART1_Send_DMA_Running(uchar *buf,uchar Len)
{
	
DMA1_CCR4=0;
DMA1_CNDTR4=Len;   				//һ�δ��������
DMA1_CMAR4=(ulong)(buf);	//�Ĵ�����ַ
DMA1_CCR4=UART1_DMA_ConfigVal;
DMA1_CCR4|=1;	        //Ϊ1������ͨ������
	
}

/*
����0������δ��ɣ�1���Ѿ����
*/
uchar USART1_Send_DMA_WaitFinish(void)
{
	
if(DMA1_ISR & ((ulong)1<<13) )   //�ȴ��ϴδ������
{
DMA1_IFCR |= (1<<13);//���

return 1;
}
return 0;

}


//----------------------------------------------------

ulong UART2_DMA_ConfigVal=0;
void USART2_Send_DMA_Init(void)
{
	
USART2_CR3|=(1<<7);      //ʹ��USART DMA,����
	
//------------------DMA����----------------------------
Dev_Sck_En(SckEn_DMA1,1);      //����DMA1ʱ��
	
DMA1_CCR7=0;
//DMA1_CCR7|=(1<<14);   //�����洢�����洢��ģʽ
DMA1_CCR7|=0x01<<12;    //��ͨ�����ȼ�
//DMA1_CCR7|=0x01<<10;    //��ͨ���洢�����ݿ��,=0,8
//DMA1_CCR7|=0x01<<8;     //��ͨ���������ݿ��,=0,8

DMA1_CCR7|=1<<7;		//Ϊ1��ִ�д洢����ַ����ģʽ
//DMA1_CCR7|=1<<6;		//Ϊ1��ִ�������ַ����ģʽ
//DMA1_CCR7|=1<<5;		//Ϊ1��ѭ��ģʽ
DMA1_CCR7|=1<<4;		//���ݷ���0�����������1���Ӵ洢����
//DMA1_CCR7|=1<<2;		//����봫���ж�
//DMA1_CCR7|=1<<1;		//�������ж�
DMA1_CPAR7=(ulong)(&USART2_DR);	    //�����ַ

UART2_DMA_ConfigVal=DMA1_CCR7;
}


void USART2_Send_DMA_Running(uchar *buf,uchar Len)
{
	
DMA1_CCR7=0;
DMA1_CNDTR7=Len;   				//һ�δ��������
DMA1_CMAR7=(ulong)(buf);	//�Ĵ�����ַ
DMA1_CCR7=UART2_DMA_ConfigVal;
DMA1_CCR7|=1;	        //Ϊ1������ͨ������
	
}

/*
����0������δ��ɣ�1���Ѿ����
*/
uchar USART2_Send_DMA_WaitFinish(void)
{
	
if(DMA1_ISR & ((ulong)1<<25) )   //�ȴ��ϴδ������
{
DMA1_IFCR |= (1<<25);//���

return 1;
}
return 0;

}


//----------------------------------------

ulong UART3_DMA_ConfigVal=0;

void USART3_Send_DMA_Init(void)
{
	
USART3_CR3|=(1<<7);      //ʹ��USART DMA,����
	
//------------------DMA����----------------------------
Dev_Sck_En(SckEn_DMA1,1);      //����DMA1ʱ��
DMA1_CCR2=0;
//DMA1_CCR2|=(1<<14);   //�����洢�����洢��ģʽ
DMA1_CCR2|=0x01<<12;    //��ͨ�����ȼ�
//DMA1_CCR2|=0x01<<10;    //��ͨ���洢�����ݿ��,=0,8
//DMA1_CCR2|=0x01<<8;     //��ͨ���������ݿ��,=0,8

DMA1_CCR2|=1<<7;		//Ϊ1��ִ�д洢����ַ����ģʽ
//DMA1_CCR2|=1<<6;		//Ϊ1��ִ�������ַ����ģʽ
//DMA1_CCR2|=1<<5;		//Ϊ1��ѭ��ģʽ
DMA1_CCR2|=1<<4;		//���ݷ���0�����������1���Ӵ洢����
//DMA1_CCR2|=1<<2;		//����봫���ж�
//DMA1_CCR2|=1<<1;		//�������ж�
DMA1_CPAR2=(ulong)(&USART3_DR);	    //�����ַ

UART3_DMA_ConfigVal=DMA1_CCR2;
}


void USART3_Send_DMA_Running(uchar *buf,uchar Len)
{
	
DMA1_CCR2=0;
DMA1_CNDTR2=Len;   				//һ�δ��������
DMA1_CMAR2=(ulong)(buf);	//�Ĵ�����ַ
DMA1_CCR2=UART3_DMA_ConfigVal;
DMA1_CCR2|=1;	        //Ϊ1������ͨ������
	
}

/*
����0������δ��ɣ�1���Ѿ����
*/
uchar USART3_Send_DMA_WaitFinish(void)
{
	
if(DMA1_ISR & ((ulong)1<<5) )   //�ȴ��ϴδ������
{
DMA1_IFCR |= (1<<5);//���

return 1;
}
return 0;

}
void UART1_Send_String(uchar *path)//���ڷ��ͣ�����0���
{
	while( (*path)!= '\0')
	{
		USART1_TxDat(*path);
		path++;	
	}
}

void UART2_Send_String(uchar *path)//���ڷ��ͣ�����0���
{
	while( (*path)!= '\0')
	{
		USART2_TxDat(*path);
		path++;	
	}
}



void UART3_Send_String(uchar *path)//���ڷ��ͣ�����0���
{
	while( (*path)!= '\0')
	{
		USART3_TxDat(*path);
		path++;	
	}
}

void UART4_Send_String(uchar *path)//���ڷ��ͣ�����0���
{
	while( (*path)!= '\0')
	{
		USART4_TxDat(*path);
		path++;	
	}
}


void UART5_Send_String(uchar *path)//���ڷ��ͣ�����0���
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
usart_Fclk=SYSCLK_DAT_reg.SYSCLK_DAT_APB2;	   //APB2���ߵ�ʱ��\\
else
usart_Fclk=SYSCLK_DAT_reg.SYSCLK_DAT_APB1;	   //APB2���ߵ�ʱ��\\

if(usart_NO==1)
temp=(float)(usart_Fclk)/(usart_BB*8);//�õ�USARTDIV
else
temp=(float)(usart_Fclk)/(usart_BB*16);//�õ�USARTDIV
	
mantissa=temp;				 //�õ���������
fraction=(temp-mantissa)*16; //�õ�С������	 
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


static uchar ADCChgNum=0;   //ADCת����ͨ����Ŀ����ÿ����һ�κ�������һ�Σ�
static uchar ADCDMAEn=0;    //Ϊ1����MDA����

/***********************************
ADC��ʼ�����ã�������ADC��Ч
SCAN		ɨ��ģʽ
DMA			����DMA
Loop		ѭ��ת��
DatLay		���ݶ����ʽ
************************************/
void ADC1_Init(uchar SCAN,uchar DMA,uchar Loop,uchar DatLay)
{
Dev_Sck_En(SckEn_AFIO, 1);	   //��������ʱ��
Dev_Sck_En(SckEn_ADC1,1);      //����ADC1ʱ��

ADCDMAEn=DMA;	
ADC1_CR2=0;
ADC1_CR1|=(SCAN<<8);        //ɨ��ģʽ
ADC1_CR2|=(DMA <<8);        //DMAģʽ
ADC1_CR2|=(Loop<<1);        //����ת��ģʽ����
ADC1_CR2|=(DatLay<<11);    //���������

ADCChgNum=0;   //ת��������0��
}










/*****************************************************
ADC����ͨ�����ã��ɶ���ظ�����
chann_val :ͨ��ֵ��ȡֵ0~15
chann_ord����ͨ���ڹ���ת���µ�˳��ȡֵ1~16
ADCChgNumrst�� �Ƿ���0ת������Ϊ0 �����Ǽ����ۼӣ� Ϊ0����0��
*****************************************************/
void  ADC1_ChannelSET(uchar chann_val,uchar chann_time,uchar chann_ord,uchar ADCChgNumrst)
{

if(chann_val>15) return;
if((chann_ord>16)||(chann_ord==0)) return;


//----д����ʱ�䣡
if(chann_val<10)    //0��ͨ��9
ADC1_SMPR2|=((ulong)chann_time<<(chann_val*3));
else
ADC1_SMPR1|=((ulong)chann_time<<((chann_val-10)*3));

if(ADCChgNumrst==0)  	 //Ϊ0����0��
ADCChgNum=0;

//ת����������----
ADC1_SQR1&=~((ulong)0X0F<<20);     //��λת������
ADC1_SQR1|=((ulong)ADCChgNum<<20); //���¸���,00��ת������Ϊ1��

ADCChgNum++;   //ת��ͨ����++

switch((chann_ord-1)/6)	 //ת��˳�����
{
case 0:
ADC1_SQR3&=   ~((ulong)0X1F<<((chann_ord-1)*5));     //��λ
ADC1_SQR3|=((ulong)chann_val<<((chann_ord-1)*5)); //д��ֵ
break;

case 1:
ADC1_SQR2&=   ~((ulong)0X1F<<((chann_ord-7)*5));  //��λ
ADC1_SQR2|=((ulong)chann_val<<((chann_ord-7)*5)); //д��ֵ
break;

case 2:
ADC1_SQR1&=    ~((ulong)0X1F<<((chann_ord-13)*5));     //��λ
ADC1_SQR1|=((ulong)chann_val<<((chann_ord-13)*5)); //д��ֵ
break;
default: 
break;
}

switch(chann_val)
{
case 0:
Dev_Sck_En(SckEn_GPIOA,1);
Port_In_Out(PortA,BIT0,0x00);  //ADC1_IN0 //�������Ź���,ģ������
break;
case 1:
Dev_Sck_En(SckEn_GPIOA,1);
Port_In_Out(PortA,BIT1,0x00);  //ADC1_IN1 //�������Ź���,ģ������
break;
case 2:
Dev_Sck_En(SckEn_GPIOA,1);
Port_In_Out(PortA,BIT2,0x00);  //ADC1_IN2 //�������Ź���,ģ������

case 3:
Dev_Sck_En(SckEn_GPIOA,1);
Port_In_Out(PortA,BIT3,0x00);  //ADC1_IN3 //�������Ź���,ģ������
break;
case 4:
Dev_Sck_En(SckEn_GPIOA,1);
Port_In_Out(PortA,BIT4,0x00);  //ADC1_IN4 //�������Ź���,ģ������
break;
case 5:
Dev_Sck_En(SckEn_GPIOA,1);
Port_In_Out(PortA,BIT5,0x00);  //ADC1_IN5 //�������Ź���,ģ������
break;
case 6:
Dev_Sck_En(SckEn_GPIOA,1);
Port_In_Out(PortA,BIT6,0x00);  //ADC1_IN6 //�������Ź���,ģ������
break;
case 7:
Dev_Sck_En(SckEn_GPIOA,1);
Port_In_Out(PortA,BIT7,0x00);  //ADC1_IN7 //�������Ź���,ģ������
break;

case 8:
Dev_Sck_En(SckEn_GPIOB,1);
Port_In_Out(PortB,BIT0,0x00);  //ADC1_IN8 //�������Ź���,ģ������
break;
case 9:
Dev_Sck_En(SckEn_GPIOB,1);
Port_In_Out(PortB,BIT1,0x00);  //ADC1_IN9 //�������Ź���,ģ������	
break;
case 10:
Dev_Sck_En(SckEn_GPIOC,1);
Port_In_Out(PortC,BIT0,0x00);  //ADC1_IN10//�������Ź���,ģ������
break;
case 11:
Dev_Sck_En(SckEn_GPIOC,1);
Port_In_Out(PortC,BIT1,0x00);  //ADC1_IN11//�������Ź���,ģ������
break;
case 12:
Dev_Sck_En(SckEn_GPIOC,1);
Port_In_Out(PortC,BIT2,0x00);  //ADC1_IN12//�������Ź���,ģ������
break;
case 13:
Dev_Sck_En(SckEn_GPIOC,1);
Port_In_Out(PortC,BIT3,0x00);  //ADC1_IN13//�������Ź���,ģ������
break;
case 14:
Dev_Sck_En(SckEn_GPIOC,1);
Port_In_Out(PortC,BIT4,0x00);  //ADC1_IN14//�������Ź���,ģ������
break;
case 15:
Dev_Sck_En(SckEn_GPIOC,1);
Port_In_Out(PortC,BIT5,0x00);  //ADC1_IN15//�������Ź���,ģ������
break;
default: 
break;
}


}





/*
����ADC1_Init�� ADC1_ChannelSET�󣬱���Կ���ADCת��
�ú���ֻ�ܵ��ε��ã�����DMAģʽ�µĹ���ת��˳����ҵ�
ADCBuffer:DMAģʽ�� ��ADC���ݱ���ĵ�ַ��ע�ⳤ��>=ADCChgNum;
*/
void ADC1_Start(usint * ADCBuffer)
{
usint i=0;
	

if(ADCDMAEn)
{
Dev_Sck_En(SckEn_DMA1,1);      //����DMA1ʱ��

DMA1_CCR1=0;
//DMA1_CCR1|=(1<<14);   //�����洢�����洢��ģʽ
DMA1_CCR1|=0x01<<12;    //��ͨ�����ȼ�
DMA1_CCR1|=0x01<<10;	//��ͨ���洢�����ݿ��16
DMA1_CCR1|=0x01<<8;		//��ͨ���������ݿ��16
DMA1_CCR1|=1<<7;		//Ϊ1��ִ�д洢����ַ����ģʽ
//DMA1_CCR1|=1<<6;		//Ϊ1��ִ�������ַ����ģʽ
DMA1_CCR1|=1<<5;		//Ϊ1��ѭ��ģʽ
//DMA1_CCR1|=1<<4;		//���ݷ���0�����������1���Ӵ洢����

DMA1_CNDTR1=ADCChgNum;   		//һ�δ����������

DMA1_CPAR1=(ulong)(&ADC1_DR);	  //�����ַ
DMA1_CMAR1=(ulong)(ADCBuffer);	  //�Ĵ�����ַ
DMA1_CCR1|=1<<0;	              //Ϊ1������ͨ������
}	

ADC1_CR2|=(1<<0);   //����ADC��Դ��
for(i=0;i<1000;i++);  //��ʱ

ADC1_CR2|=(1<<3);
while(ADC1_CR2&(1<<3));  //�ȴ�У׼���
ADC1_CR2|=(1<<2);
while(ADC1_CR2&(1<<2));  //�ȴ�У׼���
ADC1_CR2|=(1<<0);        //����ADCת����

}





//--------------------------------------------------------------------------

void SPI_Init(uchar spi_NO,uchar spi_speed)
{
ulong REG_temp1=0;
ulong REG_temp2=0;

//REG_temp1|=1<<15;    //1������˫��ģʽ
//REG_temp1|=1<<14;    //1������˫��ģʽ�£�ֻ����0��ֻ��
//REG_temp1|=1<<13;    //1��CRCʹ��
//REG_temp1|=1<<12;    //1��CRC���ֵ
//REG_temp1|=1<<11;    //0��8λģʽ��1��16λ
//REG_temp1|=1<<10;    //˫��ģʽ�£�0��ȫ˫����1��ֻ����
REG_temp1|=1<<9;     //0����ֹ�������Ƭѡ��1�������������SII����
REG_temp1|=1<<8;     //1������Ƭѡ��1<<9=1ʱ��Ч��
//REG_temp1|=1<<7;     //0��MSB�ȷ���1��LSB�ȷ�
REG_temp1|=(ulong)spi_speed<<3;  //�ٶ�����  Fplk/16
REG_temp1|=1<<2;     //0:����Ϊ��ģʽ��1����ģʽ
//REG_temp1|=1<<1;     //���Կ��� 0������ʱSCKΪ0
//REG_temp1|=1<<0;     //��λ���� 0����һ��ʱ�ӱ��ز�����һ������

//REG_temp2|=1<<7;   //1:���ͻ��������ж�ʹ��
//REG_temp2|=1<<6;   //1���������ǿ��ж�ʹ��
//REG_temp2|=1<<5;   //1�������ж�ʹ��
//REG_temp2|=1<<2;   //0����ֹ��ģʽʱSS���
//REG_temp2|=1<<1;   //1��TX����DMAʹ��
//REG_temp2|=1<<0;   //1��RX����DMAʹ��

if(REG_temp2&0x03)
Dev_Sck_En(SckEn_DMA1,1);      //����DMA1ʱ��


switch(spi_NO)
{
case 1:
Dev_Sck_En(SckEn_GPIOA,1);     //����IO��ʱ��
Dev_Sck_En(SckEn_AFIO,1);	   //��������ʱ��
Dev_Sck_En(SckEn_SPI1,1);      //����SPI1ʱ��

Port_In_Out(PortA,BIT5,0x0b);   //SPI1_SCK������������� ����ģʽ
Port_In_Out(PortA,BIT6,0x08);	//SPI1_MISO�����������룬����ģʽ
Port_In_Out(PortA,BIT7,0x0b);	//SPI1_MOSI�����������������ģʽ

SPI1_CR1=REG_temp1;
SPI1_CR2=REG_temp2;
SPI1_CR1|=1<<6;      //����SPI1
break;

case 2:

Port_In_Out(PortB,BIT13,0x0b);  //SPI2_SCK������������� ����ģʽ
Port_In_Out(PortB,BIT14,0x08);	//SPI2_MISO�����������룬����ģʽ
Port_In_Out(PortB,BIT15,0x0b);	//SPI2_MOSI�����������������ģʽ

Dev_Sck_En(SckEn_GPIOB,1);     //����IO��ʱ��
Dev_Sck_En(SckEn_AFIO, 1);	   //��������ʱ��
Dev_Sck_En(SckEn_SPI2,1);      //����SPI2ʱ��

SPI2_CR1=REG_temp1;
SPI2_CR2=REG_temp2;
SPI2_CR1|=1<<6;      //����SPI2
break;

case 3:
Dev_Sck_En(SckEn_GPIOB,1);     //����IO��ʱ��
Dev_Sck_En(SckEn_AFIO, 1);	   //��������ʱ��
Dev_Sck_En(SckEn_SPI3, 1);      //����SPI3ʱ��

Port_In_Out(PortB,BIT3,0x0b);   //SPI1_SCK������������� ����ģʽ
Port_In_Out(PortB,BIT4,0x08);	//SPI1_MISO�����������룬����ģʽ
Port_In_Out(PortB,BIT5,0x0b);	//SPI1_MOSI�����������������ģʽ
SPI3_CR1=REG_temp1;
SPI3_CR2=REG_temp2;
SPI3_CR1|=1<<6;      //����SPI3

break;

default: 
return;
}
}

//��ʱ������豸����-----------
//PWMƪ
//ע��ͨ�ö�ʱ����APCK1==1ʱ�Ǳ�Ƶ�ģ�72MHZ

void PWM_Init(uchar TIM_NO,uchar CH_NO)
{

switch(TIM_NO)
{
case 2:
Dev_Sck_En(SckEn_GPIOA,1);     //����IO��ʱ��
Dev_Sck_En(SckEn_AFIO, 1);	   //��������ʱ��
Dev_Sck_En(SckEn_TIM2, 1);     //����TIM2ʱ��

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
TIM2_CCMR1|=0x0068;		//PWM1	ģʽ
TIM2_CCER|=0X0001;         //���ʹ��
TIM2_CCR1=0XFFFF;
Port_In_Out(PortA,BIT0,AF_TW_50M); 
break;
case 2:
TIM2_CCMR1&=0x00ff;
TIM2_CCMR1|=0x6800;		 //PWM1	ģʽ
TIM2_CCER|=0X0010;         //���ʹ��
TIM2_CCR2=0XFFFF;
Port_In_Out(PortA,BIT1,AF_TW_50M); 
break;
case 3:
TIM2_CCMR2&=0xff00;
TIM2_CCMR2|=0x0068;		//PWM1	ģʽ
TIM2_CCER|=0X0100;         //���ʹ��
TIM2_CCR3=0XFFFF;
Port_In_Out(PortA,BIT2,AF_TW_50M); 
break;

case 4:
TIM2_CCMR2&=0x00FF;
TIM2_CCMR2|=0x6800;		//PWM1	ģʽ
TIM2_CCER|=0X1000;         //���ʹ��
TIM2_CCR4=0XFFFF;
Port_In_Out(PortA,BIT3,AF_TW_50M); 
break;
}

TIM2_CR1=0x01;	//����ʱ�Ӽ���
TIM2_EGR=0X01;  //������������¼�
break;

//----------------------------------------------
case 3:
Dev_Sck_En(SckEn_GPIOA,1);     //����IO��ʱ��
Dev_Sck_En(SckEn_GPIOB,1);     //����IO��ʱ��
Dev_Sck_En(SckEn_AFIO, 1);	   //��������ʱ��
Dev_Sck_En(SckEn_TIM3, 1);     //����TIM2ʱ��

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
TIM3_CCMR1|=0x0068;		//PWM1	ģʽ
TIM3_CCER|=0X0001;         //���ʹ��
TIM3_CCR1=0XFFFF;
Port_In_Out(PortA,BIT6,AF_TW_50M); 
break;
case 2:
TIM3_CCMR1&=0x00ff;
TIM3_CCMR1|=0x6800;		 //PWM1	ģʽ
TIM3_CCER|=0X0010;         //���ʹ��
TIM3_CCR2=0XFFFF;
Port_In_Out(PortA,BIT7,AF_TW_50M); 
break;
case 3:
TIM3_CCMR2&=0xff00;
TIM3_CCMR2|=0x0068;		//PWM1	ģʽ
TIM3_CCER|=0X0100;         //���ʹ��
TIM3_CCR3=0XFFFF;
Port_In_Out(PortB,BIT0,AF_TW_50M); 
break;

case 4:
TIM3_CCMR2&=0x00FF;
TIM3_CCMR2|=0x6800;	     	//PWM1	ģʽ
TIM3_CCER|=0X1000;         //���ʹ��
TIM3_CCR4=0XFFFF;
Port_In_Out(PortB,BIT1,AF_TW_50M); 
break;
}

TIM3_CR1=0x01;	//����ʱ�Ӽ���
TIM3_EGR=0X01;  //������������¼�
break;
//--------------------------------------------------------
case 4:
Dev_Sck_En(SckEn_GPIOB,1);     //����IO��ʱ��
Dev_Sck_En(SckEn_AFIO, 1);	   //��������ʱ��
Dev_Sck_En(SckEn_TIM4, 1);     //����TIM2ʱ��

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
TIM4_CCMR1|=0x0068;		//PWM1	ģʽ
TIM4_CCER|=0X0001;         //���ʹ��
TIM4_CCR1=0XFFFF;
Port_In_Out(PortB,BIT6,AF_TW_50M); 
break;
case 2:
TIM4_CCMR1&=0x00ff;
TIM4_CCMR1|=0x6800;		 //PWM1	ģʽ
TIM4_CCER|=0X0010;         //���ʹ��
TIM4_CCR2=0XFFFF;
Port_In_Out(PortB,BIT7,AF_TW_50M); 
break;
case 3:
TIM4_CCMR2&=0xff00;
TIM4_CCMR2|=0x0068;		//PWM1	ģʽ
TIM4_CCER|=0X0100;         //���ʹ��
TIM4_CCR3=0XFFFF;
Port_In_Out(PortB,BIT8,AF_TW_50M); 
break;

case 4:
TIM4_CCMR2&=0x00FF;
TIM4_CCMR2|=0x6800;		//PWM1	ģʽ
TIM4_CCER|=0X1000;         //���ʹ��
TIM4_CCR4=0XFFFF;
Port_In_Out(PortB,BIT9,AF_TW_50M); 
break;
}

TIM4_CR1=0x01;	//����ʱ�Ӽ���
TIM4_EGR=0X01;  //������������¼�
break;
//------------------------------------------------------------------
case 5:
Dev_Sck_En(SckEn_GPIOA,1);     //����IO��ʱ��
Dev_Sck_En(SckEn_AFIO, 1);	   //��������ʱ��
Dev_Sck_En(SckEn_TIM5, 1);     //����TIM2ʱ��

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
TIM5_CCMR1|=0x0068;		//PWM1	ģʽ
TIM5_CCER|=0X0001;         //���ʹ��
TIM5_CCR1=0XFFFF;
Port_In_Out(PortA,BIT0,AF_TW_50M); 
break;
case 2:
TIM5_CCMR1&=0x00ff;
TIM5_CCMR1|=0x6800;		 //PWM1	ģʽ
TIM5_CCER|=0X0010;         //���ʹ��
TIM5_CCR2=0XFFFF;
Port_In_Out(PortA,BIT1,AF_TW_50M); 
break;
case 3:
TIM5_CCMR2&=0xff00;
TIM5_CCMR2|=0x0068;		//PWM1	ģʽ
TIM5_CCER|=0X0100;         //���ʹ��
TIM5_CCR3=0XFFFF;
Port_In_Out(PortA,BIT2,AF_TW_50M); 
break;

case 4:
TIM5_CCMR2&=0x00FF;
TIM5_CCMR2|=0x6800;		//PWM1	ģʽ
TIM5_CCER|=0X1000;         //���ʹ��
TIM5_CCR4=0XFFFF;
Port_In_Out(PortA,BIT3,AF_TW_50M); 
break;
}

TIM5_CR1=0x01;	//����ʱ�Ӽ���
TIM5_EGR=0X01;  //������������¼�
break;
//-------------------------------------------------------

}
}




//----------------------�ж���غ���----------------------

/*
����������ƫ�Ƶ�ַ
NVIC_VectTab:��ַ
Offset:ƫ����		

Flash��NVIC_VectTab��0x08000000
SRAM ��NVIC_VectTab��0x20000000
*/
void Nvic_SetVectorTable(ulong NVIC_VectTab, ulong Offset)	 
{ 	
SCB_VTOR = (ulong)0x00000000;
SCB_VTOR = NVIC_VectTab|(Offset & (ulong)0x1FFFFF80);//����NVIC��������ƫ�ƼĴ���
//���ڱ�ʶ����������CODE��������RAM��
}



//����NVIC����
//NVIC_Group:NVIC���� 0~4 �ܹ�5�� 
//ע�����ȼ����ܳ����趨����ķ�Χ!����������벻���Ĵ���
//�黮��:
//��0:0λ��ռ���ȼ�,4λ��Ӧ���ȼ�
//��1:1λ��ռ���ȼ�,3λ��Ӧ���ȼ�
//��2:2λ��ռ���ȼ�,2λ��Ӧ���ȼ�
//��3:3λ��ռ���ȼ�,1λ��Ӧ���ȼ�
//��4:4λ��ռ���ȼ�,0λ��Ӧ���ȼ�
//��ֵԽС,Խ����
void Nvic_GroupConfig(uchar NVIC_Group)	 
{ 
	ulong temp,temp1;	  
	temp1=(~NVIC_Group)&0x07;//ȡ����λ
	temp1<<=8;
	temp=SCB_AIRCR;   //��ȡ��ǰ������
	temp&=0X0000F8FF; //�����ǰ����
	temp|=0X05FA0000; //д��Կ��
	temp|=temp1;	   
	SCB_AIRCR=temp;   //���÷���	    	  				   
}

//���ø����ж� 
//NVIC_PreemptionPriority:��ռ���ȼ�
//NVIC_SubPriority       :��Ӧ���ȼ�
//NVIC_Channel           :�жϱ��
//NVIC_Group             :�жϷ��� 0~4

//����ϵͳ�жϷ��飬��Ӧ�жϺŵ����ȼ�������ȫ���ж�ʹ��
void Nvic_Init(uchar NVIC_Group,uchar NVIC_PreemptionPriority,uchar NVIC_SubPriority,uchar NVIC_Channel)	 
{ 
	ulong temp;	
	uchar IPRADDR=NVIC_Channel/4;  //ÿ��ֻ�ܴ�4��,�õ����ַ 
	uchar IPROFFSET=NVIC_Channel%4;//�����ڵ�ƫ��
	IPROFFSET=IPROFFSET*8+4;    //�õ�ƫ�Ƶ�ȷ��λ��
	Nvic_GroupConfig(NVIC_Group);//���÷���
	temp=NVIC_PreemptionPriority<<(4-NVIC_Group);	  
	temp|=NVIC_SubPriority&(0x0f>>NVIC_Group);
	temp&=0xf;//ȡ����λ

	if(NVIC_Channel<32) 
	NVIC->ISER[0]|=1<<NVIC_Channel;//ʹ���ж�λ(Ҫ����Ļ�,�෴������OK)
	else		
	NVIC->ISER[1]|=1<<(NVIC_Channel-32);   
	
	NVIC->IPR[IPRADDR]|=temp<<IPROFFSET;//������Ӧ���ȼ����������ȼ�   	    	  				   
}




/******************************************
�ⲿ�����ж�����
RorFSR=0 --�������½��ض�������
RorFSR=1 --�����ش���
RorFSR=2 --�½��ش���
RorFSR=3 --�ϣ����ض�����
*******************************************/
void EXTI_Config(uchar GPIO_x,uchar BIT_x,uchar RorFSR)
{
Dev_Sck_En(SckEn_AFIO, 1);	        //��������ʱ��
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



//����жϱ�־
void EXTI_MarkCLR(uchar Lin_x)  
{
EXTI_PR|=(ulong)1 << Lin_x;
}
/*
�رպ��жϱ�־����λ�������¼
=1������
���ж�ʹ��/�ر�
*/
void EXTI_Enable(uchar Lin_x,uchar ENorC)
{
if(ENorC)
EXTI_IMR|=(ulong)0x01<<Lin_x;
else
EXTI_IMR&=~((ulong)0x01<<Lin_x);
}


//��ѯ��������·�ж�
ulong EXTI_LineINQ(void)
{
	return EXTI_PR&0x0007ffff;
}

//����жϱ�־,�����λ�ķ�ʽ������һ����������־
void EXTI_MarkCLR_BIT(ulong BitCLR)
{
	EXTI_PR=BitCLR;
}

/*
0��JTAG����
1��JTAG����ͨIO��,��SWD����
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
AFIO_MAPR=0X02000000;  //JTAG����ͨIO�ڣ���SWD����
}
return;

}






