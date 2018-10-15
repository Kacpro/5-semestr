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
