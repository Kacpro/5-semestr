#include <stdio.h>
#include <math.h>
#include <time.h>

float recSum(float value, int size){
	if (size == 1) return value;
	return recSum(value, size/2) + recSum(value, size - size/2);
}

void foo(float value, int size){
	time_t start = clock();	
	recSum(value, size);
	time_t end = clock();
	printf("%d %f\n", size, ((double) (end - start)) / CLOCKS_PER_SEC);	
}

void bar(float value, int size){	
	float sum = recSum(value, size);
	printf("%d %f %f\n", size, fabs(sum - value*size), fabs((sum - value*size)/(value * size)));	
}


int main(){
	for (int i=0; i<1000; i++)
		bar(0.1111111, 100000 * (i+1));

	return 0;
}

