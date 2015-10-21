#ifndef TARGETRECOGNITION_H_INCLUDED
#define TARGETRECOGNITION_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "DataCalibrator.h"

double convertReadingToHeading(double y,double x);

int convertHeadingToTarget(double z, double headingFrom2To1);

int pickTarget(DataHeadNode head, double headingFrom2To1);

#endif // TARGETRECOGNITION_H_INCLUDED
