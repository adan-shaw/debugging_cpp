//编译:
//		g++ -g3 ./substr根据长度进行切割.cpp -o x



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
