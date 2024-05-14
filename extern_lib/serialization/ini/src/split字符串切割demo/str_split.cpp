//编译:
//		g++ -g3 ./str_split.cpp -o x


#include <vector>
#include <string>
#include <iostream>

std::vector < std::string > split (const std::string & str, char delimiter)
{
	std::vector < std::string > tokens;
	size_t start = 0;
	size_t end = str.find (delimiter);

	while (end != std::string::npos)
	{
		tokens.push_back (str.substr (start, end - start));
		start = end + 1;
		end = str.find (delimiter, start);
	}

	tokens.push_back (str.substr (start));// 添加最后一个片段
	return tokens;
}



//问题: 切割后, 如果有空格, 退格, 则没办法处理(只能在ini 文件中, 消除所有空格, 退格)
int main (void)
{
	std::string str = "one,two,three";
	char delimiter = ',';//分隔符
	std::vector < std::string > tokens = split (str, delimiter);//向量容器装载切割后的字符串

	// 输出切割后的字符串
	for (const std::string & token:tokens)
	{
		std::cout << token << std::endl;
	}

	return 0;
}
