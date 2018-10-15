#include <stdio.h>
#include "matrix.c"
#include "convolutionalLayer.c"
#include "fullyConnectedLayer.c"
#include "testCase.c"
#include "imageParser.c"
#include "mnistParser.c"

int main() {
    ConvolutionalLayer cl = convLayerInit(3);
    FullyConnectedLayer fl = fullLayerInit(10, 16);

    TestCase * tests = parseMnist("../train-images.idx3-ubyte", "../train-labels.idx1-ubyte");
//    createPGM("test.pgm", tests[100].input);

    Matrix* t = calloc(1, sizeof(Matrix));
    t[0] = tests[0].input;
    t[0] = convForwardFeed(cl, t, 1)[0];
    t[0] = convForwardFeed(cl, t, 1)[2];
    t[0] = flattenSources(fl, t, 1);

    matrixPrint(fullForwardFeed(fl, t, 1));

    return 0;
}