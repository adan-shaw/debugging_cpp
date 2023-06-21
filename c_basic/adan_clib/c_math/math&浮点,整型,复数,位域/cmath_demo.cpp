//编译:
//		g++ -lm -g3 ./cmath_demo.cpp -o x 


#include <math.h>
#include <stdio.h>
#include <numeric>



void math_test(void){
	float fTmp;
	double dTmp;
	long double ldTmp;



	//1. pow计算x 的y 次幂
	dTmp = pow(2.0,2.0);
	printf("double pow(2.0,2.0) = %lf\n",dTmp);

	fTmp = powf(2.0,2.0);
	printf("float powf(2.0,2.0) = %f\n",fTmp);

	ldTmp = powl(2.0,2.0);
	printf("long double powl(2.0,2.0) = %llf\n",ldTmp);



	//2.log是对数函数,是幂的逆运算.( C语言只有log10和log(e) )
	//换底公式: loga(b)=loge(b)/log(a)=log10(b)/log10(a)
	//log2(8):
	dTmp = log(8)/log(2);
	printf("log2(8) = log(8)/log(2) = %lf\n",dTmp);

	return;
}





int main(void){
	math_test();

	return 0;
}
