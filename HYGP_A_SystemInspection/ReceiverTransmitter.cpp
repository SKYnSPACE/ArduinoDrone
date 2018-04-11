#include <Arduino.h>

#include "ReceiverTransmitter.h"

void ReceiverInspection()
{
  unsigned long timer;

//![1]
  delay(2000);
  Serial.println(F(">>>>>>[안내] 수신기 입력을 1분간 송출합니다. 내용을 확인하세요!"));
  delay(4000);

  timer = millis() + 60000;
  while(timer > millis())
  {
    sprintf(str, ">>>>>>[안내] CH1: %d, CH2: %d, CH3: %d, CH4: %d", 
            Sensor.Receiver.channel1Input, Sensor.Receiver.channel2Input,
            Sensor.Receiver.channel3Input, Sensor.Receiver.channel4Input);
    Serial.println(str);
  }
  
}

