//
//* 重点 *
//
/*
	如果对cmath 数学函数的用法有什么疑问, 请查看c++ 中文手册html!!
	c++ 中文手册html, 里面有详细的用法说明:
		file:///home/adan/svn2/http_docu_lib/cpp_cn/reference/zh/cpp/numeric.html
*/



//标准库头文件<bit>, 此头文件是数值库的一部分.(c++20 才有)

//一般情况下, 找不到这个头文件
#include <bit>



//API list
/*
	bit_cast()		将一个类型的对象表示重解释为另一类型的对象表示
	ispow2()			检查一个数是否为二的整数次幂
	ceil2()				寻找不小于给定值的最小的二的整数次幂
	floor2()			寻找不大于给定值的最大的二的整数次幂
	log2p1()			寻找表示给定值所需的最小位数
	rotl()				计算逐位左旋转的结果
	rotr()				计算逐位右旋转的结果
	countl_zero()	从最高位起计量连续的 0 位的数量
	countl_one()	从最高位起计量连续的 1 位的数量
	countr_zero()	从最低位起计量连续的 0 位的数量
	countr_one()	从最低位起计量连续的 1 位的数量
	popcount()		计量无符号整数中为 1 的位的数量

*/
