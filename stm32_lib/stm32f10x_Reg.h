#ifndef _STM32F10_REG_H_
#define _STM32F10_REG_H_

#include "stm32f10x_type.h"

#define PERIPH_BB_BASE        ((u32)0x42000000)
#define SRAM_BB_BASE          ((u32)0x22000000)

#define SRAM_BASE             ((u32)0x20000000)
#define PERIPH_BASE           ((u32)0x40000000)

#define FSMC_R_BASE           ((u32)0xA0000000)

#define APB1PERIPH_BASE        PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x10000)
#define AHBPERIPH_BASE        (PERIPH_BASE + 0x20000)

#define TIM2_BASE             (APB1PERIPH_BASE + 0x0000)
#define TIM3_BASE             (APB1PERIPH_BASE + 0x0400)
#define TIM4_BASE             (APB1PERIPH_BASE + 0x0800)
#define TIM5_BASE             (APB1PERIPH_BASE + 0x0C00)
#define TIM6_BASE             (APB1PERIPH_BASE + 0x1000)
#define TIM7_BASE             (APB1PERIPH_BASE + 0x1400)
#define RTC_BASE              (APB1PERIPH_BASE + 0x2800)
#define WWDG_BASE             (APB1PERIPH_BASE + 0x2C00)
#define IWDG_BASE             (APB1PERIPH_BASE + 0x3000)
#define SPI2_BASE             (APB1PERIPH_BASE + 0x3800)
#define SPI3_BASE             (APB1PERIPH_BASE + 0x3C00)
#define USART2_BASE           (APB1PERIPH_BASE + 0x4400)
#define USART3_BASE           (APB1PERIPH_BASE + 0x4800)
#define UART4_BASE            (APB1PERIPH_BASE + 0x4C00)
#define UART5_BASE            (APB1PERIPH_BASE + 0x5000)
#define I2C1_BASE             (APB1PERIPH_BASE + 0x5400)
#define I2C2_BASE             (APB1PERIPH_BASE + 0x5800)
#define CAN_BASE              (APB1PERIPH_BASE + 0x6400)
#define BKP_BASE              (APB1PERIPH_BASE + 0x6C00)
#define PWR_BASE              (APB1PERIPH_BASE + 0x7000)
#define DAC_BASE              (APB1PERIPH_BASE + 0x7400)

#define AFIO_BASE             (APB2PERIPH_BASE + 0x0000)
#define EXTI_BASE             (APB2PERIPH_BASE + 0x0400)
#define GPIOA_BASE            (APB2PERIPH_BASE + 0x0800)
#define GPIOB_BASE            (APB2PERIPH_BASE + 0x0C00)
#define GPIOC_BASE            (APB2PERIPH_BASE + 0x1000)
#define GPIOD_BASE            (APB2PERIPH_BASE + 0x1400)
#define GPIOE_BASE            (APB2PERIPH_BASE + 0x1800)
#define GPIOF_BASE            (APB2PERIPH_BASE + 0x1C00)
#define GPIOG_BASE            (APB2PERIPH_BASE + 0x2000)
#define ADC1_BASE             (APB2PERIPH_BASE + 0x2400)
#define ADC2_BASE             (APB2PERIPH_BASE + 0x2800)
#define TIM1_BASE             (APB2PERIPH_BASE + 0x2C00)
#define SPI1_BASE             (APB2PERIPH_BASE + 0x3000)
#define TIM8_BASE             (APB2PERIPH_BASE + 0x3400)
#define USART1_BASE           (APB2PERIPH_BASE + 0x3800)
#define ADC3_BASE             (APB2PERIPH_BASE + 0x3C00)

#define SDIO_BASE             (PERIPH_BASE + 0x18000)

#define DMA1_BASE             (AHBPERIPH_BASE + 0x0000)
#define DMA1_Channel1_BASE    (AHBPERIPH_BASE + 0x0008)
#define DMA1_Channel2_BASE    (AHBPERIPH_BASE + 0x001C)
#define DMA1_Channel3_BASE    (AHBPERIPH_BASE + 0x0030)
#define DMA1_Channel4_BASE    (AHBPERIPH_BASE + 0x0044)
#define DMA1_Channel5_BASE    (AHBPERIPH_BASE + 0x0058)
#define DMA1_Channel6_BASE    (AHBPERIPH_BASE + 0x006C)
#define DMA1_Channel7_BASE    (AHBPERIPH_BASE + 0x0080)
#define DMA2_BASE             (AHBPERIPH_BASE + 0x0400)
#define DMA2_Channel1_BASE    (AHBPERIPH_BASE + 0x0408)
#define DMA2_Channel2_BASE    (AHBPERIPH_BASE + 0x041C)
#define DMA2_Channel3_BASE    (AHBPERIPH_BASE + 0x0430)
#define DMA2_Channel4_BASE    (AHBPERIPH_BASE + 0x0444)
#define DMA2_Channel5_BASE    (AHBPERIPH_BASE + 0x0458)
#define RCC_BASE              (AHBPERIPH_BASE + 0x1000)
#define CRC_BASE              (AHBPERIPH_BASE + 0x3000)

/* Flash registers base address */
#define FLASH_R_BASE          (AHBPERIPH_BASE + 0x2000)
/* Flash Option Bytes base address */
#define OB_BASE               ((u32)0x1FFFF800)

/* FSMC Bankx registers base address */
#define FSMC_Bank1_R_BASE     (FSMC_R_BASE + 0x0000)
#define FSMC_Bank1E_R_BASE    (FSMC_R_BASE + 0x0104)
#define FSMC_Bank2_R_BASE     (FSMC_R_BASE + 0x0060)
#define FSMC_Bank3_R_BASE     (FSMC_R_BASE + 0x0080)
#define FSMC_Bank4_R_BASE     (FSMC_R_BASE + 0x00A0)

/* Debug MCU registers base address */
#define DBGMCU_BASE          ((u32)0xE0042000)

/* System Control Space memory map */
#define SCS_BASE              ((u32)0xE000E000)

#define SysTick_BASE          (SCS_BASE + 0x0010)
#define NVIC_BASE             (SCS_BASE + 0x0100)
#define SCB_BASE              (SCS_BASE + 0x0D00)

//--------------软件复位---------------
#define SCB_AIRCR  (*(volatile unsigned long *)(SCB_BASE+0x0C)) 


//-----------中断相关寄存器---------------------

typedef struct
{
  vu32 ISER[2];
  u32  RESERVED0[30];
  vu32 ICER[2];
  u32  RSERVED1[30];
  vu32 ISPR[2];
  u32  RESERVED2[30];
  vu32 ICPR[2];
  u32  RESERVED3[30];
  vu32 IABR[2];
  u32  RESERVED4[62];
  vu32 IPR[15];
} NVIC_TypeDef;

#define NVIC ((NVIC_TypeDef *) NVIC_BASE)  //指向结构的指针。NVIC->ISER[0].

#define  SCB_VTOR  *(volatile unsigned long *)(SCB_BASE+8)  //中断向量表设置寄存器


//位带操作,实现51类似的GPIO控制功能
/*计算对应的别名区地址（对别名区的一个字（32位）读写1操作，就相当于对位段区对应的一位操作*/

#define BITBAND(addr, bitnum) (0x42000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))



//---------------------------------------------------------------------------------
//计算真是的IO口地址映射,如GPIOA_ODR_Addr只是代表地址！！！

#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 

#define GPIOA_CRL_Addr    (GPIOA_BASE) 
#define GPIOB_CRL_Addr    (GPIOB_BASE) 
#define GPIOC_CRL_Addr    (GPIOC_BASE) 
#define GPIOD_CRL_Addr    (GPIOD_BASE) 
#define GPIOE_CRL_Addr    (GPIOE_BASE) 
#define GPIOF_CRL_Addr    (GPIOF_BASE)    
#define GPIOG_CRL_Addr    (GPIOG_BASE)     

