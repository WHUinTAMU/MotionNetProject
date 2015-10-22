#include "GSLMatrixs.h"

/*
Function:
	This file contains matrix operations by using GSL library.
Notice:
	don't forget to call freeMatrix and freeVector to avoid memory leak.
Author:
	Amy
EditTime:
	10/6/2015
*/

/////////////////////    MATRIX   ///////////////////////////
//Create a zero matrix
gsl_matrix * createEmptyMatrix(int row, int column) {
    gsl_matrix* result = gsl_matrix_alloc(row, column);
    gsl_matrix_set_zero(result);
    return result;
}

//Create another matrix and copy 'm' to new matrix
gsl_matrix * copyMatrix(gsl_matrix * m) {
    int row = m->size1;
    int column = m->size2;
    gsl_matrix * result = createEmptyMatrix(row, column);
    gsl_matrix_memcpy (result, m);
    return result;
}

void freeMatrix(gsl_matrix * m) {
    gsl_matrix_free(m);
}

void printMatrix(gsl_matrix * m) {
    printf("\n");
    int i, j;
    for( i = 0; i < m->size1; i ++) {
        for( j = 0; j < m->size2; j ++)
            printf(" %g ",
                   gsl_matrix_get(m, i,j));
        printf("\n");
    }
}

int setMatrixColumn(gsl_matrix * m, int i, const gsl_vector * v) {
    return gsl_matrix_set_col(m, i, v);
}

int setMatrixRow(gsl_matrix * m, int i, const gsl_vector * v) {
    return gsl_matrix_set_row(m, i, v);
}

//Create a new matrix, storing the invert of 'm' to new matrix
//Notice: 'm' should be square matrix, else return NULL
gsl_matrix * invert(gsl_matrix * m) {
    int row = m->size1;
    if(row != m->size2) {
        printf("Only square matrix can be inverted! Exit calculation\n");
        exit(-1);
    }
    gsl_matrix* result = createEmptyMatrix(row, row);

    //Calculate invert matrix by GSL
    gsl_permutation *p = gsl_permutation_alloc(row);
    int sign = 0;

    gsl_linalg_LU_decomp(m, p, &sign);

    gsl_linalg_LU_invert(m, p, result);

    //Don't forget to free memory
    gsl_permutation_free(p);

    return result;
}

gsl_matrix * matrixDotMultiply(gsl_matrix* m1, gsl_matrix* m2) {
    gsl_matrix * result = copyMatrix(m1);
    gsl_matrix_mul_elements (result, m2);
    return result;
}

gsl_matrix *  matrixMultiplyMatrix(gsl_matrix * m1, gsl_matrix * m2) {
    if(m1->size2 != m2->size1) {
        printf("Matrix1's column must equals to Matrix2's row! Exit calculation\n");
        exit(-1);
    }

    gsl_matrix * result = createEmptyMatrix(m1->size1,m2->size2);
    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans,
                   1.0, m1, m2,
                   0.0, result);
    return result;
}

gsl_matrix * matrixMultiplyConstant(gsl_matrix * m, double val) {
    gsl_matrix * result = copyMatrix(m);
    gsl_matrix_scale (result, val);
    return result;
}

gsl_matrix *  matrixAddConstant(gsl_matrix * m, double val) {
    gsl_matrix * result = copyMatrix(m);
    gsl_matrix_add_constant(result, val);
    return result;
}

//A LeftDivide B = invert(A) * B
gsl_matrix *  leftDivide(gsl_matrix * lM, gsl_matrix * rM) {
    gsl_matrix * invertlM = invert(lM);
    gsl_matrix * result = matrixMultiplyMatrix(invertlM, rM);
    gsl_matrix_free(invertlM);
    return result;
}

gsl_matrix *  constantMatrix(int row, int column, double val) {
    gsl_matrix * result = createEmptyMatrix(row,column);
    gsl_matrix_set_all(result, val);
    return result;
}

//A'[i,j] = A[j,i]
gsl_matrix *  transpose(gsl_matrix * m) {
    gsl_matrix * result = createEmptyMatrix(m->size2, m->size1);
    gsl_matrix_transpose_memcpy (result, m);
    return result;
}

gsl_matrix *  selectMatrix(gsl_matrix * m, int sub_row, int sub_column) {
    gsl_matrix * result = createEmptyMatrix(sub_row, sub_column);
    int i, j;
    for(i = 0; i < sub_row; i ++) {
        for(j = 0; j < sub_column; j ++) {
            gsl_matrix_set(result, i, j,
                           gsl_matrix_get(m, i, j));
        }
    }
    return result;
}

gsl_matrix *  sqrtMatrix(gsl_matrix *  m) {
    int row = m->size1;
    int column = m->size2;
    gsl_matrix * result = copyMatrix(m);
    int i, j;
    for(i = 0; i < row; i ++) {
        for(j = 0; j < column; j ++) {
            gsl_matrix_set(result, i, j,
                           sqrt( gsl_matrix_get(m, i, j)));
        }
    }
    return result;
}


