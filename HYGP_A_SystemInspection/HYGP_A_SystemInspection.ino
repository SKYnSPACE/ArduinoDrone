////////////////////////////////////////////////////////////////////////////////
// 2018 항공우주공학과 역량강화 프로젝트 코드A.
// 드론 조립후 시스템 점검을 위하여 사용하는 코드입니다.

//!경고: 안전수칙//
// 시스템 조립 상태를 최종 점검한 후
// 모터에 장착된 "프로펠러를 모두 제거"하고 업로드 하십시오.

// 사용순서 //
// 1. TBD
// 2. TBD
// 3. TBD

// 로그 //
// 2018.APR.10. 초안작성: 이성헌 skynspace@kaist.ac.kr
//   본 프로그램은 다음의 사항들을 점검 및 셋팅합니다
//   1. PC - 아두이노간 시리얼통신, 2. 센서 점검, 3. 수신기 연결상태 점검 4. 조종스틱 범위확인
// 2018.AAA.00. 수정1: ㅇㅇㅇ
//   ㅇㅇㅇ 을 수정함

////////////////////////////////////////////////////////////////////////////////

#include <Wire.h>
#include <avr/interrupt.h>

#include "Messages.h"
#include "DataStructure.h"
#include "SensorInspection.h"
#include "ReceiverTransmitter.h"

#include "Macros.h"

unsigned short keyInput;

struct _Flags Flags;
struct _Sensor Sensor;
struct PCONCAT_(,GYRO_MODEL) GYRO_MODEL;

char str[80];
volatile unsigned long timer, timer1, timer2, timer3, timer4; 

void setup() {
//![0] put your setup code here, to run once:

//![1] EEPROM 초기화
  // TBD

  
//![2] 시리얼 통신 준비 @Baudrate 57600
  Serial.begin(57600); 


//![3] I2C통신 준비
  //참고: Two Wire Bit Rate Register, TWBR = ((cpu freq./twi freq.) - 16) / 2
  #if F_CPU == 16000000L
    Flags.mainFrequencyCheck = 1;
  #else
    Flags.mainFrequencyCheck = 0;
  #endif
  Wire.begin(); //아두이노를 I2C 마스터로 설정
  TWBR = 12; //twi freq.를 400kHz로 증폭 (기본값 cpu freq. 16000000L, twi freq. 100000L)
  FUNCTION_(GYRO_MODEL,Init)();


//![4] 수신기 입력핀 설정
  cli();
  PCICR |= (1 << PCIE0);    // B 포트 (PCINT0 - PCINT7, 8-13) 오픈  
  PCMSK0 |= (1 << PCINT0);  // 인터럽트 설정 (PCINT0, 8)
  PCMSK0 |= (1 << PCINT1);  // 인터럽트 설정 (PCINT1, 9)
  PCMSK0 |= (1 << PCINT2);  // 인터럽트 설정 (PCINT2, 10)
  PCMSK0 |= (1 << PCINT3);  // 인터럽트 설정 (PCINT3, 11)
  sei();
  
//![5] 변속기 출력핀 설정

//![6] 각종 변수 초기화
  Flags.exitCommand = 0;

//![7] 설정 완료후 환영 메시지 송출
  MessageWelcome();
  delay(1000);
  MessageMenu();
}

void loop() {
//![0] put your main code here, to run repeatedly:
  delay(1000);

//![1] 점검기능 시작
  if((Serial.available() > 0) && (Flags.exitCommand == 0))
  {
    keyInput = Serial.read();
    switch(keyInput)
    {
//![1-1] 메뉴 송출
      case 'm':
        MessageMenu();
        break;
//![1-2] 시리얼통신 점검
      case 's':
        MessageSerial();
        MessageSerialNormal();
        break;
//![1-3] 센서상태 점검 (자이로만 있음)
      case 'i':
        MessageI2C();
        GyroInspection();
        if(Flags.gyroStatus == 1) MessageI2CNormal();
        else MessageI2CAbnormal();
        break;
//![1-4] 송수신 상태 점검 및 조종기 Calibration
      case 't':
        MessageRTR();
        ReceiverInspection();
        if(Flags.receiverStatus == 1) MessageRTRNormal();
        else MessageRTRAbnormal();
        break;
//![1-5] 배터리 전압 확인
      case 'v':
        break;
//![1-6] 설정값 저장
      case 'w':
        break;
//![1-7] 종료
      case 'q':
        MessageExit();
        Flags.exitCommand = 1;
        while(1){}
        break;
//![-] 예외처리
      default:
        MessageWrongInput();
        break;
    }
  }
  else
  {
    while(!Serial.available()){}
  }
  
}

ISR (PCINT0_vect) // Port B, PCINT0 - PCINT7: Handle pin change interrupt from 8 to 13.
{ 
//![0]   
  timer = micros();
  
//![1] 8, PB0, 1번 채널과 연결
  if(PINB & (1<<PB0)) // 8번핀 Rising인 경우 
  {
    if(Flags.pin8 == 0) // Low상태를 high로 변경
    {
      timer1 = timer;
      Flags.pin8 = 1;
    }
  }
  else // 8번핀 Falling인 경우
  { 
    if(Flags.pin8 == 1) // High상태를 low로 변경하면서 시간 측정 (duty cycle) 
    {
      Flags.pin8 = 0;
      Sensor.Receiver.channel1Input = timer - timer1;
    }
  }
  
//![2] 9, PB1, 2번 채널과 연결
  if(PINB & (1<<PB1)) // 9번핀 Rising인 경우 
  {
    if(Flags.pin9 == 0) // Low상태를 high로 변경
    {
      timer2 = timer;
      Flags.pin9 = 1;
    }
  }
  else // 9번핀 Falling인 경우
  { 
    if(Flags.pin9 == 1) // High상태를 low로 변경하면서 시간 측정 (duty cycle) 
    {
      Flags.pin9 = 0;
      Sensor.Receiver.channel2Input = timer - timer2;
    }
  }
  
//![3] 10, PB2, 3번 채널과 연결
  if(PINB & (1<<PB2)) // 10번핀 Rising인 경우 
  {
    if(Flags.pin10 == 0) // Low상태를 high로 변경
    {
      timer3 = timer;
      Flags.pin10 = 1;
    }
  }
  else // 10번핀 Falling인 경우
  { 
    if(Flags.pin10 == 1) // High상태를 low로 변경하면서 시간 측정 (duty cycle) 
    {
      Flags.pin10 = 0;
      Sensor.Receiver.channel3Input = timer - timer3;
    }
  }

//![4] 11, PB3, 11번 채널과 연결
  if(PINB & (1<<PB3)) // 11번핀 Rising인 경우 
  {
    if(Flags.pin11 == 0) // Low상태를 high로 변경
    {
      timer4 = timer;
      Flags.pin11 = 1;
    }
  }
  else // 11번핀 Falling인 경우
  { 
    if(Flags.pin11 == 1) // High상태를 low로 변경하면서 시간 측정 (duty cycle) 
    {
      Flags.pin11 = 0;
      Sensor.Receiver.channel4Input = timer - timer4;
    }
  }
}
ISR(PCINT1_vect){}    // Port C, PCINT8 - PCINT14
ISR(PCINT2_vect){}    // Port D, PCINT16 - PCINT23
