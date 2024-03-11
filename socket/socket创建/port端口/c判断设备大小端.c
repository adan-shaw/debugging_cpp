//编译:
//		gcc -g3 ./c判断设备大小端.c -o x



#include <stdio.h>
#include <stdbool.h>



bool testBigEndin(void) {
	union __testBigEndin {
		int number;
		char s;
	};

	typedef union __testBigEndin testBigEndin_t;

	testBigEndin_t test;
	test.number = 0x01000002;
	return (test.s == 0x01);
}

int main(void) {
	if(testBigEndin())
		printf("big endin\n");
	else
		printf("small endin\n");
	return 0;
}

