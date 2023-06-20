//编译:
//		gcc -g3 ./select_sort.c -o x 



//选择排序(Select Sort)[比较排序]
/*
	简述:
		将数组分为已排序和待排序两个区间, 即有序区和无序区;
		每次从无序区中选出最小/最大的元素与无序区的第一个元素(即有序区尾部)交换位置,
		直到无序区只剩一个元素, 排序完成;


	算法操作步骤:
		1.将数组划分为有序区和无序区;(开始有序区为空, 无序区[0,n-1])
		2.在无序区中找出最大的那个元素, 与该区间第一个元素交换;(有序区元素个数加1, 无序区减1)
		3.重复'步骤2'n-1次, 排序完成;(排序趟数为n-1, 最后一个元素无需排序)


	算法分析:
		时间复杂度最稳定, 无论什么时候都为: T(n)=0(n^2)
*/



#include <stdio.h>
#include <stdlib.h>
#include "tool_func.h"



//#define arr_t int		//兼容int整形
#define arr_t double//兼容double浮点
#define arr_buf_max (8192)



//选择排列(从小到大排列, 每次找出一个最小元素, 交换到最左边)
void select_sort(arr_t* a, int len){
	int i, j, pos_min;
	arr_t sw_tmp;
	for(i=0; i < len-1; ++i){
		pos_min=i;
		for(j=i; j < len; ++j)								//在无序区找出'相对的最小元素'的pos
			if(a[j] < a[pos_min])
				pos_min=j;
		SWAP_NUM(a[i], a[pos_min], sw_tmp);		//每次交换一个'相对的最小元素'到左边
	}
	return;
}



int main(void){
	arr_t a[] = { 520,0,1,9,56,100,1,85,5,3,-16,10,8,4,3,1,5,7,9,-2,6 };
	int len = sizeof(a) / sizeof(a[0]);
	arr_t* parr = malloc(arr_buf_max*sizeof(arr_t));

	select_sort(a, len);											//easy
	//parr_int(a, len);
	parr_double(a, len);
	printf("\n");

	len = arr_buf_max;
	full_random2arr(parr, len);								//hard
	select_sort(parr, arr_buf_max);
	len = arr_buf_max;
	//parr_int(parr, len);
	//parr_double(parr, len);
	len = arr_buf_max;
	//check_arr_sort_int(parr, len);					//check
	check_arr_sort_double(parr, len);
	free(parr);
	return 0;
}

