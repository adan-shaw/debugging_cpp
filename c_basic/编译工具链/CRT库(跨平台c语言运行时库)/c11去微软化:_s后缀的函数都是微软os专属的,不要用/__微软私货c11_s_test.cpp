//编译:
//		g++ ./__微软私货c11_s_test.cpp -std=c++11 -o x 



#define __STDC_WANT_LIB_EXT1__ 1

#include <string.h>
#include <stdio.h>
#include <stdlib.h>



void t1(void){
	#ifdef __STDC_LIB_EXT1__
	set_constraint_handler_s(ignore_handler_s);
	// 写入 0 到 r1 ,  6 个字符到 dest
	char dest[6], src[100] = "hello";
	int r1 = strncpy_s(dest, 6, src, 100);
	printf("dest = \"%s\", r1 = %d\n", dest,r1); // 'h','e','l','l','o','\0' 到 dest
	#endif
	return;
}

void t2(void){
	#ifdef __STDC_LIB_EXT1__
	set_constraint_handler_s(ignore_handler_s);
	// 复制溢出目标数组
	char dest[5], src[7] = {'g','o','o','d','b','y','e'};
	int r2 = strncpy_s(dest, 5, src, 7);
	printf("dest = \"%s\", r2 = %d\n", dest,r2); // 写入非零到 r2 ,  '\0' 到 dest[0]
	#endif
	return;
}

void t3(void){
	#ifdef __STDC_LIB_EXT1__
	set_constraint_handler_s(ignore_handler_s);
	// 写入 0 到 r3 ,  5 个字符到 dest
	char dest[5];
	int r3 = strncpy_s(dest, 5, src, 4);
	printf("dest = \"%s\", r3 = %d\n", dest,r3); // 'g', 'o', 'o', 'd', '\0' 到 dest
	#endif
	return;
}



int main(void){
	char src[100] = "hi";
	char dest[6] = "abcde"; // 无空字符
	size_t n;



	//
	// 1.strncpy() test
	//
	// 写入五个字符 'h', 'i', '\0', '\0', '\0' 到 dest
	strncpy(dest, src, 5);
	printf("strncpy(dest, src, 5) to a 6-byte dest gives : ");
	for(n = 0; n < sizeof dest; ++n){
		char c = dest[n];
		c ? printf("'%c' ", c) : printf("'\\0' ");
	}

	// 截断:写入二个字符 'h', 'i', 到 dest2
	printf("\nstrncpy(dest2, src, 2) to a 2-byte dst gives : ");
	strncpy(dest, src, 2);
	for(n = 0; n < sizeof dest; ++n) {
		char c = dest[n];
		c ? printf("'%c' ", c) : printf("'\\0' ");
	}
	printf("\n");



	//
	// 2.strncpy_s() test
	//
	t1();
	t2();
	t3();



	return 0;
}
