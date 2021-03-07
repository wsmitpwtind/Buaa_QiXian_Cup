#include "stm32f10x_Reg.h"
#include "stm32f10x_Init.h"
#include "stm32f10x_type.h"
#include "delay.h"



/********************************************************

以下STM32系统 滴答时钟配置，用于中断时基
中断周期为SysBeat_Cycle，在文件Interrupt.c中执行SysTick_Handler（）中断函数

********************************************************/

void  SysTick_Init(void)
{
	ulong facms=0;
	SysTick_CTRL&=0xfffffffb;//bit2清空,选择外部时钟  HCLK/8
	facms=Sys_HSE*Sys_PLL/8*1000;

	SysTick_LOAD=facms*SysBeat_Cycle;//时间加载	  		 
	SysTick_VAL=0x00;          				//清空计数器
	SysTick_CTRL |=(1<<0 |1<<1); 			//开始倒数 ,开启中断使能
}

void  SysTick_Init_1ms(void)
{
	ulong facms=0;
	SysTick_CTRL&=0xfffffffb;//bit2清空,选择外部时钟  HCLK/8
	facms=Sys_HSE*Sys_PLL/8*1000;

	SysTick_LOAD=facms;//时间加载	  		 
	SysTick_VAL=0x00;          				//清空计数器
	SysTick_CTRL |=(1<<0 |1<<1); 			//开始倒数 ,开启中断使能
}



/********************************************************
使用STM32其中的一个定时器

实现：
1：可获取系统运行时间
2：精准延时函数，不受中断处理时间影响
3：周期计数

精度1us，每50ms中断一次，中断中不能执行其他函数
优先级最最高
********************************************************/

void SysTimeBase_Init(void)  
{
Dev_Sck_En(SckEn_AFIO, 1);	   //开启复用时钟
Dev_Sck_En(SckEn_TIM3, 1);     //开启TIM2时钟
Nvic_Init(2,0,0,TIM3_IRQChannel);

TIM3_CR1=0;
TIM3_CR2=0;
TIM3_SMCR=0;
TIM3_DIER=0;
TIM3_SR=0;
TIM3_CNT=0;
TIM3_PSC=71;   	 //f=Fck/PSC+1=72M/72.  1us计数一次
TIM3_ARR=49999;  //50ms中断一次
 
TIM3_DIER|=1<<0;   //溢出中断使能
	
//这里利用4个比较通道，可做单脉冲模式----------

TIM3_CCMR1=0;  //通道12做输出，匹配后管脚不输出信息
TIM3_CCMR2=0;  //通道34做输出，匹配后管脚不输出信息
TIM3_CCR1=0;
TIM3_CCR2=0;
TIM3_CCR3=0;
TIM3_CCR4=0;

//---------------------------------------------
TIM3_CR1=0x01;     //开启时钟计数
}


/*************************************

用定时器实现的延时

*************************************/

ulong SysRunTimeCnt=0;

//定时器溢出中调用，用于延时和计数，50ms中断一次
void BeatHandle_delay(void)
{
	SysRunTimeCnt++;
}


//获取系统运行时间，程序中使用频繁，返回值单位：毫秒
ulong SysRunTime(void)
{

	ulong i=0,k=0;
	//这里的话高要求应该用中断挂起
	i=SysRunTimeCnt;
	k=TIM3_CNT;
	return( i*50 + k/1000 );
}


/*
延时函数
最大延时4000000ms
入口单位：ms
*/
void delay(ulong ms)
{
	ulong  nowTim_ms;
	ulong  nowTim_us;	
	ulong  gapTim_ms=SysRunTimeCnt;
	ulong  gapTim_us=TIM3_CNT&0x0000ffff;

	do
	{
	nowTim_ms=SysRunTimeCnt;
	nowTim_us=TIM3_CNT&0x0000ffff;
	}
	while( ((nowTim_ms-gapTim_ms)*50000+nowTim_us)< gapTim_us+ms*1000 );
}

