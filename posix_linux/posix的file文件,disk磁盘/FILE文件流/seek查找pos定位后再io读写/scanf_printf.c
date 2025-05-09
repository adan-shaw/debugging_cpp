//编译:
//		gcc -o x ./scanf_printf.c -g3

//格式化输入输出系列'汇总':
/*
	1、scanf()格式化输入系列
		int scanf(const char *format, ...);														//格式化输入 from tty
		int fscanf(FILE *stream, const char *format, ...);						//格式化输入 from 文件流FILE
		int sscanf(const char *str, const char *format, ...);					//格式化输入 from 内存(变量) [弃用], 无截断

		//自定义参数列表版(少用), 功能一样的, 只不过参数输入方式更自由而已;
		int vscanf(const char *format, va_list ap);
		int vsscanf(const char *str, const char *format, va_list ap);
		int vfscanf(FILE *stream, const char *format, va_list ap);

		如果scanf()在开头遇到前次函数留下的空格或换行符, scanf()一律跳过,继续运行;

		如果scanf()在运行过程中, 遇到空格或遇到换行符, scanf()将会停止录入数据, 并在已读入的字符串的最后加上'\0'.



	2、printf()格式化输出系列:
	int printf(const char *format, ...);														//格式化输出 to tty
	int fprintf(FILE *stream, const char *format, ...);							//格式化输出 to 文件流FILE
	int dprintf(int fd, const char *format, ...);										//格式化输出 to fd文件句柄(同步无缓冲, 直写)
	int sprintf(char *str, const char *format, ...);								//格式化输出 to 内存(变量), 无截断版
	int snprintf(char *str, size_t size, const char *format, ...);	//格式化输出 to 内存(变量), 截断版


	//自定义参数列表版(少用)
	int vprintf(const char *format, va_list ap);
	int vfprintf(FILE *stream, const char *format, va_list ap);

	int vdprintf(int fd, const char *format, va_list ap);
	int vsprintf(char *str, const char *format, va_list ap);
	int vsnprintf(char *str, size_t size, const char *format, va_list ap);
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>



//1.格式化-'缓冲区'输入to缓冲区/file文件流-字符串截断式(scanf() 系列)
void scanf_test(void){
	char buf[64];
	int tmp;

	errno = 0;
	tmp = fscanf(stdin,buf);
	if(errno != 0 && tmp <= 0){
		perror("fscanf()");
		return;
	}
	else
		printf("%d=fscanf()\n",tmp);


	memset(buf,'\0',64);
	tmp = sscanf(buf,"%s%d%s","fuck",999,"you");//由于没有截断, 如果"fuck",999,"you"的内存缓冲区大于buf, 就会发生缓冲区溢出
	printf("%d=sscanf()=%s\n",tmp,buf);					//sscanf() 弃用
	return;
}



//2.格式化-'缓冲区'输出to缓冲区-字符串截断式(printf() 系列)
void printf_test(void){
	char buf[64];
	int tmp;

	//文件流无穷无尽, 不用考虑截断问题
	fprintf(stdout,"%s%d%s","fuck",999,"you");

	//直接to 文件(前提是fd 已经打开了文件, 并且具有'可写权限')
	/*
	tmp = dprintf(fd,"%s%d%s","fuck",999,"you");
	if(tmp == -1){
		perror("dprintf()");
		return;
	}
	*/

	memset(buf,'\0',64);
	snprintf(buf,64,"%s%d%s","fuck",999,"you");//常用的截断函数
	printf("%s\n",buf);
	return;
}



int main(void){
	scanf_test();
	printf_test();
	return 0;
}

