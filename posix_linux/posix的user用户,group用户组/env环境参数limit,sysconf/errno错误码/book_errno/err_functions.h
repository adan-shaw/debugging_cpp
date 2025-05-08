#ifndef ERROR_FUNCTIONS_H
#define ERROR_FUNCTIONS_H

	#ifdef __GNUC__
		/* This macro stops 'gcc -Wall' complaining that "control reaches end of non-void function" 
			 if we use the following functions to terminate main() or some other non-void function. */
		#define NORETURN __attribute__ ((__noreturn__))
	#else
		#define NORETURN
	#endif

	//1.终止函数(error 模块内部函数)
	//static void terminate(Boolean useExit3);
	//2.打印错误汇总信息(error 模块内部函数-考虑到很多情况)
	//static void outputError(Boolean useErr, int err, Boolean flushStdout,const char *format, va_list ap);

	//3.只打印错误信息
	void errMsg(const char *format, ...);
	//4.打印错误信息并终止程序
	void errExit(const char *format, ...) NORETURN ;
	//5.打印错误信息并终止程序(但不冲刷stdout 标准流)
	void err_exit(const char *format, ...) NORETURN ;
	//6.打印错误信息并终止程序(直接传入errno 值版)
	void errExitEN(int errnum, const char *format, ...) NORETURN ;
	//7.致命的错误打印, 并终止程序(一般是程序员逻辑错误,非系统API出错.)
	void fatal(const char *format, ...) NORETURN ;
	//8.打印'程序的正确用法',并终止程序
	void usageErr(const char *format, ...) NORETURN ;
	//9.打印'命令行输入的usage程序用法错误',并终止程序
	void cmdLineErr(const char *format, ...) NORETURN ;

#endif
