#include "stm32f10x_Reg.h"
#include "stm32f10x_Init.h"
#include "stm32f10x_type.h"
#include "delay.h"



/********************************************************

����STM32ϵͳ �δ�ʱ�����ã������ж�ʱ��
�ж�����ΪSysBeat_Cycle�����ļ�Interrupt.c��ִ��SysTick_Handler�����жϺ���

********************************************************/

void  SysTick_Init(void)
{
	ulong facms=0;
	SysTick_CTRL&=0xfffffffb;//bit2���,ѡ���ⲿʱ��  HCLK/8
	facms=Sys_HSE*Sys_PLL/8*1000;

	SysTick_LOAD=facms*SysBeat_Cycle;//ʱ�����	  		 
	SysTick_VAL=0x00;          				//��ռ�����
	SysTick_CTRL |=(1<<0 |1<<1); 			//��ʼ���� ,�����ж�ʹ��
}

void  SysTick_Init_1ms(void)
{
	ulong facms=0;
	SysTick_CTRL&=0xfffffffb;//bit2���,ѡ���ⲿʱ��  HCLK/8
	facms=Sys_HSE*Sys_PLL/8*1000;

	SysTick_LOAD=facms;//ʱ�����	  		 
	SysTick_VAL=0x00;          				//��ռ�����
	SysTick_CTRL |=(1<<0 |1<<1); 			//��ʼ���� ,�����ж�ʹ��
}



/********************************************************
ʹ��STM32���е�һ����ʱ��

ʵ�֣�
1���ɻ�ȡϵͳ����ʱ��
2����׼��ʱ�����������жϴ���ʱ��Ӱ��
3�����ڼ���

����1us��ÿ50ms�ж�һ�Σ��ж��в���ִ����������
���ȼ������
********************************************************/

void SysTimeBase_Init(void)  
{
Dev_Sck_En(SckEn_AFIO, 1);	   //��������ʱ��
Dev_Sck_En(SckEn_TIM3, 1);     //����TIM2ʱ��
Nvic_Init(2,0,0,TIM3_IRQChannel);

TIM3_CR1=0;
TIM3_CR2=0;
TIM3_SMCR=0;
TIM3_DIER=0;
TIM3_SR=0;
TIM3_CNT=0;
TIM3_PSC=71;   	 //f=Fck/PSC+1=72M/72.  1us����һ��
TIM3_ARR=49999;  //50ms�ж�һ��
 
TIM3_DIER|=1<<0;   //����ж�ʹ��
	
//��������4���Ƚ�ͨ��������������ģʽ----------

TIM3_CCMR1=0;  //ͨ��12�������ƥ���ܽŲ������Ϣ
TIM3_CCMR2=0;  //ͨ��34�������ƥ���ܽŲ������Ϣ
TIM3_CCR1=0;
TIM3_CCR2=0;
TIM3_CCR3=0;
TIM3_CCR4=0;

//---------------------------------------------
TIM3_CR1=0x01;     //����ʱ�Ӽ���
}


/*************************************

�ö�ʱ��ʵ�ֵ���ʱ

*************************************/

ulong SysRunTimeCnt=0;

//��ʱ������е��ã�������ʱ�ͼ�����50ms�ж�һ��
void BeatHandle_delay(void)
{
	SysRunTimeCnt++;
}


//��ȡϵͳ����ʱ�䣬������ʹ��Ƶ��������ֵ��λ������
ulong SysRunTime(void)
{

	ulong i=0,k=0;
	//����Ļ���Ҫ��Ӧ�����жϹ���
	i=SysRunTimeCnt;
	k=TIM3_CNT;
	return( i*50 + k/1000 );
}


/*
��ʱ����
�����ʱ4000000ms
��ڵ�λ��ms
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
�����ʱ50ms
��ڵ�λ��us
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
��������
Channel��ͨ������ ֧��0~9
**************************************/
void TimerStart(uchar Channel)
{
TimerParam[Channel].ms=SysRunTimeCnt;
TimerParam[Channel].us=TIM3_CNT;
}


