#ifndef DATACALIBRATOR_H
#define DATACALIBRATOR_H

#include <stdbool.h>
#include <gsl/gsl_math.h>

#include "GSLMatrixs.h"

#define VALID_CALI_COVER_RATE 0.3

bool isCalibratorValid(double X[], double Y[], double Z[], int len);

bool isFiniteNumber(double x);

gsl_vector* calculateOffset(double X[], double Y[], double Z[], int len);

gsl_matrix* calculateConvertMatrix(gsl_vector* offsetV, double X[], double Y[], double Z[], int len);

void calculateCalibrator(double magDataX[], double magDataY[], double magDataZ[], int len);

void clearCalibrator();

bool calibrateMagData(double magDataX[], double magDataY[], double magDataZ[], double heading[], int len);

#endif // DATACALIBRATOR_H
