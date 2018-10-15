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
    return matrixAdd(convMatrix, biasMatrix);
}


Matrix applyPooling(Matrix source)
{
    double** value = calloc((size_t)source.rowNum/2, sizeof(double*));
    for (int i=0; i<source.rowNum/2; i++)
    {
        value[i] = calloc((size_t)source.columnNum/2, sizeof(double));
    }
    for (int i=0; i<source.rowNum; i += 2)
    {
        for (int j=0; j<source.columnNum; j += 2)
        {
            double max = source.value[i][j];
            if (source.value[i+1][j] > max) max = source.value[i+1][j];
            if (source.value[i][j+1] > max) max = source.value[i][j+1];
            if (source.value[i+1][j+1] > max) max = source.value[i+1][j+1];
            value[i/2][j/2] = max;
        }
    }
    return matrixInit(source.rowNum/2, source.columnNum/2, value);
}

Matrix applyConvActivationFunction(Matrix source)
{
    double** value = matrixCopy(source).value;
    for (int i=0; i<source.rowNum; i++)
    {
        for (int j=0; j<source.columnNum; j++)
        {
            value[i][j] = sigmoid(value[i][j]);
        }
    }
    return matrixInit(source.rowNum, source.columnNum, value);
}


Matrix* convForwardFeed(ConvolutionalLayer convLayer, Matrix* sources, int numberOfSources)
{
    Matrix* result = calloc((size_t)(numberOfSources * convLayer.featureMapNum), sizeof(Matrix));
    int counter = 0;

    for (int i=0; i<numberOfSources; i++)
    {
        for (int j=0; j<convLayer.featureMapNum; j++)
        {
            result[counter] = applyPooling(applyConvActivationFunction(
                    calcSingleFutureMap(sources[i], convLayer.weights[j], convLayer.biases[j])));
            counter++;
        }
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
