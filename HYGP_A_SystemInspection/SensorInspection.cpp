#include <Arduino.h>

#include "SensorInspection.h"
#include "Messages.h"
#include "Sensors.h"


#define STR(x) #x // Change the defined variable into string
//#define SHOW_DEFINE(x) printf("%s=%s\n", #x, STR(x))

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


extern struct _Flags Flags;
extern struct _Sensor Sensor;
extern struct PCONCAT_(,GYRO_MODEL) GYRO_MODEL;

bool GyroInspection()
{
  char sensorName[16];
//TBD: sensorName 없이 출력하는법 확인.
  unsigned long timer;
  
  if((TWBR == 12) && Flags.mainFrequencyCheck)
  {
//![1]
    Serial.println(F(">>>>>>[1/4] I2C 클럭 속도 설정을 완료하였습니다."));
    Flags.gyroStatus = 1;

//![2]    
    Serial.print(F(">>>>>>[안내] "));
    sprintf(sensorName, "%s", STR(GYRO_MODEL));
    Serial.print(sensorName);
    Serial.println(F(" 센서를 탐색합니다."));
    delay(1000);
    if(PCONCAT_(GYRO_MODEL, IDCheck)() == GYRO_MODEL.whoAmIValue) // 센서에 WAI 요청하여 센서 ID 확인.
    {
      Serial.print(F(">>>>>>[2/4] MPU-6050 센서가 주소 0x"));
      Serial.print(GYRO_MODEL.address,HEX);
      Serial.println(F("로 정상 연결되었습니다."));
      Flags.gyroStatus = 1;

//![3]
      Serial.println(F(">>>>>>[안내] 자이로 보정을 시작합니다 10초간 움직이지마세요!"));
      delay(2000);
      timer = millis() + 10000;
      while(timer > millis())
      {
        PCONCAT_(GYRO_MODEL, ReadData)();
        Serial.println(GYRO_MODEL.pInput);
//TBD GYRO_MODEL_CalibData(); 데이터 sensor측으로 계산하여 float type 이관도 해야함.
      }
      if(Flags.sensorReadTimeout == 0) //TBD 보정중 타임아웃 에러가 발생하였는지 점검
      {
        Serial.println(F(">>>>>>[3/4] 자이로 보정을 완료하였습니다."));
        Flags.gyroStatus = 1;
      }
      else
      {
        Serial.println(F(">>>>>>[오류] 센서값 획득 중 read time-out이 발생하였습니다!"));
        Flags.gyroStatus = 0;
        goto exception;
      }
      

//![4]
      Serial.println(F(">>>>>>[안내] 센서 데이터를 30초간 출력합니다. 축 방향을 점검하세요!"));
//TBD 보정후 30초간 센서 데이터 출력
      Serial.println(F(">>>>>>[4/4] 센서 데이터 출력을 완료하였습니다."));
      Flags.gyroStatus = 1;
    }
    else
    {
      Serial.println(F(">>>>>>[오류] 센서 연결상태, 또는 GYRO_MODEL의 값을 확인하세요."));
      Serial.print(F(">>>>>>[안내] WAI from Sensor = 0x"));
      Serial.println(PCONCAT_(GYRO_MODEL, IDCheck)(),HEX);
      Flags.gyroStatus = 0;
      goto exception;
    }
  }
  else
  {
    Serial.println(F(">>>>>>[오류] I2C 클럭 속도 조정에 실패하였습니다."));
    Serial.print(F(">>>>>>[안내] TWBR="));
    Serial.print(TWBR);
    Serial.println(F(". CPU 및 TWI 주파수를 점검하세요."));
    Flags.gyroStatus = 0;
    goto exception;
  }
  
exception:
;

}
