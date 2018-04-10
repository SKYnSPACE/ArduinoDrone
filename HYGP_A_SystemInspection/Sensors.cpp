#include <Arduino.h>
#include <Wire.h>

#include "DataStructure.h"
#include "Sensors.h"

extern struct _Sensor Sensor;

//! MPU6050 센서관련 설정
void MPU6050_Init()
{
  Sensor.gyroAddress = 0x68; // 6050 자이로센서의 통신주소
  Sensor.gyroWhoAmIREG = 0x75; // 6050 자이로센서의 Who am I register 위치
  Sensor.gyroWhoAmIValue = 0x68; // 6050 자이로센서의 Who am I value 
}

unsigned char MPU6050_IDCheck(int address, int whoAmIRegister)
{
  unsigned long timer;
  
  Wire.beginTransmission(address);
  Wire.write(whoAmIRegister);
  Wire.endTransmission();
  Wire.requestFrom(address, 1);

  // 최대 100ms 대기
  timer = millis() + 100;
  while(Wire.available() < 1 && timer > millis()); 
  
  return Wire.read();
}

