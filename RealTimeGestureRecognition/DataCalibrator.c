#include "DataCalibrator.h"

// Global params, used by all sensors in the same room
gsl_vector* offset = NULL;
gsl_matrix* w_invert = NULL;

// Whether reasonable ...
bool isCalibratorValid(double X[], double Y[], double Z[], int len) {
    // - 90 ~ + 90  ===> 0 ~ 180
    int isValidBitMap[3][181] = {{0}};
    int roundX, roundY, roundZ;
    int i;
    for(i = 0; i < len; i ++) {
        roundX = (int)(X[i]) + 90;
        roundY = (int)(Y[i]) + 90;
        roundZ = (int)(Z[i]) + 90;

        if((roundX <= 180) && (roundX >= 0))
            isValidBitMap[0][roundX] = 1;
        if((roundY <= 180) && (roundY >= 0))
            isValidBitMap[1][roundY] = 1;
        if((roundZ <= 180) && (roundZ >= 0))
            isValidBitMap[2][roundZ] = 1;
    }
    //Now use round as count
    roundX = 0;
    roundY = 0;
    roundZ = 0;

    for(i = 0; i <= 180; i ++) {
        if(isValidBitMap[0][i] == 1)
            roundX ++;
        if(isValidBitMap[1][i] == 1)
            roundY ++;
        if(isValidBitMap[2][i] == 1)
            roundZ ++;
        //printf("%d %d %d \n", isValidBitMap[0][i], isValidBitMap[1][i], isValidBitMap[2][i]);
    }
    double coverRate = (roundX + roundY + roundZ) / 180.0 / 3;
    printf("Your rotate cover rate is %f, default least cover rate is %f\n", coverRate, VALID_CALI_COVER_RATE);

    return (coverRate > VALID_CALI_COVER_RATE) ? true : false ;
}

bool isFiniteNumber(double x) {
    int result = gsl_finite(x);
    if(result == 0)
        printf("invert matrix is not a real matrix!\n");
    return result;
}

gsl_vector* calculateOffset(double X[], double Y[], double Z[], int len) {
    gsl_vector * x = createVector(X, len);
    gsl_vector * y = createVector(Y, len);
    gsl_vector * z = createVector(Z, len);

    gsl_vector_scale(x, 0.3);
    gsl_vector_scale(y, 0.3);
    gsl_vector_scale(z, 0.3);

    gsl_vector * xx = vectorDotMultiply(x,x);
    gsl_vector * yy = vectorDotMultiply(y,y);
    gsl_vector * zz = vectorDotMultiply(z,z);

    gsl_vector * xy2 = vectorDotMultiply(x,y);
    gsl_vector * xz2 = vectorDotMultiply(x,z);
    gsl_vector * yz2 = vectorDotMultiply(y,z);

    gsl_vector_scale(xy2,2);
    gsl_vector_scale(xz2,2);
    gsl_vector_scale(yz2,2);

    gsl_vector * x2 = vectorMultiplyConstant(x, 2);
    gsl_vector * y2 = vectorMultiplyConstant(y, 2);
    gsl_vector * z2 = vectorMultiplyConstant(z, 2);

    /** Improve:
    gsl_vector_scale(x,2);
    gsl_vector_scale(y,2);
    gsl_vector_scale(z,2);
    **/

    gsl_matrix * D = createEmptyMatrix(len, 9);
    setMatrixColumn(D, 0, xx);
    setMatrixColumn(D, 1, yy);
    setMatrixColumn(D, 2, zz);
    setMatrixColumn(D, 3, xy2);
    setMatrixColumn(D, 4, xz2);
    setMatrixColumn(D, 5, yz2);
    setMatrixColumn(D, 6, x2);
    setMatrixColumn(D, 7, y2);
    setMatrixColumn(D, 8, z2);

    freeVector(x);
    freeVector(y);
    freeVector(z);
    freeVector(xx);
    freeVector(yy);
    freeVector(zz);
    freeVector(xy2);
    freeVector(xz2);
    freeVector(yz2);
    freeVector(x2);
    freeVector(y2);
    freeVector(z2);

    gsl_matrix * v = solveEquationMatrix(D);
    freeMatrix(D);
    gsl_matrix * A = generateEllipsoid(v);

    gsl_matrix * subV = createEmptyMatrix(3,1);
    gsl_matrix_set(subV, 0, 0, gsl_matrix_get(v, 6, 0));
    gsl_matrix_set(subV, 1, 0, gsl_matrix_get(v, 7, 0));
    gsl_matrix_set(subV, 2, 0, gsl_matrix_get(v, 8, 0));

    freeMatrix(v);

    // offset = A( 1:3, 1:3 ) \ [ v(7); v(8); v(9) ];
    gsl_matrix * offsetM = leftDivide(A, subV);
    freeMatrix(A);
    freeMatrix(subV);

    gsl_vector * offsetV = createEmptyVector(3);
    gsl_matrix_get_col(offsetV, offsetM, 0);

    printf("offset vector: ");
    printVector(offsetV);

    freeMatrix(offsetM);

    return offsetV;
}