#define GPIOA_CRH_Addr    (GPIOA_BASE+4) 
#define GPIOB_CRH_Addr    (GPIOB_BASE+4) 
#define GPIOC_CRH_Addr    (GPIOC_BASE+4) 
#define GPIOD_CRH_Addr    (GPIOD_BASE+4) 
#define GPIOE_CRH_Addr    (GPIOE_BASE+4) 
#define GPIOF_CRH_Addr    (GPIOF_BASE+4) 
#define GPIOG_CRH_Addr    (GPIOG_BASE+4) 





/*
以下代码实现：（需要配置FSMC）
操作（读写）LCD_REG  这个虚拟寄存器则操作LCE内部寄存器，即操作的是命令
操作（读写）LCD_GRAM 这个虚拟寄存器则操作LCE内部数据暂存器，即操作的是数据
*/

//-------神舟开发板上外部LCD操作地址-----选用了BANK1的第4个FSMC，64M--------
#define LCD_REG    *(volatile unsigned short int *)(0x6C000000) //操作的是LCD的寄存器
#define LCD_GRAM   *(volatile unsigned short int *)(0x6C000002) //操作的是LCD的GRAM,因为是16位数据总线，内部HADD（25~1）和FSMC_ADD(24~0)意义对应



//----------计算真是的IO寄存器---------------如 GPIOA_CRL 就是代表寄存器，可以直接访问的！！

#define GPIOA_CRL      (*(volatile unsigned long *)(GPIOA_BASE+0x00)) 
#define GPIOA_CRH      (*(volatile unsigned long *)(GPIOA_BASE+0x04)) 
#define GPIOA_IDR      (*(volatile unsigned long *)(GPIOA_BASE+0x08))
#define GPIOA_ODR      (*(volatile unsigned long *)(GPIOA_BASE+0x0c))
#define GPIOA_BSRR     (*(volatile unsigned long *)(GPIOA_BASE+0x10))
#define GPIOA_BRR      (*(volatile unsigned long *)(GPIOA_BASE+0x14))
#define GPIOA_LCKR     (*(volatile unsigned long *)(GPIOA_BASE+0x18))


#define GPIOB_CRL      (*(volatile unsigned long *)(GPIOB_BASE+0x00)) 
#define GPIOB_CRH      (*(volatile unsigned long *)(GPIOB_BASE+0x04))
#define GPIOB_IDR      (*(volatile unsigned long *)(GPIOB_BASE+0x08))
#define GPIOB_ODR      (*(volatile unsigned long *)(GPIOB_BASE+0x0c))
#define GPIOB_BSRR     (*(volatile unsigned long *)(GPIOB_BASE+0x10))
#define GPIOB_BRR      (*(volatile unsigned long *)(GPIOB_BASE+0x14))
#define GPIOB_LCKR     (*(volatile unsigned long *)(GPIOB_BASE+0x18))

#define GPIOC_CRL      (*(volatile unsigned long *)(GPIOC_BASE+0x00)) 
#define GPIOC_CRH      (*(volatile unsigned long *)(GPIOC_BASE+0x04)) 
#define GPIOC_IDR      (*(volatile unsigned long *)(GPIOC_BASE+0x08))
#define GPIOC_ODR      (*(volatile unsigned long *)(GPIOC_BASE+0x0c))
#define GPIOC_BSRR     (*(volatile unsigned long *)(GPIOC_BASE+0x10))
#define GPIOC_BRR      (*(volatile unsigned long *)(GPIOC_BASE+0x14))
#define GPIOC_LCKR     (*(volatile unsigned long *)(GPIOC_BASE+0x18))

#define GPIOD_CRL      (*(volatile unsigned long *)(GPIOD_BASE+0x00)) 
#define GPIOD_CRH      (*(volatile unsigned long *)(GPIOD_BASE+0x04))
#define GPIOD_IDR      (*(volatile unsigned long *)(GPIOD_BASE+0x08))
#define GPIOD_ODR      (*(volatile unsigned long *)(GPIOD_BASE+0x0c))
#define GPIOD_BSRR     (*(volatile unsigned long *)(GPIOD_BASE+0x10))
#define GPIOD_BRR      (*(volatile unsigned long *)(GPIOD_BASE+0x14))
#define GPIOD_LCKR     (*(volatile unsigned long *)(GPIOD_BASE+0x18))

#define GPIOE_CRL      (*(volatile unsigned long *)(GPIOE_BASE+0x00)) 
#define GPIOE_CRH      (*(volatile unsigned long *)(GPIOE_BASE+0x04))
#define GPIOE_IDR      (*(volatile unsigned long *)(GPIOE_BASE+0x08))
#define GPIOE_ODR      (*(volatile unsigned long *)(GPIOE_BASE+0x0c))
#define GPIOE_BSRR     (*(volatile unsigned long *)(GPIOE_BASE+0x10))
#define GPIOE_BRR      (*(volatile unsigned long *)(GPIOE_BASE+0x14))
#define GPIOE_LCKR     (*(volatile unsigned long *)(GPIOE_BASE+0x18))

#define GPIOF_CRL      (*(volatile unsigned long *)(GPIOF_BASE+0x00)) 
#define GPIOF_CRH      (*(volatile unsigned long *)(GPIOF_BASE+0x04))
#define GPIOF_IDR      (*(volatile unsigned long *)(GPIOF_BASE+0x08))
#define GPIOF_ODR      (*(volatile unsigned long *)(GPIOF_BASE+0x0c))
#define GPIOF_BSRR     (*(volatile unsigned long *)(GPIOF_BASE+0x10))
#define GPIOF_BRR      (*(volatile unsigned long *)(GPIOF_BASE+0x14))
#define GPIOF_LCKR     (*(volatile unsigned long *)(GPIOF_BASE+0x18))

#define GPIOG_CRL      (*(volatile unsigned long *)(GPIOG_BASE+0x00)) 
#define GPIOG_CRH      (*(volatile unsigned long *)(GPIOG_BASE+0x04))
#define GPIOG_IDR      (*(volatile unsigned long *)(GPIOG_BASE+0x08))
#define GPIOG_ODR      (*(volatile unsigned long *)(GPIOG_BASE+0x0c))
#define GPIOG_BSRR     (*(volatile unsigned long *)(GPIOG_BASE+0x10))
#define GPIOG_BRR      (*(volatile unsigned long *)(GPIOG_BASE+0x14))
#define GPIOG_LCKR     (*(volatile unsigned long *)(GPIOG_BASE+0x18))

#define AFIO_EVCE        (*(volatile unsigned long *)(AFIO_BASE+0x00))
#define AFIO_MAPR        (*(volatile unsigned long *)(AFIO_BASE+0x04))
#define AFIO_EXTICR1     (*(volatile unsigned long *)(AFIO_BASE+0x08))
#define AFIO_EXTICR2     (*(volatile unsigned long *)(AFIO_BASE+0x0C))
#define AFIO_EXTICR3     (*(volatile unsigned long *)(AFIO_BASE+0x10))
#define AFIO_EXTICR4     (*(volatile unsigned long *)(AFIO_BASE+0x14))


#define EXTI_IMR    (*(volatile unsigned long *)(EXTI_BASE+0x00))
#define EXTI_EMR    (*(volatile unsigned long *)(EXTI_BASE+0x04))
#define EXTI_RTSR   (*(volatile unsigned long *)(EXTI_BASE+0x08))
#define EXTI_FTSR   (*(volatile unsigned long *)(EXTI_BASE+0x0C))
#define EXTI_SWIER  (*(volatile unsigned long *)(EXTI_BASE+0x10))
#define EXTI_PR     (*(volatile unsigned long *)(EXTI_BASE+0x14))


//------------系统滴答时钟-----------
#define SysTick_CTRL   (*(volatile unsigned long *)(SysTick_BASE+0))
#define SysTick_LOAD   (*(volatile unsigned long *)(SysTick_BASE+4))
#define SysTick_VAL    (*(volatile unsigned long *)(SysTick_BASE+8))
#define SysTick_CALIB  (*(volatile unsigned long *)(SysTick_BASE+12))


//--------FLASH相关------

