//编译:
//		gcc ./getline.c -g3 -o x 



#include <stdio.h>
#include <stdlib.h>
#include <string.h>



//getline()测试函数
void getline_test(void){
	FILE *pfs;
	unsigned long int nRead=0, buf_len=0;
	char *line = NULL;



	//用FILE流, 打开文件
	pfs = fopen("./getline.c", "r");
	if(pfs == NULL){
		perror("fopen()");
		return;
	}

	//读一行
	line = NULL;																				//当*lineptr = NULL, 忽略第二参数buf_len的值
	nRead = getline(&line, &buf_len, pfs);
	printf("buf_len=%d,getline():%s\n",buf_len,line);		//打印malloc 分配的buf_len 长度(每次自动分配120 bit)



	//将整个文件按照'行读'的方式, 逐行读取&&打印
	while((nRead = getline(&line, &buf_len, pfs)) != -1)//重复利用malloc 分配的*lineptr 缓冲区
		printf("buf_len:%d, read len:%d, data:%s\n",buf_len,nRead,line);
																											//如果缓冲区不够用, getline 会自动调用realloc来分配更大的内存块

	//getline *lineptr 缓冲区需要手动释放, 但只需要释放一次;
	free(line);

	fclose(pfs);
	return;
}



int main(void){
	getline_test();
	return 0;
}



//getline() 函数分析(getline()没有fgets()好用):
/*
	1.getline()类似于fgets(), 只识别LF, 不会去除LF,
		遇到'\0'是不会识别的, 以'\n'为行结束标志, 不同的是: 
			getline() 可以自动分配读取缓冲区的内存;

		函数原型如下:
		#include <stdio.h>
		ssize_t getline(char **lineptr, size_t *n, FILE *stream);
		ssize_t getdelim(char **lineptr,size_t *n,int delim,FILE *stream);

		参数分析:
		char **lineptr:
			指向存放该行字符的指针, 如果是NULL, 则有系统帮助malloc(请在使用完成后free释放)
		size_t *n:
			指明**lineptr 缓冲区的实际长度, 如果是由系统malloc的指针, 请填0.
		FILE *stream:
			目标FILE*文件流

		返回值分析:
			如果getline没有读入字符, 将返回false, 可用于判断文件是否结束.

		其中*lineptr指向一个动态分配的内存区域, *n是所分配内存的长度. 
		如果*lineptr是NULL的话, getline函数会自动进行动态内存的分配(忽略*n的大小), 
		所以使用这个函数非常注意的就使用要注意自己进行内存的释放.

		如果*lineptr分配了内存, 但在使用过程中发现所分配的内存不足的话, 
		getline函数会调用realloc函数来重新进行内存的分配, 同时更新*lineptr和*n


	2.getline()与fgets的不同
		getline()函数会自动为读取的字符末尾加一个空字符'\0'.
		fgets(),则不会理会'\0', 而是以'\n'作为结束符号.
		所以你写入的时候不添加'\n',那么fgets()可能会两串字符串起来读取.


	3.getline()与fgets()的性能对比
		打开一个5M的文本, fgets()200ms, getline() 1000ms, fgets()性能更优!!


	4.getline() 在不同编译器中的差异:
		g++:
			getline() 是c++ 全局函数.
		gcc:
			在标准c语言中, getline()函数是不存在的.
			在gcc编译器中, 对标准c库进行了扩展, 加入了一个getline()函数.

		C++中读取一行的getline()函数是不读入换行符的(不读入换行符,即遇到换行即当成一串字符串),
		而C语言中GCC编译器扩展的getline()函数和fgets()都是是读入换行符的.


	5.readline() 为什么没有对称的putline(), 写入一行的操作, 怎么实现[用fputs() 就足够了]?
		c 语言FILE流text数据-序列化写入, 为什么用fputs() 就足够了? why??

		因为每次写入数据, 程序员一般都自己封装好格式, 再写入, 
		所以一般写入数据, '文件数据存放格式'都有序;

		但读取的时候, 由于不同程序员的'文件数据存放格式', 每个人都不一样, 
		导致每次读取数据, '文件数据存放格式'的差异性都比较大, 
		因此读取数据的压力, 远比写入数据的压力大;
*/
