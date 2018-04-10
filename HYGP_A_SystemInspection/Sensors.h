#ifndef SENSORS_H
#define SENSORS_H

void MPU6050_Init();
unsigned char MPU6050_IDCheck(int address, int whoAmIRegister);

#endif