#define FLASH_ACR 		 (*(volatile unsigned long *)(0x40022000+0))
#define FLASH_KEYR 	 	 (*(volatile unsigned long *)(0x40022000+4))
#define FLASH_OPTKEYR 	 (*(volatile unsigned long *)(0x40022000+8))
#define FLASH_SR 	 	 (*(volatile unsigned long *)(0x40022000+12))
#define FLASH_CR	     (*(volatile unsigned long *)(0x40022000+16))
#define FLASH_AR 	 	 (*(volatile unsigned long *)(0x40022000+20))

#define FLASH_OBR 	 	 (*(volatile unsigned long *)(0x40022000+28))
#define FLASH_WRPR 	 	 (*(volatile unsigned long *)(0x40022000+32))

//#define FLASH_WRPR 	 	 (*(volatile unsigned long *)(FLASH_R_BASE+32))



//------------电源控制寄存器--------
#define PWR_CR   (*(volatile unsigned long *)(PWR_BASE+0x00))
#define PWR_CSR  (*(volatile unsigned long *)(PWR_BASE+0x04))


//-----------备份区-----------------

#define BKP_DR1     (*(volatile unsigned long *)(BKP_BASE+4))
#define BKP_DR2     (*(volatile unsigned long *)(BKP_BASE+8))
#define BKP_DR3     (*(volatile unsigned long *)(BKP_BASE+12))
#define BKP_DR4     (*(volatile unsigned long *)(BKP_BASE+16))
#define BKP_DR5     (*(volatile unsigned long *)(BKP_BASE+20))
#define BKP_DR6     (*(volatile unsigned long *)(BKP_BASE+24))
#define BKP_DR7     (*(volatile unsigned long *)(BKP_BASE+28))
#define BKP_DR8     (*(volatile unsigned long *)(BKP_BASE+32))
#define BKP_DR9     (*(volatile unsigned long *)(BKP_BASE+36))
#define BKP_DR10    (*(volatile unsigned long *)(BKP_BASE+40))
#define BKP_DR11    (*(volatile unsigned long *)(BKP_BASE+64))
#define BKP_DR12    (*(volatile unsigned long *)(BKP_BASE+68))
#define BKP_DR13    (*(volatile unsigned long *)(BKP_BASE+72))
#define BKP_DR14    (*(volatile unsigned long *)(BKP_BASE+76))
#define BKP_DR15    (*(volatile unsigned long *)(BKP_BASE+80))
#define BKP_DR16    (*(volatile unsigned long *)(BKP_BASE+84))
#define BKP_DR17    (*(volatile unsigned long *)(BKP_BASE+88))
#define BKP_DR18    (*(volatile unsigned long *)(BKP_BASE+92))
#define BKP_DR19    (*(volatile unsigned long *)(BKP_BASE+96))
#define BKP_DR20    (*(volatile unsigned long *)(BKP_BASE+100))
#define BKP_DR21    (*(volatile unsigned long *)(BKP_BASE+104))
#define BKP_DR22    (*(volatile unsigned long *)(BKP_BASE+108))
#define BKP_DR23    (*(volatile unsigned long *)(BKP_BASE+112))
#define BKP_DR24    (*(volatile unsigned long *)(BKP_BASE+116))
#define BKP_DR25    (*(volatile unsigned long *)(BKP_BASE+120))
#define BKP_DR26    (*(volatile unsigned long *)(BKP_BASE+124))
#define BKP_DR27    (*(volatile unsigned long *)(BKP_BASE+128))
#define BKP_DR28    (*(volatile unsigned long *)(BKP_BASE+132))
#define BKP_DR29    (*(volatile unsigned long *)(BKP_BASE+136))
#define BKP_DR30    (*(volatile unsigned long *)(BKP_BASE+140))
#define BKP_DR31    (*(volatile unsigned long *)(BKP_BASE+144))
#define BKP_DR32    (*(volatile unsigned long *)(BKP_BASE+148))
#define BKP_DR33    (*(volatile unsigned long *)(BKP_BASE+152))
#define BKP_DR34    (*(volatile unsigned long *)(BKP_BASE+156))
#define BKP_DR35    (*(volatile unsigned long *)(BKP_BASE+160))
#define BKP_DR36    (*(volatile unsigned long *)(BKP_BASE+164))
#define BKP_DR37    (*(volatile unsigned long *)(BKP_BASE+168))
#define BKP_DR38    (*(volatile unsigned long *)(BKP_BASE+172))
#define BKP_DR39    (*(volatile unsigned long *)(BKP_BASE+176))
#define BKP_DR40    (*(volatile unsigned long *)(BKP_BASE+180))
#define BKP_DR41    (*(volatile unsigned long *)(BKP_BASE+184))
#define BKP_DR42    (*(volatile unsigned long *)(BKP_BASE+188))

#define BKP_RTCCR    (*(volatile unsigned long *)(BKP_BASE+44))
#define BKP_CR       (*(volatile unsigned long *)(BKP_BASE+48))
#define BKP_CSR      (*(volatile unsigned long *)(BKP_BASE+52))

//-------------复位和时钟控制-----------------//

#define RCC_CR          (*(volatile unsigned long *)(RCC_BASE+0))
#define RCC_CFGR        (*(volatile unsigned long *)(RCC_BASE+4))
#define RCC_CIR         (*(volatile unsigned long *)(RCC_BASE+8))
#define RCC_APB2RSTR    (*(volatile unsigned long *)(RCC_BASE+12))
#define RCC_APB1RSTR    (*(volatile unsigned long *)(RCC_BASE+16))
#define RCC_AHBENR      (*(volatile unsigned long *)(RCC_BASE+20))
#define RCC_APB2ENR     (*(volatile unsigned long *)(RCC_BASE+24))
#define RCC_APB1ENR     (*(volatile unsigned long *)(RCC_BASE+28))
#define RCC_BDCR        (*(volatile unsigned long *)(RCC_BASE+32))
#define RCC_CSR         (*(volatile unsigned long *)(RCC_BASE+36))

//---------USART---------------------------------------

#define USART1_SR          (*(volatile unsigned long *)(USART1_BASE+0))
#define USART1_DR          (*(volatile unsigned long *)(USART1_BASE+4))
#define USART1_BRR         (*(volatile unsigned long *)(USART1_BASE+8))
#define USART1_CR1         (*(volatile unsigned long *)(USART1_BASE+12))
#define USART1_CR2         (*(volatile unsigned long *)(USART1_BASE+16))
#define USART1_CR3         (*(volatile unsigned long *)(USART1_BASE+20))
#define USART1_GTPR        (*(volatile unsigned long *)(USART1_BASE+24))

#define USART2_SR          (*(volatile unsigned long *)(USART2_BASE+0))
#define USART2_DR          (*(volatile unsigned long *)(USART2_BASE+4))
#define USART2_BRR         (*(volatile unsigned long *)(USART2_BASE+8))
#define USART2_CR1         (*(volatile unsigned long *)(USART2_BASE+12))
#define USART2_CR2         (*(volatile unsigned long *)(USART2_BASE+16))
#define USART2_CR3         (*(volatile unsigned long *)(USART2_BASE+20))
#define USART2_GTPR        (*(volatile unsigned long *)(USART2_BASE+24))

#define USART3_SR          (*(volatile unsigned long *)(USART3_BASE+0))
#define USART3_DR          (*(volatile unsigned long *)(USART3_BASE+4))
#define USART3_BRR         (*(volatile unsigned long *)(USART3_BASE+8))
#define USART3_CR1         (*(volatile unsigned long *)(USART3_BASE+12))
#define USART3_CR2         (*(volatile unsigned long *)(USART3_BASE+16))
#define USART3_CR3         (*(volatile unsigned long *)(USART3_BASE+20))
#define USART3_GTPR        (*(volatile unsigned long *)(USART3_BASE+24))


#define USART4_SR          (*(volatile unsigned long *)(UART4_BASE+0))
#define USART4_DR          (*(volatile unsigned long *)(UART4_BASE+4))
#define USART4_BRR         (*(volatile unsigned long *)(UART4_BASE+8))
#define USART4_CR1         (*(volatile unsigned long *)(UART4_BASE+12))
#define USART4_CR2         (*(volatile unsigned long *)(UART4_BASE+16))
#define USART4_CR3         (*(volatile unsigned long *)(UART4_BASE+20))
#define USART4_GTPR        (*(volatile unsigned long *)(UART4_BASE+24))