gsl_matrix* calculateConvertMatrix(gsl_vector* offsetV, double X[], double Y[], double Z[], int len) {
    gsl_vector * x = createVector(X, len);
    gsl_vector * y = createVector(Y, len);
    gsl_vector * z = createVector(Z, len);

    gsl_vector_scale(x, 0.3);
    gsl_vector_scale(y, 0.3);
    gsl_vector_scale(z, 0.3);

    gsl_vector_add_constant(x, gsl_vector_get (offsetV, 0 ));
    gsl_vector_add_constant(y, gsl_vector_get (offsetV, 1 ));
    gsl_vector_add_constant(z, gsl_vector_get (offsetV, 2 ));

    gsl_vector * xx = vectorDotMultiply(x,x);
    gsl_vector * yy = vectorDotMultiply(y,y);
    gsl_vector * zz = vectorDotMultiply(z,z);

    gsl_vector * xy2 = vectorDotMultiply(x,y);
    gsl_vector * xz2 = vectorDotMultiply(x,z);
    gsl_vector * yz2 = vectorDotMultiply(y,z);

    gsl_vector_scale(xy2,2);
    gsl_vector_scale(xz2,2);
    gsl_vector_scale(yz2,2);

    gsl_matrix * K = createEmptyMatrix(len, 6);
    setMatrixColumn(K, 0, xx);
    setMatrixColumn(K, 1, yy);
    setMatrixColumn(K, 2, zz);
    setMatrixColumn(K, 3, xy2);
    setMatrixColumn(K, 4, xz2);
    setMatrixColumn(K, 5, yz2);

    freeVector(x);
    freeVector(y);
    freeVector(z);
    freeVector(xx);
    freeVector(yy);
    freeVector(zz);
    freeVector(xy2);
    freeVector(xz2);
    freeVector(yz2);

    gsl_matrix* p = solveEquationMatrix(K);
    freeMatrix(K);

    gsl_matrix * A = generateEllipsoid(p);
    freeMatrix(p);

    //EIG SYSTEM
    gsl_vector *eval = gsl_vector_alloc (3);
    gsl_matrix *evec = gsl_matrix_alloc (3, 3);

    gsl_eigen_symmv_workspace * w = gsl_eigen_symmv_alloc (3);
    gsl_eigen_symmv (A, eval, evec, w);
    gsl_eigen_symmv_free (w);
    freeMatrix(A);

    int i;
    gsl_vector* radii = constantDivideVector(1.0, eval);
    for(i = 0; i < 3; i ++)
        gsl_vector_set(radii, i,
                       sqrt( gsl_vector_get(radii, i)));

    double Bfield = pow( gsl_vector_get(radii, 0) * gsl_vector_get(radii, 1) * gsl_vector_get(radii, 2) , 1.0 /3 );
    freeVector(radii);

    // calculate transformation matrix elipsoidal to spherical
    // W_inverted = evecs * sqrt(evals) * inv(evecs) * Bfield
    gsl_matrix * sqrtEvals = createEmptyMatrix(3,3);

    gsl_matrix_set(sqrtEvals, 0, 0,
                   sqrt(gsl_vector_get(eval, 0))) ;
    gsl_matrix_set(sqrtEvals, 1, 1,
                   sqrt(gsl_vector_get(eval, 1))) ;
    gsl_matrix_set(sqrtEvals, 2, 2,
                   sqrt(gsl_vector_get(eval, 2))) ;

    gsl_matrix* mult = matrixMultiplyMatrix(evec, sqrtEvals);

    gsl_matrix* invEvecs = invert(evec);

    gsl_matrix* w_invertM = matrixMultiplyMatrix(mult, invEvecs);

    gsl_matrix_scale(w_invertM, Bfield);

    printf("\ninverted matrix: ");
    printMatrix(w_invertM);

    freeVector (eval);
    freeMatrix (evec);
    freeMatrix(sqrtEvals);
    freeMatrix(mult);
    freeMatrix(invEvecs);
    if(! isFiniteNumber(gsl_matrix_get(w_invertM, 0, 0))) {
        freeMatrix(w_invertM);
        return NULL;
    }
    return w_invertM;
}

