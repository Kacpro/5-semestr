#include "testCase.c"
#include "mnistParser.c"
#include "network.c"



int main() {
    struct testData tests = parseMnist("../train-images.idx3-ubyte", "../train-labels.idx1-ubyte");

    int futureMapNums[] = {8, 16};
    int neuronNums[] = {120, 84, 10};
    Network network = networkInit(2, futureMapNums, 3, neuronNums, 256);

    learn(network, 100, tests.testSet, 50000, tests.testSet, 10000, 5000);

    testsFree(tests.checkSet, 10000);
    testsFree(tests.testSet, 50000);
    networkFree(network);

    return 0;
}