#define USART5_SR          (*(volatile unsigned long *)(UART5_BASE+0))
#define USART5_DR          (*(volatile unsigned long *)(UART5_BASE+4))
#define USART5_BRR         (*(volatile unsigned long *)(UART5_BASE+8))
#define USART5_CR1         (*(volatile unsigned long *)(UART5_BASE+12))
#define USART5_CR2         (*(volatile unsigned long *)(UART5_BASE+16))
#define USART5_CR3         (*(volatile unsigned long *)(UART5_BASE+20))
#define USART5_GTPR        (*(volatile unsigned long *)(UART5_BASE+24))


//---------------DMA1-----------------------------------
#define DMA1_ISR        (*(volatile unsigned long *)(DMA1_BASE+0X00))
#define DMA1_IFCR       (*(volatile unsigned long *)(DMA1_BASE+0x04))

#define DMA1_CCR1        (*(volatile unsigned long *)(DMA1_BASE+0x08))
#define DMA1_CNDTR1      (*(volatile unsigned long *)(DMA1_BASE+0x0c))
#define DMA1_CPAR1       (*(volatile unsigned long *)(DMA1_BASE+0x10))
#define DMA1_CMAR1       (*(volatile unsigned long *)(DMA1_BASE+0x14))

#define DMA1_CCR2        (*(volatile unsigned long *)(DMA1_BASE+0x1c))
#define DMA1_CNDTR2      (*(volatile unsigned long *)(DMA1_BASE+0x20))
#define DMA1_CPAR2       (*(volatile unsigned long *)(DMA1_BASE+0x24))
#define DMA1_CMAR2       (*(volatile unsigned long *)(DMA1_BASE+0x28))

#define DMA1_CCR3        (*(volatile unsigned long *)(DMA1_BASE+0x30))
#define DMA1_CNDTR3      (*(volatile unsigned long *)(DMA1_BASE+0x34))
#define DMA1_CPAR3       (*(volatile unsigned long *)(DMA1_BASE+0x38))
#define DMA1_CMAR3       (*(volatile unsigned long *)(DMA1_BASE+0x3c))

#define DMA1_CCR4        (*(volatile unsigned long *)(DMA1_BASE+0x44))
#define DMA1_CNDTR4      (*(volatile unsigned long *)(DMA1_BASE+0x48))
#define DMA1_CPAR4       (*(volatile unsigned long *)(DMA1_BASE+0x4c))
#define DMA1_CMAR4       (*(volatile unsigned long *)(DMA1_BASE+0x50))

#define DMA1_CCR5        (*(volatile unsigned long *)(DMA1_BASE+0x58))
#define DMA1_CNDTR5      (*(volatile unsigned long *)(DMA1_BASE+0x5c))
#define DMA1_CPAR5       (*(volatile unsigned long *)(DMA1_BASE+0x60))
#define DMA1_CMAR5       (*(volatile unsigned long *)(DMA1_BASE+0x64))

#define DMA1_CCR6        (*(volatile unsigned long *)(DMA1_BASE+0x6c))
#define DMA1_CNDTR6      (*(volatile unsigned long *)(DMA1_BASE+0x70))
#define DMA1_CPAR6       (*(volatile unsigned long *)(DMA1_BASE+0x74))
#define DMA1_CMAR6       (*(volatile unsigned long *)(DMA1_BASE+0x78))

#define DMA1_CCR7        (*(volatile unsigned long *)(DMA1_BASE+0x80))
#define DMA1_CNDTR7      (*(volatile unsigned long *)(DMA1_BASE+0x84))
#define DMA1_CPAR7       (*(volatile unsigned long *)(DMA1_BASE+0x88))
#define DMA1_CMAR7       (*(volatile unsigned long *)(DMA1_BASE+0x8c))

//-----------MDA2--------
#define DMA2_ISR        (*(volatile unsigned long *)(DMA2_BASE+0X00))
#define DMA2_IFCR       (*(volatile unsigned long *)(DMA2_BASE+0x04))

#define DMA2_CCR1        (*(volatile unsigned long *)(DMA2_BASE+0x08))
#define DMA2_CNDTR1      (*(volatile unsigned long *)(DMA2_BASE+0x0c))
#define DMA2_CPAR1       (*(volatile unsigned long *)(DMA2_BASE+0x10))
#define DMA2_CMAR1       (*(volatile unsigned long *)(DMA2_BASE+0x14))
		
#define DMA2_CCR2        (*(volatile unsigned long *)(DMA2_BASE+0x1c))
#define DMA2_CNDTR2      (*(volatile unsigned long *)(DMA2_BASE+0x20))
#define DMA2_CPAR2       (*(volatile unsigned long *)(DMA2_BASE+0x24))
#define DMA2_CMAR2       (*(volatile unsigned long *)(DMA2_BASE+0x28))

#define DMA2_CCR3        (*(volatile unsigned long *)(DMA2_BASE+0x30))
#define DMA2_CNDTR3      (*(volatile unsigned long *)(DMA2_BASE+0x34))
#define DMA2_CPAR3       (*(volatile unsigned long *)(DMA2_BASE+0x38))
#define DMA2_CMAR3       (*(volatile unsigned long *)(DMA2_BASE+0x3c))

#define DMA2_CCR4        (*(volatile unsigned long *)(DMA2_BASE+0x44))
#define DMA2_CNDTR4      (*(volatile unsigned long *)(DMA2_BASE+0x48))
#define DMA2_CPAR4       (*(volatile unsigned long *)(DMA2_BASE+0x4c))
#define DMA2_CMAR4       (*(volatile unsigned long *)(DMA2_BASE+0x50))

#define DMA2_CCR5        (*(volatile unsigned long *)(DMA2_BASE+0x58))
#define DMA2_CNDTR5      (*(volatile unsigned long *)(DMA2_BASE+0x5c))
#define DMA2_CPAR5       (*(volatile unsigned long *)(DMA2_BASE+0x60))
#define DMA2_CMAR5       (*(volatile unsigned long *)(DMA2_BASE+0x64))
//-----------------------------------------------------------------------




//----------------ADC1转换---------------

#define ADC1_SR          (*(volatile unsigned long *)(ADC1_BASE+0x00))
#define ADC1_CR1         (*(volatile unsigned long *)(ADC1_BASE+0x04))
#define ADC1_CR2         (*(volatile unsigned long *)(ADC1_BASE+0x08))
#define ADC1_SMPR1       (*(volatile unsigned long *)(ADC1_BASE+0x0C))
#define ADC1_SMPR2       (*(volatile unsigned long *)(ADC1_BASE+0x10))
#define ADC1_JOFR1       (*(volatile unsigned long *)(ADC1_BASE+0x14))
#define ADC1_JOFR2       (*(volatile unsigned long *)(ADC1_BASE+0x18))
#define ADC1_JOFR3       (*(volatile unsigned long *)(ADC1_BASE+0x1C))
#define ADC1_JOFR4       (*(volatile unsigned long *)(ADC1_BASE+0x20))
#define ADC1_HTR         (*(volatile unsigned long *)(ADC1_BASE+0x24))
#define ADC1_LTR         (*(volatile unsigned long *)(ADC1_BASE+0x28))
#define ADC1_SQR1        (*(volatile unsigned long *)(ADC1_BASE+0x2C))
#define ADC1_SQR2        (*(volatile unsigned long *)(ADC1_BASE+0x30))
#define ADC1_SQR3        (*(volatile unsigned long *)(ADC1_BASE+0x34))
#define ADC1_JSQR        (*(volatile unsigned long *)(ADC1_BASE+0x38))
#define ADC1_JDR1        (*(volatile unsigned long *)(ADC1_BASE+0x3C))
#define ADC1_JDR2        (*(volatile unsigned long *)(ADC1_BASE+0x40))
#define ADC1_JDR3        (*(volatile unsigned long *)(ADC1_BASE+0x44))
#define ADC1_JDR4        (*(volatile unsigned long *)(ADC1_BASE+0x48))
#define ADC1_DR          (*(volatile unsigned long *)(ADC1_BASE+0x4C))

