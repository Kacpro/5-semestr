#include "matrix.c"


double costDerivative(double output, double y)
{
    return (output - y)/(output * (1 - output));
}


Matrix applyCostDerivative(Matrix output, Matrix y)
{
    if (output.rowNum != y.rowNum || output.columnNum != 1 || y.columnNum != 1)
    {
        exit(-1);
    }

    double** value = calloc((size_t)output.rowNum, sizeof(double*));
    for (int i=0; i<output.rowNum; i++)
    {
        value[i] = calloc(1, sizeof(double));
        value[i][0] = costDerivative(output.value[i][0], y.value[i][0]);
    }

    return matrixInit(output.rowNum, 1, value);
}