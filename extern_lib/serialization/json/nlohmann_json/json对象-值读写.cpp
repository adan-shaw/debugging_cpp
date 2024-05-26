//编译:
//		g++ -std=c++11 -g3 -I /home/tarball/nlohmann_json/include ./json对象-值读写.cpp -o x 
//		g++ -std=c++11 -g3 -I . ./json对象-值读写.cpp -o x 

#include <iostream>
#include <string>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main(void){
	//json对象hututu (复杂元素, 复杂内嵌)
	json hututu = {
		{"name","hututu"},
		{"age",18},
		{"gender",'m'},
		{"score",88.99},
		{"location",{"sh","bj","sz"}}
	};

	//json对象eas (简单元素, 没有内嵌)
	json eas = {
		"abc",
		"bbb",
	};



	//值读取: 方式一
	std::string name = hututu["name"].get<std::string>();//获取"name"对应的value值, 并转为string类型
	std::cout << "name = " << name << std::endl;
	std::cout << "type name = " << typeid(name).name() << std::endl;
	std::cout << "----------------------" << std::endl;

	//值读取: 方式二
	std::string location0 = hututu["location"][0].get<std::string>();
	std::string location1 = hututu["location"][1].get<std::string>();
	std::string location2 = hututu["location"].at(2).get<std::string>();
	std::cout << "location0 = " << location0 << std::endl;
	std::cout << "location1 = " << location1 << std::endl;
	std::cout << "location2 = " << location2 << std::endl;



	//值写入: 方式一
	hututu["GF"][0] = "Miley";
	hututu["GF"][1] = "Taylor";
	hututu["GF"][2] = "Jan";

	//值写入: 方式二(像stl 顺序容器一样操作, push, pop, 但不支持复杂内嵌的容器, 简单有规律的元素内嵌还是支持的, 详情请看: ./json对象-容器化操作元素.cpp)
	eas.push_back("this is a smart guy");//添加元素
	//eas.push_back({"toy","debian linux"});
	eas.emplace_back("and his handsome");//C++11新方式添加元素, 减少申请内存
	//eas.emplace_back({"friend","robot No.1"});



	//打印整个json 容器
	std::cout << hututu << std::endl;
	std::cout << eas << std::endl;
	return 0;
}
