#include <Arduino.h>

#include "PWMOut.h"

void PWMOutRaiser()
{
  while(micros() - systemTimer < 2500){} // 모터 출력 속도인 2.5ms(400Hz) 내에 모든 계산이 완료된 경우 대기.
  systemTimer = micros(); //다음 루프를 위한 시스템 시계 업데이트

  PORTD |= B00111100; //모든 출력 high.
  motorTimer1 = systemTimer + Motor.outputPWM1; // high 상태가 유지되어야 하는 시간을 기록
  motorTimer2 = systemTimer + Motor.outputPWM2; 
  motorTimer3 = systemTimer + Motor.outputPWM3;
  motorTimer4 = systemTimer + Motor.outputPWM4;
  
}

// 모터 동작을 위해서는 PWMOutRise() 함수 호출 이후 PWMOutFall() 함수가 순차적으로 반복 호출되어야 함
// 참고로 모터 출력 high의 지속은 최소 1ms 보장되므로,
// 해당시간안에 처리하고자 하는 작업을 두 함수 사이에 넣는것이 좋음. 
// e.g. KalmanFilter();
// 아님 timer interrupt를 쓰던지..

void PWMOutFaller()
{
  while(PORTD >= B00000100)
  {
    timer = micros();
    if(motorTimer1 <= timer)PORTD &= B11111011; // high 필요시간 종료시 fall.
    if(motorTimer2 <= timer)PORTD &= B11110111;
    if(motorTimer3 <= timer)PORTD &= B11101111;
    if(motorTimer4 <= timer)PORTD &= B11011111;
  }
}

void PWMOutInspection()
{
  
}

