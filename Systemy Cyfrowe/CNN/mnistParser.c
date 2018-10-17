#ifndef _MNISTPARSER_
#define _MNISTPARSER_

#include "testCase.c"
#include "imageParser.c"


long readWord(FILE* file)
{
    unsigned char a, b, c, d;
    fscanf(file, "%c", &a);
    fscanf(file, "%c", &b);
    fscanf(file, "%c", &c);
    fscanf(file, "%c", &d);

    return (a << 24) + (b << 16) + (c << 8) + d;
}



TestCase* parseMnist(char* sourcePath, char* labelsPath)
{
    FILE* source = fopen(sourcePath, "rb");
    FILE* labels = fopen(labelsPath, "rb");

    long imageNum, height, width;
    readWord(source);
    imageNum = readWord(source);
    height = readWord(source);
    width = readWord(source);

    readWord(labels);
    readWord(labels);

    TestCase* result = calloc((size_t)imageNum, sizeof(TestCase));

    for (int i=0; i<imageNum; i++)
    {
        unsigned char pixelValue;
        double** value = calloc((size_t)(height), sizeof(double*));
        for (int j=0; j<height; j++)
        {
            value[j] = calloc((size_t)(width), sizeof(double));
            for (int k=0; k<width; k++)
            {
                fscanf(source, "%c", &pixelValue);
                value[j][k] = pixelValue/255.0;
            }
        }


        unsigned char expectedValue;
        fscanf(labels, "%c", &expectedValue);

        Matrix matrix = matrixInit((int)(height), (int)width, value);
        TestCase testCase = testCaseInit(matrix, (int)expectedValue);
        result[i] = testCase;
    }

    fclose(labels);
    fclose(source);
    printf("parsing finished\n");
    return result;
}


#endif