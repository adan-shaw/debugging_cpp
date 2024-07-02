//编译:
//		gcc -g3 ./fcgi2_demo.c -o x -l fcgi



//安装:
//		apt-get install spawn-fcgi libcgicc-dev libcgicc-doc libcgicc3 libfcgi-bin libfcgi-dev libfcgi0ldbl

//fcgi 共享链接库的存放路径
/*
	/usr/lib/x86_64-linux-gnu/libfcgi.so
	/usr/lib/x86_64-linux-gnu/libfcgi++.so
	/usr/lib/x86_64-linux-gnu/libcgicc.so
*/



#include <fcgi_stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(void) {
	int count = 0;
	while (FCGI_Accept() >= 0) {
		printf("Content-type: text/html\r\n"
			"\r\n"
			""
			"FastCGI Hello!"
			"Request number %d running on host%s "
			"Process ID: %d\n", ++count, getenv("SERVER_NAME"), getpid());
	}
	return 0;
}
