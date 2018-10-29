#include "matrix.c"
#include "fullyConnectedLayer.c"
#include "convolutionalLayer.c"
#include "costFunction.c"
#include "testCase.c"


struct network
{
    int convLayerNum;
    int fullLayerNum;

    ConvolutionalLayer* convLayers;
    FullyConnectedLayer* fullLayers;
};

typedef struct network Network;


void networkFree(Network network)
{
    for (int i=0; i<network.convLayerNum; i++)
    {
        for (int j=0; j<network.convLayers[i].featureMapNum; j++)
        {
            matrixFree(network.convLayers[i].weights[j]);
        }
        free(network.convLayers[i].biases);
        free(network.convLayers[i].weights);
    }

    for (int i=0; i<network.fullLayerNum; i++)
    {
        matrixFree(network.fullLayers[i].weights);
        matrixFree(network.fullLayers[i].biases);
    }

    free(network.fullLayers);
    free(network.convLayers);
}


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
        Matrix* newBuf = convForwardFeed(network.convLayers[i], buf, i>0 ? network.convLayers[i-1].featureMapNum : 1);
        for (int j=0; j< (i > 0 ? network.convLayers[i-1].featureMapNum : 0); j++)
        {
            matrixFree(buf[j]);
        }
        free(buf);
        buf = newBuf;
    }

    Matrix flatBuf;
    flatBuf = flattenSources(network.fullLayers[0], buf, network.convLayers[network.convLayerNum - 1].featureMapNum);

    for (int i=0; i<network.convLayers[network.convLayerNum - 1].featureMapNum; i++)
    {
        matrixFree(buf[i]);
    }
    free(buf);

    for (int i=0; i<network.fullLayerNum; i++)
    {
        Matrix newFlatBuf = fullForwardFeed(network.fullLayers[i], flatBuf);
        matrixFree(flatBuf);
        flatBuf = newFlatBuf;
    }

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


//double applyCostFunction(Matrix output, Matrix expected)
//{
//    double sum = 0;
//    for (int i=0; i<10; i++)
//    {
//        sum += costFunction(output[i][0], expected[i][0]);
//    }
//    return sum/10.0;
//}
//
//
//double calcSingleInputError(Network network, TestCase testCase)
//{
//    Matrix output = feedForward(network, testCase.input);
//    return applyCostFunction(output, testCase.expected);
//}




