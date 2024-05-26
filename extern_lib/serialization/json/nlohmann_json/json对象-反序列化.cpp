//编译:
//		g++ -std=c++11 -g3 -I /home/tarball/nlohmann_json/include ./json对象-反序列化.cpp -o x 
//		g++ -std=c++11 -g3 -I . ./json对象-反序列化.cpp -o x 

//相当滑稽!!
//	json::parse() 解析一下json 文件, 在c++ 里面就叫'反序列化', 呵呵, c语言里面只是一个文件解析的过程而已, 功能一样, 叫法不同;
//	如果是这样子的话, 为什么要用c++ json 库呢? cjson 多好, 性能还不错;



#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main (void){
	//使用两种方式, 来定义json 字符串
	const char *str1 = "{\"name\":\"hututu\",\"age\":18,\"score\":88.99}";		//(弃用)
	const char *str2 = R"({" name ":" hututu "," age ":18," score ":88.99})";	//使用原生字符串关键字R来避免转移字符

	//反序列化构建json对象, 方式1: 通过"_json"实现反序列化
	json hututu1 = "{\"name\":\"hututu\",\"age\":18,\"score\":88.99}"_json;
	//json hututu1 = str1_json;																								//"_json"不能这样写的, 建议直接使用json::parse(), 别浪费时间;

	//反序列化构建json对象, 方式2: 通过静态函数"parse"实现反序列化
	json hututu2 = json::parse (str2);

	std::cout << "/----------反序列化-----------/" << std::endl;
	std::cout << "hututu1 = " << hututu1 << std::endl;
	std::cout << "hututu2 = " << hututu2 << std::endl;
	std::cout << "/----------反序列化-----------/" << std::endl;

	std::cout << "/----------序列化-----------/" << std::endl;
	//序列化(Serialization): dump(number), number为打印出的空格数
	std::string hututu1_string = hututu1.dump (); //不要空格间隔
	std::string hututu2_string = hututu2.dump (2);//输出2个空格做间隔
	//std::string hututu2_string = hututu2.dump (4);//输出4个空格做间隔
	std::cout << "hututu1_string = " << hututu1_string << std::endl;
	std::cout << "hututu2_string = " << hututu2_string << std::endl;
	std::cout << "/----------序列化-----------/" << std::endl;

	return 0;
}
