#ifndef PWM_CONTROL_H
#define PWM_CONTROL_H

#include "bindings.h"
#include <iostream>

#define PWM_MIN 1100
#define PWM_MAX 1500

void controlFan(int channel, int value);

#endif
