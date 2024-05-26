//编译(使用最低c++ 标准编译, 提高兼容性):
//		g++ -std=c++11 -g3 -I /home/tarball/nlohmann_json/include ./first_test.cpp -o x 
//		g++ -std=c++11 -g3 -I . ./first_test.cpp -o x 

#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main(void){
	//构建json对象
	json config_json = json::parse(R"({"happy": true, "pi": 3.141})");
	//nlohmann::json config_json = nlohmann::json::parse(R"({"happy": true, "pi": 3.141})");

	//输出json对象值
	std::cout << config_json << std::endl;
	return 0;
}
