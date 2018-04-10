#include <Arduino.h>
#include <Wire.h>

#include "DataStructure.h"
#include "Sensors.h"

/*
 * Concatenate preprocessor tokens A and B without expanding macro definitions
 * (however, if invoked from a macro, macro arguments are expanded).
 */
#define PPCAT_NX(A, B) A ##_## B
/*
 * Concatenate preprocessor tokens A and B after macro-expanding them.
 * Then, e.g., PCONCAT_(s, 1) produces the identifier s1.
 */
#define PCONCAT_(A, B) PPCAT_NX(A, B)

extern struct _Sensor Sensor;
extern struct _Flags Flags;
extern struct PCONCAT_(,GYRO_MODEL) GYRO_MODEL;

//! MPU6050 센서관련 설정
void MPU6050_Init()
{
  GYRO_MODEL.address = 0x68; // 6050 자이로센서의 통신주소
  GYRO_MODEL.whoAmIREG = 0x75; // 6050 자이로센서의 Who am I register 위치
  GYRO_MODEL.whoAmIValue = 0x68; // 6050 자이로센서의 Who am I value 
  
  //센서 작동시작
  Wire.beginTransmission(GYRO_MODEL.address);
  Wire.write(0x6B); // 레지스터 PWR_MGMT_1에 접근
  Wire.write(0x00); // 값 변경
  Wire.endTransmission();
  
  //자이로 설정 변경
  Wire.beginTransmission(GYRO_MODEL.address);
  Wire.write(0x1B); // GYRO_CONFIG
  Wire.write(0x08); // Full scale range를 +/- 500도/초 로 변경
  Wire.endTransmission();
  //가속도계 설정 변경
  Wire.beginTransmission(GYRO_MODEL.address);
  Wire.write(0x1C); // ACCEL_CONFIG
  Wire.write(0x10); // Full scale range를 +/- 8g로 변경 
  Wire.endTransmission();
  
  //내장 Digital LPF 설정
  Wire.beginTransmission(GYRO_MODEL.address);
  Wire.write(0x1A); // CONFIG
  Wire.write(0x03); // 대역폭: 가속도계-44Hz(4.9ms delay), 자이로-42Hz(4.8ms delay)
  Wire.endTransmission();
  
}

unsigned char MPU6050_IDCheck()
{
  unsigned long timeToWait;
  
  Wire.beginTransmission(GYRO_MODEL.address);
  Wire.write(GYRO_MODEL.whoAmIREG);
  Wire.endTransmission();
  Wire.requestFrom(GYRO_MODEL.address, 1);

  // 최대 100ms 대기
  timeToWait = millis() + 100;
  while(Wire.available() < 1 && timeToWait > millis()){}
  
  return Wire.read();
}

void MPU6050_ReadData()
{
  unsigned long timeToWait;
  
  Wire.beginTransmission(GYRO_MODEL.address);
  Wire.write(0x3B); // 0x3B 부터 읽기 접근
  Wire.endTransmission();
  Wire.requestFrom(GYRO_MODEL.address,14); //14 바이트 추출

  // 최대 100ms 대기
  timeToWait = millis() + 100;
  while(Wire.available() < 14 && timeToWait > millis()){} // (100ms > 대기시간) 이면서 데이터가 모자라면 대기
  if(timeToWait < millis()) // (100ms < 대기시간) 으로 루프가 종료된 경우 비정상판정
  {
    Flags.sensorReadTimeout=1;
  }
  else
  {
    GYRO_MODEL.accXInput = Wire.read()<<8|Wire.read();
    GYRO_MODEL.accYInput = Wire.read()<<8|Wire.read();
    GYRO_MODEL.accZInput = Wire.read()<<8|Wire.read();
    GYRO_MODEL.temperatureInput = Wire.read()<<8|Wire.read();
    GYRO_MODEL.pInput = Wire.read()<<8|Wire.read();
    GYRO_MODEL.qInput = Wire.read()<<8|Wire.read();
    GYRO_MODEL.rInput = Wire.read()<<8|Wire.read();
  }
  
}

