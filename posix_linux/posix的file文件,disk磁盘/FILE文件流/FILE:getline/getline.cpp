//编译:
//		g++ -o x ./getline.cpp -g3



#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;



#define BUF_MAX (4096)



//测试字符串一行未读完, getline() 和c++ 流可以收到通知.
//也可以清楚流错误.(或者你用c 获取流错误, 应该也能获取相关信息)
void test_str_line_overflow(void){
	const char *filePath = "./getline.cpp";
	char buf[BUF_MAX];
	std::ifstream fin;



	//清空ifstream 的错误标记
	fin.clear(std::ifstream::goodbit);

	//用ifstream流, 打开文件"./getline.cpp"
	fin.open(filePath,std::ifstream::in);
	if(fin.fail()){
		perror("fin.open()");
		return;
	}


	//memset(buf,'\0',BUF_MAX);
	fin.getline(buf,BUF_MAX);	//读取一行, 截断长度为BUF_MAX
	if(fin.fail()){						//fin读取错误(读取错误,或者到达文件尾部,或者读取不够一行,都会触发)
		perror("fin.getline()");
		if(fin.bad())						//io 出错
			std::cout << "I/O error while reading\n" << std::endl;
		if(fin.eof())						//pos游标已经到达EOF 文件尾
			std::cout << "End of file reached successfully\n" << std::endl;
		fin.close();
		return;
	}
	std::cout << buf << std::endl;


	//回到流的开始位置
	fin.seekg(std::ifstream::beg);
	//执行重头再读
	while(!fin.fail()){
		//memset(buf,'\0',BUF_MAX);
		fin.getline(buf,BUF_MAX);
		if(fin.fail()){
			perror("fin.getline()");
			if(fin.bad())					//io 出错
				std::cout << "I/O error while reading\n" << std::endl;
			if(fin.eof())					//pos游标已经到达EOF 文件尾
				std::cout << "End of file reached successfully\n" << std::endl;
			fin.close();
			return;
		}
		std::cout << buf << std::endl;
	}

	fin.close();
	return;
}



int main(void){
	test_str_line_overflow();
	return 0;
}




