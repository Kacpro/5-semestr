#include "matrix.c"

struct fullyConnectedLayer
{
    int filterSize;
    int featureMapNum;

    Matrix* weights;
    Matrix biases;
};

typedef struct fullyConnectedLayer FullyConnectedLayer;

