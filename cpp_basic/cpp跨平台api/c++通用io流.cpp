//编译:
//		g++ -g3 ./c++通用io流.cpp -o x



//c++ io流基础(主要关注: 文件流 & 字符串流)
/*
	*.open()									//只读打开文件
	*.open()									//只写打开文件
	*.open()									//读写打开文件(跟posix API中的open()有点不一样,也可在构造函数中输入,省略掉这一步)

	*.open() 打开文件API的模式标志简介:
		ios::in									读方式打开文件
		ios::out								写方式打开文件
		ios::trunc							如果此文件已存在, 就会打开文件之前把文件长度截断为0
		ios::app								尾部最加方式(在尾部写入)
		ios::ate								文件打开后, 定位到文件尾
		ios::binary							二进制方式(默认是文本方式)

	*.is_open()								// ifstream/ofstream/fstream; 特有的询问API, 检查文件是否已经打开

	*.close()									//关闭文件流

	<<												//输出-标志符
	>>												//输入-标志符
														//标志符只能用在<<, >> 这两个流输出, 输入符号中;
	std::ends ends;						//std::ends  标志符: 插入一个'\0' 空白符到流中, 不进行冲刷操作;
	std::flush flush;					//std::flush 标志符: 立刻冲刷(不添加任何符号)
	std::endl endl;						//std::endl  标志符: 添加一个'\n'换行, 并立刻冲刷

	*.rdbuf()									// ifstream/ofstream/fstream; istringstream/ostringstream/stringstream 都有rdbuf(), 可读出'流buf'中的内容;
														// 原则上不修改'流buf'的内容(想修改为什么不在流输入时修改, 不能等到已输入'流buf'了, 有回头进行修改, 浪费时间!!)

	*.seekg()									//设置pos(第一参数: 偏移量, 第二参数: 设置起始点(省缺为: std::ifstream::cur 当前位))
	*.tellg()									//获取当前pos(不能输入参数, 只返回当前pos)

	std::ifstream::beg				//开始pos
	std::ifstream::cur				//当前pos
	std::ifstream::end				//结束pos
*/



#include <iostream>
//(编译通过, 但无实用意义, 一般通用io 流对象, 也必须在构造函数中, 套入一个'文件流/字符串'流对象实体)
std::istream is();					//定义'通用输入流'的对象
std::ostream os();					//定义'通用输出流'的对象
std::iostream ios();				//定义'通用输出流'的对象

//标准输出流对象(对象本身已存在, 不可修改, 不可重新创建, 标准输出流对象本身也不是全功能的io流对象, 有功能限制的)
//std::cin cin;							//定义'标准输入流'的对象
//std::cout cout;						//定义'标准输出流'的对象
//std::cerr cerr;						//定义'标准错误流'的对象
//std::clog clog;						//定义'标准syslog流'的对象

#include <fstream>					//等价于c 语言中的: open() read() write() close() 等API
std::ifstream ifs;					//定义'文件输入流'的对象
std::ofstream ofs;					//定义'文件输出流'的对象
std::fstream fs;						//定义'文件输入、输出流'的对象

#include <sstream>					//等价于c 语言中的: sprintf() sscanf() 等API
std::istringstream isstr;		//定义'字符串输入流'的对象
std::ostringstream osstr;		//定义'字符串输出流'的对象
std::stringstream sstr;			//定义'字符串输入、输出流'的对象

#include <strstream>
std::ostrstream oss;				//新版字符串流
std::istrstream iss();
std::strstream ss();

//把内存buf, 当作输出流,   请用: std::ostringstream
//把内存buf, 当作输入流,   请用: std::istringstream

//把标准输出流, 当作输出流, 请用: std::cout
//把标准输入流, 当作输入流, 请用: std::cin

//把文件输出流, 当作输出流, 请用: std::ofstream
//把文件输入流, 当作输入流, 请用: std::ifstream
//把文件io流, 当作io流,    请用: std::fstream



//设置流缓冲区or pos 流定位
#include <streambuf>				//支撑流的buf 实现, 一般不用
//#include <stringbuf>			//不存在, 基于streambuf 的派生类
//#include <filebuf>				//不存在, 基于streambuf 的派生类



//测试: 通用io 流对象, 套用fstream 文件流 / sstream 字符串流
void stream_test(void){
	std::istringstream s1("hello");
	std::istream s2(s1.rdbuf());                        //OK: s2 与s1 共享缓冲

	//std::istream s3(std::istringstream("test"));      //错误: 移动构造函数为受保护
	//std::istream s4(s2);                              //错误: 复制构造函数被删除
	std::istringstream s5(std::istringstream("world")); //OK: 导出类调用移动构造函数

	std::cout << s2.rdbuf() << ' ' << s5.rdbuf() << std::endl;
	return;
}



//std::ws 标志符: 去除前面的空格
void ws(void){
	std::istringstream s("     this is a test");
	std::string line;
	std::getline(s >> std::ws, line);
	std::cout << "ws + getline returns: \"" << line << "\"" << std::endl;
	return;
}



//本file 文件流demo: 主要完成一个文件拷贝工作
void file_stream(void){
	char * buffer;
	long size;

	//构造函数打开文件, 可以省掉open(), 不过没办法得知是否打开成功(文件不存在时会崩溃)
	std::ifstream infile("c++通用io流.cpp",std::ifstream::binary);
	//写文件时, 文件不存在会自动创建新的文件, 安全
	std::ofstream outfile("new.txt",std::ofstream::binary);

	//设置file pos 偏移量
	infile.seekg(10,std::ifstream::beg);//起始位偏移10 字节
	infile.seekg(10,std::ifstream::cur);//当前位置偏移10 字节
	infile.seekg(std::ifstream::beg);		//pos 复位()
	infile.seekg(0,std::ifstream::end);	//以std::ifstream::end 为起点, 偏移0 字节
	size=infile.tellg();								//获取当前pos位(pos 偏移量 = file文件大小)
	printf("infile size = %d\n", size);
	infile.seekg(std::ifstream::beg);		//pos 复位()


	//创建buf 容纳文件内容
	buffer = new char [size];

	//读取old 文件的所有内容到buf
	infile.read(buffer,size);

	//把buf 内的所有内容写到新文件中
	outfile.write(buffer,size);
	outfile.flush();

	delete[] buffer;
	outfile.close();
	infile.close();
	return ;
}



int main(void){
	stream_test();
	ws();
	file_stream();
	return 0;
}


