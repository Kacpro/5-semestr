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
