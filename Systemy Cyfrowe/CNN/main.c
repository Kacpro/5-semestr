#include <stdio.h>
#include "matrix.c"
#include "convolutionalLayer.c"
#include "fullyConnectedLayer.c"
#include "testCase.c"
#include "imageParser.c"
#include "mnistParser.c"
#include "network.c"

int main() {
    TestCase * tests = parseMnist("../train-images.idx3-ubyte", "../train-labels.idx1-ubyte");
//    createPGM("test.pgm", tests[100].input);

    int futureMapNums[] = {8, 2};
    int neuronNums[] = {120, 84, 10};
    Network network = networkInit(2, futureMapNums, 3, neuronNums, 256);
    printf("after networkInit\n");
    feedForward(network, tests[0].input);
    printf("\n");
    feedForward(network, tests[100].input);

    return 0;
}