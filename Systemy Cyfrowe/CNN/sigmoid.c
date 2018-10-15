#ifndef _SIGMOID_
#define _SIGMOID_


#include <math.h>

double sigmoid(double x)
{
    return 1.0/(1 + exp(-x));
}

double sigmoidDerivative(double x)
{
    return sigmoid(x) * (1 - sigmoid(x));
}

#endif