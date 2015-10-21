#ifndef GSLMATRIXS_H
#define GSLMATRIXS_H

#include <stdio.h>
#include <math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_eigen.h>


//MATRIX
gsl_matrix * createEmptyMatrix(int row, int column);

gsl_matrix * copyMatrix(gsl_matrix * m);

void freeMatrix(gsl_matrix * m);

void printMatrix(gsl_matrix * m);

int setMatrixColumn(gsl_matrix * m, int i, const gsl_vector * v);

int setMatrixRow(gsl_matrix * m, int i, const gsl_vector * v);

gsl_matrix * invert(gsl_matrix * m);

gsl_matrix * matrixDotMultiply(gsl_matrix* m1, gsl_matrix* m2);

gsl_matrix * matrixMultiplyMatrix(gsl_matrix * m1, gsl_matrix * m2);

gsl_matrix * matrixMultiplyConstant(gsl_matrix * m, double val);

gsl_matrix * matrixAddConstant(gsl_matrix * m, double val);

gsl_matrix * leftDivide(gsl_matrix * lM, gsl_matrix * rM);

gsl_matrix * constantMatrix(int row, int column, double val);

gsl_matrix * transpose(gsl_matrix * m);

gsl_matrix * selectMatrix(gsl_matrix * m, int sub_row, int sub_column);

gsl_matrix * sqrtMatrix(gsl_matrix *  m);


//VECTOR
gsl_vector * createEmptyVector(int len);

gsl_vector * copyVector(gsl_vector * v);

gsl_vector * createVector(double array[], int len);

void freeVector(gsl_vector * v);

void printVector(gsl_vector * v);

gsl_vector * vectorDotMultiply(gsl_vector* v1, gsl_vector* v2);

gsl_vector * vectorAddConstant(gsl_vector * v, double val);

gsl_vector * vectorMultiplyConstant(gsl_vector * v, double val);

gsl_vector *  constantDivideVector(double divisor, gsl_vector * v);

gsl_vector *  sqrtVector(gsl_vector *  v);

gsl_vector *  diag(gsl_matrix * m );


//OTHER
gsl_matrix *  solveEquationMatrix(gsl_matrix * m);

gsl_matrix *  generateEllipsoid(gsl_matrix * m);

#endif  //GSLMATRIXS_H
