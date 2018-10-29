#ifndef _FULLYCONNECTEDLAYER_
#define _FULLYCONNECTEDLAYER_

#include "matrix.c"
#include "sigmoid.c"

struct fullyConnectedLayer
{
    int numberOfInputs;

    Matrix weights;
    Matrix biases;
};

typedef struct fullyConnectedLayer FullyConnectedLayer;


Matrix flattenSources(FullyConnectedLayer fullLayer, Matrix* sources, int numberOfSources)
{
    double** value = calloc((size_t)fullLayer.numberOfInputs, sizeof(double*));
    for (int i=0; i<fullLayer.numberOfInputs; i++)
    {
        value[i] = calloc(1, sizeof(double));
    }
    int counter = 0;
    for (int i=0; i<numberOfSources; i++)
    {
        for (int j=0; j<sources[i].rowNum; j++)
        {
            for (int k=0; k<sources[i].columnNum; k++)
            {
                value[counter][0] = sources[i].value[j][k];
                counter++;
            }
        }
    }
    return matrixInit(fullLayer.numberOfInputs, 1, value);
}


Matrix applyFullActivationFunction(Matrix source)
{
    if (source.columnNum != 1)
    {
        exit(-1);
    }
    double** value = calloc((size_t)source.rowNum, sizeof(double*));
    for (int i=0; i<source.rowNum; i++)
    {
        value[i] = calloc(1, sizeof(double));
        value[i][0] = sigmoid(source.value[i][0]);
    }

    return matrixInit(source.rowNum, source.columnNum, value);
}



Matrix fullForwardFeed(FullyConnectedLayer fullLayer, Matrix input)
{
    Matrix mul = matrixMul(fullLayer.weights, input);
    Matrix add = matrixAdd(mul, fullLayer.biases);
    Matrix result = applyFullActivationFunction(add);
    matrixFree(mul);
    matrixFree(add);
    return result;
}


FullyConnectedLayer fullLayerInit(int numberOfNeurons, int numberOfInputs)
{
    FullyConnectedLayer fullLayer;

    Matrix weights = matrixGenerate(numberOfNeurons, numberOfInputs);
    fullLayer.weights = weights;

    Matrix biases = matrixGenerate(numberOfNeurons, 1);
    fullLayer.biases = biases;

    fullLayer.numberOfInputs = numberOfInputs;

    return fullLayer;
}







#endif
