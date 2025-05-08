#include <stdarg.h>
#include <stdio.h>		/* Standard I/O functions */
#include <stdlib.h>		/* Prototypes of commonly used library functions, plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <errno.h>		/* Declares errno and defines error constants */
#include "err_functions.h"



#ifdef __GNUC__
__attribute__ ((__noreturn__))
#endif



typedef enum { FALSE, TRUE } Boolean;



#define BUF_SIZE 4096
#define MAX_ENAME 133


/* ename.c.inc Built on GNU/Linux x86_64 with glibc 2.28 */
static char *ename[] = {
	/*   0 */ "",
	/*   1 */ "EPERM", "ENOENT", "ESRCH", "EINTR", "EIO", "ENXIO",
	/*   7 */ "E2BIG", "ENOEXEC", "EBADF", "ECHILD",
	/*  11 */ "EAGAIN/EWOULDBLOCK", "ENOMEM", "EACCES", "EFAULT",
	/*  15 */ "ENOTBLK", "EBUSY", "EEXIST", "EXDEV", "ENODEV",
	/*  20 */ "ENOTDIR", "EISDIR", "EINVAL", "ENFILE", "EMFILE",
	/*  25 */ "ENOTTY", "ETXTBSY", "EFBIG", "ENOSPC", "ESPIPE",
	/*  30 */ "EROFS", "EMLINK", "EPIPE", "EDOM", "ERANGE",
	/*  35 */ "EDEADLK/EDEADLOCK", "ENAMETOOLONG", "ENOLCK", "ENOSYS",
	/*  39 */ "ENOTEMPTY", "ELOOP", "", "ENOMSG", "EIDRM", "ECHRNG",
	/*  45 */ "EL2NSYNC", "EL3HLT", "EL3RST", "ELNRNG", "EUNATCH",
	/*  50 */ "ENOCSI", "EL2HLT", "EBADE", "EBADR", "EXFULL", "ENOANO",
	/*  56 */ "EBADRQC", "EBADSLT", "", "EBFONT", "ENOSTR", "ENODATA",
	/*  62 */ "ETIME", "ENOSR", "ENONET", "ENOPKG", "EREMOTE",
	/*  67 */ "ENOLINK", "EADV", "ESRMNT", "ECOMM", "EPROTO",
	/*  72 */ "EMULTIHOP", "EDOTDOT", "EBADMSG", "EOVERFLOW",
	/*  76 */ "ENOTUNIQ", "EBADFD", "EREMCHG", "ELIBACC", "ELIBBAD",
	/*  81 */ "ELIBSCN", "ELIBMAX", "ELIBEXEC", "EILSEQ", "ERESTART",
	/*  86 */ "ESTRPIPE", "EUSERS", "ENOTSOCK", "EDESTADDRREQ",
	/*  90 */ "EMSGSIZE", "EPROTOTYPE", "ENOPROTOOPT",
	/*  93 */ "EPROTONOSUPPORT", "ESOCKTNOSUPPORT",
	/*  95 */ "EOPNOTSUPP/ENOTSUP", "EPFNOSUPPORT", "EAFNOSUPPORT",
	/*  98 */ "EADDRINUSE", "EADDRNOTAVAIL", "ENETDOWN", "ENETUNREACH",
	/* 102 */ "ENETRESET", "ECONNABORTED", "ECONNRESET", "ENOBUFS",
	/* 106 */ "EISCONN", "ENOTCONN", "ESHUTDOWN", "ETOOMANYREFS",
	/* 110 */ "ETIMEDOUT", "ECONNREFUSED", "EHOSTDOWN", "EHOSTUNREACH",
	/* 114 */ "EALREADY", "EINPROGRESS", "ESTALE", "EUCLEAN",
	/* 118 */ "ENOTNAM", "ENAVAIL", "EISNAM", "EREMOTEIO", "EDQUOT",
	/* 123 */ "ENOMEDIUM", "EMEDIUMTYPE", "ECANCELED", "ENOKEY",
	/* 127 */ "EKEYEXPIRED", "EKEYREVOKED", "EKEYREJECTED",
	/* 130 */ "EOWNERDEAD", "ENOTRECOVERABLE", "ERFKILL", "EHWPOISON"
};



