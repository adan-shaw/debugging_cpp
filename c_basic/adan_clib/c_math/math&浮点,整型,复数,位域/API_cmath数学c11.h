//
//* 重点 *
//
/*
	如果对cmath 数学函数的用法有什么疑问, 请查看c++ 中文手册html!!
	c++ 中文手册html, 里面有详细的用法说明:
		file:///home/adan/svn2/http_docu_lib/cpp_cn/reference/zh/cpp/numeric.html
*/



//
//0.命名规律:
//
/*
	标准库float,double,long double 浮点-实数/复数-数学函数命名规则如下:
		实数-浮点函数:
		float       变体 XXXf --> 函数名+字母'f'(float)
		double      变体 XXX  --> 不变
		long double 变体 XXXl --> 函数名+字母'l'(long)


		复数-浮点函数:(函数名全部都要+c, +c前缀表示复数函数)
		float       变体 cXXXf
		double      变体 cXXX
		long double 变体 cXXXl


	cmath的默认类型: double;
	cmath的变体版本: float, long double
*/





//
//1.定义于头文件 <cstdlib>
//
/*
	abs(int)labsllabs(C++11)					计算整数值的绝对值( |x| )
	div(int)ldivlldiv(C++11)					计算整数除法的商和余数
*/





//
//2.定义于头文件 <cinttypes>
//
/*
	abs(std::intmax_t)imaxabs(C++11)	计算整数值的绝对值( |x| )
	div(std::intmax_t)imaxdiv(C++11)	计算整数除法的商和余数
*/





//
//3.定义于头文件 <cmath>
//
/*
3.1 基础运算
	abs(float)
	fabs
	fabsf
	fabsl(C++11)					浮点值的绝对值(|x|)

	fmod
	fmodf
	fmodl(C++11)					浮点除法运算的余数

	remainder
	remainderf
	remainderl(C++11)			除法运算的有符号余数

	remquo
	remquof
	remquol(C++11)				除法运算的有符号余数和最后三个二进制位

	fma
	fmaf
	fmal(C++11)						混合的乘加运算

	fmax
	fmaxf
	fmaxl(C++11)					两个浮点值的较大者

	fmin
	fminf
	fminl(C++11)					两个浮点值的较小者

	fdim
	fdimf
	fdiml(C++11)					两个浮点值的正数差(max(0, x-y))

	nan
	nanf
	nanl(C++11)						非数(NaN)



3.2 指数函数
	exp
	expf
	expl(C++11)						返回 e 的给定次幂(ex)

	exp2
	exp2f
	exp2l(C++11)					返回 2 的给定次幂(2x)

	expm1
	expm1f
	expm1l(C++11)					返回 e 的给定次幂减一(ex-1)

	log
	logf
	logl(C++11)						计算自然(以 e 为底)对数(ln(x))

	log10
	log10f
	log10l(C++11)					计算常用(以 10 为底)对数(log10(x))

	log2
	log2f
	log2l(C++11)					给定数值的以 2 为底的对数(log2(x))

	log1p
	log1pf
	log1pl(C++11)					1 加上给定数值的自然(以 e 为底)对数(ln(1+x))



3.3 幂函数
	pow
	powf
	powl(C++11)						求某数的给定次幂(xy)

	sqrt
	sqrtf
	sqrtl(C++11)					计算平方根(√x)

	cbrt
	cbrtf
	cbrtl(C++11)					计算立方根(3√x)

	hypot
	hypotf
	hypotl(C++11)					计算两个给定数的平方和的平方根(√x2+y2)



3.4 三角函数
	sin
	sinf
	sinl(C++11)						计算正弦(sin(x))
	cos
	cosf
	cosl(C++11)						计算余弦(cos(x))
	tan
	tanf
	tanl(C++11)						计算正切(tan(x))

	asin
	asinf
	asinl
	(C++11)								计算反正弦(arcsin(x))
	acos
	acosf
	acosl
	(C++11)								计算反余弦(arccos(x))
	atan
	atanf
	atanl
	(C++11)								计算反正切(arctan(x))
	atan2
	atan2f
	atan2l(C++11)					反正切, 用符号确定象限



3.5 双曲函数
	sinh
	sinhf
	sinhl(C++11)					计算双曲正弦(sh(x))
	cosh
	coshf
	coshl(C++11)					计算双曲余弦(ch(x))
	tanh
	tanhf
	tanhl(C++11)					双曲正切

	asinh
	asinhf
	asinhl(C++11)					计算反双曲正弦(arsinh(x))
	acosh
	acoshf
	acoshl(C++11)					计算反双曲余弦(arcosh(x))
	atanh
	atanhf
	atanhl(C++11)					计算反双曲正切(artanh(x))



3.6 误差与伽马函数
	erf
	erff
	erfl(C++11)						误差函数

	erfc
	erfcf
	erfcl(C++11)					补误差函数

	tgamma
	tgammaf
	tgammal(C++11)				gamma 函数

	lgamma
	lgammaf
	lgammal(C++11)				gamma 函数的自然对数



3.7 最接近整数的浮点运算
	ceil
	ceilf
	ceill(C++11)					不小于给定值的最接近整数值

	floor
	floorf
	floorl(C++11)					不大于给定值的最接近整数

	trunc
	truncf
	truncl(C++11)					绝对值不大于给定值的最接近整数

	round
	roundf
	roundl
	lround
	lroundf
	lroundl
	llround
	llroundf
	llroundl(C++11)				最接近整数, 中间情况下向远离零舍入

	nearbyint
	nearbyintf
	nearbyintl(C++11)			使用当前舍入模式的最接近整数

	rint
	rintf
	rintl
	lrint
	lrintf
	lrintl
	llrint
	llrintf
	llrintl(C++11)				使用当前舍入模式的最接近整数, 若结果有别则有异常.



3.8 浮点操作函数
	frexp
	frexpf
	frexpl(C++11)					将数分解为有效数字和 2 的幂次

	ldexp
	ldexpf
	ldexpl(C++11)					将数乘以 2 的幂次

	modf
	modff
	modfl(C++11)					分解数为整数和小数部分

	scalbn
	scalbnf
	scalbnl
	scalbln
	scalblnf
	scalblnl(C++11)				将数乘以 FLT_RADIX 的幂次

	ilogb
	ilogbf
	ilogbl(C++11)					提取数的指数

	logb
	logbf
	logbl(C++11)					提取数的指数

	nextafter
	nextafterf
	nextafterl
	nexttoward
	nexttowardf
	nexttowardl(C++11)		趋向给定值的下个可表示浮点值

	copysign
	copysignf
	copysignl(C++11)			复制浮点值的符号




3.9 分类与比较
	fpclassify(C++11)			归类给定的浮点值
	isfinite(C++11)				检查给定数是否拥有有限值
	isinf(C++11)					检查给定数是否为无限
	isnan(C++11)					检查给定的数是否 NaN
	isnormal(C++11)				检查给定数是否正规
	signbit(C++11)				检查给定数是否为负
	isgreater(C++11)			检查第一个浮点实参是否大于第二个
	isgreaterequal(C++11)	检查第一个浮点实参是否大于或等于第二个
	isless(C++11)					检查第一个浮点实参是否小于第二个
	islessequal(C++11)		检查第一个浮点实参是否小于或等于第二个
	islessgreater(C++11)	检查第一个浮点实参是否小于或大于第二个
	isunordered(C++11)		检查两个浮点值是否无顺序

*/
