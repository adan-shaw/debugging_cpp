//编译:
//		g++ -std=c++11 -g3 -I /home/tarball/nlohmann_json/include ./json对象值读取和值类型强转.cpp -o x 
//		g++ -std=c++11 -g3 -I . ./json对象值读取和值类型强转.cpp -o x 

#include <iostream>
#include <string>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main(void){
	//构建一个json对象hututu
	json hututu = {
		{"name","hututu"},
		{"age",18},
		{"gender",'m'},
		{"score",88.99},
		{"location",{"sh","bj","sz"}},
	};

	//方式一
	std::string name = hututu["name"].get<std::string>();//获取"name"对应的value值, 并转为string类型
	std::cout<<"name = "<<name<<std::endl;
	std::cout<<"type name = "<<typeid(name).name()<<std::endl;
	std::cout<<"----------------------"<<std::endl;

	//方式二
	std::string location0 = hututu["location"][0].get<std::string>();
	std::string location1 = hututu["location"][1].get<std::string>();
	std::string location2 = hututu["location"].at(2).get<std::string>();
	std::cout<<"location0 = "<<location0<<std::endl;
	std::cout<<"location1 = "<<location1<<std::endl;
	std::cout<<"location2 = "<<location2<<std::endl;

	return 0;
}