//------------------ADC2转换---------------------

#define ADC2_SR          (*(volatile unsigned long *)(ADC2_BASE+0x00))
#define ADC2_CR1         (*(volatile unsigned long *)(ADC2_BASE+0x04))
#define ADC2_CR2         (*(volatile unsigned long *)(ADC2_BASE+0x08))
#define ADC2_SMPR1       (*(volatile unsigned long *)(ADC2_BASE+0x0C))
#define ADC2_SMPR2       (*(volatile unsigned long *)(ADC2_BASE+0x10))
#define ADC2_JOFR1       (*(volatile unsigned long *)(ADC2_BASE+0x14))
#define ADC2_JOFR2       (*(volatile unsigned long *)(ADC2_BASE+0x18))
#define ADC2_JOFR3       (*(volatile unsigned long *)(ADC2_BASE+0x1C))
#define ADC2_JOFR4       (*(volatile unsigned long *)(ADC2_BASE+0x20))
#define ADC2_HTR         (*(volatile unsigned long *)(ADC2_BASE+0x24))
#define ADC2_LTR         (*(volatile unsigned long *)(ADC2_BASE+0x28))
#define ADC2_SQR1        (*(volatile unsigned long *)(ADC2_BASE+0x2C))
#define ADC2_SQR2        (*(volatile unsigned long *)(ADC2_BASE+0x30))
#define ADC2_SQR3        (*(volatile unsigned long *)(ADC2_BASE+0x34))
#define ADC2_JSQR        (*(volatile unsigned long *)(ADC2_BASE+0x38))
#define ADC2_JDR1        (*(volatile unsigned long *)(ADC2_BASE+0x3C))
#define ADC2_JDR2        (*(volatile unsigned long *)(ADC2_BASE+0x40))
#define ADC2_JDR3        (*(volatile unsigned long *)(ADC2_BASE+0x44))
#define ADC2_JDR4        (*(volatile unsigned long *)(ADC2_BASE+0x48))
#define ADC2_DR          (*(volatile unsigned long *)(ADC2_BASE+0x4C))


//-------------SPI---------------------------------------
#define  SPI1_CR1      (*(volatile unsigned long *)(SPI1_BASE+0x00))
#define  SPI1_CR2      (*(volatile unsigned long *)(SPI1_BASE+0x04))
#define  SPI1_SR       (*(volatile unsigned long *)(SPI1_BASE+0x08))
#define  SPI1_DR   	   (*(volatile unsigned long *)(SPI1_BASE+0x0C))
#define  SPI1_CRCPR    (*(volatile unsigned long *)(SPI1_BASE+0x10))
#define  SPI1_RXCRCR   (*(volatile unsigned long *)(SPI1_BASE+0x14))
#define  SPI1_TXCRR    (*(volatile unsigned long *)(SPI1_BASE+0x18))
#define  SPI1_I2S_CFGR (*(volatile unsigned long *)(SPI1_BASE+0x1C))
#define  SPI1_I2SPR    (*(volatile unsigned long *)(SPI1_BASE+0x20))

#define  SPI2_CR1      (*(volatile unsigned long *)(SPI2_BASE+0x00))
#define  SPI2_CR2      (*(volatile unsigned long *)(SPI2_BASE+0x04))
#define  SPI2_SR       (*(volatile unsigned long *)(SPI2_BASE+0x08))
#define  SPI2_DR   	   (*(volatile unsigned long *)(SPI2_BASE+0x0C))
#define  SPI2_CRCPR    (*(volatile unsigned long *)(SPI2_BASE+0x10))
#define  SPI2_RXCRCR   (*(volatile unsigned long *)(SPI2_BASE+0x14))
#define  SPI2_TXCRR    (*(volatile unsigned long *)(SPI2_BASE+0x18))
#define  SPI2_I2S_CFGR (*(volatile unsigned long *)(SPI2_BASE+0x1C))
#define  SPI2_I2SPR    (*(volatile unsigned long *)(SPI2_BASE+0x20))


#define  SPI3_CR1      (*(volatile unsigned long *)(SPI3_BASE+0x00))
#define  SPI3_CR2      (*(volatile unsigned long *)(SPI3_BASE+0x04))
#define  SPI3_SR       (*(volatile unsigned long *)(SPI3_BASE+0x08))
#define  SPI3_DR   	   (*(volatile unsigned long *)(SPI3_BASE+0x0C))
#define  SPI3_CRCPR    (*(volatile unsigned long *)(SPI3_BASE+0x10))
#define  SPI3_RXCRCR   (*(volatile unsigned long *)(SPI3_BASE+0x14))
#define  SPI3_TXCRR    (*(volatile unsigned long *)(SPI3_BASE+0x18))
#define  SPI3_I2S_CFGR (*(volatile unsigned long *)(SPI3_BASE+0x1C))
#define  SPI3_I2SPR    (*(volatile unsigned long *)(SPI3_BASE+0x20))


//------------------------SD卡相关寄存器------------------------
#define  SDIO_POWER    (*(volatile unsigned long *)(SDIO_BASE+0x00))
#define  SDIO_CLKCR    (*(volatile unsigned long *)(SDIO_BASE+0x04))
#define  SDIO_ARG      (*(volatile unsigned long *)(SDIO_BASE+0x08))
#define  SDIO_CMD      (*(volatile unsigned long *)(SDIO_BASE+0x0C))
#define  SDIO_RESPCMD  (*(volatile unsigned long *)(SDIO_BASE+0x10))
#define  SDIO_RESP1    (*(volatile unsigned long *)(SDIO_BASE+0x14))
#define  SDIO_RESP2    (*(volatile unsigned long *)(SDIO_BASE+0x18))
#define  SDIO_RESP3    (*(volatile unsigned long *)(SDIO_BASE+0x1C))
#define  SDIO_RESP4    (*(volatile unsigned long *)(SDIO_BASE+0x20))
#define  SDIO_DTIMER   (*(volatile unsigned long *)(SDIO_BASE+0x24))
#define  SDIO_DLEN     (*(volatile unsigned long *)(SDIO_BASE+0x28))
#define  SDIO_DCTRL    (*(volatile unsigned long *)(SDIO_BASE+0x2C))
#define  SDIO_DCOUNT   (*(volatile unsigned long *)(SDIO_BASE+0x30))
#define  SDIO_STA      (*(volatile unsigned long *)(SDIO_BASE+0x34))
#define  SDIO_ICR      (*(volatile unsigned long *)(SDIO_BASE+0x38))
#define  SDIO_MASK     (*(volatile unsigned long *)(SDIO_BASE+0x3C))
#define  SDIO_FIFOCNT  (*(volatile unsigned long *)(SDIO_BASE+0x48))
#define  SDIO_FIFO     (*(volatile unsigned long *)(SDIO_BASE+0x80))

//--------------------------FSMC控制寄存器----------------------//
//----NOD Flash,SRAM 控制器
#define  FSMC_BCR1   (*(volatile unsigned long *)(FSMC_Bank1_R_BASE+0x00))
#define  FSMC_BTR1   (*(volatile unsigned long *)(FSMC_Bank1_R_BASE+0x04))

#define  FSMC_BCR2   (*(volatile unsigned long *)(FSMC_Bank1_R_BASE+0x08))
#define  FSMC_BTR2   (*(volatile unsigned long *)(FSMC_Bank1_R_BASE+0x0c))

#define  FSMC_BCR3   (*(volatile unsigned long *)(FSMC_Bank1_R_BASE+0x10))
#define  FSMC_BTR3   (*(volatile unsigned long *)(FSMC_Bank1_R_BASE+0x14))

#define  FSMC_BCR4   (*(volatile unsigned long *)(FSMC_Bank1_R_BASE+0x18))
#define  FSMC_BTR4   (*(volatile unsigned long *)(FSMC_Bank1_R_BASE+0x1c))

