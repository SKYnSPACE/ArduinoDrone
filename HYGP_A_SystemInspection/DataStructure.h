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

struct _Sensor
{

  _Acc Acc;
  _Gyro Gyro;

  float temperature;
};



struct _Flags
{
  bool mainFrequencyCheck;

  // Status during inspections [0]: Abnormal, [1]: Normal or End of inspection
  bool serialStatus;
  bool gyroStatus;

  // Critical situations [0]: Normal, [1]: Abnormal
  bool sensorReadTimeout;
  bool smallSizedBiasData;
  
  bool exitCommand;
};

#endif
