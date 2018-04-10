#ifndef SENSORINSPECTION_H
#define SENSORINSPECTION_H

#include "DataStructure.h"
#include "Sensors.h"

#define GYRO_MODEL MPU6050

extern struct _Sensor Sensor;

bool GyroInspection();

#endif
