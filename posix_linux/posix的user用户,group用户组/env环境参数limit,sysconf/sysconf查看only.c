//编译:
//		gcc ./sysconf查看only.c -g3 -o x 


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>



//宏定义1 MB = 1024 * 1024 bit
#define UNIT_MB (1024 * 1024)



//1.sysconf() 函数原型;(失败返回-1)
//long sysconf(int name);



int main(void){
	int tmp;

	printf("1.CPU 的核心数量: %ld\n",sysconf(_SC_NPROCESSORS_CONF));
	printf("2.可用的CPU 核心数量(available): %ld\n",sysconf(_SC_NPROCESSORS_ONLN));
	printf("3.系统页面大小(一般是4k一页): %ld\n",sysconf(_SC_PAGESIZE));
	printf("4.当前系统一共有多少页(页总数量): %ld\n",sysconf(_SC_PHYS_PAGES));
	printf("5.当前系统'可用页'有多少(页的数量): %ld\n",sysconf(_SC_AVPHYS_PAGES));

	tmp = ((long long)sysconf(_SC_PAGESIZE)*(long long)sysconf(_SC_PHYS_PAGES)) / UNIT_MB;
	printf("6.当前系统内存总量={所有'页大小(4kb)*页数量' / UNIT_MB}: %ld MB\n",tmp);

	printf("7.可打开的文件句柄max: %ld\n",sysconf(_SC_OPEN_MAX));
	printf("8.总线时钟(intel一般是100Hz, amd一般是200Hz): %ld\n",sysconf(_SC_CLK_TCK));
	printf("9.主机名的最大长度: %ld\n",sysconf(_SC_HOST_NAME_MAX));
	printf("10.用户名的最大长度: %ld\n",sysconf(_SC_LOGIN_NAME_MAX));
	printf("11.exec()函数的参数最大长度(字节数): %ld\n",sysconf(_SC_ARG_MAX));
	printf("12.可用atexit()函数登记的最大函数个数: %ld\n",sysconf(_SC_ATEXIT_MAX));
	printf("13.实用程序'命令行'输入的最大长度: %ld\n",sysconf(_SC_LINE_MAX));

	return 0;
}