#define  FSMC_BWTR1  (*(volatile unsigned long *)(FSMC_Bank1_R_BASE+0x0104))
#define  FSMC_BWTR2  (*(volatile unsigned long *)(FSMC_Bank1_R_BASE+0x010C))
#define  FSMC_BWTR3  (*(volatile unsigned long *)(FSMC_Bank1_R_BASE+0x0114))
#define  FSMC_BWTR4  (*(volatile unsigned long *)(FSMC_Bank1_R_BASE+0x011C))


//----NAND Flash 控制器
#define  FSMC_PCR2   (*(volatile unsigned long *)(FSMC_Bank1_R_BASE+0x60))
#define  FSMC_SC2    (*(volatile unsigned long *)(FSMC_Bank1_R_BASE+0x64))
#define  FSMC_PMEM2  (*(volatile unsigned long *)(FSMC_Bank1_R_BASE+0x68))
#define  FSMC_PATT2  (*(volatile unsigned long *)(FSMC_Bank1_R_BASE+0x6C))

#define  FSMC_PCR3   (*(volatile unsigned long *)(FSMC_Bank1_R_BASE+0x80))
#define  FSMC_SR3    (*(volatile unsigned long *)(FSMC_Bank1_R_BASE+0x84))
#define  FSMC_PMEM3  (*(volatile unsigned long *)(FSMC_Bank1_R_BASE+0x88))
#define  FSMC_PATT3  (*(volatile unsigned long *)(FSMC_Bank1_R_BASE+0x8C))

#define  FSMC_PCR4   (*(volatile unsigned long *)(FSMC_Bank1_R_BASE+0xA0))
#define  FSMC_SR4    (*(volatile unsigned long *)(FSMC_Bank1_R_BASE+0xA4))
#define  FSMC_PMEM4  (*(volatile unsigned long *)(FSMC_Bank1_R_BASE+0xA8))
#define  FSMC_PATT4  (*(volatile unsigned long *)(FSMC_Bank1_R_BASE+0xAC))
#define  FSMC_PIO4   (*(volatile unsigned long *)(FSMC_Bank1_R_BASE+0xB0))

//--------------------------定时器相关-----------------------------------

#define  TIM1_CR1    (*(volatile unsigned long *)(TIM1_BASE+0x00))
#define  TIM1_CR2    (*(volatile unsigned long *)(TIM1_BASE+0x04))
#define  TIM1_SMCR   (*(volatile unsigned long *)(TIM1_BASE+0x08))
#define  TIM1_DIER   (*(volatile unsigned long *)(TIM1_BASE+0x0C))
#define  TIM1_SR     (*(volatile unsigned long *)(TIM1_BASE+0x10))
#define  TIM1_EGR    (*(volatile unsigned long *)(TIM1_BASE+0x14))
#define  TIM1_CCMR1  (*(volatile unsigned long *)(TIM1_BASE+0x18))
#define  TIM1_CCMR2  (*(volatile unsigned long *)(TIM1_BASE+0x1C))
#define  TIM1_CCER   (*(volatile unsigned long *)(TIM1_BASE+0x20))
#define  TIM1_CNT    (*(volatile unsigned long *)(TIM1_BASE+0x24))
#define  TIM1_PSC    (*(volatile unsigned long *)(TIM1_BASE+0x28))
#define  TIM1_ARR    (*(volatile unsigned long *)(TIM1_BASE+0x2C))
#define  TIM1_RCR    (*(volatile unsigned long *)(TIM1_BASE+0x30))
#define  TIM1_CCR1   (*(volatile unsigned long *)(TIM1_BASE+0x34))
#define  TIM1_CCR2   (*(volatile unsigned long *)(TIM1_BASE+0x38))
#define  TIM1_CCR3   (*(volatile unsigned long *)(TIM1_BASE+0x3C))
#define  TIM1_CCR4   (*(volatile unsigned long *)(TIM1_BASE+0x40))
#define  TIM1_BDTR   (*(volatile unsigned long *)(TIM1_BASE+0x44))
#define  TIM1_DCR    (*(volatile unsigned long *)(TIM1_BASE+0x48))
#define  TIM1_DMAR   (*(volatile unsigned long *)(TIM1_BASE+0x4C))


#define  TIM2_CR1    (*(volatile unsigned long *)(TIM2_BASE+0x00))
#define  TIM2_CR2    (*(volatile unsigned long *)(TIM2_BASE+0x04))
#define  TIM2_SMCR   (*(volatile unsigned long *)(TIM2_BASE+0x08))
#define  TIM2_DIER   (*(volatile unsigned long *)(TIM2_BASE+0x0C))
#define  TIM2_SR     (*(volatile unsigned long *)(TIM2_BASE+0x10))
#define  TIM2_EGR    (*(volatile unsigned long *)(TIM2_BASE+0x14))
#define  TIM2_CCMR1  (*(volatile unsigned long *)(TIM2_BASE+0x18))
#define  TIM2_CCMR2  (*(volatile unsigned long *)(TIM2_BASE+0x1C))
#define  TIM2_CCER   (*(volatile unsigned long *)(TIM2_BASE+0x20))
#define  TIM2_CNT    (*(volatile unsigned long *)(TIM2_BASE+0x24))
#define  TIM2_PSC    (*(volatile unsigned long *)(TIM2_BASE+0x28))
#define  TIM2_ARR    (*(volatile unsigned long *)(TIM2_BASE+0x2C))
#define  TIM2_CCR1   (*(volatile unsigned long *)(TIM2_BASE+0x34))
#define  TIM2_CCR2   (*(volatile unsigned long *)(TIM2_BASE+0x38))
#define  TIM2_CCR3   (*(volatile unsigned long *)(TIM2_BASE+0x3C))
#define  TIM2_CCR4   (*(volatile unsigned long *)(TIM2_BASE+0x40))
#define  TIM2_DCR    (*(volatile unsigned long *)(TIM2_BASE+0x48))
#define  TIM2_DMAR   (*(volatile unsigned long *)(TIM2_BASE+0x4C))

#define  TIM3_CR1    (*(volatile unsigned long *)(TIM3_BASE+0x00))
#define  TIM3_CR2    (*(volatile unsigned long *)(TIM3_BASE+0x04))
#define  TIM3_SMCR   (*(volatile unsigned long *)(TIM3_BASE+0x08))
#define  TIM3_DIER   (*(volatile unsigned long *)(TIM3_BASE+0x0C))
#define  TIM3_SR     (*(volatile unsigned long *)(TIM3_BASE+0x10))
#define  TIM3_EGR    (*(volatile unsigned long *)(TIM3_BASE+0x14))
#define  TIM3_CCMR1  (*(volatile unsigned long *)(TIM3_BASE+0x18))
#define  TIM3_CCMR2  (*(volatile unsigned long *)(TIM3_BASE+0x1C))
#define  TIM3_CCER   (*(volatile unsigned long *)(TIM3_BASE+0x20))
#define  TIM3_CNT    (*(volatile unsigned long *)(TIM3_BASE+0x24))
#define  TIM3_PSC    (*(volatile unsigned long *)(TIM3_BASE+0x28))
#define  TIM3_ARR    (*(volatile unsigned long *)(TIM3_BASE+0x2C))
#define  TIM3_CCR1   (*(volatile unsigned long *)(TIM3_BASE+0x34))
#define  TIM3_CCR2   (*(volatile unsigned long *)(TIM3_BASE+0x38))
#define  TIM3_CCR3   (*(volatile unsigned long *)(TIM3_BASE+0x3C))
#define  TIM3_CCR4   (*(volatile unsigned long *)(TIM3_BASE+0x40))
#define  TIM3_DCR    (*(volatile unsigned long *)(TIM3_BASE+0x48))
#define  TIM3_DMAR   (*(volatile unsigned long *)(TIM3_BASE+0x4C))