void calculateCalibrator(double magDataX[], double magDataY[], double magDataZ[], int len) {
    offset = calculateOffset(magDataX,magDataY,magDataZ, len);
    w_invert = calculateConvertMatrix(offset, magDataX,magDataY,magDataZ, len);
}

void clearCalibrator() {
    printf("\n===========================  Start Clear Calibrator  =======================\n");
    if(offset != NULL) {
        freeVector(offset);
        printf("clear offset\n");
    }
    if(w_invert != NULL) {
        freeMatrix(w_invert);
        printf("clear invert matrix");
    }
    printf("\n===========================  Clear Calibrator OK =======================\n");
}

/**
magData = magData*0.3;
magDataX = magDataX + offset(1);
magDataY = magDataY + offset(2);
magDataZ = magDataZ + offset(3);

correctedM [3][n] = W_inverted[3][3] * 	  [ magDataX';
											magDataY';
											magDataZ'];
*/
bool calibrateMagData(double magDataX[], double magDataY[], double magDataZ[], double heading[], int len) {
    if(offset == NULL || w_invert == NULL || !isFiniteNumber(gsl_matrix_get(w_invert, 0, 0)))
        return false;

    gsl_vector * x = createVector(magDataX, len);
    gsl_vector * y = createVector(magDataY, len);
    gsl_vector * z = createVector(magDataZ, len);

    gsl_vector_scale(x, 0.3);
    gsl_vector_scale(y, 0.3);
    gsl_vector_scale(z, 0.3);

    gsl_vector_add_constant(x, gsl_vector_get(offset, 0));
    gsl_vector_add_constant(y, gsl_vector_get(offset, 1));
    gsl_vector_add_constant(z, gsl_vector_get(offset, 2));

    gsl_matrix * magData = createEmptyMatrix(3, len);
    setMatrixRow(magData, 0, x);
    setMatrixRow(magData, 1, y);
    setMatrixRow(magData, 2, z);

    freeVector(x);
    freeVector(y);
    freeVector(z);

    gsl_matrix * result = matrixMultiplyMatrix(w_invert, magData);

    int i;
    for(i = 0; i < len; i ++) {
        magDataX[i] = gsl_matrix_get(result, 0, i);
        magDataY[i] = gsl_matrix_get(result, 1, i);
        magDataZ[i] = gsl_matrix_get(result, 2, i);
        heading[i] = atan2(-1.0 * magDataY[i], magDataX[i]) * 57.3 ;

        // Change the pointing direction from X-axis to Y-axis
        heading[i] = heading[i] - 90;

        // Normalize to 0-360
        if (heading[i] < 0) {
            heading[i] = 360 + heading[i];
        }

        if (heading[i] > 360) {
            heading[i] = heading[i] - 360;
        }
    }

    freeMatrix(magData);
    freeMatrix(result);
    printf("Calibration Done!\n");
    return true;
}
