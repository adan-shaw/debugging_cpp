//
//* 重点 *
//
/*
	如果对cmath 数学函数的用法有什么疑问, 请查看c++ 中文手册html!!
	c++ 中文手册html, 里面有详细的用法说明:
		file:///home/adan/svn2/http_docu_lib/cpp_cn/reference/zh/cpp/numeric.html
*/



//数值运算,定义于头文件 <numeric>

//一般c11 也有这个文件
#include <numeric>



//1.数值运算
/*
	//c11 的部分
	iota()											用从起始值开始连续递增的值填充一个范围
	accumulate()								对一个范围内的元素求和
	inner_product()							计算两个范围的元素的内积
	adjacent_difference()				计算范围内各相邻元素之间的差
	partial_sum()								计算范围内元素的部分和


	//c17 新增的部分
	transform_reduce()					应用一个函数对象, 然后以乱序规约
	reduce()										类似 std::accumulate, 但不依序执行
	inclusive_scan()						类似 std::partial_sum, 第 i 个和中包含第 i 个输入
	exclusive_scan()						类似 std::partial_sum, 第 i 个和中排除第 i 个输入
	transform_inclusive_scan()	应用一个函数对象, 然后进行包含扫描
	transform_exclusive_scan()	应用一个函数对象, 然后进行排除扫描
*/



//2.因数运算(C++17)
/*
	gcd()	返回二个整数最大公约数的 constexpr 函数模板
	lcm()	返回二个整数最小公倍数的 constexpr 函数模板
*/



//3.插值运算(C++20)
/*
	midpoint()									两个数值或指针间的中点
	lerp()											线性插值函数(定义于头文件<cmath>)
*/
