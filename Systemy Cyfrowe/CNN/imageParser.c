#ifndef _IMAGEPARSER_
#define _IMAGEPARSER_

#include "matrix.c"


void createPGM(char* fileName, Matrix image)
{
    FILE* file = fopen(fileName, "w");
    fprintf(file, "P2\n%d %d\n255\n", image.columnNum, image.rowNum);
    for (int n=0; n < image.columnNum * image.rowNum; n++)
    {
        fprintf(file, "%d ", (unsigned char) (image.value[n / image.columnNum][n % image.rowNum] * 255));
        if (n % 15 == 0) {
            fprintf(file, "\n");
        }
    }
    fclose(file);
}


#endif