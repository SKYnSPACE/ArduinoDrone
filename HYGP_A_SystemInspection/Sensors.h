#ifndef SENSORS_H
#define SENSORS_H

#define GYRO_MODEL MPU6050

void MPU6050_Init();
unsigned char MPU6050_IDCheck();
void MPU6050_ReadData();

struct _MPU6050
{
  int address; // Arduino HAL takes int, not unsigned char.
  int whoAmIREG;
  int whoAmIValue;
  
  short accXInput, accYInput, accZInput;
  float accX, accY, accZ;
  
  short pInput, qInput, rInput;
  float p, q, r;
  float pCalib, qCalib, rCalib;

  short temperatureInput;
};

#endif
