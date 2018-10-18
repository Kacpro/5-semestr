#include "matrix.c"
#include "fullyConnectedLayer.c"
#include "convolutionalLayer.c"


struct network
{
    int convLayerNum;
    int fullLayerNum;

    ConvolutionalLayer* convLayers;
    FullyConnectedLayer* fullLayers;
};

typedef struct network Network;


Network networkInit(int convLayerNum, int* futureMapNum, int fullLayerNum, int* neuronNum, int firstFullLayerInputNum)
{
    Network network;
    network.convLayerNum = convLayerNum;
    network.fullLayerNum = fullLayerNum;

    ConvolutionalLayer* convLayers = calloc((size_t)convLayerNum, sizeof(ConvolutionalLayer));
    for (int i=0; i<convLayerNum; i++)
    {
        convLayers[i] = convLayerInit(futureMapNum[i]);
    }
    network.convLayers = convLayers;

    FullyConnectedLayer* fullLayers = calloc((size_t)fullLayerNum, sizeof(FullyConnectedLayer));
    for (int i=0; i<fullLayerNum; i++)
    {
        fullLayers[i] = fullLayerInit(neuronNum[i], i==0 ? firstFullLayerInputNum : neuronNum[i-1]);
    }
    network.fullLayers = fullLayers;

    return network;
}


Matrix feedForward(Network network, Matrix input)
{
    Matrix* buf = calloc(1, sizeof(Matrix));
    buf[0] = input;

    for (int i=0; i<network.convLayerNum; i++)
    {
        buf = convForwardFeed(network.convLayers[i], buf, i>0 ? network.convLayers[i-1].featureMapNum : 1);
    }

    Matrix flatBuf;
    flatBuf = flattenSources(network.fullLayers[0], buf, network.convLayers[network.convLayerNum - 1].featureMapNum);

    for (int i=0; i<network.fullLayerNum; i++)
    {
        flatBuf = fullForwardFeed(network.fullLayers[i], flatBuf);
    }

    matrixPrint(flatBuf);
    return flatBuf;
}


int getResult(Matrix output)
{
    double max = output.value[0][0];
    int index = 0;
    for (int i=1; i<10; i++)
    {
        if (output.value[i][0] > max)
        {
            max = output.value[i][0];
            index = i;
        }
    }
    return index;
}


int checkResult(Matrix output,TestCase testCase)
{
    if (getResult(output) == testCase.expectedValue)
    {
        return 1;
    }
    return 0;
}


double applyCostFunction(Matrix output, Matrix expected)
{
    double sum = 0;
    for (int i=0; i<10; i++)
    {
        sum += costFunction(output[i][0], expected[i][0]);
    }
    return sum/10.0;
}


double calcSingleInputError(Network network, TestCase testCase)
{
    Matrix output = feedForward(network, testCase.input);
    return applyCostFunction(output, testCase.expected);
}




