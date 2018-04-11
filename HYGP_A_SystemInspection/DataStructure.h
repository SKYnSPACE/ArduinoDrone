#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

struct _Acc
{
  float x, y, z;
};

struct _Gyro
{
  float p, q, r;
};

struct _Receiver
{
  int channel1Input, channel2Input, channel3Input, channel4Input;

  float channel1, channel2, channel3, channel4; // Normalized value (-1~1)
};

struct _Sensor
{

  _Acc Acc;
  _Gyro Gyro;
  _Receiver Receiver;

  float temperature;
};



struct _Flags
{
  bool mainFrequencyCheck;
  
  //PIN Digital value?
  bool pin8;
  bool pin9;
  bool pin10;
  bool pin11;

  // Status during inspections [0]: Abnormal, [1]: Normal or End of inspection
  bool serialStatus;
  bool gyroStatus;
  bool receiverStatus;
  bool transmitterStatus;

  // Critical situations [0]: Normal, [1]: Abnormal
  bool sensorReadTimeout;
  bool smallSizedBiasData;
  
  bool exitCommand;
};

#endif
