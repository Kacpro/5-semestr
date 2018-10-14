
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

