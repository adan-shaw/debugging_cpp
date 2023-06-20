//编译:
//		gcc ./seq_find.c -g3 -o x 


//顺序查找
/*
	基本思想:
		顺序查找也称为线形查找, 属于'无序查找'算法(只适用于线性容器).
		从数据结构线形表的一端开始, 顺序扫描,
		依次将扫描到的结点关键字与给定值k相比较,
		若相等则表示查找成功;
		若扫描结束仍没有找到关键字等于k的结点, 表示查找失败.


	复杂度分析:
		查找成功时的平均查找长度为:(假设每个数据元素的概率相等)
		ASL = 1/n(1+2+3+...+n) = (n+1)/2 ;

		当查找不成功时, 需要n+1次比较, 时间复杂度为O(n);

		所以, 顺序查找的时间复杂度为O(n).


	顺序查找C语言实现:
		用顺序结构存储数据(数组、链表), 从前到后依次查询目标值,
		如果发现则返回查找到的值, 否则返回0.
*/



#include <stdio.h>
#include <math.h>



//#define arr_t int		//兼容int整形
#define arr_t double//兼容double浮点



//从'无序arr'中, 从右往左寻找目标(找到值, 则pos=len[大于0]; 找不到值, 则len=-1)
#define arr_seq_find_int(arr, len, target) { while(len >= 0) if(arr[len--] == target) break; }

//浮点版本
#define Epslion (1e-8)
#define arr_seq_find_float(arr, len, target) { while(len >= 0) if(fabsf(arr[len--]-target)<(1e-8)) break; }
#define arr_seq_find_double(arr, len, target) { while(len >= 0) if(fabs(arr[len--]-target)<(1e-8)) break; }
#define arr_seq_find_ldouble(arr, len, target) { while(len >= 0) if(fabsl(arr[len--]-target)<(1e-8)) break; }



int main(void){
	//									 0		1		2		3		4		5		6		7
	int TestData[8] =  { 34, 35, 26, 89, 56, 56, 89, 56 };
	float TestData2[8] = { 34, 35, 26, 89, 56, 56, 89, 56 };
	double TestData3[8] = { 34, 35, 26, 89, 56, 56, 89, 56 };
	long double TestData4[8] = { 34, 35, 26, 89, 56, 56, 89, 56 };

	int len,pos;

	pos = len = 8;
	arr_seq_find_int(TestData, len, 89);
	printf("test pos: %d\n", len);

	pos = len = 8;
	arr_seq_find_float(TestData2, len, 89);
	printf("test2 pos2: %d\n", len);

	pos = len = 8;
	arr_seq_find_double(TestData3, len, 89);
	printf("test3 pos3: %d\n", len);

	pos = len = 8;
	arr_seq_find_ldouble(TestData4, len, 89);
	printf("test4 pos4: %d\n", len);

	return 0;
}
