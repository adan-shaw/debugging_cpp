//编译: 
//	g++ -lm -g3 ./rand_random\(时钟创建随机数\).cpp -o x 

//依赖(编译出-m32):
//	sudo apt-get install libc6-dev-i386 gcc-multilib g++-multilib



// 关于随机数设计
/*
	1.尽量不要使用fopen() 读取/dev/urandom, 因为跨平台效果不好

	2.自己设计随机数也行, 方便嵌入式使用, clock()*time() 的策略不错, 线程安全

	3.直接使用random_r(), 方便快捷.
		如果嵌入式没有random_r(), 你可以自己实现一个random_r()函数, 替换上去即可.
*/




//
// 随机数API 简介
//
/*
1.1 rand 类的随机数api
	//如果直接调用, 默认会自动执行srand(1), 返回[0,RAND_MAX] 的值
	int rand(void);

	//设置种子(专门为rand()而设计的, 
		因为rand()里面的变量是static 静态的, 所以需要srand()函数进行固定式修改)
	void srand(unsigned int seed);


	//带[缓冲区参数]的线程安全版本, 不需要srand().
	int rand_r(unsigned int *seedp);





1.2 宏常量
	RAND_MAX 生成的最大可能值, rand(), rand_r() 的返回值区间: [0,RAND_MAX]





2.1 random 类的随机数api(加强版)
	//获取随机数
	long int random(void);
	//初始化种子+state字典[8,256字节], 一般最大使用256
	char *initstate(unsigned int seed, char *state, size_t n);
	//改变字典
	char *setstate(char *state);
	//改变种子
	void srandom(unsigned int seed);


	//带[缓冲区参数]的线程安全版本
	int random_r(struct random_data *buf, int32_t *result);
	int initstate_r(unsigned int seed, char *statebuf, \
				size_t statelen, struct random_data *buf);
	int setstate_r(char *statebuf, struct random_data *buf);
	int srandom_r(unsigned int seed, struct random_data *buf);


	//random_r 初始化之后的载体(可移动,可转移,需要malloc()创建)
	struct random_data *buf;
*/



#include <time.h>
#include <stdio.h>
#include <stdlib.h>//for srandom_r
#include <limits.h>//int unsigned long 溢出问题
#include <errno.h>
#include "adan_clib/debug_locating_errors.h"



// 字典MAX大小(byte char)
#define RANDOM_DIST_MAX 256





//1.rand 类随机API测试
void rand_test(void){
	unsigned int uiTmp;



	//线程安全版本(必须传入unsigned int 实体变量, 不能是time() 返回的常量值, 
	//					 因为这个缓冲区要用来做运算, 这样才会线程安全)
	uiTmp = (unsigned int) ( clock() % 2000 ) * ( clock() % 2000 ) * ( clock() % 1000 );
	printf("rand_r((&uiTmp): %d\n", rand_r(&uiTmp));


	//非线程安全版本
	uiTmp = (unsigned int) ( clock() % 2000 ) * ( clock() % 2000 ) * ( clock() % 1000 );
	srand(uiTmp);
	printf("rand(): %d\n", rand());

	return;
}






//用'随机字符'填充char缓冲区(随机字符ascii[33-126], 都是可见字符, 填充缓冲区)
void init_strbuf(char *pBuf, unsigned int uiLen);

//2.random 类随机API测试
void random_test(void){
	unsigned int uiTmp;
	int iTmp;
	char *pDist;
	//random_r 初始化之后的载体(可移动,可转移,需要malloc()创建)
	struct random_data *pstRandomData;



	//申请字典buf 内存
	pDist = NULL;
	pDist = (char*)malloc( sizeof(char) * RANDOM_DIST_MAX );
	if(pDist == NULL){
		MY_DEBUG("malloc()");
		return;
	}
	//向字典buf 填充随机字符
	init_strbuf(pDist, RANDOM_DIST_MAX);

	//打印'已经填充随机字符完毕'的字典buf
	for(iTmp = 0; iTmp < RANDOM_DIST_MAX; iTmp++)
		printf("%c ", pDist[iTmp]);

	printf("\n\n\n");





	//
	//线程安全版
	//
	//申请struct RandomData{} 内存
	pstRandomData = NULL;
	pstRandomData = (struct random_data*)malloc( sizeof(struct random_data) );
	if(pstRandomData == NULL){
		MY_DEBUG("malloc()");
		free(pDist);
		return;
	}


	//初始化'字典+种子'
	uiTmp = (unsigned int) ( clock() % 2000 ) * ( clock() % 2000 ) * ( clock() % 1000 );
	iTmp = initstate_r(uiTmp, pDist, RANDOM_DIST_MAX, pstRandomData);
	if(iTmp == -1){
		MY_DEBUG("initstate_r()");
		goto random_test_failed;
	}

	//得到random_r()随机数
	if(random_r(pstRandomData, &iTmp) == -1){
		MY_DEBUG("random_r()");
		goto random_test_failed;
	}
	else
		printf("random_r(): %d\n", iTmp);



	//改变种子
	uiTmp = (unsigned int) ( clock() % 2000 ) * ( clock() % 2000 ) * ( clock() % 1000 );
	iTmp = srandom_r(uiTmp, pstRandomData);
	if(iTmp == -1){
		MY_DEBUG("srandom_r()");
		goto random_test_failed;
	}

	//得到random_r()随机数
	if(random_r(pstRandomData, &iTmp) == -1){
		MY_DEBUG("random_r()");
		goto random_test_failed;
	}
	else
		printf("random_r(): %d\n", iTmp);





	//
	//非线程安全版
	//
	uiTmp = (unsigned int) ( clock() % 2000 ) * ( clock() % 2000 ) * ( clock() % 1000 );
	//初始化'字典+种子'
	if(initstate(uiTmp, pDist, RANDOM_DIST_MAX) == NULL){
		MY_DEBUG("initstate()");
		goto random_test_failed;
	}

	//得到random()随机数
	printf("random(): %d\n", random());



	//改变种子
	uiTmp = (unsigned int) ( clock() % 2000 ) * ( clock() % 2000 ) * ( clock() % 1000 );
	srandom(uiTmp);//无返回值的

	//得到random()随机数
	printf("random(): %d\n", iTmp);





random_test_failed:
	free(pDist);
	free(pstRandomData);
	return;
}





int main(void){
	//1.rand 类随机API测试
	rand_test();

	//2.random 类随机API测试
	random_test();

	return 0;
}





//初始化字典(随机字符ascii[33-126], 都是可见字符, 填充缓冲区)
void init_strbuf(char *pBuf, unsigned int uiLen){
	long int liTmp,iCount;
	unsigned int uiTmp;
	unsigned char ucTmp;



	for(iCount = 0; iCount < uiLen; iCount++){
		//对126 求余, 则随机数liTmp肯定少于126
		uiTmp = (unsigned int) ( clock() % 2000 ) * ( clock() % 2000 ) * ( clock() % 1000 );
		liTmp = rand_r(&uiTmp) % 126;

		//如果liTmp少于33, 则用126 - liTmp, 得到一个反方向的值;
		if(liTmp < 33)
			liTmp = 126 - liTmp;
		//最终liTmp一定在[33,126]的区间内!!

		//int 强转为unsigned char
		ucTmp = liTmp;

		//unsigned char 强转为char
		pBuf[iCount] = ucTmp;
	}

	return;
}