/*
最大延时50ms
入口单位：us
*/
void delayus(ulong us)
{
	ulong  gapTim=TIM3_CNT&0x0000ffff;
	ulong  gapTimMs=SysRunTimeCnt;

	gapTim+=us;

	if( gapTim < 50000 )
	{
		while(TIM3_CNT<gapTim);
		return;
	}
	else
	{
		while( !((gapTimMs!=SysRunTimeCnt)&&(TIM3_CNT>=(gapTim-50000)) ));
		return;
	}
}





TypeTimerPar TimerParam[10];

/**************************************
开启计数
Channel：通道号码 支持0~9
**************************************/
void TimerStart(uchar Channel)
{
TimerParam[Channel].ms=SysRunTimeCnt;
TimerParam[Channel].us=TIM3_CNT;
}


/***************************************

关闭计数，返回计数时长，单位us
Channel：通道号码 支持0~9
返回值单位：微秒，且保存该值在TimerParam[Channel].cnt

***************************************/
ulong TimerStop(uchar Channel)
{
ulong  gapTim_ms=SysRunTimeCnt;
ulong  gapTim_us=TIM3_CNT;
	
if(gapTim_ms<TimerParam[Channel].ms) return 0;
	
TimerParam[Channel].cnt=(gapTim_ms-TimerParam[Channel].ms)*50000 + gapTim_us- TimerParam[Channel].us;

return TimerParam[Channel].cnt;

}



ulong TimerGet(uchar Channel)
{
	return TimerParam[Channel].cnt;
}




//------------------------------------------------------------------------

//循环时间函数，用在在一定时间内循环函数，可以用于防堵塞
/*
用法： 在2000ms内调用fun函数

LoopTime(2000);  // 在2000ms内，执行fun（）
while(LoopSTA())
{
	fun()...
}
*/


static ulong lastT=0;
static ulong loopT=0;

void LoopTime(ulong ms)
{
loopT=ms;
lastT=SysRunTime();
}


uchar LoopSTA(void)
{
	if(SysRunTime()>=lastT+loopT)
	return 0;
	else
	return 1;
}



//以下是做单脉冲模式下的配置----------

void NullFun(void)
{
}

void  (*UserFun0)()=NullFun; //函数指针
void  (*UserFun1)()=NullFun; //函数指针
void  (*UserFun2)()=NullFun; //函数指针
void  (*UserFun3)()=NullFun; //函数指针




/*
调同后，在us时间后，产生一个中断

Channel:支持 0~3个硬件通道
us：单位us,取值 1~49999 ，最大50ms
*/
void SinglePulseStart(uchar Channel,usint us, void (*UFun)())
{
	ulong temp=0;
	if(Channel>3) return;
	if((us<1)||(us>49999)) return;
	

	switch(Channel)
	{
		case 0:UserFun0=UFun; break;
		case 1:UserFun1=UFun; break;
		case 2:UserFun2=UFun; break;
		case 3:UserFun3=UFun; break;		
	}
	
	temp=TIM3_CNT;
	temp+=us;
	if(temp>50000) temp-=50000; //超出最大值

	switch(Channel)
	{
		case 0:TIM3_CCR1=temp; break;
		case 1:TIM3_CCR2=temp; break;
		case 2:TIM3_CCR3=temp; break;
		case 3:TIM3_CCR4=temp; break;	
	}
	TIM3_SR=0;
	TIM3_DIER|=1<<(1+Channel); //开启该通道的中断使能
}



void BeatHandle_SinglePulse(void)
{
	uchar i=0;
	usint TIMSR=0;
	
	for(i=0;i<10;i++)  //延时，多读几次，防止比较同时置位。
	{
		TIMSR=TIM3_SR;
	}
	
	if(TIMSR&0x0002) 
	{
		TIM3_SR&=~0x0002;
		TIM3_DIER&=~(0x02);//比较匹配，关闭该通道的中断使能
		UserFun0();
	}
	
	if(TIMSR&0x0004) 
	{
		
		TIM3_SR&=~0x0004;
		TIM3_DIER&=~(0x04);//比较匹配，关闭该通道的中断使能
		UserFun1();
	}
	

	if(TIMSR&0x0008) 
	{
		TIM3_SR&=~0x0008;
		TIM3_DIER&=~(0x08);//比较匹配，关闭该通道的中断使能
		UserFun2();
	}
	
	
	if(TIMSR&0x0010)
	{
		TIM3_SR&=~0x0010;
		TIM3_DIER&=~(0x10);//比较匹配，关闭该通道的中断使能
		UserFun3();	
	}
	

	
}