/***************************************

�رռ��������ؼ���ʱ������λus
Channel��ͨ������ ֧��0~9
����ֵ��λ��΢�룬�ұ����ֵ��TimerParam[Channel].cnt

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

//ѭ��ʱ�亯����������һ��ʱ����ѭ���������������ڷ�����
/*
�÷��� ��2000ms�ڵ���fun����

LoopTime(2000);  // ��2000ms�ڣ�ִ��fun����
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



//��������������ģʽ�µ�����----------

void NullFun(void)
{
}

void  (*UserFun0)()=NullFun; //����ָ��
void  (*UserFun1)()=NullFun; //����ָ��
void  (*UserFun2)()=NullFun; //����ָ��
void  (*UserFun3)()=NullFun; //����ָ��




/*
��ͬ����usʱ��󣬲���һ���ж�

Channel:֧�� 0~3��Ӳ��ͨ��
us����λus,ȡֵ 1~49999 �����50ms
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
	if(temp>50000) temp-=50000; //�������ֵ

	switch(Channel)
	{
		case 0:TIM3_CCR1=temp; break;
		case 1:TIM3_CCR2=temp; break;
		case 2:TIM3_CCR3=temp; break;
		case 3:TIM3_CCR4=temp; break;	
	}
	TIM3_SR=0;
	TIM3_DIER|=1<<(1+Channel); //������ͨ�����ж�ʹ��
}



void BeatHandle_SinglePulse(void)
{
	uchar i=0;
	usint TIMSR=0;
	
	for(i=0;i<10;i++)  //��ʱ��������Σ���ֹ�Ƚ�ͬʱ��λ��
	{
		TIMSR=TIM3_SR;
	}
	
	if(TIMSR&0x0002) 
	{
		TIM3_SR&=~0x0002;
		TIM3_DIER&=~(0x02);//�Ƚ�ƥ�䣬�رո�ͨ�����ж�ʹ��
		UserFun0();
	}
	
	if(TIMSR&0x0004) 
	{
		
		TIM3_SR&=~0x0004;
		TIM3_DIER&=~(0x04);//�Ƚ�ƥ�䣬�رո�ͨ�����ж�ʹ��
		UserFun1();
	}
	

	if(TIMSR&0x0008) 
	{
		TIM3_SR&=~0x0008;
		TIM3_DIER&=~(0x08);//�Ƚ�ƥ�䣬�رո�ͨ�����ж�ʹ��
		UserFun2();
	}
	
	
	if(TIMSR&0x0010)
	{
		TIM3_SR&=~0x0010;
		TIM3_DIER&=~(0x10);//�Ƚ�ƥ�䣬�رո�ͨ�����ж�ʹ��
		UserFun3();	
	}
	

	
}




//---------------------------------------------------------------
/*
ÿ���N��ms�����ֵ��Ϊ��
��������ѭ����
Num:֧��10�鲻ͬ���ж�
GapTime��ÿ�μ��GapTime����󷵻���
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
���¹������������ִ��ĳһ�麯�������ҳ��򲻶���
ÿ���������Լ�������ʱ��
����
1���ȴ��������߷��䣬Ȼ�����������ݣ�ÿ���10ms���һ��
2���ȴ�������ɣ�Ȼ���жϷ���״̬��ÿ���5ms���һ��
3���ȴ���������....

***********************************************************/
 
struct Str_TaskLine Strtk[5]; //5��ṹ��

uchar WaitTrue_TL(uchar Num,ulong GapTime)  //������WaitTrue
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
	
	
	if(setp!=setpSV)  //������ӵļ�¼ִ��ʱ��
	{
		setpSV=setp;
		Strtk[Num].RunTim=SysRunTime();
	}
	
	if( WaitTrue_TL(Num,Strtk[Num].CyTime[setp]) ) //���ʱ��ִ��һ��
	{
		(*Strtk[Num].TaskLineFP[setp])();  //ִ�е�TLSetp������
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
	//Strtk[Num].StopMark=0; //���������־λ
	Strtk[Num].RunTim=SysRunTime();
	Strtk[Num].TLSetp=0;
}


void TaskLineSetp_Stop(uchar Num)
{
//	Strtk[Num].StopMark=1; //���������־λ
	Strtk[Num].TLSetp=TKLNull;
	
}

uchar TaskLineSetp_Get(uchar Num)
{
	return Strtk[Num].TLSetp;
}

