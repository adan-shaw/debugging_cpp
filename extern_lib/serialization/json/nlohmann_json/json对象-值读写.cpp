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

	//值写入: 方式二(像stl 顺序容器一样操作, push_back(), 但不支持pop_back(), 只进不出;
	//						 push_back() 复杂数据时, 需要注意入栈顺序, 要先易后难(一般先推简单数据, 再推复杂数据);
	//						 不是不支持复杂数据, 但数据入栈顺序不对, 随时给你跳出报错: 
	//						 terminate called after throwing an instance of 'nlohmann::json_abi_v3_11_3::detail::type_error' )
	eas.push_back("this is a smart guy");				//添加元素
	eas.push_back({"toy","debian linux"});
	eas.emplace_back("and his handsome");				//C++11新方式添加元素, 减少申请内存
	//eas.emplace_back({"friend","robot No.1"});
	eas.push_back({"location",{"sh","bj","sz"}});
	eas.push_back({"location",{"sh","bj","sz"}});
	eas.push_back({"location",{"sh","bj","sz"}});

	hututu.push_back({"toy","debian linux"});
	//hututu.pop_back();												//没有pop_back() 函数? 源码明明有的, 晕了
	//hututu.push_front("this is a smart guy");	//没有push_front() 函数, 只有push_back()

	hututu.push_back("and his handsome");			//先推复杂数据, 再推简单数据, 导致报错(编译没问题, 运行就报错!!) demo



	//打印整个json 容器
	std::cout << hututu << std::endl;
	std::cout << eas << std::endl;
	return 0;
}