/////////////////////    VECTOR   ///////////////////////////
gsl_vector * createEmptyVector(int len) {
    gsl_vector* result =  gsl_vector_alloc(len);
    gsl_vector_set_zero(result);
    return result;
}

gsl_vector * copyVector(gsl_vector * v) {
    gsl_vector* result =  createEmptyVector(v->size);
    gsl_vector_memcpy(result, v);
    return result;
}

gsl_vector * createVector(double array[], int len) {
    int i;
    gsl_vector * vec = createEmptyVector(len);
    for(i = 0; i < len; i ++) {
        gsl_vector_set(vec, i, array[i]);
    }
    return vec;
}

void freeVector(gsl_vector * v) {
    gsl_vector_free(v);
}

void printVector(gsl_vector * v) {
    int i;
    for( i = 0; i < v->size; i ++) {
        printf(" %g ",
               gsl_vector_get(v, i));
    }
}

gsl_vector * vectorDotMultiply(gsl_vector* v1, gsl_vector* v2) {
    gsl_vector * result = copyVector(v1);
    gsl_vector_mul (result, v2);
    return result;
}

gsl_vector * vectorAddConstant(gsl_vector * v, double val) {
    gsl_vector * result = copyVector(v);
    gsl_vector_add_constant(result, val);
    return result;
}

gsl_vector * vectorMultiplyConstant(gsl_vector * v, double val) {
    gsl_vector * result = copyVector(v);
    gsl_vector_scale(result, val);
    return result;
}

//Used when divisor ./ v , which means each new v[i] = divisor/ old v[i]
gsl_vector *  constantDivideVector(double divisor, gsl_vector * v) {
    gsl_vector * result = createEmptyVector(v->size);
    gsl_vector_set_all(result, divisor);
    gsl_vector_div(result, v);
    return result;
}

gsl_vector *  sqrtVector(gsl_vector *  v) {
    gsl_vector * result = createEmptyVector(v->size);
    int i;
    for(i = 0; i < v->size; i ++)
        gsl_vector_set(result, i,
                       sqrt( gsl_vector_get(v, i)));
    return result;
}

gsl_vector *  diag(gsl_matrix * m ) {
    int i;
    int len = (m->size1 < m->size2)? m->size1: m->size2;
    gsl_vector * result = createEmptyVector(len);
    for(i = 0; i < len; i ++) {
        gsl_vector_set(result, i,
                       gsl_matrix_get(m, i, i));
    }
    return result;
}


// Notice: if we use methods that malloc some memory, we MUST free the memory.
// solve the normal system of equations and find fitted ellipsoid parameters
gsl_matrix *  solveEquationMatrix(gsl_matrix * m) {
    gsl_matrix * transM = transpose(m);

    gsl_matrix * mul1 = matrixMultiplyMatrix(transM, m);

    gsl_matrix * consM = constantMatrix(m->size1, 1, 1);

    gsl_matrix * mul2 = matrixMultiplyMatrix(transM, consM);

    gsl_matrix * result = leftDivide(mul1, mul2);

    gsl_matrix_free(transM);
    gsl_matrix_free(mul1);
    gsl_matrix_free(consM);
    gsl_matrix_free(mul2);

    return result;
}

// form the algebraic form of the ellipsoid
gsl_matrix *  generateEllipsoid(gsl_matrix * m) {
    gsl_matrix * result = createEmptyMatrix( 3, 3);

    gsl_matrix_set(result, 0, 0,
                   ( gsl_matrix_get(m, 0,0)));
    gsl_matrix_set(result, 0, 1,
                   ( gsl_matrix_get(m, 3,0)));
    gsl_matrix_set(result, 0, 2,
                   ( gsl_matrix_get(m, 4,0)));
    gsl_matrix_set(result, 1, 0,
                   ( gsl_matrix_get(m, 3,0)));
    gsl_matrix_set(result, 1, 1,
                   ( gsl_matrix_get(m, 1,0)));
    gsl_matrix_set(result, 1, 2,
                   ( gsl_matrix_get(m, 5,0)));
    gsl_matrix_set(result, 2, 0,
                   ( gsl_matrix_get(m, 4,0)));
    gsl_matrix_set(result, 2, 1,
                   ( gsl_matrix_get(m, 5,0)));
    gsl_matrix_set(result, 2, 2,
                   ( gsl_matrix_get(m, 2,0)));
    return result;
}