Network backPropagation(Network network, TestCase* testCases, int numberOfTestCases, double learningRate)
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

    for (int t=0; t<numberOfTestCases; t++)
    {
        Matrix** convNablaWbuf = calloc((size_t)network.convLayerNum, sizeof(Matrix*));
        double** convNablaBbuf = calloc((size_t)network.convLayerNum, sizeof(double*));
        for (int i=0; i<network.convLayerNum; i++)
        {
            convNablaWbuf[i] = calloc((size_t)network.convLayers[i].featureMapNum, sizeof(Matrix));
            convNablaBbuf[i] = calloc((size_t)network.convLayers[i].featureMapNum, sizeof(double));
            for (int j=0; j<network.convLayers[i].featureMapNum; j++)
            {
                convNablaBbuf[i][j] = 0;
            }
        }

        Matrix* fullNablaWbuf = calloc((size_t)network.fullLayerNum, sizeof(Matrix));
        Matrix* fullNablaBbuf = calloc((size_t)network.fullLayerNum, sizeof(Matrix));

        Matrix** convActivations = calloc((size_t)network.convLayerNum, sizeof(Matrix*));
        Matrix** convZs = calloc((size_t)network.convLayerNum, sizeof(Matrix*));
        Matrix** pollPositions = calloc((size_t)network.convLayerNum, sizeof(Matrix*));

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        Matrix* sources = calloc(1, sizeof(Matrix));
        sources[0] = matrixCopy(testCases[t].input);

        for (int i=0; i<network.convLayerNum; i++)
        {
            Matrix* singleConvZ = calloc((size_t)network.convLayers[i].featureMapNum, sizeof(Matrix));
            Matrix* singleConvA = calloc((size_t)network.convLayers[i].featureMapNum, sizeof(Matrix));
            Matrix* singlePollPos = calloc((size_t)network.convLayers[i].featureMapNum, sizeof(Matrix));
            Matrix* newSources = calloc((size_t)network.convLayers[i].featureMapNum, sizeof(Matrix));
            int sourceNum = i > 0 ? network.convLayers[i-1].featureMapNum : 1;
            for (int j = 0; j < network.convLayers[i].featureMapNum; j++)
            {
                singleConvZ[j] = calcSingleFutureMap(sources[j % sourceNum],
                                                     network.convLayers[i].weights[j],
                                                     network.convLayers[i].biases[j]);
                singleConvA[j] = applyConvActivationFunction(singleConvZ[j]);
                struct pollingResult result = applyPooling(singleConvA[j]);
                newSources[j] = result.value;
                singlePollPos[j] = result.positions;
            }

            for (int j=0; j < (i>0 ? network.convLayers[i-1].featureMapNum : 1); j++)
            {
                matrixFree(sources[j]);
            }

            free(sources);
            sources = newSources;
            convZs[i] = singleConvZ;
            convActivations[i] = singleConvA;
            pollPositions[i] = singlePollPos;
        }


        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        Matrix flatSources = flattenSources(network.fullLayers[0], sources, network.convLayers[network.convLayerNum - 1].featureMapNum);
        Matrix* fullActivations = calloc((size_t)network.fullLayerNum, sizeof(Matrix));
        Matrix* fullZs = calloc((size_t)network.fullLayerNum, sizeof(Matrix));
        Matrix flatSourceBuf = flatSources;
        for (int i=0; i<network.fullLayerNum; i++)
        {
            Matrix mul = matrixMul(network.fullLayers[i].weights, flatSources);
            fullZs[i] = matrixAdd(mul, network.fullLayers[i].biases);
            fullActivations[i] = fullForwardFeed(network.fullLayers[i], flatSources);
            flatSources = fullActivations[i];
            matrixFree(mul);
        }

        matrixFree(flatSourceBuf);


        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        Matrix activ = applyCostDerivative(fullActivations[network.fullLayerNum - 1], testCases[t].expected);
        Matrix sigmoidDeriv = applySigmoidDerivative(fullZs[network.fullLayerNum - 1]);
        Matrix delta = matrixElementMul(activ, sigmoidDeriv);
        matrixFree(activ);
        matrixFree(sigmoidDeriv);
        fullNablaBbuf[network.fullLayerNum - 1] = delta;
        Matrix activTrans = matrixTranspose(fullActivations[network.fullLayerNum - 2]);
        fullNablaWbuf[network.fullLayerNum - 1] = matrixMul(delta, activTrans);
        matrixFree(activTrans);

        for (int i=network.fullLayerNum - 2; i>=0; i--)
        {
            Matrix z = fullZs[i];
            Matrix sd = applySigmoidDerivative(z);
            Matrix trans = matrixTranspose(network.fullLayers[i+1].weights);
            Matrix mul = matrixMul(trans, delta);
            delta = matrixElementMul(mul, sd);
            fullNablaBbuf[i] = delta;
            Matrix flatten = flattenSources(network.fullLayers[0], sources, network.convLayers[network.convLayerNum - 1].featureMapNum);
            Matrix transposed = matrixTranspose(i-1 >= 0 ? fullActivations[i-1] : flatten);
            fullNablaWbuf[i] = matrixMul(delta, transposed);

            matrixFree(transposed);
            matrixFree(flatten);
            matrixFree(trans);
            matrixFree(mul);
            matrixFree(sd);
        }

        matrixFree(flatSources);

        for (int i=0; i<network.fullLayerNum; i++)
        {
            matrixFree(fullZs[i]);
        }

        for (int i=0; i<network.fullLayerNum - 1; i++)
        {
            matrixFree(fullActivations[i]);
        }
        free(fullActivations);
        free(fullZs);

        for (int j=0; j < network.convLayers[network.convLayerNum - 1].featureMapNum; j++)
        {
            matrixFree(sources[j]);
        }
        free(sources);

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        Matrix* convInputW = matrixToList(network.fullLayers[0].weights, convZs[network.convLayerNum - 1][0].columnNum/2);
        Matrix* convDelta = vectorToList(delta, network.fullLayers[0].numberOfInputs);

        for (int i=network.convLayerNum - 1; i>=0; i--)
        {
            int sourcesNum = i<network.convLayerNum - 1 ? network.convLayers[i+1].featureMapNum : network.fullLayers[0].weights.columnNum * network.fullLayers[0].weights.rowNum;
            Matrix* newConvDelta = calloc((size_t)network.convLayers[i].featureMapNum, sizeof(Matrix));
            Matrix* partialDeltas = calloc((size_t)network.convLayers[i].featureMapNum, sizeof(Matrix));

            for (int j=0; j<network.convLayers[i].featureMapNum; j++)
            {
                partialDeltas[j] = matrixInit(convZs[i][0].rowNum, convZs[i][0].columnNum, NULL);
            }

            for (int j=0; j<sourcesNum; j++)
            {
                Matrix rotated = matrixRotation(convInputW[j]);
                Matrix outerConv = outerConvolution(convDelta[j], rotated);
                Matrix revPool = reversePooling(outerConv, pollPositions[i][j%network.convLayers[i].featureMapNum]);
                Matrix singlePartialDelta = partialDeltas[j%network.convLayers[i].featureMapNum];
                Matrix newPartialDelta = matrixAdd(singlePartialDelta, revPool);
                partialDeltas[j%network.convLayers[i].featureMapNum] = newPartialDelta;

                matrixFree(revPool);
                matrixFree(singlePartialDelta);
                matrixFree(outerConv);
                matrixFree(rotated);
                matrixFree(convInputW[j]);
                matrixFree(convDelta[j]);
            }

            for (int j=0; j<network.convLayers[i].featureMapNum; j++)
            {
                matrixFree(pollPositions[i][j]);
            }
            free(pollPositions[i]);

            for (int j=0; j<network.convLayers[i].featureMapNum; j++)
            {
                Matrix sigmDer = applySigmoidDerivative(convZs[i][j]);
                newConvDelta[j] = matrixElementMul(partialDeltas[j], sigmDer);
                Matrix rotated = matrixRotation(newConvDelta[j]);
                if (i > 0)
                {
                    struct pollingResult input = applyPooling(convActivations[i-1][j % network.convLayers[i-1].featureMapNum]);
                    convNablaWbuf[i][j] = innerConvolution(input.value, rotated);
                    matrixFree(input.value);
                    matrixFree(input.positions);
                }
                else
                {
                    convNablaWbuf[i][j] = innerConvolution(testCases[t].input, rotated);
                }
                convNablaBbuf[i][j] = matrixSumElements(newConvDelta[j]);

                matrixFree(sigmDer);
                matrixFree(convZs[i][j]);
                matrixFree(rotated);
            }

            for (int j=0; j<network.convLayers[i].featureMapNum; j++)
            {
                matrixFree(convActivations[i][j]);
            }
            free(convZs[i]);
            free(convActivations[i]);

            for (int j=0; j<network.convLayers[i].featureMapNum; j++)
            {
                matrixFree(partialDeltas[j]);
            }
            free(convDelta);
            free(partialDeltas);

            convDelta = newConvDelta;

            for (int j=0; j<network.convLayers[i].featureMapNum; j++)
            {
                convInputW[j] = matrixCopy(network.convLayers[i].weights[j]);
            }
        }

        free(pollPositions);
        free(convZs);
        free(convActivations);

        for (int j=0; j<network.convLayers[0].featureMapNum; j++)
        {
            matrixFree(convDelta[j]);
            matrixFree(convInputW[j]);
        }

        free(convDelta);
        free(convInputW);

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        for (int i=0; i<network.convLayerNum; i++)
        {
            for (int j=0; j<network.convLayers[i].featureMapNum; j++)
            {
                Matrix oldConvNablaW = convNablaW[i][j];
                convNablaW[i][j] = matrixAdd(oldConvNablaW, convNablaWbuf[i][j]);
                convNablaB[i][j] = convNablaB[i][j] + convNablaBbuf[i][j];
                matrixFree(convNablaWbuf[i][j]);
                matrixFree(oldConvNablaW);
            }
            free(convNablaWbuf[i]);
            free(convNablaBbuf[i]);
        }
        free(convNablaBbuf);
        free(convNablaWbuf);


        for (int i=0; i<network.fullLayerNum; i++)
        {
            Matrix newFullNablaW = matrixAdd(fullNablaW[i], fullNablaWbuf[i]);
            Matrix newFullNablaB = matrixAdd(fullNablaB[i], fullNablaBbuf[i]);
            matrixFree(fullNablaW[i]);
            matrixFree(fullNablaB[i]);
            fullNablaW[i] = newFullNablaW;
            fullNablaB[i] = newFullNablaB;
            matrixFree(fullNablaWbuf[i]);
            matrixFree(fullNablaBbuf[i]);
        }
        free(fullNablaWbuf);
        free(fullNablaBbuf);


        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }


    for (int i=0; i<network.convLayerNum; i++)
    {
        for (int j=0; j<network.convLayers[i].featureMapNum; j++)
        {
            Matrix scalarMul = matrixScalarMul(convNablaW[i][j], learningRate/numberOfTestCases);
            Matrix sub = matrixSubtract(network.convLayers[i].weights[j], scalarMul);
            matrixFree(network.convLayers[i].weights[j]);
            network.convLayers[i].weights[j] = sub;
            network.convLayers[i].biases[j] -= learningRate/numberOfTestCases * convNablaB[i][j];

            matrixFree(convNablaW[i][j]);
            matrixFree(scalarMul);
        }
        free(convNablaW[i]);
        free(convNablaB[i]);
    }
    free(convNablaB);
    free(convNablaW);

    for (int i=0; i<network.fullLayerNum; i++)
    {
        Matrix scalarMulW = matrixScalarMul(fullNablaW[i], learningRate/numberOfTestCases);
        Matrix scalarMulB = matrixScalarMul(fullNablaB[i], learningRate/numberOfTestCases);
        Matrix newWeights = matrixSubtract(network.fullLayers[i].weights, scalarMulW);
        Matrix newBiases = matrixSubtract(network.fullLayers[i].biases, scalarMulB);
        matrixFree(network.fullLayers[i].weights);
        matrixFree(network.fullLayers[i].biases);
        network.fullLayers[i].weights = newWeights;
        network.fullLayers[i].biases = newBiases;

        matrixFree(fullNablaW[i]);
        matrixFree(fullNablaB[i]);
        matrixFree(scalarMulB);
        matrixFree(scalarMulW);
    }

    free(fullNablaB);
    free(fullNablaW);

    return network;
}


void updateBatch(Network network, int firstIndex, int length, TestCase* tests)
{
    backPropagation(network, tests + firstIndex * sizeof(TestCase), length, 0.10);
    printf("...\n");
}


void learn(Network network, int epochNum, TestCase* testSet, int testSetSize, TestCase* checkSet, int checkSetSize, int batchNum)
{
    for (int i=0; i<epochNum; i++)
    {
        for (int k=0; k<batchNum; k++)
        {
            updateBatch(network, k*testSetSize/batchNum, testSetSize/batchNum, testSet);
        }
        int sum = 0;
        for (int j = 0; j < checkSetSize; j++)
        {
            Matrix result = feedForward(network, checkSet[j].input);
            sum += checkResult(result, checkSet[j]);
            matrixFree(result);
        }
        printf("Epoch: %d Correct: %d\n", i, sum);
        testShuffle(testSet, testSetSize);
    }
}