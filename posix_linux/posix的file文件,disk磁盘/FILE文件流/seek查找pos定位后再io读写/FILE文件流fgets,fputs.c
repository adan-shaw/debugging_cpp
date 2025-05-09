//编译:
//		gcc -g3 ./FILE文件流fgets,fputs.c -o x 

//c 语言FILE流text数据-序列化的实现:
/*
	1.gets()
		char *gets(char *s); //gets()是个不安全的函数, 应该坚决不用.

		gets()每遇到换行符就会返回, 即使是一开始就遇到一个换行符, 返回后会将换行符变为'\0'.
		由于gets()无法知道字符串的大小, 遇到'\n 换行符'或'EOF文件结束符'才会结束, 因此容易造成缓存溢出的安全性问题.

		C11标准已经不让用gets()了, 可以用fgets()来代替.
		比如对于同一个字符串str[50], fgets()的用法是:
			fgets(str, 50, stdin);
		遇到'\n'换行符, EOF文件结束符, 或者缓冲区溢出, 都会截断并结束fgets(), 返回读到的字符串.
		但fgets() 没法判断'\0'字符串结束符号, fgets() 是以'\n'作为结束标准的.



	2.fgets()
		char *fgets(char *buf, int bufsize, FILE *stream);
		遇到'\n'换行符, EOF文件结束符, 或者缓冲区溢出, 都会截断并结束fgets(), 返回读到的字符串.

		如果是'\n换行符'终结fgets(), 则'\n'会被保存在缓冲区中, 只是后面再加个'\0'.
		对于fgets()来说,'\n'是一个特别的字符,
		而'\0'并无任何特别之处, 读到'\0'就当作普通字符读入.

		如果文件中存在'\0'字符(或者0x00字节),
		fgets()就无法判断缓冲区中的'\0'究竟是'从文件读上来的字符', 还是由fgets() 自动添加的结束符.

		所以fgets()只适合读'文本文件', 而不适合读'二进制文件',
		因为'文本文件'中的所有字符都应该是可见字符, 不能有'\0'.
		但'二进制文件'并不理会'\0', '二进制文件'中没有特殊字符.



	3.puts()与fputs()
		int puts(const char *str);
		int fputs(const char *str, FILE *pfs);//注意区别于fputc(c, fin).

		fputs()向指定的文件写入一个字符串, 
		puts()向'标准输出'写入一个字符串.
		两者都不会在结尾自动插入'\0',
		所以, 你写入的字符串必须有结束符号,否则程序可能会溢出.

		fputs()并不关心的字符串中的'\n'字符, 字符串中可以有'\n'也可以没有'\n'.
		puts()将字符串str写到标准输出, 然后自动写一个'\n'到标准输出.



	4.阻塞问题:
		gets(),fgets(),puts(),fputs()操作都可能会触发阻塞, 出现阻塞的主要原因是: 
			* 其它进程对目标文件使用了文件锁;
			* 其它进程对目标文件正在进行写操作, 或者独占访问, 其他进程只能等file 资源;
*/



#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


#define BUF_MAX (256)



//1.fgets() test
void fgets_test(void){
	FILE *pfs;
	char buf[BUF_MAX];

	pfs = fopen("./FILE:fgets&fputs.c", "r");
	if(pfs == NULL){
		perror("fopen()");
		return;
	}

	printf("ferror(pfs)=%d\n",ferror(pfs));


	//fgets()每次读取一个字符串,或一行字符[fgets()基本不会出错],
	//不会返回实际读取到的字符串长度,自己使用strnlen() 检查即可;
	fgets(buf,BUF_MAX,pfs);
	printf("the first line: %s\n",buf);

	//重置到SEEK_SET(文件头)
	rewind(pfs);

	//遍历逐行读取文件:
	while(!feof(pfs) && !ferror(pfs)){	//未到达文件尾 && 没错流错误
		fgets(buf,BUF_MAX,pfs);
		printf("%s\n",buf);
	}

fgets_test_failed:
	fclose(pfs);
	return;
}



//2.fputs() test
void fputs_test(void){
	FILE *pfs;
	int tmp;
	char buf[64];

	//pfs = fopen("./test_data", "w");	//以text 模式打开, 重写模式
	pfs = fopen("./test_data", "a");		//以text 模式打开, 追加模式
	if(pfs == NULL){
		perror("fopen()");
		return;
	}

	snprintf(buf,64,"%s%d%s\n","love",999,"you");

	tmp = fputs(buf,pfs);
	if(tmp == EOF || ferror(pfs)){
		perror("fputs()");
		goto fputs_test_failed;
	}
	fflush(pfs);

fputs_test_failed:
	fclose(pfs);
	return;
}



int main(void){
	fgets_test();
	fputs_test();
	return 0;
}




