/*
	demo_sha1.cpp - demo program of
 
	============
	SHA-1 in C++
	============
 
	100% Public Domain.
 
	Original C Code
		-- Steve Reid <steve@edmweb.com>
	Small changes to fit into bglibs
		-- Bruce Guenter <bruce@untroubled.org>
	Translation to simpler C++ Code
		-- Volker Diels-Grabsch <v@njh.eu>
*/

//快捷使用demo

#include "sha1.hpp"
#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

int main(void)
{
	const string input = "abc";

	SHA1 checksum;												//创建sha1 类实体
	checksum.update(input);								//输入: 需要校验的数据
	const string hash = checksum.final();	//输出: sha1 校验码

	cout << "The SHA-1 of \"" << input << "\" is: " << hash << endl;

	return 0;
}
