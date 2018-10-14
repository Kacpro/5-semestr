#include<stdio.h>
#include<math.h>
#include<time.h>

void foo(float value, int size){
	time_t start = clock();	
	float c = 0;
	float sum = value;
	for (int i=1; i<size; i++){
		float y = value - c;
		float t = sum + y;
		c = (t - sum) - y;
		sum = t;
	}
	time_t end = clock();
	printf("%d %f\n", size, ((double) (end - start)) / CLOCKS_PER_SEC);

}

void bar(float value, int size){
	float c = 0;

	float sum = value;
	for (int i=1; i<size; i++){
		float y = value - c;
		float t = sum + y;
		c = (t - sum) - y;
		sum = t;
	}
	
	float abs_error = fabs(sum - value * size);
        float rel_error = fabs(abs_error / (value * size));
        printf("%f %f %f\n", sum, abs_error, rel_error);
}

int main(){
	for (int i=0; i<1000; i++)
		bar(0.1111111, 100000 * (i+1));

	return 0;
}
