//编译:
//		gcc -g3 ./shell_sort.c -o x 



//希尔排序(Shell Sort)
/*
	简述:
		希尔排序是以某个增量h为步长跳跃分组进行插入排序;
		由于增量是一个从h逐渐缩小至1的过程, 所以又称缩小增量排序;
		其核心在于间隔序列设定, 即增量的设定, 这也是也插入排序的本质区别;
		插入排序始终增量为1;


	最佳增量:
		k趟排序增量步长为(2^k)-1,即增量序列(2^k)-1,...15,7,3,1


	算法操作步骤:
		1.确定增量序列(t1,t2...tk)ti>tj,tk=1;
		2.按增量序列个数k分成k趟排序;
		3.每趟排序按对应增量ti, 将序列分割成若干子序列, 分别进行直接插入排序;


	简述:
		希尔排序实际上是将一维数组分成具有不同列数的二维数组(对应方式, a[i]->a[i/h][i%h]);
		在每一趟排序中对每一列进行插入排序;
		列宽也就是增量, 增量减小也就是列数减小;
		以排序数组a[]={10,8,4,3,1,5,7,9,2,6}, 增量分别为5,2,1为例;
*/



#include <stdio.h>
#include <stdlib.h>
#include "tool_func.h"



//#define arr_t int		//兼容int整形
#define arr_t double//兼容double浮点
#define arr_buf_max (8192)



//希尔排序(从小到大排列)
void shell_sort(arr_t* a, int len){
	arr_t key;
	int i, j, tmp = len;

	while(tmp = tmp/2){											//增量
		for(i = tmp; i < len; i++){
			key = a[i];													//待排序元素
			j = i-tmp;
			for(; (j+1>0) && (a[j]>key); j-=tmp)//插入排序
				a[j+tmp] = a[j];
			a[j+tmp] = key;
		}
	}
	return;
}



int main(void){
	arr_t a[] = { 7,0,1,2,8,5,9,3,4,520,-11,56,100,771,85,57,-3,6,10,87,44,34,14,-54,74,94,24,64 };
	int len = sizeof(a) / sizeof(a[0]);
	arr_t* parr = malloc(arr_buf_max*sizeof(arr_t));

	shell_sort(a, len);											//easy
	//parr_int(a, len);
	parr_double(a, len);

	len = arr_buf_max;
	full_random2arr(parr, len);							//hard
	shell_sort(parr, arr_buf_max);
	len = arr_buf_max;
	//parr_int(parr, len);
	//parr_double(parr, len);
	len = arr_buf_max;
	//check_arr_sort_int(parr, len);				//check
	check_arr_sort_double(parr, len);
	free(parr);
	return 0;
}
