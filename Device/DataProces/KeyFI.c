#include "KeyFI.h"
#include "DataProces.h"



#define KF_Group  3 //滤波的组数
#define KF_Num 	  5  //每组的滤波长度，数量（FIFO数量）


uchar KFBuf[KF_Group][KF_Num];
uchar KeySTA[KF_Group];
uchar KeySTA_UD[KF_Group];

/*
按键滤波输入，应该间隔一段时间，周期调用
*/
void  KeyFiltIn(uchar Group,uchar Vol)
{
	uchar Add=0,i=0;
	if(Vol) Vol=1;
	FIFO_uchar(Vol,&KFBuf[Group][0],KF_Num);
	Add=0;
	for(i=0;i<KF_Num;i++) Add+=KFBuf[Group][i];
	
	if(Add>=KF_Num) //连续8次有障碍
	{
		KeySTA[Group]=1;
	}
	else if(Add==0)//连续8次无障碍
	{
		KeySTA[Group]=0;
	}
}


uchar  KeyFiltOut(uchar Group)
{
	return KeySTA[Group];
}


/*
返回边沿状态，在KFSynchronous()调用期间检测的
需要KeyFiltUD_CLR清0
=1; 遇到上升沿
=2; 遇到下沿

*/
uchar  KeyFiltOut_UD(uchar Group)
{
	return KeySTA_UD[Group];
}

uchar  KeyFiltUD_CLR(uchar Group)
{
	return KeySTA_UD[Group]=0;
}





/*
以下是额外的函数，需要测量按键状态时间才用到
在同步时间里面调用
*/



uchar KeySTASave[KF_Group];
uchar KKeepTime[KF_Group];

void KFSynchronous(void)
{
uchar i=0;

for(i=0;i<KF_Group;i++) 
{
	//记录按下和放开的时间，最大255
	if(KeySTASave[i]!=KeySTA[i])  //按键状态变化
	{
		KKeepTime[i]=0;
		if(KeySTA[i])
		{
			KeySTA_UD[i]=1; //遇到上升沿
		}
		else
		{
			KeySTA_UD[i]=2; //遇到下沿
		}
	}

	if(KKeepTime[i]<255)
	KKeepTime[i]++;
	KeySTASave[i]=KeySTA[i];
}


}




/*
返回按键值变化开始计数，直到255停止，单位为间隔时间
*/
uchar KeyKeepTime(uchar Group)
{
	return KKeepTime[Group];
}







