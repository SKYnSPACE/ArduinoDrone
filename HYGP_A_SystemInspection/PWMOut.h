#ifndef PWMOUT_H
#define PWMOUT_H

#include "DataStructure.h"

extern struct _Motor Motor;
extern struct _Flags Flags;

extern volatile unsigned long systemTimer;
extern volatile unsigned long timer;
extern volatile unsigned long motorTimer1, motorTimer2, motorTimer3, motorTimer4;

void PWMOutRaiser();
void PWMOutFaller();
void PWMOutInspection();


#endif
