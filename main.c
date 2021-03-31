#include "BoardLib.h"



sint memsBuf[6];

_st_Mpu mpu_read;
_st_AngE mpu_out;

void PS2Running(void);

int main (void)
{
	
	
int i=0;
	
BoardInit();
	

	
printf("P2MDA-KM4（STM32四电机驱动板）\r\n");
set_walkingmotor_speed(600,600,600,600);
	
Init_time();

i=MPU6050_initialize();


PID_Data_Recived(90,3,0);
PIDParSPDUpdate();
	
while(1)
{
	
//	if( WaitTrue(0,200) ) //50ms运行一次
//	{
//		MPU6050_GetXYZ(memsBuf);
//		mpu_read.accX=(int16_t) memsBuf[0];
//		mpu_read.accY=(int16_t) memsBuf[1];
//		mpu_read.accZ=(int16_t) memsBuf[2];
//		mpu_read.gyroX=(int16_t) memsBuf[3];
//		mpu_read.gyroY=(int16_t) memsBuf[4];
//		mpu_read.gyroZ=(int16_t) memsBuf[5];
//		GetAngle(&mpu_read,&mpu_out,200);
//		
//		
//	}
//	
//	//set_walkingmotor_speed(600,600,600,600);
//	if( TimeOn(3000) ) 
//	{
//		set_walkingmotor_speed(0,0,0,0);
////		MotA(0);	
////		MotB(0);
////		MotC(0);
////		MotD(0);
//	}
//	if( TimeOn(5000) ) 
//	{
//		 set_walkingmotor_speed(-400,600,-400,600);
//	}
//	if( TimeOn(10000) ) 
//	{
//		 set_walkingmotor_speed(0,0,0,0);
//	}
//  if( TimeOn(12000) ) 
//	{
//		 set_walkingmotor_speed(600,600,600,600);
//	}
//	if( WaitTrue(0,50) ) //50ms运行一次
//	{

//		 GetMileage();		//用于计算里程
//		 //MPU6050_GetXYZ(memsBuf);
//		 //printf();
//	}
//	
//	if( WaitTrue(1,200) ) //200MS执行一次
//	{

//	
//		//串口打印
////		printf("A电机转速=%ld 转/s\r\n",MotorRPM_A()/10);
////		printf("A电机线速度=%d mm/s\r\n",GetLinearVelocity_A());
////		printf("A电机线里程=%ld mm\r\n",walkingmotor_cumulate_A_mm());
////		
////		printf("----------------------\r\n");
////		
////		printf("B电机转速=%ld 转/s\r\n",MotorRPM_B()/10);
////		printf("B电机线速度=%d mm/s\r\n",GetLinearVelocity_B());
////		printf("B电机线里程=%ld mm\r\n",walkingmotor_cumulate_B_mm());
////		
////		printf("----------------------\r\n");
////		printf("C电机转速=%ld 转/s\r\n",MotorRPM_C()/10);
////		printf("C电机线速度=%d mm/s\r\n",GetLinearVelocity_C());
////		printf("C电机线里程=%ld mm\r\n",walkingmotor_cumulate_C_mm());
////		
////		printf("----------------------\r\n");
////		
////		printf("D电机转速=%ld 转/s\r\n",MotorRPM_D()/10);
////		printf("D电机线速度=%d mm/s\r\n",GetLinearVelocity_D());
////		printf("D电机线里程=%ld mm\r\n",walkingmotor_cumulate_D_mm());
////		
////		printf("----------------------\r\n");
//		
//		
//	}
//	
//	//AT_CommRunning();
//	LedFlash();// LED闪烁
	
}

}

