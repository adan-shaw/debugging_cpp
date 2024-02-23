//编译:
//		g++ -g3 ./substr根据长度进行切割.cpp -o x

//做字符串操作, 还是c++ 好做, c 语言实现字符串操作函数, 较为复杂, 麻烦;

#include<iostream>
#include<string>



int main(void){
	std::string s = "sfsa";
	std::string a = s.substr(0,3);
	std::string b = s.substr();
	std::string c = s.substr(2,3);
	std::cout << a << std::endl;
	std::cout << b << std::endl;
	std::cout << c << std::endl;
	return 0;
}