#define  TIM4_CR1    (*(volatile unsigned long *)(TIM4_BASE+0x00))
#define  TIM4_CR2    (*(volatile unsigned long *)(TIM4_BASE+0x04))
#define  TIM4_SMCR   (*(volatile unsigned long *)(TIM4_BASE+0x08))
#define  TIM4_DIER   (*(volatile unsigned long *)(TIM4_BASE+0x0C))
#define  TIM4_SR     (*(volatile unsigned long *)(TIM4_BASE+0x10))
#define  TIM4_EGR    (*(volatile unsigned long *)(TIM4_BASE+0x14))
#define  TIM4_CCMR1  (*(volatile unsigned long *)(TIM4_BASE+0x18))
#define  TIM4_CCMR2  (*(volatile unsigned long *)(TIM4_BASE+0x1C))
#define  TIM4_CCER   (*(volatile unsigned long *)(TIM4_BASE+0x20))
#define  TIM4_CNT    (*(volatile unsigned long *)(TIM4_BASE+0x24))
#define  TIM4_PSC    (*(volatile unsigned long *)(TIM4_BASE+0x28))
#define  TIM4_ARR    (*(volatile unsigned long *)(TIM4_BASE+0x2C))
#define  TIM4_CCR1   (*(volatile unsigned long *)(TIM4_BASE+0x34))
#define  TIM4_CCR2   (*(volatile unsigned long *)(TIM4_BASE+0x38))
#define  TIM4_CCR3   (*(volatile unsigned long *)(TIM4_BASE+0x3C))
#define  TIM4_CCR4   (*(volatile unsigned long *)(TIM4_BASE+0x40))
#define  TIM4_DCR    (*(volatile unsigned long *)(TIM4_BASE+0x48))
#define  TIM4_DMAR   (*(volatile unsigned long *)(TIM4_BASE+0x4C))


#define  TIM5_CR1    (*(volatile unsigned long *)(TIM5_BASE+0x00))
#define  TIM5_CR2    (*(volatile unsigned long *)(TIM5_BASE+0x04))
#define  TIM5_SMCR   (*(volatile unsigned long *)(TIM5_BASE+0x08))
#define  TIM5_DIER   (*(volatile unsigned long *)(TIM5_BASE+0x0C))
#define  TIM5_SR     (*(volatile unsigned long *)(TIM5_BASE+0x10))
#define  TIM5_EGR    (*(volatile unsigned long *)(TIM5_BASE+0x14))
#define  TIM5_CCMR1  (*(volatile unsigned long *)(TIM5_BASE+0x18))
#define  TIM5_CCMR2  (*(volatile unsigned long *)(TIM5_BASE+0x1C))
#define  TIM5_CCER   (*(volatile unsigned long *)(TIM5_BASE+0x20))
#define  TIM5_CNT    (*(volatile unsigned long *)(TIM5_BASE+0x24))
#define  TIM5_PSC    (*(volatile unsigned long *)(TIM5_BASE+0x28))
#define  TIM5_ARR    (*(volatile unsigned long *)(TIM5_BASE+0x2C))
#define  TIM5_CCR1   (*(volatile unsigned long *)(TIM5_BASE+0x34))
#define  TIM5_CCR2   (*(volatile unsigned long *)(TIM5_BASE+0x38))
#define  TIM5_CCR3   (*(volatile unsigned long *)(TIM5_BASE+0x3C))
#define  TIM5_CCR4   (*(volatile unsigned long *)(TIM5_BASE+0x40))
#define  TIM5_DCR    (*(volatile unsigned long *)(TIM5_BASE+0x48))
#define  TIM5_DMAR   (*(volatile unsigned long *)(TIM5_BASE+0x4C))

//----------------DAC转换---------------
#define DAC_CR           (*(volatile unsigned long *)(DAC_BASE+0x00))
#define DAC_SWTRIGR      (*(volatile unsigned long *)(DAC_BASE+0x04))
#define DAC_DHR12R1      (*(volatile unsigned long *)(DAC_BASE+0x08))
#define DAC_DHR12L1      (*(volatile unsigned long *)(DAC_BASE+0x0C))
#define DAC_DHR8R1       (*(volatile unsigned long *)(DAC_BASE+0x10))

#define DAC_DHR12R2       (*(volatile unsigned long *)(DAC_BASE+0x14))
#define DAC_DHR12L2       (*(volatile unsigned long *)(DAC_BASE+0x18))
#define DAC_DHR8R2        (*(volatile unsigned long *)(DAC_BASE+0x1C))
	
#define DAC_DHR12RD       (*(volatile unsigned long *)(DAC_BASE+0x20))
#define DAC_DHR12LD       (*(volatile unsigned long *)(DAC_BASE+0x24))
#define DAC_DHR8RD        (*(volatile unsigned long *)(DAC_BASE+0x28))

