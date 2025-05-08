//编译:
//		gcc -o x ./std标准FILE流.c -g3


//1.'标准FILE*文件流'简述:
/*
	0.stdin,stdout,stderr是进程创建的时候,就会自动创建的FILE*;
		errno 是每个线程都自带一个, 而stdin,stdout,stderr 都还是进程共享的;

	1.'标准FILE*文件流'和普通文件流一样,可读可写,但读写不一定有效:
			stdin 可读不可写,
			stdout 可写不可读,
			stderr 可写不可读,
			stdin,stdout,stderr都不可以seek.

	2.'标准FILE*文件流'设置异步模式, 不知道是否有效, 可否提高一点点性能?
		'标准FILE*文件流'是内存FILE*, 速度这么快, 一般不需要异步.

	3.'标准FILE*文件流'的长度&pos:
		无论读取stdin,stdout,stderr文件流的长度, 都只会返回-1, 操作失败!
		(亲测试: fscanf, fgets, fseek, ftell 全部都是返回1)

	4.不要对'标准FILE*文件流'拷贝字符串, 无效的, 还是要用流打印函数才有用的;
		sprintf(stderr,"%s","xxxx");	//无效, 不可直接拷贝内存, 不会输出的
		fprintf(stderr,"%s","xxxx");	//正确, 会输出字符
		fwrite()/fread() 等FILE*文件流API, 都可以用;
		哪怕你fclose() 标准输出都行(不过关闭后, 很容易报错)

		fread()是阻塞的,fwrite()也是阻塞的,都是'同步'操作.
		用fread()读取stderr/stdout,你的程序就会陷入阻塞.
		用fwrite()写入stdin,你的程序就会陷入阻塞.
		因为你没办法从stderr,stdout里面输入字符,所以就会一直阻塞等待字符串.

	5.标准FILE*文件流, '冲刷读写法'/'截断读写法':
		用截断和冲刷, 效果都是一样的, example:
		*1.截断法(std out 输出截断)
			std::cout<<"test"<<std::endl;

		*2.冲刷法(std out 输出冲刷)
			std::cout<<"test";
			fflush(stdout);

			//默认冲刷'当前进程'的所有流:stdin,stdout,自己打开的流,等.
			fflush();

		'FILE*文件流'并没有安全截断函数, 因为本身'FILE*文件流'就是严格按照数据size 读出和写入的,
		所以'FILE*文件流'不存在缓冲区溢出的风险, 
		但'FILE*文件流'需要flush() 冲刷数据, 防止缓冲区中的数据因为断电丢失, 或者程序崩溃而丢失;
*/


//无论读取stdin,stdout,stderr文件流的长度, 都只会返回-1, 操作失败!
/*
	printf("ftell(stderr)=%d\n",ftell(stderr));
	printf("ftell(stderr)=%d\n",ftell(stderr));
*/



#include <stdio.h>
#include <string.h>



//'标准FILE*文件流'demo:
void std_stream_demo(void){
	char buf[256];
	int tmp;



	//stderr fwrite() 测试
	strncpy(buf,"stderr fwrite() 测试\n",256);
	tmp = fwrite(buf,sizeof(char),strnlen(buf,256)+1,stderr);
	if(tmp <= 0 || ferror(stderr)){
		perror("fwrite()");
		return;
	}

	//stderr fprintf()
	fprintf(stderr,"%s\n%s\n","fprintf() to stderr1","fprintf() to stderr2");



	//fread() 阻塞读取5字节测试
	memset(buf,'\0',256);
	tmp = fread(buf,sizeof(char),5,stdin);
	if(tmp <= 0 || ferror(stdin)){
		perror("fread()");
		return;
	}

	//(错误测试, stdin 是不能写入的!!只能读取by scanf()系列函数!!)
	tmp = fwrite(buf,sizeof(char),strlen(buf)+1,stdin);
	if(tmp <= 0 || ferror(stdin)){
		perror("fwrite()");
		return;
	}


	//3: 标准FILE*文件流的长度:
	printf("ftell(stderr)=%d\n",ftell(stderr));
	perror("ftell(stderr)");

	printf("fseek(stdout,10,0)=%d\n",fseek(stdout,10,0));//向后移动10字节
	perror("fseek(stdout,10,0)");
}



int main(void){
	std_stream_demo();
	return 0;
}
