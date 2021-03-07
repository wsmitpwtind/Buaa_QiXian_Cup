#ifndef __IMU_H
#define	__IMU_H

#include <stdint.h>

typedef struct{
	int16_t accX;
	int16_t accY;
	int16_t accZ;
	int16_t gyroX;
	int16_t gyroY;
	int16_t gyroZ;
}_st_Mpu;

typedef struct{
	float roll;
	float pitch;
	float yaw;
}_st_AngE;

extern _st_Mpu MPU6050;
extern _st_AngE Angle;

extern void GetAngle(const _st_Mpu *pMpu,_st_AngE *pAngE, float dt);
extern float GetNormAccz(void);

#endif
