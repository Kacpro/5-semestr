#include <time.h>
#include <stdio.h>


double loop_test(int n, int m){
	clock_t start, end;
	start = clock();
	long sum = 0;
	for (int i=0; i<n; i++)
		for (int j=0; j<m; j++)
			sum += i*j;
	end = clock();
	double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	return cpu_time_used;
}




int main(){
	for (int i=1; i<1000; i+=10)
		printf("%d %f %f\n", i, loop_test(i, 10000*i), loop_test(10000*i, i));
	return 0;
}