//1.终止函数(error 模块内部函数)
static void terminate(Boolean useExit3){
	char *s;

	/* Dump core if EF_DUMPCORE environment variable is defined and is a nonempty string; 
		 otherwise call exit(3) or _exit(2), depending on the value of 'useExit3'. */

	s = getenv("EF_DUMPCORE");

	if(s != NULL && *s != '\0')
		abort();//产生核心转存
	else if(useExit3)
		exit(EXIT_FAILURE);//清除自身的数据后, 再交给内核'清退进程'
	else
		_exit(EXIT_FAILURE);//直接交给内核'清退进程'

	return;
}



//2.打印错误汇总信息(error 模块内部函数-考虑到很多情况)
static void outputError(Boolean useErr, int err, Boolean flushStdout,const char *format, va_list ap){
	char buf[BUF_SIZE], userMsg[BUF_SIZE], errText[BUF_SIZE];



	//1.组装'程序员的报错信息'(将传入的参数列表字符串, 按照传入好的格式, 打印到缓冲区上面)
	vsnprintf(userMsg, BUF_SIZE, format, ap);


	//2.组装'系统的报错信息'(useErr 判断该打印操作, 是否是出错打印)
	if(useErr)
		snprintf(errText, BUF_SIZE, " [%s %s]",
				(err > 0 && err <= MAX_ENAME) ? ename[err] : "?UNKNOWN ERROR?", strerror(err));
	else//没有发生错误
		snprintf(errText, BUF_SIZE, ":");


	//3.汇总打印信息
	snprintf(buf, BUF_SIZE, "ERROR%s %s\n", errText, userMsg);


	//4.如果需要冲刷stdout, 则冲刷stdout
	if(flushStdout)
		fflush(stdout);//Flush any pending stdout


	//5.将打印信息, 冲刷到stderr 流中
	fputs(buf, stderr);
	fflush(stderr);//In case stderr is not line-buffered

	return;
}



//3.只打印错误信息
void errMsg(const char *format, ...){
	va_list argList;
	int savedErrno = errno;//In case we change it here(防止errno被篡改)



	va_start(argList, format);
	outputError(TRUE, errno, TRUE, format, argList);
	va_end(argList);

	errno = savedErrno;
	return ;
}



//4.打印错误信息并终止程序
void errExit(const char *format, ...){
	va_list argList;



	va_start(argList, format);
	outputError(TRUE, errno, TRUE, format, argList);
	va_end(argList);

	terminate(TRUE);//终止程序
	return;
}



//5.打印错误信息并终止程序(但不冲刷stdout 标准流)
void err_exit(const char *format, ...){
	va_list argList;



	va_start(argList, format);
	outputError(TRUE, errno, FALSE, format, argList);
	va_end(argList);

	terminate(FALSE);
	return;
}



//6.打印错误信息并终止程序(直接传入errno 值版)
//	不信任errExit() 函数获取errno 值?
//	但是现在每个posix 线程, 都有自带的errno 值了, why?
void errExitEN(int errnum, const char *format, ...){
	va_list argList;



	va_start(argList, format);
	outputError(TRUE, errnum, TRUE, format, argList);
	va_end(argList);

	terminate(TRUE);
	return;
}



//7.致命的错误打印, 并终止程序
//	(一般是程序员逻辑错误,非系统API出错.)
void fatal(const char *format, ...){
	va_list argList;



	va_start(argList, format);
	outputError(FALSE, 0, TRUE, format, argList);
	va_end(argList);

	terminate(TRUE);
	return;
}



//8.打印'程序的正确用法',并终止程序
void usageErr(const char *format, ...){
	va_list argList;



	fflush(stdout);//Flush any pending stdout

	fprintf(stderr, "Usage: ");
	va_start(argList, format);
	vfprintf(stderr, format, argList);
	va_end(argList);

	fflush(stderr);//In case stderr is not line-buffered
	exit(EXIT_FAILURE);
}



//9.打印'命令行输入的usage程序用法错误',并终止程序
void cmdLineErr(const char *format, ...){
	va_list argList;



	fflush(stdout);//Flush any pending stdout

	fprintf(stderr, "Command-line usage error: ");
	va_start(argList, format);
	vfprintf(stderr, format, argList);
	va_end(argList);

	fflush(stderr);//In case stderr is not line-buffered
	exit(EXIT_FAILURE);
}
