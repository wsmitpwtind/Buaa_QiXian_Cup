#include "MPU6050.h"
#include "DataProces.h"


#define  Dev_ID          MPU6050_ADDRESS_AD0_LOW  //ADO管脚为0的设备地址，为高则为：MPU6050_ADDRESS_AD0_HIGH

sint   MPU6050_FIFO[6];
sint   Gx_offset=0,Gy_offset=0,Gz_offset=0;  //舵机全局偏量
uchar  buffer[14]; 
float  Acc1G_Values=0;

/*
功能:读 修改写指定设备,指定寄存器一个字节 中的多个位
返回:  成功为0;  失败为!0
*/ 
uchar IICwriteBits(uchar reg,uchar bitStart,uchar length,uchar data)
{
	uchar  i,k;
	if(I2C_ReceByte(1,Dev_ID ,reg,&i)) return 1;
	k = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
	data <<= (8 - length);
	data >>= (7 - bitStart);
	i&= k;
	i|= data;
	if(I2C_SendByte(Dev_ID ,reg,i))   return 2;
	return 0;
}

/*
功能: 读 修改 写 指定设备 指定寄存器一个字节 中的1个位
返回: 成功0  失败1
*/ 
uchar IICwriteBit(uchar reg, uchar bitNum, uchar data)
{
    uchar i;
	if(I2C_ReceByte(1,Dev_ID ,reg,&i))  return 1;
    i = (data!= 0) ? (i|(1 << bitNum)):(i & ~(1 << bitNum));
	if(I2C_SendByte(Dev_ID ,reg,i))     return 2;
	return 0;
}

//功能:读取三个轴加速度的总和
float MPU6050_1GValue(void)
{return Acc1G_Values;}

//功能:	 将新的数据更新到FIFO数组，进行平均滤波处理
void  MPU6050_Mean(sint ax,sint ay,sint az,sint gx,sint gy,sint gz)
{
static sint Filt_FIFO[3][4];

MPU6050_FIFO[0]=ax;//将新的数据放置到 数据的最后面
MPU6050_FIFO[1]=ay;
MPU6050_FIFO[2]=az;

MPU6050_FIFO[3]=Mean_Filt_int(gx,&Filt_FIFO[0][0],1);
MPU6050_FIFO[4]=Mean_Filt_int(gy,&Filt_FIFO[1][0],1);
MPU6050_FIFO[5]=Mean_Filt_int(gz,&Filt_FIFO[2][0],1);
}

/**************************实现函数********************************************
*函数原型:		void MPU6050_setClockSource(uchar source)
*功　　能:	    设置  MPU6050 的时钟源
 * CLK_SEL | Clock Source
 * --------+--------------------------------------
 * 0       | Internal oscillator
 * 1       | PLL with X Gyro reference
 * 2       | PLL with Y Gyro reference
 * 3       | PLL with Z Gyro reference
 * 4       | PLL with external 32.768kHz reference
 * 5       | PLL with external 19.2MHz reference
 * 6       | Reserved
 * 7       | Stops the clock and keeps the timing generator in reset
*******************************************************************************/
uchar MPU6050_setClockSource(uchar source)
{
return IICwriteBits(MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, source);
}


uchar MPU6050_setFullScaleGyroRange(uchar range) 
{
return IICwriteBits(MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);
}