//int
//main (void) {
//	int row = 3;
//	int column = 2;
//
//	double arr1[3] = {4,2,6};
//	double arr2[3] = {5,3,4};
//
//	gsl_matrix* m1 = createEmptyMatrix(row, column);
//	gsl_vector* v1 = createVector(arr1, row);
//	gsl_vector* v2 = createVector(arr2, row);
//
//	printf("\nmatrix m1 and vector v1, v2: \n");
//
//	printMatrix(m1);
//	printVector(v1);
//	printVector(v2);
//
//	printf("\nAfter set matrix m1: \n");
//	setMatrixColumn(m1,0,v1);
//	setMatrixColumn(m1,1,v2);
//	printMatrix(m1 );
//
//	printf("\ncopy matrix m1: \n");
//	gsl_matrix* m2 = copyMatrix(m1 );
//	printMatrix(m2 );
//
//	printf("\nsqrt matrix m1: \n");
//	gsl_matrix* m3 = sqrtMatrix(m1);
//	printMatrix(m3 );
//
//	printf("\nconstant matrix m4: \n");
//	gsl_matrix* m4 = constantMatrix(column, row, 4.0);
//	printMatrix(m4 );
//
//	printf("\nmatrix m1 + 10.0: \n");
//	gsl_matrix* m5 = matrixAddConstant(m1,   10.0);
//	printMatrix(m5 );
//
//	printf("\ntranspose matrix m1: \n");
//	gsl_matrix* m6 = transpose(m1 );
//	printMatrix(m6 );
//
//	printf("\nm7 = matrix m1 * m4: \n");
//	gsl_matrix* m7 = matrixMultiplyMatrix(m1, m4);
//	printMatrix(m7 );
//
//	printf("\nmatrix m1 .* m2: \n");
//	gsl_matrix* m8 = matrixDotMultiply(m1, m2 );
//	printMatrix(m8 );
//
//	printf("\nmatrix m1 * 2.0: \n");
//	gsl_matrix* m9 = matrixMultiplyConstant(m1 , 2.0);
//	printMatrix(m9 );
//
//
//	printf("\nm10: sub matrix m6 \n");
//	gsl_matrix* m10 = select(m6, 2, 2);
//	printMatrix(m10 );
//
//	printf("\n diag m9 \n");
//	gsl_vector* m11 = diag(m9 );
//	printVector(m11);
//
//
//	printf("\ninvert matrix m10 \n");
//	gsl_matrix* m12 = invert(m10);
//	printMatrix(m12 );
//
//	printf("\nmatrix m1 now \n");
//	printMatrix(m1 );
//
//
//	freeMatrix(m1);
//	freeMatrix(m2);
//	freeMatrix(m3);
//	freeMatrix(m4);
//	freeMatrix(m5);
//	freeMatrix(m6);
//	freeMatrix(m7);
//	freeMatrix(m8);
//	freeMatrix(m9);
//	freeMatrix(m10);
//	freeVector(m11);
//	freeMatrix(m12);
//
//
//
//	printf("\ncopy vector v1  \n");
//	gsl_vector* v3 = copyVector(v1);
//	printVector(v3);
//
//	printf("\n vector v1 .* v2  \n");
//	gsl_vector* v4 = vectorDotMultiply(v1, v2);
//	printVector(v4);
//
//	printf("\nvector v1 + 1.0 \n");
//	gsl_vector* v5 = vectorAddConstant(v1, 1.0);
//	printVector(v5);
//
//	printf("\n100.0 / vector v1  \n");
//	gsl_vector* v6 = constantDivideVector(100.0, v1);
//	printVector(v6);
//
//	printf("\nempty vector  \n");
//	gsl_vector* v7 = createEmptyVector(row);
//	printVector(v7);
//
//	printf("\nsqrt vector v1 \n");
//	gsl_vector* v8 = sqrtVector(v1);
//	printVector(v8);
//
//	printf("\n vector v1, v2 now \n");
//	printVector(v1);
//	printVector(v2);
//
//
//
//	freeVector(v1);
//	freeVector(v2);
//	freeVector(v3);
//	freeVector(v4);
//	freeVector(v5);
//	freeVector(v6);
//	freeVector(v7);
//	freeVector(v8);
//
//
//	//EIG CALCULATION
//
//	double data[] = { 1.0  , 1/2.0, 1/3.0, 1/4.0};
//	double data1[] = { 1/2.0, 1/3.0, 1/4.0, 1/5.0};
//	double data2[] = { 1/3.0, 1/4.0, 1/5.0, 1/6.0};
//	double data3[] = { 1/4.0, 1/5.0, 1/6.0, 1/7.0};
//
//
//	gsl_matrix* matrix = createEmptyMatrix(4,4);
//	gsl_vector* vec = createVector(data, 4);
//	gsl_vector* vec1 = createVector(data1, 4);
//	gsl_vector* vec2 = createVector(data2, 4);
//	gsl_vector* vec3 = createVector(data3, 4);
//
//	setMatrixColumn(matrix, 0, vec);
//	setMatrixColumn(matrix, 1, vec1);
//	setMatrixColumn(matrix, 2, vec2);
//	setMatrixColumn(matrix, 3, vec3);
//
//	printMatrix(matrix );
//	freeVector(vec);
//	freeVector(vec1);
//	freeVector(vec2);
//	freeVector(vec3);
//
//	gsl_vector *eval = gsl_vector_alloc (4);
//	gsl_matrix *evec = gsl_matrix_alloc (4, 4);
//
//	gsl_eigen_symmv_workspace * w =
//	    gsl_eigen_symmv_alloc (4);
//
//	gsl_eigen_symmv (matrix, eval, evec, w);
//
//	gsl_eigen_symmv_free (w);
//
//	printVector(eval);
//	printMatrix(evec );
//
//	gsl_vector_free (eval);
//	gsl_matrix_free (evec);
//	gsl_matrix_free(matrix);
//
//	return 0;
//}
