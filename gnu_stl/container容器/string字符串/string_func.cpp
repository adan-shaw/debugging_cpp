//编译:
//		g++ -std=c++11 -g3 ./string.cpp -o x



#include <string>
#include <iostream>



//
//整个字符串, 把子串old_str, 替换为new_str;
//
//replace_all_again(): 替换一次之后, 继续从0开始再匹配, 如果替换后的字符串, 与后续的字符串, 偶然触发再次匹配, 则会触发再次替换(会导致重复替换)
std::string& replace_all_again(std::string& str, const std::string& old_str, const std::string& new_str){
	std::string::size_type pos(0);
	while((pos=str.find(old_str))!=std::string::npos)
		str.replace(pos,old_str.length(),new_str);
	return str;
}

//replace_all_once(): 替换一次之后, 向后移动n 个字符(被替换的字符串长度), 然后再继续匹配字符串(不会导致重复替换)
std::string& replace_all_once(std::string& str, const std::string& old_str, const std::string& new_str){
	std::string::size_type pos(0);
	while((pos=str.find(old_str,pos))!=std::string::npos)
		str.replace(pos,old_str.length(),new_str);
	return str;
}



//
//整个字符串, 统计子串old_str出现的次数
//
unsigned int substr_count(std::string& str, const std::string& old_str){
	std::string::size_type pos(0);
	unsigned int count = 0;
	while((pos=str.find(old_str,pos))!=std::string::npos){
		pos+=old_str.length();
		count++;
	}
	return count;
}



//测试函数
int main(void){
	std::string s="12212";
	std::cout << replace_all_again(s,"12","21") << std::endl;
	s="12212";
	std::cout << replace_all_once(s,"12","21") << std::endl;
	std::cout << substr_count(s,"21") << std::endl;
	return 0;
}