//---------------------------------------------------------------
/*
每间隔N个ms后才其值才为真
可以用在循环中
Num:支持10组不同的判断
GapTime：每次间隔GapTime毫秒后返回真
*/

ulong WaitTrueLastTime[10]={0,0,0,0,0,0,0,0,0,0};
ulong Time_Flag[2]={0,0};

uchar WaitTrue(uchar Num,ulong GapTime)
{
	static ulong	Temp=0;
	
	ulong	i=WaitTrueLastTime[Num]+GapTime;
	
	 Temp=SysRunTime();
	
		if(  Temp >= i )
		{
			WaitTrueLastTime[Num]=Temp;
			return 1;
		}
		return 0;
}

void Init_time()
{
	static ulong	Temp=0;
	 Temp=SysRunTime();
	 Time_Flag[0]=Temp;
	 Time_Flag[1]=Temp;
}

void TimeReload()
{
	static ulong	Temp=0;
	 Temp=SysRunTime();
	 Time_Flag[0]=Temp;
	 Time_Flag[1]=Temp;
}

uchar TimeOn(ulong GapTime)
{
	static ulong	Temp=0;
	ulong	i=Time_Flag[0]+GapTime;
	 Temp=SysRunTime();
	 Time_Flag[1]=Temp;
		if(  Temp >= i )
		{
			return 1;
		}
		return 0;
}



/***********************************************************
以下功能用于有序的执行某一组函数，并且程序不堵塞
每个函数有自己的轮行时间
比如
1：等待触发无线发射，然后发射无线数据，每间隔10ms检测一次
2：等待发射完成，然后判断发送状态。每间隔5ms检测一次
3：等待接受数据....

***********************************************************/
 
struct Str_TaskLine Strtk[5]; //5组结构体

uchar WaitTrue_TL(uchar Num,ulong GapTime)  //独立的WaitTrue
{
	static ulong	Temp=0;
	
	ulong	i=Strtk[Num].WaitTrueLTime+GapTime;
	
	 Temp=SysRunTime();
	
		if(  Temp >= i )
		{
			Strtk[Num].WaitTrueLTime=Temp;
			return 1;
		}
		return 0;
}



#define TKLNull 255

void TaskLineRunning(uchar Num)
{
	static uchar setpSV=0xff;
	
	uchar setp=Strtk[Num].TLSetp;
	
	if(setp==TKLNull) return;
	
	
	if(setp!=setpSV)  //这里外加的记录执行时间
	{
		setpSV=setp;
		Strtk[Num].RunTim=SysRunTime();
	}
	
	if( WaitTrue_TL(Num,Strtk[Num].CyTime[setp]) ) //间隔时间执行一次
	{
		(*Strtk[Num].TaskLineFP[setp])();  //执行第TLSetp个函数
	}
}





ulong TaskLine_GetRunTim(uchar Num)
{
	return SysRunTime()-Strtk[Num].RunTim;
}


void TaskLineSetp_UP(uchar Num)
{
		if(Strtk[Num].TLSetp)
	Strtk[Num].TLSetp--;
}

void TaskLineSetp_Down(uchar Num)
{
	Strtk[Num].TLSetp++;
}


void TaskLineSetp_Start(uchar Num)
{
	//Strtk[Num].StopMark=0; //任务结束标志位
	Strtk[Num].RunTim=SysRunTime();
	Strtk[Num].TLSetp=0;
}


void TaskLineSetp_Stop(uchar Num)
{
//	Strtk[Num].StopMark=1; //任务结束标志位
	Strtk[Num].TLSetp=TKLNull;
	
}

uchar TaskLineSetp_Get(uchar Num)
{
	return Strtk[Num].TLSetp;
}

