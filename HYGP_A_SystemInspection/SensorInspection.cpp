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

bool GyroInspection()
{
  char sensorName[16];
  
  if((TWBR == 12) && Flags.mainFrequencyCheck)
  {
    Serial.println(F(">>>>>>[1/3] I2C 클럭 속도 설정을 완료하였습니다."));
    Serial.print(F(">>>>>>[안내] "));
    sprintf(sensorName, "%s", STR(GYRO_MODEL));
    Serial.print(sensorName);
    Serial.println(F(" 센서를 탐색합니다."));
    delay(1000);
    if(PCONCAT_(GYRO_MODEL, IDCheck)(Sensor.gyroAddress, Sensor.gyroWhoAmIREG) == Sensor.gyroWhoAmIValue) // 센서에 WAI 요청하여 센서 ID 확인.
    {
      Serial.print(F(">>>>>>[2/3] MPU-6050 센서가 주소 0x"));
      Serial.print(Sensor.gyroAddress,HEX);
      Serial.println(F("로 정상 연결되었습니다."));

      Serial.println(F(">>>>>>[안내] 센서 데이터를 30초간 출력합니다."));
      //TBD
      Serial.println(F(">>>>>>[3/3] 센서 데이터 출력을 완료하였습니다."));
      Flags.gyroStatus = 1;
    }
    else
    {
      Serial.println(F(">>>>>>[오류] 센서 연결상태, 또는 GYRO_MODEL의 값을 확인하세요."));
      Serial.print(F(">>>>>>[안내] WAI from Sensor = 0x"));
      Serial.println(PCONCAT_(GYRO_MODEL, IDCheck)(Sensor.gyroAddress, Sensor.gyroWhoAmIREG),HEX);
      Flags.gyroStatus = 0;
    }
  }
  else
  {
    Serial.println(F(">>>>>>[오류] I2C 클럭 속도 조정에 실패하였습니다."));
    Serial.print(F(">>>>>>[안내] TWBR="));
    Serial.print(TWBR);
    Serial.println(F(". CPU 및 TWI 주파수를 점검하세요."));
    Flags.gyroStatus = 0;
  }
}
