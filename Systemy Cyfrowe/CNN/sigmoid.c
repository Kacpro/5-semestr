#ifndef _SIGMOID_
#define _SIGMOID_


#include <math.h>
#include "matrix.c"

double sigmoid(double x)
{
    return 1.0/(1 + exp(-x));
}



double sigmoidDerivative(double x)
{
    return sigmoid(x) * (1 - sigmoid(x));
}



Matrix applySigmoidDerivative(Matrix x)
{
    double** value = calloc((size_t)x.rowNum, sizeof(double*));
    for (int i=0; i<x.rowNum; i++)
    {
        value[i] = calloc((size_t)x.columnNum, sizeof(double));
        for (int j=0; j<x.columnNum; j++)
        {
            value[i][j] = sigmoidDerivative(x.value[i][j]);
        }
    }

    return matrixInit(x.rowNum, x.columnNum, value);
}



#endif