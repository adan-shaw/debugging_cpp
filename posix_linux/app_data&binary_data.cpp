//编译:
//		g++ ./app_data\&binary_data.cpp -g3 -o x 


//file 存放应用数据:
/*
	1.概述:
		应用数据可以存放进file 里面, 但要用二进制读写;

		但一般情况下, 一个文件存放一种'类/结构体'是默认规则!!
		如果你在一个文件中, 存放2 个以上的应用数据, 就需要有分割符号;
		但二进制数据, 是没有换行, 没有字符串结束符号的, 也就是没有分割符号;
		所以, 最简单的策略是:
			一个文件, 一种'类/结构体'

		而且你必须保证, 每个'类/结构体'的长度都一致;
		不能出现'深拷贝差异', 例如: 
			类里面有根指针, 大部分元素, 都初始化了这根指针, 
			但是有一个类没有实化这根指针, 
			这就会出现'深拷贝差异',
			这也会导致'类/结构体'数据存放到文件时, 发生致命错误;(数据边界不清晰, 数据不能对齐)

		file 存放应用数据在同一个系统下是安全的, 但如果要将应用数据用作'跨平台跨物理机'传递,
		应当将应用数据文本序列化, json, XML, 就是文本序列化工具库;


	2.系统兼容性更新:
		新版的linux操作系统, man 3 fopen()中,已剔除了t(text文本模式), b(二进制模式);
		只剩下: r w a + 四种自由组合.也就是:
			新版linux 系统已经不区分文件类型了, 
			你写入应用数据, 它就是应用数据存放文件, 
			你写入文本数据, 它就是文本数据文件.
		所以, t b选项是用来兼容旧系统的.


	3.应用数据是不允许手动修改的, 因为是二进制数据, 手动打开基本上是乱码, 最好别手动修改;
		(不过是文本的话, 手动打开还是可以看到字符的, 只是变量数据变成了乱码而已)

		跟文本数据不一样, 应用数据存放在文本里面, 一般被破坏, 修复的可能性很低.
		如果你保存了文本数据, 那么可以人工恢复,改一下文本就行.
		所以配置数据为什么要做成文本, 存放到文件里面, 是有一定道理的.


	4.这个readv() 读取, 其实很适合读取应用数据!!
		相信writev() 也很适合写入应用数据.

		因为struct iovec iov[3]; 规定了:
			有多少个数据写入,
			每个数据多长,
		这方面的需求, 与'应用数据file'写入非常符合.
*/


#include <iostream>
#include <stdio.h>
#include <string.h>



//应用数据'存放目标结构体'
typedef struct linux_data_file_test{
	int a;
	int b;
	double c;
	float d;
	void* p;
	char f;
	char* g;
	char h[64];
}test_t;


//应用数据'类'
typedef class linux_data_file_test2{
public:
	linux_data_file_test2(){};
	~linux_data_file_test2(){};
	int a;
	int b;
	double c;
}test_t2;



//1.测试将应用数据'存放目标结构体', 写入文件中
void std_stream_write(void){
	int tmp;
	test_t x;
	test_t2 x2;
	FILE *fp;

	//初始化结构体
	x.a = 999;
	x.b = 555555;
	x.c = 123.123;
	x.d = 987.6987;
	x.p = &x.h;
	x.f = 'A';
	x.g = x.h;
	strncpy(x.h,"stderr fwrite() 测试\n",64);

	//初始化类
	x2.a = 888;
	x2.b = 555;
	x2.c = 777.444;

	//fp = fopen("./test_data", "w");		//以默认模式-自动匹配,打开文件,写入应用数据
	fp = fopen("./test_data", "wb");		//以binary模式打开, 写入应用数据
	//fp = fopen("./test_data", "wt");	//以text 模式打开, 写入应用数据
	if(fp == NULL){
		perror("fopen()");
		return;
	}

	fwrite(&x,sizeof(char),sizeof(x),fp);
	fwrite(&x2,sizeof(char),sizeof(x2),fp);
	fflush(fp);

std_stream_write_failed:
	fclose(fp);
	return;
}



//2.测试从文件中, 读取出应用数据'存放目标结构体'
void std_stream_read(void){
	int tmp;
	test_t x;
	test_t2 x2;
	FILE *fp;



	//fp = fopen("./test_data", "r");		//以默认模式-自动匹配,打开文件,读取应用数据
	fp = fopen("./test_data", "rb");		//以binary模式打开, 读取应用数据
	//fp = fopen("./test_data", "rt");	//以text 模式打开, 读取应用数据
	if(fp == NULL){
		perror("fopen()");
		return;
	}


	fread(&x,sizeof(char),sizeof(x),fp);
	printf("x.a = %d\n",x.a);
	printf("x.b = %d\n",x.b);
	printf("x.c = %lf\n",x.c);
	printf("x.d = %f\n",x.d);
	printf("x.p = %s\n",x.p);
	printf("x.f = %c\n",x.f);
	printf("x.g = %s\n",x.g);
	printf("x.h = %s\n",x.h);

	fread(&x2,sizeof(char),sizeof(x2),fp);
	printf("x2.a = %d\n",x2.a);
	printf("x2.b = %d\n",x2.b);
	printf("x2.c = %lf\n",x2.c);

std_stream_read_failed:
	fclose(fp);
	return;
}



int main(void){
	std_stream_write();
	std_stream_read();
	return 0;
}
