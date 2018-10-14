#ifndef _MATRIX_
#define _MATRIX_

#include <stdlib.h>
#include <stdio.h>


typedef struct matrix Matrix;

struct matrix
{
    int rowNum;
    int columnNum;
    double** value;
};



Matrix matrixInit(int rowNum, int columnNum, double **value)
{
    double **newValue = calloc(rowNum, sizeof(double *));
    for (int i = 0; i < rowNum; i++)
    {
        newValue[i] = calloc(columnNum, sizeof(double));
        for (int j = 0; j < columnNum; j++)
            newValue[i][j] = 0;
    }

    Matrix newMatrix =
            {
                    .rowNum = rowNum,
                    .columnNum = columnNum,
                    .value = value == NULL ? newValue : value,
            };

    return newMatrix;
}



Matrix matrixAdd(Matrix m1, Matrix m2)
{
    if (m1.rowNum != m2.rowNum || m1.columnNum != m2.columnNum)
    {
        exit(-1);
    }

    double** newValue = calloc(m1.rowNum, sizeof(double*));
    for (int i=0; i<m1.rowNum; i++)
    {
        newValue[i] = calloc(m1.columnNum, sizeof(double));
        for (int j = 0; j < m1.columnNum; j++)
        {
            newValue[i][j] = m1.value[i][j] + m2.value[i][j];
        }
    }

    return matrixInit(m1.rowNum, m1.columnNum, newValue);
}


Matrix matrixSubtract(Matrix m1, Matrix m2)
{
    if (m1.rowNum != m2.rowNum || m1.columnNum != m2.columnNum)
    {
        exit(-1);
    }

    double** newValue = calloc(m1.rowNum, sizeof(double*));
    for (int i=0; i<m1.rowNum; i++)
    {
        newValue[i] = calloc(m1.columnNum, sizeof(double));
        for (int j = 0; j < m1.columnNum; j++)
        {
            newValue[i][j] = m1.value[i][j] - m2.value[i][j];
        }
    }

    return matrixInit(m1.rowNum, m1.columnNum, newValue);
}



double matrixDot(Matrix m1, Matrix m2)
{
    if (m1.rowNum != 1 || m2.rowNum != 1 || m1.columnNum != m2.columnNum)
    {
        exit(-1);
    }

    double result = 0;
    for(int i=0; i<m1.columnNum; i++)
        result += m1.value[0][i] * m2.value[0][i];

    return result;
}



Matrix matrixElementMul(Matrix m1, Matrix m2)
{
    if (m1.columnNum != 1 || m2.columnNum != 1 || m1.rowNum != m2.rowNum)
    {
        exit(-1);
    }

    double** newValue = calloc(m1.rowNum, sizeof(double*));
    for (int i=0; i<m1.rowNum; i++)
    {
        newValue[i][0] = m1.value[i][0] * m2.value[i][0];
    }

    return matrixInit(m1.rowNum, 1, newValue);
}



Matrix matrixTranspose(Matrix m)
{
    double** newValue = calloc(m.columnNum, sizeof(double*));
    for (int i=0; i<m.columnNum; i++)
    {
        newValue[i] = calloc(m.rowNum, sizeof(double));
        for (int j=0; j<m.rowNum; j++)
            newValue[i][j] = m.value[j][i];
    }

    return matrixInit(m.columnNum, m.rowNum, newValue);
}


void matrixPrint(Matrix m)
{
    for (int i=0; i<m.rowNum; i++)
    {
        for (int j = 0; j < m.columnNum; j++)
            printf("%f ", m.value[i][j]);
        printf("\n");
    }
}


Matrix matrixScalarMul(Matrix m, double z)
{
    double** newValue = calloc(m.rowNum, sizeof(double*));
    for (int i=0; i<m.rowNum; i++)
    {
        for (int j=0; j<m.columnNum; j++)
            newValue[i][j] = m.value[i][j] * z;
    }
    return matrixInit(m.rowNum, m.columnNum, newValue);
}


Matrix matrixMul(Matrix m1, Matrix m2)
{
    if (m1.columnNum != m2.rowNum)
    {
        exit(-1);
    }

    double** newValue = calloc(m1.rowNum, sizeof(double*));
    for (int i=0; i<m1.rowNum; i++)
    {
        newValue[i] = calloc(m2.columnNum, sizeof(double));
    }

    for (int i=0; i<m1.rowNum; i++)
    {
        for (int j=0; j<m2.columnNum; j++)
        {
            double result = 0;
            for (int k=0; k<m1.columnNum; k++)
            {
                result += m1.value[i][k] * m2.value[k][j];
            }
            newValue[i][j] = result;
        }
    }
    return matrixInit(m1.rowNum, m2.columnNum, newValue);
}


Matrix matrixCopy(Matrix m)
{
    double** value = calloc((size_t)m.rowNum, sizeof(double*));
    for (int i=0; i<m.rowNum; i++)
    {
        value[i] = calloc((size_t)m.columnNum, sizeof(double));
        for (int j=0; j<m.columnNum; j++)
            value[i][j] = m.value[i][j];
    }
    Matrix result = matrixInit(m.rowNum, m.columnNum, value);
    return result;
}


Matrix innerConvolution(Matrix source, Matrix filter)
{
    if (source.columnNum < filter.columnNum || source.rowNum < filter.rowNum)
    {
        exit(-1);
    }

    double** value = calloc((size_t)(source.rowNum - filter.rowNum + 1), sizeof(double*));
    for (int i=0; i<(source.rowNum - filter.rowNum + 1); i++)
    {
        value[i] = calloc((size_t)(source.columnNum - filter.columnNum + 1), sizeof(double));
        for (int j=0; j<(source.columnNum - filter.columnNum + 1); j++)
        {
            double singleValue = 0;
            for (int n=0; n<filter.rowNum; n++)
            {
                for (int m=0; m<filter.columnNum; m++)
                {
                    singleValue += filter.value[n][m] * source.value[i+n][j+m];
                }
            }
            value[i][j] = singleValue;
        }
    }
    return matrixInit((source.rowNum - filter.rowNum + 1), (source.columnNum - filter.columnNum + 1), value);
}


Matrix outerConvolution(Matrix source, Matrix filter)
{
    double** value = calloc((size_t)(source.rowNum + filter.rowNum - 1), sizeof(double*));
    for (int i=0; i<(source.rowNum + filter.rowNum - 1); i++)
    {
        value[i] = calloc((size_t)(source.columnNum + filter.columnNum - 1), sizeof(double));
        for (int j=0; j<(source.columnNum + filter.columnNum - 1); j++)
        {
            double singleValue = 0;
            for (int n=0; n<filter.rowNum; n++)
            {
                for (int m=0; m<filter.columnNum; m++)
                {
                    singleValue += filter.value[n][m] *
                                           (i+n - filter.rowNum + 1) >= 0 && i+n - filter.rowNum + 1 < source.rowNum &&
                                           (j+m - filter.columnNum + 1) >= 0 && j+m - filter.columnNum + 1 < source.columnNum
                                                ? source.value[i+n - filter.rowNum + 1][j+m - filter.columnNum + 1]
                                                : 0;
                }
            }
            value[i][j] = singleValue;
        }
    }
    return matrixInit((source.rowNum + filter.rowNum - 1), (source.columnNum + filter.columnNum - 1), value);
}




#endif