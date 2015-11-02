#ifndef LAMPCONTROLLER_H_INCLUDED
#define LAMPCONTROLLER_H_INCLUDED
#include <stdbool.h>
#include "cJSON.h"
#include "LampCmd.h"
#include <stdio.h>

#define ON_TYPE 0
#define BRI_TYPE 1
#define HUE_TYPE 2

#define ON_STRING "on"
#define BRI_STRING "bri"
#define HUE_STRING "hue"

#define BRI_VALUE_UP 50
#define BRI_VALUE_DOWN -50
#define HUE_VALUE_UP 5000
#define HUE_VALUE_DOWN -5000

bool onValue[4];
int briValue[4];
int hueValue[4];

bool createCommand(int stateType, int valueChange, int target);

#endif // LAMPCONTROLLER_H_INCLUDED
