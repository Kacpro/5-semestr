#ifndef _TESTCASE_
#define _TESTCASE_

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "matrix.c"

struct testCase
{
    Matrix expected;
    Matrix input;
    int expectedValue;
};

typedef struct testCase TestCase;


Matrix prepareExpected(int num)
{
    double** value = calloc(10, sizeof(double));
    for (int i=0; i<10; i++)
    {
        value[i] = calloc(1, sizeof(double));
        value[i][0] = i == num ? 1 : 0;
    }

    return matrixInit(10, 1, value);
}


TestCase testCaseInit(Matrix input, int expectedValue)
{
    TestCase testCase =
            {
                    .expectedValue = expectedValue,
                    .input = input,
                    .expected = prepareExpected(expectedValue)
            };
    return testCase;
}


void testsFree(TestCase* tests, int testsNum)
{
    for (int i=0; i<testsNum; i++)
    {
        matrixFree(tests[i].input);
        matrixFree(tests[i].expected);
    }
    free(tests);
}



void testShuffle(TestCase* tests, int testsLen)
{
    srand(time(0));
    for (int i = 0; i < testsLen; i++)
    {
        int j = i + rand() % ((testsLen - i) + 1);

        TestCase t = tests[j];
        tests[j] = tests[i];
        tests[i] = t;
    }
}

#endif