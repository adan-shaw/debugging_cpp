//编译:
//		g++ -std=c++11 -g3 -I /home/tarball/nlohmann_json/include ./json对象-容器化操作元素.cpp -o x 
//		g++ -std=c++11 -g3 -I . ./json对象-容器化操作元素.cpp -o x 

//nlohmann_json 容器化操作元素, 支持简单json容器, 复杂内嵌json容器, 混杂内嵌json容器;
//但对于复杂内嵌json容器, 混杂内嵌json容器, 则需要注意数据入栈顺序, 要先易后难, 否则, 你先把复杂数据推进去, 再推一个简单数据, 会报错, 说: 数据类型错误!!
//详情自己看: ./json对象-值读写.cpp



#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;



//json 对象, 简单顺序容器, 无内嵌
void json_easy(void){
	int size;

	//构建一个json对象arr_easy
	json arr_easy = {"cat","dog"};
	arr_easy.push_back("pig");						//添加元素
	arr_easy.emplace_back("duck");				//C++11新方式添加元素, 减少申请内存

	//使用is_array()函数判断对象类型; 使用empty()函数判断数量是否为空
	if(arr_easy.is_array() && !arr_easy.empty()){
		size = arr_easy.size();
		std::cout << "arr_easy size: "<< size << std::endl;
		std::string arr_easy_last = arr_easy.at(size-1).get<std::string>();
		std::cout << "arr_easy[size-1]: "<< arr_easy_last << std::endl;
	}

	std::cout << "arr_easy: "<< arr_easy << std::endl;
	return;
}



//json 对象, 复杂顺序容器, 有内嵌, 复杂混合
void json_complicated(void){
	json arr_complicated = {{"kind","dog"},{"height",50}};
	arr_complicated.push_back({"color","red"});
	arr_complicated.erase("kind");				//删除键值对
	arr_complicated["height"] = 99;				//通过key修改value值
	std::cout << "arr_complicated: " << arr_complicated << std::endl;


	//判断是否含有某个键值方式一: 通过contains()函数判断是否包含某个key
	if(arr_complicated.contains("height")){
		auto height = arr_complicated["height"].get<double>();
		std::cout << "方式一:height: "<<height << std::endl;
	}
	//判断是否含有某个键值方式二: 通过count函数计算某一个键的数量
	auto size = arr_complicated.count("height");
	if(size > 0){
		std::cout << "方式二:存在height键值" << std::endl;
	}
	//判断是否含有某个键值方式三: 通过find函数查找某个键的迭代器
	auto iter = arr_complicated.find("height");
	if(iter != arr_complicated.end()){
		std::cout << "方式三:存在height键值" << std::endl;
	}


	std::cout << "遍历输出键值方式1:" << std::endl;
	for(auto item:arr_complicated.items()){
		std::cout << item.key() << " " << item.value() << std::endl;
	}
	std::cout << "遍历输出键值方式2:" << std::endl;
	for(auto iter = arr_complicated.begin() ; iter != arr_complicated.end() ; ++iter){
		std::cout << iter.key() << " " << iter.value() << std::endl;
	}

	return;
}



int main(void){
	json_easy();
	json_complicated();
	return 0;
}



