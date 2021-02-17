#include "BoardLib.h"



void PS2Running(void);

int main (void)
{

BoardInit();

//ShowString("<P2MDA-KM4> ",0,30,0);
printf("P2MDA-KM4��STM32�ĵ�������壩\r\n");;

while(1)
{
	if( WaitTrue(0,50) ) //50ms����һ��
	{
		 printf("Begin\r\n");
		 printf("|-----------GetMileage-----------|\r\n");
		 GetMileage();		//���ڼ������
	}
	
	if( WaitTrue(1,500) ) //200MSִ��һ��
	{
		
		//OLED��ʾ
//		ShowString("AR:       ",0,0,2);	ShowData(MotorRPM_A()/10,0,18,2);
//		ShowString("AL:       ",0,64,2); ShowData(GetLinearVelocity_A(),0,82,2);
//		
//		ShowString("BR:       ",0,0,3);	ShowData(MotorRPM_B()/10,0,18,3);
//		ShowString("BL:       ",0,64,3); ShowData(GetLinearVelocity_B(),0,82,3);
//		
//		ShowString("CR:       ",0,0,4);	ShowData(MotorRPM_C()/10,0,18,4);
//		ShowString("CL:       ",0,64,4); ShowData(GetLinearVelocity_C(),0,82,4);
//		
//		ShowString("DR:       ",0,0,5);	ShowData(MotorRPM_D()/10,0,18,5);
//		ShowString("DL:       ",0,64,5); ShowData(GetLinearVelocity_D(),0,82,5);
//		
		
		//���ڴ�ӡ
		printf("A���ת��=%ld ת/s\r\n",MotorRPM_A()/10);
		printf("A������ٶ�=%d mm/s\r\n",GetLinearVelocity_A());
		printf("A��������=%ld mm\r\n",walkingmotor_cumulate_A_mm());
		
		printf("----------------------\r\n");
		
		printf("B���ת��=%ld ת/s\r\n",MotorRPM_B()/10);
		printf("B������ٶ�=%d mm/s\r\n",GetLinearVelocity_B());
		printf("B��������=%ld mm\r\n",walkingmotor_cumulate_B_mm());
		
		printf("----------------------\r\n");
		printf("C���ת��=%ld ת/s\r\n",MotorRPM_C()/10);
		printf("C������ٶ�=%d mm/s\r\n",GetLinearVelocity_C());
		printf("C��������=%ld mm\r\n",walkingmotor_cumulate_C_mm());
		
		printf("----------------------\r\n");
		
		printf("D���ת��=%ld ת/s\r\n",MotorRPM_D()/10);
		printf("D������ٶ�=%d mm/s\r\n",GetLinearVelocity_D());
		printf("D��������=%ld mm\r\n",walkingmotor_cumulate_D_mm());
		
		printf("----------------------\r\n");
		
		
	}
	
	AT_CommRunning();
	LedFlash();// LED��˸
}

}

