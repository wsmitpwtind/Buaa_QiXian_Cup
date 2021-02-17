#ifndef _STM32F10_INIT_H_
#define _STM32F10_INIT_H_

#include   "stm32f10x_type.h"



#define  Sys_HSE  12 //��ʱ��8M
#define  Sys_PLL  6  //��Ƶϵ��

#define  Sys_DIV_AHB   1  //SYSCLK�ķ�Ƶϵ��
#define  Sys_DIV_APB2  1  //AHB�ķ�Ƶϵ��
#define  Sys_DIV_APB1  2  //APB2�ķ�Ƶϵ��


//��������ʱ����Ϣ�Ĵ�����ȫ�֣���

struct SYSCLK_DAT
{
ulong SYSCLK_DAT_SYS;	   //ʱ��ֵ
ulong SYSCLK_DAT_AHB;	   //ʱ��ֵ
ulong SYSCLK_DAT_APB2;
ulong SYSCLK_DAT_APB1;	  

ulong SYSCLK_DAT_R_AHB;	   //�ڼĴ�������ľ����Ƶֵ
ulong SYSCLK_DAT_R_APB2;
ulong SYSCLK_DAT_R_APB1;
};






/*
һ����ӷ���ֻ���MDK��GCC��������
��������ӷ�����core_cm3.h�ļ���
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



uchar sys_Init (void);            //ϵͳ��λ����ʼ��
void  Sys_OpenHSI(uchar ON_OFF);
void  Port_In_Out(uchar port,uchar p_bit,uchar mode);  //IO��ģʽ����
void  Dev_Sck_En(uchar Sck_EnNO,uchar ON_OFF);;        //Ƭ����Դ��ʱ��ʹ�ܡ��ر�
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
uchar DMA1_STA_HTIF(uchar DMA1_NO);	  //0���봫����ɱ�
uchar DMA1_STA_TCIF(uchar DMA1_NO);   //0��ȫ������ɱ�־

void DMA1_STA_CHTIF(uchar DMA1_NO);	//����봫����ɱ�־
void DMA1_STA_CTCIF(uchar DMA1_NO);	//����봫����ɱ�־
void PWM_Init(uchar TIM_NO,uchar CH_NO);
void Soft_Reset(void);               	    //�����λ

void Nvic_SetVectorTable(ulong NVIC_VectTab, ulong Offset);
//����ϵͳ�жϷ��飬��Ӧ�жϺŵ����ȼ���������Ӧ�жϺ�ȫ���ж�ʹ��
void Nvic_Init(uchar NVIC_Group,uchar NVIC_PreemptionPriority,uchar NVIC_SubPriority,uchar NVIC_Channel);


void UART_ALLMkCLR(void);

void EXTI_Config(uchar GPIO_x,uchar BIT_x,uchar RorFSR);
void EXTI_MarkCLR(uchar Lin_x);
void EXTI_Enable(uchar Lin_x,uchar ENorC);
ulong EXTI_LineINQ(void);
void EXTI_MarkCLR_BIT(ulong BitCLR);
void JTAG_ENable(uchar JTAG_STA);



#endif





