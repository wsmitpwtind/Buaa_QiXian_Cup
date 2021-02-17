#include "KeyFI.h"
#include "DataProces.h"



#define KF_Group  3 //�˲�������
#define KF_Num 	  5  //ÿ����˲����ȣ�������FIFO������


uchar KFBuf[KF_Group][KF_Num];
uchar KeySTA[KF_Group];
uchar KeySTA_UD[KF_Group];

/*
�����˲����룬Ӧ�ü��һ��ʱ�䣬���ڵ���
*/
void  KeyFiltIn(uchar Group,uchar Vol)
{
	uchar Add=0,i=0;
	if(Vol) Vol=1;
	FIFO_uchar(Vol,&KFBuf[Group][0],KF_Num);
	Add=0;
	for(i=0;i<KF_Num;i++) Add+=KFBuf[Group][i];
	
	if(Add>=KF_Num) //����8�����ϰ�
	{
		KeySTA[Group]=1;
	}
	else if(Add==0)//����8�����ϰ�
	{
		KeySTA[Group]=0;
	}
}


uchar  KeyFiltOut(uchar Group)
{
	return KeySTA[Group];
}


/*
���ر���״̬����KFSynchronous()�����ڼ����
��ҪKeyFiltUD_CLR��0
=1; ����������
=2; ��������

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
�����Ƕ���ĺ�������Ҫ��������״̬ʱ����õ�
��ͬ��ʱ���������
*/



uchar KeySTASave[KF_Group];
uchar KKeepTime[KF_Group];

void KFSynchronous(void)
{
uchar i=0;

for(i=0;i<KF_Group;i++) 
{
	//��¼���ºͷſ���ʱ�䣬���255
	if(KeySTASave[i]!=KeySTA[i])  //����״̬�仯
	{
		KKeepTime[i]=0;
		if(KeySTA[i])
		{
			KeySTA_UD[i]=1; //����������
		}
		else
		{
			KeySTA_UD[i]=2; //��������
		}
	}

	if(KKeepTime[i]<255)
	KKeepTime[i]++;
	KeySTASave[i]=KeySTA[i];
}


}




/*
���ذ���ֵ�仯��ʼ������ֱ��255ֹͣ����λΪ���ʱ��
*/
uchar KeyKeepTime(uchar Group)
{
	return KKeepTime[Group];
}







