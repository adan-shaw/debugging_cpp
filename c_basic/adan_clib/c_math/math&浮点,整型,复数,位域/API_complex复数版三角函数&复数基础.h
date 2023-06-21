//111111111111111111111111111111111111111111111111111111111111111111111111
//邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵邵
//作者:    adan shaw
//起始日期:
//完成日期:
//************************************************************************
//修改日志:
//, , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , , ,, , ,



//要附加编译选项: -lm.



//0.复数的变量类型:
/*
	对于一个复数, 你更不能用传统的if(),for(),while()等逻辑判断语句,
	对if(){} 条件进行判别, 否则会出现bug, 时好时坏, 很难调试.

	ps: 复数函数, 也是返回浮点类型!! (肯定!!)
			本质上判别跟浮点差不多, 或者只是运算方法不同而已(猜测)
*/



//复数专用头文件(基本实数有的函数, 在前面+c 前缀, 都可以得到复数版本)
#include <complex.h>



//复数-三角函数类(默认double类型)
double csin(double x);		//求正弦
double ccos(double x);		//求余弦
double ctan(double x);		//求正切
double casin(double x);		//求反正弦
double cacos(double x);		//求反余弦
double catan(double x);		//求反正切
double catan2(double y, double x);//求反正切, 按符号判定象限
double csinh(double x);		//求双曲正弦
double ccosh(double x);		//求双曲余弦
double ctanh(double x);		//求双曲正切
double casinh(double x);	//求反双曲正弦
double cacosh(double x);	//求反双曲余弦
double catanh(double x);	//求反双曲正切





//复数-多类型的三角函数(float, double, long double)
/*
//1.
	//求正弦
	double csin(double x);
	float csinf(float x);
	long double csinl(long double x);

	//求余弦
	double ccos(double x);
	float ccosf(float x);
	long double ccosl(long double x);

	//求正切
	double ctan(double x);
	float ctanf(float x);
	long double ctanl(long double x);





//2.
	//求反正弦
	double casin(double x);
	float casinf(float x);
	long double casinl(long double x);

	//求反余弦
	double cacos(double x);
	float cacosf(float x);
	long double cacosl(long double x);

	//求反正切
	double catan(double x);
	float catanf(float x);
	long double catanl( long double x);

	//求反正切, 按符号判定象限
	double catan2(double y, double x);
	float catan2f(float y, float x);
	long double catan2l(long double y, long double x);





//3.
	//求双曲正弦
	double csinh(double x);
	float csinhf(float x);
	long double csinhl(long double x);

	//求双曲余弦
	double ccosh(double x);
	float ccoshf(float x);
	long double ccoshl(long double x);

	//求双曲正切
	double ctanh(double x);
	float ctanhf(float x);
	long double ctanhl(long double x);





//4.
	//求反双曲正弦
	double casinh(double x);
	float casinhf(float x);
	long double casinhl(long double x);

	//求反双曲余弦
	double cacosh(double x);
	float cacoshf(float x);
	long double cacoshl(long double x);

	//求反双曲正切
	double catanh(double x);
	float catanhf(float x);
	long double catanhl(long double x);

*/