/**************************实现函数********************************************
*函数原型:		void MPU6050_setFullScaleAccelRange(uchar range)
*功　　能:	    设置  MPU6050 加速度计的最大量程
*******************************************************************************/
uchar MPU6050_setFullScaleAccelRange(uchar range) 
{
return IICwriteBits(MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
}

/**************************实现函数********************************************
*函数原型:		void MPU6050_setSleepEnabled(uchar enabled)
*功　　能:	    设置  MPU6050 是否进入睡眠模式
				enabled =1   睡觉
			    enabled =0   工作
*******************************************************************************/
uchar MPU6050_setSleepEnabled(uchar enabled) 
{
return IICwriteBit(MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, enabled);
}



//功能:读取  MPU6050 WHO_AM_I 标识	 将返回 Dev_ID (0x68)
uchar MPU6050_getDeviceID(void) 
{
	
uchar i;
I2C_ReceByte(1,Dev_ID ,MPU6050_RA_WHO_AM_I,&i);
	
//ShowData(i,0,50, 5);
return i;
}

//功能:	检测MPU6050 是否已经连接 0:有设备   1：无
uchar MPU6050_testConnection(void) 
{
if(MPU6050_getDeviceID()== Dev_ID) 
return 0;
return 1;
}


//功能:	设置 MPU6050 是否为AUX I2C线的主机
uchar MPU6050_setI2CMasterModeEnabled(uchar enabled) 
{
return IICwriteBit(MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_EN_BIT, enabled);
}


//功能:	设置 MPU6050 是否为AUX I2C线的主机
uchar MPU6050_setI2CBypassEnabled(uchar enabled) 
{
return IICwriteBit(MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, enabled);
}


/*
功能: 
初始化IMU相关	初始化各个传感器
输出参数:
0xff: 无设备 
其他：通讯故障
*/
uchar MPU6050_initialize(void) 
{
sint temp[6];
	uchar i;
	

	//I2C_Init(200); //和OLED共用，已经初始化了
	
	if(MPU6050_testConnection())   return 0xff;   //功能:	检测MPU6050 是否已经连接 0:有设备 1:无
	delay(50);
	
    i=MPU6050_setClockSource(MPU6050_CLOCK_PLL_XGYRO);    if(i)return 1;  //设置时钟
    i=MPU6050_setFullScaleGyroRange(MPU6050_GYRO_FS_2000);  if(i)return 2; //陀螺仪最大量程 +-2000度每秒
    i=MPU6050_setFullScaleAccelRange(MPU6050_ACCEL_FS_2);	   if(i)return 3; //加速度度最大量程 +-2G
    i=MPU6050_setSleepEnabled(0);                       if(i)return 4; //进入工作状态
		i=MPU6050_setI2CMasterModeEnabled(0);	               if(i)return 5; //不让MPU6050 控制AUXI2C
		i=MPU6050_setI2CBypassEnabled(1);	                   if(i)return 6; //主控制器的I2C与	MPU6050的AUXI2C	直通
	
	//配置MPU6050 的中断模式 和中断电平模式
	i=IICwriteBit(MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_INT_LEVEL_BIT, 0);   if(i)return 7; 
	i=IICwriteBit(MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_INT_OPEN_BIT, 0);   if(i)return 8; 
	i=IICwriteBit(MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_LATCH_INT_EN_BIT, 1);if(i)return 9; 
	i=IICwriteBit(MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_INT_RD_CLEAR_BIT, 1);if(i)return 10; 
    i=IICwriteBit(MPU6050_RA_INT_ENABLE, MPU6050_INTERRUPT_DATA_RDY_BIT, 1); if(i)return 11;  //开数据转换完成中断
	
    for(i=0;i<20;i++)//更新FIFO数组
	{delayus(500);MPU6050_GetXYZ(temp);}
	MPU6050_InitGyro_Offset(); //初始化陀螺仪的偏置，此时模块应该不要被移动
	delay(50);
	return 0; 
}

/*
功能：检查 MPU6050的中断引脚，测试是否完成转换
返回：1  转换完成
      0 数据寄存器还没有更新
*/
/*
uchar MPU6050_is_DRY(void)
{
if(Mpu6050_INT_input==1) 
return 1;
return 0;
}
*/

//功能:	读取 MPU6050的当前测量值
void MPU6050_GetXYZ(sint * values) 
{
	//if(MPU6050_is_DRY())
	{
		I2C_ReceString(Dev_ID ,MPU6050_RA_ACCEL_XOUT_H,14,buffer); 


		MPU6050_Mean   //将新的数据更新到FIFO数组，进行平均滤波处理
		(
		(((sint)buffer[0]) << 8) | buffer[1],
		(((sint)buffer[2]) << 8) | buffer[3],
		(((sint)buffer[4]) << 8) | buffer[5], //跳过温度ADC
		(((sint)buffer[8]) << 8) | buffer[9],
		(((sint)buffer[10]) << 8)| buffer[11],
		(((sint)buffer[12]) << 8)| buffer[13]
		);
		
	 }
	values[0] = MPU6050_FIFO[0];
	values[1] = MPU6050_FIFO[1];
	values[2] = MPU6050_FIFO[2];
	values[3] = (MPU6050_FIFO[3]-Gx_offset); 
	values[4] = (MPU6050_FIFO[4]-Gy_offset); 
	values[5] = (MPU6050_FIFO[5]-Gz_offset);
	 
}

/*
void MPU6050_GetXYZ_last(sint* ax, sint* ay,sint* az, sint* gx, sint* gy, sint* gz)
{
	*ax  =MPU6050_FIFO[0];
	*ay  =MPU6050_FIFO[1];
	*az = MPU6050_FIFO[2];
	*gx  =MPU6050_FIFO[3]-Gx_offset;
	*gy = MPU6050_FIFO[4]-Gy_offset;
	*gz = MPU6050_FIFO[5]-Gz_offset;
}
*/
/**************************实现函数********************************************
*函数原型:		void MPU6050_InitGyro_Offset(void)
*功　　能:	    读取 MPU6050的陀螺仪偏置
此时模块应该被静止放置。以测试静止时的陀螺仪输出
*******************************************************************************/
void MPU6050_InitGyro_Offset(void)
{
unsigned char i;
sint temp[6];
long	tempgx=0,tempgy=0,tempgz=0;
long	tempax=0,tempay=0,tempaz=0;
Gx_offset=0;
Gy_offset=0;
Gz_offset=0;
for(i=0;i<50;i++){
delayus(100);
MPU6050_GetXYZ(temp);

}
 for(i=0;i<100;i++){
	delayus(200);
	MPU6050_GetXYZ(temp);
	tempax+= temp[0];
	tempay+= temp[1];
	tempaz+= temp[2];
	tempgx+= temp[3];
	tempgy+= temp[4];
	tempgz+= temp[5];

	}

Gx_offset=tempgx/100;//MPU6050_FIFO[3][10];
Gy_offset=tempgy/100;//MPU6050_FIFO[4][10];
Gz_offset=tempgz/100;//MPU6050_FIFO[5][10];
tempax/=100;
tempay/=100;
tempaz/=100;
Acc1G_Values= (float)(tempax+tempay+tempaz);
}



//------------------End of File----------------------------
