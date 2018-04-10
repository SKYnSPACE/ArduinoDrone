#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H

struct _Accel
{
  float x, y, z;
};

struct _Gyro
{
  float p, q, r;
};

struct _Sensor
{
  unsigned short gyroAddress;
  unsigned short gyroWhoAmIREG;
  unsigned short gyroWhoAmIValue;
  
  _Accel Accel;
  _Gyro Gyro;
};



struct _Flags
{
  bool mainFrequencyCheck;

  bool serialStatus;
  bool gyroStatus;
  
  bool exitCommand;
};

#endif