#define DAC_DOR1       	 (*(volatile unsigned long *)(DAC_BASE+0x2C))
#define DAC_DOR2         (*(volatile unsigned long *)(DAC_BASE+0x30))

 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR (GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR (GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR (GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR (GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR (GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR (GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR (GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR (GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR (GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR (GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR (GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR (GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR (GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR (GPIOG_IDR_Addr,n)  //输入


//---------------对其他的的GPIO控制寄存器未操作，如输入、输出模式操作----------------

#define PA_modeH(n)   BIT_ADDR(GPIOA_CRH_Addr,n) 
#define PA_modeL(n)   BIT_ADDR(GPIOA_CRL_Addr,n) 

#define PB_modeH(n)   BIT_ADDR(GPIOB_CRH_Addr,n) 
#define PB_modeL(n)   BIT_ADDR(GPIOB_CRL_Addr,n) 

#define PC_modeH(n)   BIT_ADDR(GPIOC_CRH_Addr,n) 
#define PC_modeL(n)   BIT_ADDR(GPIOC_CRL_Addr,n) 

#define PD_modeH(n)   BIT_ADDR(GPIOD_CRH_Addr,n) 
#define PD_modeL(n)   BIT_ADDR(GPIOD_CRL_Addr,n) 

#define PE_modeH(n)   BIT_ADDR(GPIOE_CRH_Addr,n) 
#define PE_modeL(n)   BIT_ADDR(GPIOE_CRL_Addr,n) 

#define PF_modeH(n)   BIT_ADDR(GPIOF_CRH_Addr,n) 
#define PF_modeL(n)   BIT_ADDR(GPIOF_CRL_Addr,n) 

#define PG_modeH(n)   BIT_ADDR(GPIOG_CRH_Addr,n) 
#define PG_modeL(n)   BIT_ADDR(GPIOG_CRL_Addr,n) 


/*-------神舟开发板上外部LCD操作----选用了BANK1的第4个FSMC，64M--------*/


#define BIT15  15
#define BIT14  14
#define BIT13  13
#define BIT12  12
#define BIT11  11
#define BIT10  10
#define BIT9   9
#define BIT8   8
#define BIT7   7
#define BIT6   6
#define BIT5   5
#define BIT4   4
#define BIT3   3
#define BIT2   2
#define BIT1   1
#define BIT0   0

#define PortA 0
#define PortB 1
#define PortC 2
#define PortD 3
#define PortE 4
#define PortF 5
#define PortG 6



//--------------------------片内资源时钟使能---------------------

//--------------------------AHB总线
#define SckEn_DMA1  0
#define SckEn_DMA2	1
#define SckEn_SRAM	2
#define SckEn_FLITF	4
#define SckEn_CRC	6
#define SckEn_FSMC	8
#define SckEn_SDIO	10

//--------------------------APB2总线
#define SckEn_AFIO	 0+0x40
#define SckEn_GPIOA	 2+0x40
#define SckEn_GPIOB	 3+0x40
#define SckEn_GPIOC	 4+0x40
#define SckEn_GPIOD	 5+0x40
#define SckEn_GPIOE	 6+0x40
#define SckEn_GPIOF	 7+0x40
#define SckEn_GPIOG	 8+0x40
#define SckEn_ADC1	 9+0x40
#define SckEn_ADC2	 10+0x40
#define SckEn_TIM1	 11+0x40
#define SckEn_SPI1	 12+0x40
#define SckEn_TIM8	 13+0x40
#define SckEn_USART1 14+0x40
#define SckEn_ADC3	 15+0x40

//--------------------------APB1总线
#define SckEn_TIM2	0+0x80
#define SckEn_TIM3	1+0x80
#define SckEn_TIM4	2+0x80
#define SckEn_TIM5	3+0x80
#define SckEn_TIM6	4+0x80
#define SckEn_TIM7	5+0x80
#define SckEn_WWDG  11+0x80

#define SckEn_SPI2   14+0x80
#define SckEn_SPI3   15+0x80
#define SckEn_USART2 17+0x80
#define SckEn_USART3 18+0x80
#define SckEn_USART4 19+0x80
#define SckEn_USART5 20+0x80
#define SckEn_I2C1   21+0x80
#define SckEn_I2C2	 22+0x80
#define SckEn_USB    23+0x80
#define SckEn_CAN    25+0x80
#define SckEn_BKP    27+0x80
#define SckEn_PWR    28+0x80
#define SckEn_DAC    29+0x80

#define GE_TW_2M   0x02
#define GE_TW_10M  0x01
#define GE_TW_50M  0x03

#define GE_OC_2M   0x06
#define GE_OC_10M  0x05
#define GE_OC_50M  0x07

#define AF_TW_2M   0x0a
#define AF_TW_10M  0x09
#define AF_TW_50M  0x0b

#define AF_OC_2M   0x0E
#define AF_OC_10M  0x0D
#define AF_OC_50M  0x0F

#define Analog_IN  0x00
#define Suspen_IN  0x04
#define Updown_IN  0x08


//-------------中断号定义--------------------

#define WWDG_IRQChannel              ((u8)0x00)  /* Window WatchDog Interrupt */
#define PVD_IRQChannel               ((u8)0x01)  /* PVD through EXTI Line detection Interrupt */
#define TAMPER_IRQChannel            ((u8)0x02)  /* Tamper Interrupt */
#define RTC_IRQChannel               ((u8)0x03)  /* RTC global Interrupt */
#define FLASH_IRQChannel             ((u8)0x04)  /* FLASH global Interrupt */
#define RCC_IRQChannel               ((u8)0x05)  /* RCC global Interrupt */
#define EXTI0_IRQChannel             ((u8)0x06)  /* EXTI Line0 Interrupt */
#define EXTI1_IRQChannel             ((u8)0x07)  /* EXTI Line1 Interrupt */
#define EXTI2_IRQChannel             ((u8)0x08)  /* EXTI Line2 Interrupt */
#define EXTI3_IRQChannel             ((u8)0x09)  /* EXTI Line3 Interrupt */
#define EXTI4_IRQChannel             ((u8)0x0A)  /* EXTI Line4 Interrupt */
#define DMA1_Channel1_IRQChannel     ((u8)0x0B)  /* DMA1 Channel 1 global Interrupt */
#define DMA1_Channel2_IRQChannel     ((u8)0x0C)  /* DMA1 Channel 2 global Interrupt */
#define DMA1_Channel3_IRQChannel     ((u8)0x0D)  /* DMA1 Channel 3 global Interrupt */
#define DMA1_Channel4_IRQChannel     ((u8)0x0E)  /* DMA1 Channel 4 global Interrupt */
#define DMA1_Channel5_IRQChannel     ((u8)0x0F)  /* DMA1 Channel 5 global Interrupt */
#define DMA1_Channel6_IRQChannel     ((u8)0x10)  /* DMA1 Channel 6 global Interrupt */
#define DMA1_Channel7_IRQChannel     ((u8)0x11)  /* DMA1 Channel 7 global Interrupt */
#define ADC1_2_IRQChannel            ((u8)0x12)  /* ADC1 et ADC2 global Interrupt */
#define USB_HP_CAN_TX_IRQChannel     ((u8)0x13)  /* USB High Priority or CAN TX Interrupts */
#define USB_LP_CAN_RX0_IRQChannel    ((u8)0x14)  /* USB Low Priority or CAN RX0 Interrupts */
#define CAN_RX1_IRQChannel           ((u8)0x15)  /* CAN RX1 Interrupt */
#define CAN_SCE_IRQChannel           ((u8)0x16)  /* CAN SCE Interrupt */
#define EXTI9_5_IRQChannel           ((u8)0x17)  /* External Line[9:5] Interrupts */
#define TIM1_BRK_IRQChannel          ((u8)0x18)  /* TIM1 Break Interrupt */
#define TIM1_UP_IRQChannel           ((u8)0x19)  /* TIM1 Update Interrupt */
#define TIM1_TRG_COM_IRQChannel      ((u8)0x1A)  /* TIM1 Trigger and Commutation Interrupt */
#define TIM1_CC_IRQChannel           ((u8)0x1B)  /* TIM1 Capture Compare Interrupt */
#define TIM2_IRQChannel              ((u8)0x1C)  /* TIM2 global Interrupt */
#define TIM3_IRQChannel              ((u8)0x1D)  /* TIM3 global Interrupt */
#define TIM4_IRQChannel              ((u8)0x1E)  /* TIM4 global Interrupt */
#define I2C1_EV_IRQChannel           ((u8)0x1F)  /* I2C1 Event Interrupt */
#define I2C1_ER_IRQChannel           ((u8)0x20)  /* I2C1 Error Interrupt */
#define I2C2_EV_IRQChannel           ((u8)0x21)  /* I2C2 Event Interrupt */
#define I2C2_ER_IRQChannel           ((u8)0x22)  /* I2C2 Error Interrupt */
#define SPI1_IRQChannel              ((u8)0x23)  /* SPI1 global Interrupt */
#define SPI2_IRQChannel              ((u8)0x24)  /* SPI2 global Interrupt */
#define USART1_IRQChannel            ((u8)0x25)  /* USART1 global Interrupt */
#define USART2_IRQChannel            ((u8)0x26)  /* USART2 global Interrupt */
#define USART3_IRQChannel            ((u8)0x27)  /* USART3 global Interrupt */
#define EXTI15_10_IRQChannel         ((u8)0x28)  /* External Line[15:10] Interrupts */
#define RTCAlarm_IRQChannel          ((u8)0x29)  /* RTC Alarm through EXTI Line Interrupt */
#define USBWakeUp_IRQChannel         ((u8)0x2A)  /* USB WakeUp from suspend through EXTI Line Interrupt */
#define TIM8_BRK_IRQChannel          ((u8)0x2B)  /* TIM8 Break Interrupt */
#define TIM8_UP_IRQChannel           ((u8)0x2C)  /* TIM8 Update Interrupt */
#define TIM8_TRG_COM_IRQChannel      ((u8)0x2D)  /* TIM8 Trigger and Commutation Interrupt */
#define TIM8_CC_IRQChannel           ((u8)0x2E)  /* TIM8 Capture Compare Interrupt */
#define ADC3_IRQChannel              ((u8)0x2F)  /* ADC3 global Interrupt */
#define FSMC_IRQChannel              ((u8)0x30)  /* FSMC global Interrupt */
#define SDIO_IRQChannel              ((u8)0x31)  /* SDIO global Interrupt */
#define TIM5_IRQChannel              ((u8)0x32)  /* TIM5 global Interrupt */
#define SPI3_IRQChannel              ((u8)0x33)  /* SPI3 global Interrupt */
#define UART4_IRQChannel             ((u8)0x34)  /* UART4 global Interrupt */
#define UART5_IRQChannel             ((u8)0x35)  /* UART5 global Interrupt */
#define TIM6_IRQChannel              ((u8)0x36)  /* TIM6 global Interrupt */
#define TIM7_IRQChannel              ((u8)0x37)  /* TIM7 global Interrupt */
#define DMA2_Channel1_IRQChannel     ((u8)0x38)  /* DMA2 Channel 1 global Interrupt */
#define DMA2_Channel2_IRQChannel     ((u8)0x39)  /* DMA2 Channel 2 global Interrupt */
#define DMA2_Channel3_IRQChannel     ((u8)0x3A)  /* DMA2 Channel 3 global Interrupt */
#define DMA2_Channel4_5_IRQChannel   ((u8)0x3B)  /* DMA2 Channel 4 and DMA2 Channel 5 global Interrupt */

#endif 



