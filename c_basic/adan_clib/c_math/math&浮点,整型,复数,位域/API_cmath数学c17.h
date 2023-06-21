//
//* 重点 *
//
/*
	如果对cmath 数学函数的用法有什么疑问, 请查看c++ 中文手册html!!
	c++ 中文手册html, 里面有详细的用法说明:
		file:///home/adan/svn2/http_docu_lib/cpp_cn/reference/zh/cpp/numeric.html
*/



//
//0.简介
//
/*
	数学特殊函数库原是库 TR1 ISO/IEC TR 19768:2007 的一部分,
	后作为独立 ISO 标准, ISO/IEC 29124:2010 出版, 
	最终从 C++17 起并入 ISO C++
*/





//
//1.命名规律:
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
//2.c++17 新增的cmath函数API详表:
//
/*

assoc_laguerre(C++17)
assoc_laguerref(C++17)
assoc_laguerrel(C++17)	连带拉盖尔多项式

assoc_legendre(C++17)
assoc_legendref(C++17)
assoc_legendrel(C++17)	连带勒让德多项式

beta
betaf
betal(C++17)	beta 函数

comp_ellint_1
comp_ellint_1f
comp_ellint_1l(C++17)		第一类(完全)椭圆积分

comp_ellint_2
comp_ellint_2f
comp_ellint_2l(C++17)		第二类(完全)椭圆积分

comp_ellint_3
comp_ellint_3f
comp_ellint_3l(C++17)		第三类(完全)椭圆积分

cyl_bessel_i
cyl_bessel_if
cyl_bessel_il(C++17)		规则变形圆柱贝塞尔函数

cyl_bessel_j
cyl_bessel_jf
cyl_bessel_jl(C++17)		(第一类)圆柱贝塞尔函数

cyl_bessel_k
cyl_bessel_kf
cyl_bessel_kl(C++17)		不规则变形圆柱贝塞尔函数

cyl_neumann
cyl_neumannf
cyl_neumannl(C++17)			圆柱诺依曼函数

ellint_1
ellint_1f
ellint_1l(C++17)				第一类(不完全)椭圆积分

ellint_2
ellint_2f
ellint_2l(C++17)				第二类(不完全)椭圆积分

ellint_3
ellint_3f
ellint_3l(C++17)				第三类(不完全)椭圆积分

expint
expintf
expintl(C++17)					指数积分

hermite
hermitef
hermitel(C++17)					埃尔米特多项式

legendre
legendref
legendrel(C++17)				勒让德多项式

laguerre
laguerref
laguerrel(C++17)				拉盖尔多项式

riemann_zeta
riemann_zetaf
riemann_zetal(C++17)		黎曼 zeta 函数

sph_bessel
sph_besself
sph_bessell(C++17)			(第一类)球面贝塞尔函数

sph_legendre
sph_legendref
sph_legendrel(C++17)		球面连带勒让德函数

sph_neumann
sph_neumannf
sph_neumannl(C++17)			球面诺依曼函数

*/
