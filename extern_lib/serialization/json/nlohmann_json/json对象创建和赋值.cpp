//编译:
//		g++ -std=c++11 -g3 -I /home/tarball/nlohmann_json/include ./json对象创建和赋值.cpp -o x 
//		g++ -std=c++11 -g3 -I . ./json对象创建和赋值.cpp -o x 

#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main(void){
	//方式一: 赋值构造
	json j1;
	j1["name"]="LeBorn Jame";																//字符串
	j1["number"]=23;																				//整数
	j1["man"]=true;																					//布尔值
	j1["children"]={"LeBorn Jr","Bryce Maximus","Zhuri"};		//数组
	j1["behavior"]["funny"]="gigigigigigi";									//对象中元素值
	j1["wife"]={{"name","Savannah Brinson"},{"man",false}};	//对象

	//方式二: 直接构造
	json j2={
		{"name","LeBorn Jame"},
		{"number",23},
		{"man",true},
		{"children",{"LeBorn Jr","Bryce Maximus","Zhuri"}},
		{"behavior",{{"funny","gigigigigigi"}}},
		{"wife",{{"name","Savannah Brinson"},{"man",false}}}
	};

	std::cout << "j1: "<<j1 << std::endl;//输出json对象值
	std::cout << "j2: "<<j2 << std::endl;//输出json对象值
	return 0;
}
