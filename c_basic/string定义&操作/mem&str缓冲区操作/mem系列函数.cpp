//编译:
//		g++ -g3 ./mem系列函数.cpp  -o x



//内存操作(格式化填充, 拷贝, 清0), 同样是引用<strings.h> 头文件
#include <strings.h>

/*
	//将n 字节的内存块指针*s, 全部填充为c(c虽然是int,但填充粒度是1 byte字节,因此不应该超出255)
	void *memset(void *s, int c, size_t n);

	//将n 字节的内存块指针*s, 全部填充为0
	void bzero(void *s, size_t n);

	//从*src拷贝n 字节数据到*dest, 拷贝完后清0 *src
	void *memmove(void *dest, const void *src, size_t n);

	//从*src拷贝n 字节数据到*dest, 拷贝完后不进行*src清0 
	void *memcpy(void *dest, const void *src, size_t n);


	//比较两个缓冲区*s1, *s2 是否一致?
	//*s1 = *s2, 则返回0;
	//*s1->c1 小于 *s2->c2, 则返回值<0;
	//*s1->c1 大于 *s2->c2, 则返回值>0;
	int memcmp(const void *s1, const void *s2, size_t n);


	//从*s所指内存区域的前n个字节, 查找字符c;
	//当第一次遇到字符c时停止查找;
	//如果成功, 返回起始地址指向'字符c的指针', 否则返回NULL;
	void *memchr(const void *s, int c, size_t n);

	//(慎用)memchr() 线程安全版, This function is missing on many non-glibc platforms [许多非glibc平台上缺少此功能]
	void *memrchr(const void *s, int c, size_t n);

	//(慎用)未知
	void *rawmemchr(const void *s, int c);
*/



#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <unistd.h> // for getpagesize()
#include <string.h> // for memset()




//测试'堆内存'操作类api
void operation_RAM(void){
	unsigned int uiSize;
	int iTmp;
	void *pTmp = NULL, *pTest, *pTest2;
	char *cTmp;



	uiSize = getpagesize();
	pTest = NULL;
	iTmp = posix_memalign(&pTest, uiSize, uiSize*4);
	if(iTmp != 0){
		printf("posix_memalign() failed");
		return;
	}

	pTest2 = NULL;
	iTmp = posix_memalign(&pTest2, uiSize, uiSize*4);
	if(iTmp != 0){
		printf("posix_memalign() failed");
		return;
	}



	//1.void *memset(void *s, int c, size_t n);
	memset(pTest, 'A', uiSize*4);

	//2.void bzero(void *s, size_t n);
	bzero(pTest, uiSize*4);

	//3.void *memmove(void *dest, const void *src, size_t n);
	memmove(pTest2, pTest, uiSize*4);

	//4.void *memcpy(void *dest, const void *src, size_t n);
	memcpy(pTest, pTest2, uiSize*4);

	//5.int memcmp(const void *s1, const void *s2, size_t n);
	//	比较两个缓冲区pTest2, pTest的内容是否相同, 相同返回0
	iTmp = memcmp(pTest2, pTest, uiSize*4);
	if(iTmp == 0)
		printf("pTest = pTest2, memcmp() okay\n");

	//6.memfrob
	//printf("%s\n",(char*)memfrob(des,64));
	//printf("%s\n",(char*)memfrob(des,64));


	cTmp = (char*)pTest;
	cTmp[0] = '0';
	cTmp[1] = '1';
	cTmp[2] = '2';
	cTmp[3] = '3';
	cTmp[4] = '4';
	cTmp[5] = '5';
	cTmp[6] = '\0';
	//打印整串字符串
	printf("%s\n", (char*)pTest);

	//6.void *memchr(const void *s, int c, size_t n);
	//	搜索pTest缓冲区的前uiSize*4字节, 查找第一个匹配的字符'4', 返回'从这个字符'4'开始, 字符串的指针'
	pTmp = memrchr(pTest, '4', uiSize*4);
	if(pTmp == NULL)
		printf("memchr() doesnt have char %c(找不到)\n", '4');
	else
		printf("memchr() found char %s(找到了)\n", (char*)pTmp);



	//释放测试资源
	free(pTest);
	free(pTest2);
	return ;
}



int main(void){
	operation_RAM();
	printf("operation_RAM() okay\n");
	return 0;
}




