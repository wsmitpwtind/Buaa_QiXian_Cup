#ifndef _STM32F10_INIT_H_
#define _STM32F10_INIT_H_

#include   "stm32f10x_type.h"



#define  Sys_HSE  12 //外时钟8M
#define  Sys_PLL  6  //倍频系数

#define  Sys_DIV_AHB   1  //SYSCLK的分频系数
#define  Sys_DIV_APB2  1  //AHB的分频系数
#define  Sys_DIV_APB1  2  //APB2的分频系数


//各个总线时钟信息寄存器，全局！！

struct SYSCLK_DAT
{
ulong SYSCLK_DAT_SYS;	   //时钟值
ulong SYSCLK_DAT_AHB;	   //时钟值
ulong SYSCLK_DAT_APB2;
ulong SYSCLK_DAT_APB1;	  

ulong SYSCLK_DAT_R_AHB;	   //在寄存器里面的具体分频值
ulong SYSCLK_DAT_R_APB2;
ulong SYSCLK_DAT_R_APB1;
};






/*
一下添加方法只针对MDK（GCC编译器）
其他的添加方法见core_cm3.h文件！
*/
#if (defined (__GNUC__)) /*-- GNU Compiler --*/

/* GNU gcc specific functions */

static __INLINE void __NOP()                      { __ASM volatile ("nop"); }
static __INLINE void __enable_irq()               { __ASM volatile ("cpsie i"); }
static __INLINE void __disable_irq()              { __ASM volatile ("cpsid i"); }

static __INLINE void __enable_fault_irq()         { __ASM volatile ("cpsie f"); }
static __INLINE void __disable_fault_irq()        { __ASM volatile ("cpsid f"); }

static __INLINE void __WFI()                      { __ASM volatile ("wfi");   }
static __INLINE void __WFE()                      { __ASM volatile ("wfe");   }
static __INLINE void __SEV()                      { __ASM volatile ("sev");   }
static __INLINE void __ISB(arg)                   { __ASM volatile ("isb");   }
static __INLINE void __DSB(arg)                   { __ASM volatile ("dsb");   }
static __INLINE void __DMB(arg)                   { __ASM volatile ("dmb");   }
static __INLINE void __CLREX()                    { __ASM volatile ("clrex"); }

#endif



uchar sys_Init (void);            //系统复位，初始化
void  Sys_OpenHSI(uchar ON_OFF);
void  Port_In_Out(uchar port,uchar p_bit,uchar mode);  //IO口模式设置
void  Dev_Sck_En(uchar Sck_EnNO,uchar ON_OFF);;        //片内资源的时钟使能、关闭
void  USART_Init(uchar usart_NO,ulong usart_BB);

void USART_ChangeBAUD(uchar usart_NO,ulong usart_BB);
void  SysTick_Init_1ms(void);

void USART1_TxDat(uchar d);
void USART2_TxDat(uchar d);
void USART3_TxDat(uchar d);
void USART4_TxDat(uchar d);
void USART5_TxDat(uchar d);

void  USART1_Send_DMA_Init(void);
void  USART1_Send_DMA_Running(uchar *buf,uchar Len);
uchar USART1_Send_DMA_WaitFinish(void);

void  USART2_Send_DMA_Init(void);
void  USART2_Send_DMA_Running(uchar *buf,uchar Len);
uchar USART2_Send_DMA_WaitFinish(void);

void  USART3_Send_DMA_Init(void);
void  USART3_Send_DMA_Running(uchar *buf,uchar Len);
uchar USART3_Send_DMA_WaitFinish(void);

void UART1_Send_String(uchar *path);
void UART2_Send_String(uchar *path);
void UART3_Send_String(uchar *path);
void UART4_Send_String(uchar *path);
void UART5_Send_String(uchar *path);

void UART1_Send_Length( uchar *TemBuf,usint len );
void UART2_Send_Length( uchar *TemBuf,usint len );
void UART3_Send_Length( uchar *TemBuf,usint len );
void UART4_Send_Length( uchar *TemBuf,usint len );
void UART5_Send_Length( uchar *TemBuf,usint len );
	

void UART1_Clear_RL_Mark(void);
void UART2_Clear_RL_Mark(void);
void UART3_Clear_RL_Mark(void);

void ADC1_Init(uchar SCAN,uchar DMA,uchar Loop,uchar DatLay);
void ADC1_ChannelSET(uchar chann_val,uchar chann_time,uchar chann_ord,uchar ADCChgNumrst);
void ADC1_Start(usint * ADCBuffer);

void SPI_Init(uchar spi_NO,uchar spi_speed);

void  USART_MDAEN (uchar usart_NO,uchar TxDMA_EN,uchar RxDMA_EN);
uchar DMA1_STA_HTIF(uchar DMA1_NO);	  //0：半传输完成标
uchar DMA1_STA_TCIF(uchar DMA1_NO);   //0：全传输完成标志

void DMA1_STA_CHTIF(uchar DMA1_NO);	//清除半传输完成标志
void DMA1_STA_CTCIF(uchar DMA1_NO);	//清除半传输完成标志
void PWM_Init(uchar TIM_NO,uchar CH_NO);
void Soft_Reset(void);               	    //软件复位

void Nvic_SetVectorTable(ulong NVIC_VectTab, ulong Offset);
//设置系统中断分组，相应中断号的优先级，开启相应中断号全局中断使能
void Nvic_Init(uchar NVIC_Group,uchar NVIC_PreemptionPriority,uchar NVIC_SubPriority,uchar NVIC_Channel);


void UART_ALLMkCLR(void);

void EXTI_Config(uchar GPIO_x,uchar BIT_x,uchar RorFSR);
void EXTI_MarkCLR(uchar Lin_x);
void EXTI_Enable(uchar Lin_x,uchar ENorC);
ulong EXTI_LineINQ(void);
void EXTI_MarkCLR_BIT(ulong BitCLR);
void JTAG_ENable(uchar JTAG_STA);



#endif





