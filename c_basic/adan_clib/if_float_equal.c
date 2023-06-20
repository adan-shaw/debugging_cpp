//编译:
//	gcc -g3 ./if_float_equal.c ./if_float_equal.h -o x



#include<stdio.h>
#include<stdlib.h>
#include"if_float_equal.h"



//main() 测试函数
int main(void){
	double x = 4.5;
	double y = 4.5;
	float x1 = 4.5;
	float y1 = 4.5;
	long double x2 = 4.5;
	long double y2 = 4.5;



	//double
	if(fabs(x-y)<(1e-8))
		printf("yes, x=y=%lf\n", x);

	if(if_double_equal(x,y))
		printf("yes, x=y=%lf\n", x);


	//float
	if(fabsf(x1-y1)<(1e-8))
		printf("yes, x1=y1=%f\n", x1);

	if(if_float_equal(x1,y1))
		printf("yes, x1=y1=%f\n", x1);


	//long double
	if(fabsl(x2-y2)<(1e-8))
		printf("yes, x2=y2=%llf\n", x2);

	if(if_ldouble_equal(x2,y2))
		printf("yes, x2=y2=%llf\n", x2);


	exit(0);
}