//
//2.可获取的参数列表:
//
/*
	POSIX.1 variables
		We give the name of the variable, the name of the sysconf() argument
		used to inquire about its value, and a short description.

		First, the POSIX.1 compatible values.

		ARG_MAX - _SC_ARG_MAX
			The maximum length of the arguments to the exec(3) family of functions. 
			Must not be less than _POSIX_ARG_MAX (4096).

		CHILD_MAX - _SC_CHILD_MAX
			The maximum number of simultaneous processes per user ID.
			Must not be less than _POSIX_CHILD_MAX (25).

		HOST_NAME_MAX - _SC_HOST_NAME_MAX
			Maximum length of a hostname, not including the terminating null byte, as returned by gethostname(2). 
			Must not be less than _POSIX_HOST_NAME_MAX (255).

		LOGIN_NAME_MAX - _SC_LOGIN_NAME_MAX
			Maximum length of a login name, including the terminating null byte. 
			Must not be less than _POSIX_LOGIN_NAME_MAX (9).

		NGROUPS_MAX - _SC_NGROUPS_MAX
			Maximum number of supplementary group IDs.

		clock ticks - _SC_CLK_TCK
			The number of clock ticks per second.  The corresponding variable is obsolete. 
			It was of course called CLK_TCK.
			(Note: the macro CLOCKS_PER_SEC does not give information: it must equal 1000000.)

		OPEN_MAX - _SC_OPEN_MAX
			The maximum number of files that a process can have open at any time. 
			Must not be less than _POSIX_OPEN_MAX (20).

		PAGESIZE - _SC_PAGESIZE
			Size of a page in bytes. 
			Must not be less than 1.

		PAGE_SIZE - _SC_PAGE_SIZE
			A synonym for PAGESIZE/_SC_PAGESIZE. (Both PAGESIZE and PAGE_SIZE are specified in POSIX.)

		RE_DUP_MAX - _SC_RE_DUP_MAX
			The number of repeated occurrences of a BRE permitted by regexec(3) and regcomp(3). 
			Must not be less than _POSIX2_RE_DUP_MAX (255).

		STREAM_MAX - _SC_STREAM_MAX
			The maximum number of streams that a process can have open at any time. 
			If defined, it has the same value as the standard C macro FOPEN_MAX. 
			Must not be less than _POSIX_STREAM_MAX (8).

		SYMLOOP_MAX - _SC_SYMLOOP_MAX
			The maximum number of symbolic links seen in a pathname before resolution returns ELOOP.
			Must not be less than _POSIX_SYMLOOP_MAX (8).

		TTY_NAME_MAX - _SC_TTY_NAME_MAX
			The maximum length of terminal device name, including the terminating null byte. 
			Must not be less than _POSIX_TTY_NAME_MAX (9).

		TZNAME_MAX - _SC_TZNAME_MAX
			The maximum number of bytes in a timezone name. 
			Must not be less than _POSIX_TZNAME_MAX (6).

		_POSIX_VERSION - _SC_VERSION
			indicates the year and month the POSIX.1 standard was approved in the format YYYYMML; 
			the value 199009L indicates the Sept. 1990 revision.



	POSIX.2 variables
		Next, the POSIX.2 values, giving limits for utilities.

		BC_BASE_MAX - _SC_BC_BASE_MAX
			indicates the maximum obase value accepted by the bc(1) utility.

		BC_DIM_MAX - _SC_BC_DIM_MAX
			indicates the maximum value of elements permitted in an array by bc(1).

		BC_SCALE_MAX - _SC_BC_SCALE_MAX
			indicates the maximum scale value allowed by bc(1).

		BC_STRING_MAX - _SC_BC_STRING_MAX
			indicates the maximum length of a string accepted by bc(1).

		COLL_WEIGHTS_MAX - _SC_COLL_WEIGHTS_MAX
			indicates the maximum numbers of weights that can be assigned
			to an entry of the LC_COLLATE order keyword in the locale definition file,

		EXPR_NEST_MAX - _SC_EXPR_NEST_MAX
			is the maximum number of expressions which can be nested within parentheses by expr(1).

		LINE_MAX - _SC_LINE_MAX
			The maximum length of a utility's input line, either from standard input or from a file. 
			This includes space for a trailing newline.

		RE_DUP_MAX - _SC_RE_DUP_MAX
			The maximum number of repeated occurrences of a regular expression 
			when the interval notation \{m,n\} is used.

		POSIX2_VERSION - _SC_2_VERSION
			indicates the version of the POSIX.2 standard in the format of YYYYMML.

		POSIX2_C_DEV - _SC_2_C_DEV
			indicates whether the POSIX.2 C language development facilities are supported.

		POSIX2_FORT_DEV - _SC_2_FORT_DEV
			indicates whether the POSIX.2 FORTRAN development utilities are supported.

		POSIX2_FORT_RUN - _SC_2_FORT_RUN
			indicates whether the POSIX.2 FORTRAN run-time utilities are supported.

		_POSIX2_LOCALEDEF - _SC_2_LOCALEDEF
			indicates whether the POSIX.2 creation of locates via localedef(1) is supported.

		POSIX2_SW_DEV - _SC_2_SW_DEV
			indicates whether the POSIX.2 software development utilities option is supported.

		These values also exist, but may not be standard.

		- _SC_PHYS_PAGES
			The number of pages of physical memory. 
			Note that it is possible for the product of this value and the value of _SC_PAGESIZE to overflow.

		- _SC_AVPHYS_PAGES
			The number of currently available pages of physical memory.

		- _SC_NPROCESSORS_CONF
			The number of processors configured. See also get_nprocs_conf(3).

		- _SC_NPROCESSORS_ONLN
			The number of processors currently online (available). See also get_nprocs_conf(3).
*/
