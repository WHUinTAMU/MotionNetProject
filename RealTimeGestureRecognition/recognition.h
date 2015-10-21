#ifndef RECOGNITION_H_INCLUDED
#define RECOGNITION_H_INCLUDED

#include "SPRING.h"

bool recognize_target(SqQueue* queue, int *target, int *target_signal);

bool recognize_gesture(SqQueue* queue, int *target, int *gesture_signal);

#endif // RECOGNITION_H_INCLUDED
