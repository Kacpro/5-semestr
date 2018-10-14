#include<stdio.h>
#include<math.h>
#include<time.h>

void foo(double value, int size){
	double sum = 0;
	time_t start = clock();	
	for (int i=0; i<size; i++){
		sum += value;
	}
	time_t end = clock();
	printf("%d %f\n", size, ((double) (end - start)) / CLOCKS_PER_SEC);	
}

void bar(double value,  long long size){
	double sum = 0;
	for (long long i=0; i<size; i++){
		sum += value;
//		if (i % 10000 == 0)
//			printf("%d %f %f\n", i, fabs(value * (i+1) - sum), fabs((value * (i+1) - sum)/(value * (i+1))));
	}
	double abs_error = fabs(sum - value * size);
	double rel_error = fabs(abs_error / (value * size));
	printf("%f %f %f\n", sum, abs_error, rel_error);	
}

int main(){
	
//	for (int i=0; i<1000; i++)
//		foo(0.1111111, 100000 * i);
	bar(0.5312523432452345, 100000000000);
	return 0;
}