Network backPropagation(Network network, TestCase* testCases, int numerOfTestCases)
{
    Matrix** convNablaW = calloc((size_t)network.convLayerNum, sizeof(Matrix*));
    double** convNablaB = calloc((size_t)network.convLayerNum, sizeof(double*));
    for (int i=0; i<network.convLayerNum; i++)
    {
        convNablaW[i] = calloc((size_t)network.convLayers[i].featureMapNum, sizeof(Matrix));
        convNablaB[i] = calloc((size_t)network.convLayers[i].featureMapNum, sizeof(double));
        for (int j=0; j<network.convLayers[i].featureMapNum; j++)
        {
            convNablaW[i][j] = matrixInit(network.convLayers[i].weights[j].rowNum, network.convLayers[i].weights[j].columnNum, NULL);
            convNablaB[i][j] = 0;
        }
    }


    Matrix* fullNablaW = calloc((size_t)network.fullLayerNum, sizeof(Matrix));
    Matrix* fullNablaB = calloc((size_t)network.fullLayerNum, sizeof(Matrix));
    for (int i=0; i<network.fullLayerNum; i++)
    {
        fullNablaW[i] = matrixInit(network.fullLayers[i].weights.rowNum, network.fullLayers[i].weights.columnNum, NULL);
        fullNablaB[i] = matrixInit(network.fullLayers[i].biases.rowNum, 1, NULL);
    }



    for (int t=0; t<numerOfTestCases; t++)
    {
        Matrix** convNablaWbuf = calloc((size_t)network.convLayerNum, sizeof(Matrix*));
        double** convNablaBbuf = calloc((size_t)network.convLayerNum, sizeof(double*));
        for (int i=0; i<network.convLayerNum; i++)
        {
            convNablaWbuf[i] = calloc((size_t)network.convLayers[i].featureMapNum, sizeof(Matrix));
            convNablaBbuf[i] = calloc((size_t)network.convLayers[i].featureMapNum, sizeof(double));
            for (int j=0; j<network.convLayers[i].featureMapNum; j++)
            {
                convNablaWbuf[i][j] = matrixInit(network.convLayers[i].weights[j].rowNum, network.convLayers[i].weights[j].columnNum, NULL);
                convNablaBbuf[i][j] = 0;
            }
        }


        Matrix* fullNablaWbuf = calloc((size_t)network.fullLayerNum, sizeof(Matrix));
        Matrix* fullNablaBbuf = calloc((size_t)network.fullLayerNum, sizeof(Matrix));
        for (int i=0; i<network.fullLayerNum; i++)
        {
            fullNablaWbuf[i] = matrixInit(network.fullLayers[i].weights.rowNum, network.fullLayers[i].weights.columnNum, NULL);
            fullNablaBbuf[i] = matrixInit(network.fullLayers[i].biases.rowNum, 1, NULL);
        }

        Matrix** convActivations = calloc((size_t)network.convLayerNum, sizeof(Matrix*));
        Matrix** convZs = calloc((size_t)network.convLayerNum, sizeof(Matrix*));

        int mul = 1;
        for (int i=0; i<network.convLayerNum; i++)
        {
            convActivations[i] = calloc((size_t)network.convLayers[i].featureMapNum * mul, sizeof(Matrix));
            convZs[i] = calloc((size_t)network.convLayers[i].featureMapNum * mul, sizeof(Matrix));
        }

        convActivations[0][0] = testCases[t].input;
        mul = 1;

        Matrix* sources = calloc(1, sizeof(Matrix));
        sources[0] = testCases[t].input;

        for (int i=0; i<network.convLayerNum; i++)
        {
            int counter = 0;
            Matrix* singleConvZ = calloc((size_t)network.convLayers[i].featureMapNum * mul, sizeof(Matrix));
            Matrix* singleConvA = calloc((size_t)network.convLayers[i].featureMapNum * mul, sizeof(Matrix));
            for (int m=0; m<mul; m++)
            {
                for (int j = 0; j < network.convLayers[i].featureMapNum; j++)
                {
                    singleConvZ[counter] = calcSingleFutureMap(sources[counter], network.convLayers[i].weights[j], network.convLayers[i].biases[j]);
                    singleConvA[counter] = applyConvActivationFunction(calcSingleFutureMap(sources[counter], network.convLayers[i].weights[j], network.convLayers[i].biases[j]));
                    counter++;
                }
            }
            convZs[i] = singleConvZ;
            convActivations[i] = singleConvA;
            sources = convForwardFeed(network.convLayers[i], sources, i == 0 ? 1 : network.convLayers[i-1].featureMapNum * mul);
            mul *= network.convLayers[i].featureMapNum;
        }

        Matrix flatSources = flattenSources(network.fullLayers[0], sources, mul);
        Matrix* fullActivations = calloc((size_t)network.fullLayerNum, sizeof(Matrix));
        Matrix* fullZs = calloc((size_t)network.fullLayers, sizeof(Matrix));

        for (int i=0; i<network.fullLayerNum; i++)
        {
            fullZs[i] = matrixAdd(matrixMul(network.fullLayers[i].weights, flatSources), network.fullLayers[i].biases);
            fullActivations[i] = fullForwardFeed(network.fullLayers[i], flatSources);
            flatSources = fullActivations[i];
        }

    }
}
