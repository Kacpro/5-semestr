#ifndef _CONVOLUTIONALLAYER_
#define _CONVOLUTIONALLAYER_

#include "matrix.c"
#include "sigmoid.c"


struct convolutionalLayer
{
    int featureMapNum;

    Matrix* weights;
    double* biases;
};

typedef struct convolutionalLayer ConvolutionalLayer;


Matrix calcSingleFutureMap(Matrix source, Matrix weights, double bias)
{
    Matrix convMatrix = innerConvolution(source, weights);
    double** value = calloc((size_t)(source.rowNum - weights.rowNum + 1), sizeof(double*));
    for (int i=0; i<source.rowNum - weights.rowNum + 1; i++)
    {
        value[i] = calloc((size_t)(source.columnNum - weights.columnNum + 1), sizeof(double));
        for (int j=0; j< source.columnNum - weights.columnNum + 1; j++)
            value[i][j] = bias;
    }
    Matrix biasMatrix = matrixInit(source.rowNum - weights.rowNum + 1, source.columnNum - weights.columnNum + 1, value);
    Matrix result = matrixAdd(convMatrix, biasMatrix);
    matrixFree(convMatrix);
    matrixFree(biasMatrix);
    return result;
}


struct pollingResult
{
    Matrix value;
    Matrix positions;
};


struct pollingResult applyPooling(Matrix source)
{
    double** value = calloc((size_t)source.rowNum/2, sizeof(double*));
    double** posValue = calloc((size_t)source.rowNum/2, sizeof(double*));
    for (int i=0; i<source.rowNum/2; i++)
    {
        value[i] = calloc((size_t)source.columnNum/2, sizeof(double));
        posValue[i] = calloc((size_t)source.columnNum/2, sizeof(double));
    }
    for (int i=0; i<source.rowNum; i += 2)
    {
        for (int j=0; j<source.columnNum; j += 2)
        {
            double max = source.value[i][j];
            double maxPos = 0;
            if (source.value[i+1][j] > max) { max = source.value[i+1][j]; maxPos = 2; }
            if (source.value[i][j+1] > max) { max = source.value[i][j+1]; maxPos = 1; }
            if (source.value[i+1][j+1] > max) { max = source.value[i+1][j+1]; maxPos = 3; }
            value[i/2][j/2] = max;
            posValue[i/2][j/2] = maxPos;
        }
    }
    struct pollingResult result;
    result.value = matrixInit(source.rowNum/2, source.columnNum/2, value);
    result.positions = matrixInit(source.rowNum/2, source.columnNum/2, posValue);

    return result;
}

Matrix applyConvActivationFunction(Matrix source)
{
    double** value = calloc((size_t)source.rowNum, sizeof(double*));
    for (int i=0; i<source.rowNum; i++)
    {
        value[i] = calloc((size_t)source.columnNum, sizeof(double));
        for (int j=0; j<source.columnNum; j++)
        {
            value[i][j] = sigmoid(source.value[i][j]);
        }
    }
    return matrixInit(source.rowNum, source.columnNum, value);
}


Matrix* convForwardFeed(ConvolutionalLayer convLayer, Matrix* sources, int numberOfSources)
{
    Matrix* result = calloc((size_t)(convLayer.featureMapNum), sizeof(Matrix));

    for (int j=0; j<convLayer.featureMapNum; j++)
    {
        Matrix singleFeatureMap = calcSingleFutureMap(sources[j % numberOfSources], convLayer.weights[j], convLayer.biases[j]);
        Matrix convActiv = applyConvActivationFunction(singleFeatureMap);
        struct pollingResult poolRes = applyPooling(convActiv);
        result[j] = poolRes.value;
        matrixFree(poolRes.positions);
        matrixFree(singleFeatureMap);
        matrixFree(convActiv);
    }
    return result;
}


ConvolutionalLayer convLayerInit(int futureMapNum)
{
    ConvolutionalLayer convLayer;
    convLayer.featureMapNum = futureMapNum;

    Matrix* weights = calloc((size_t)futureMapNum, sizeof(Matrix));
    double* biases = calloc((size_t)futureMapNum, sizeof(double));
    for (int i=0; i<futureMapNum; i++)
    {
        weights[i] = matrixGenerate(5, 5);
        biases[i] = genRand();
    }
    convLayer.biases = biases;
    convLayer.weights = weights;

    return convLayer;
}


Matrix* matrixToList(Matrix m, int mapSize)
{
    Matrix* result = calloc((size_t)m.rowNum * m.columnNum, sizeof(Matrix));
    int sourcesNum = m.columnNum / (mapSize * mapSize);
    for (int i=0; i<m.rowNum * m.columnNum; i++)
    {
        double** value = calloc((size_t)mapSize, sizeof(double*));
        for (int j=0; j<mapSize; j++)
        {
            value[j] = calloc((size_t)mapSize, sizeof(double));
        }

        int pos = (i % m.columnNum)/sourcesNum;
        value[pos / mapSize][pos % mapSize] = m.value[i / m.columnNum][mapSize * mapSize * (i % sourcesNum) + ((i % m.columnNum) / sourcesNum)];
        result[i] = matrixInit(mapSize, mapSize, value);
    }
    return result;
}


Matrix* vectorToList(Matrix m, int sourcesNum)
{
    Matrix* result = calloc((size_t)sourcesNum * m.rowNum, sizeof(Matrix));
    for (int i=0; i<sourcesNum * m.rowNum; i++)
    {
        double** value = calloc(1, sizeof(double*));
        value[0] = calloc(1, sizeof(double));
        value[0][0] = m.value[i/sourcesNum][0];
        result[i] = matrixInit(1, 1, value);
    }
    return result;
}


Matrix reversePooling(Matrix m, Matrix positions)
{
    double** value = calloc((size_t)m.rowNum * 2, sizeof(double*));
    for (int i=0; i<m.rowNum * 2; i++)
    {
        value[i] = calloc((size_t)m.columnNum * 2, sizeof(double));
        for (int j=0; j<m.columnNum; j++)
        {
            value[i][j] = 0.0;
        }
    }
    for (int i=0; i<m.rowNum; i++)
    {
        for (int j=0; j<m.columnNum; j++)
        {
            value[2*i + (int)positions.value[i][j] / 2][2*j + (int)positions.value[i][j] % 2] = m.value[i][j];
        }
    }
    Matrix resultMatrix = matrixInit(m.rowNum * 2, m.columnNum * 2, value);

    return resultMatrix;
}


#endif