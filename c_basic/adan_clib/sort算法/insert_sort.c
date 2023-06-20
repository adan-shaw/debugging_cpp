//编译:
//		gcc -g3 tool_func.h insert_sort.c -o x 



//插入排序(Insert Sort)
/*
	简述:
		插入排序跟选择排序很像, 都分为有序区和无序区;
		但选择排序是每次都从无序区中选出最小元素插入到有序区末尾;
		而插入排序是直接将数组的第一个元素作为有序区的第一个元素;
		每次都拿出无序区第个一元素插入到有序区合适的位置上, 直到无序区为空, 排序完成;


	算法操作步骤:
		1.将数组分为有序区和无序区, 有序区0, 无序区[1,n-1];
		2.取下无序区第一个元素, 保存其值;
		3.有序区中元素从后往前与新元素比较, 如果新元素更小, 旧元素往后移;
		3.重复步骤3, 直到新元素大于或等于旧元素, 将新元素插入该元素之后;
		4.重复步骤234, n-1次, 排序完成;


	算法分析:
		最好: T(n)=o(n), 数组元素正序排列
		最坏: T(n)=o(n^2)数组元素反序排列
		平均: T(n)=o(n^2)

	算法改进:
		新元素在插入到有序区时, 使用二分法查找位置而非一个一个依次查找;
*/



#include "tool_func.h"
#include <stdlib.h>



//#define arr_t int		//兼容int整形
#define arr_t double//兼容double浮点
#define arr_buf_max (8192)



//插入排序(从小到大排列)
void insert_sort(arr_t* a, int len){
	int i,j;
	arr_t key;
	for(i=1; i<len; ++i){
		key=a[i];
		j=i-1;
		while(!(j<0) && a[j]>key){
			a[j+1]=a[j];
			j--;
		}
		a[j+1]=key;
	}
	return;
}

//插入排序改进版(从小到大排列)
void insert_sortEx(arr_t* a, int len){
	int i,j,left,right,middle;
	arr_t key;
	for(i=1; i<len; ++i){
		key=a[i];
		left=0;
		right=i-1;
		while(!(left > right)){
			middle=(left+right) / 2;
			if(a[middle] > key)
				right=middle-1;
			else
				left=middle+1;
		}
		for(j=i-1; !(j<left); --j)
			a[j+1]=a[j];
		a[left]=key;
	}
	return;
}

void insert_sort_test(void){
	arr_t a[] = { 520,0,1,9,56,100,1,85,5,3,-16,10,8,4,3,1,5,7,9,-2,6 };
	int len = sizeof(a) / sizeof(a[0]);
	arr_t* parr = malloc(arr_buf_max*sizeof(arr_t));

	insert_sort(a, len);											//easy
	//parr_int(a, len);
	parr_double(a, len);
	printf("\n");

	len = arr_buf_max;
	full_random2arr(parr, len);								//hard
	insert_sort(parr, arr_buf_max);
	len = arr_buf_max;
	//parr_int(parr, len);
	//parr_double(parr, len);
	len = arr_buf_max;
	//check_arr_sort_int(parr, len);					//check
	check_arr_sort_double(parr, len);
	free(parr);
	return;
}

void insert_sortEx_test(void){
	arr_t a[] = { 520,0,1,9,56,100,1,85,5,3,-16,10,8,4,3,1,5,7,9,-2,6 };
	int len = sizeof(a) / sizeof(a[0]);
	arr_t* parr = malloc(arr_buf_max*sizeof(arr_t));

	insert_sortEx(a, len);										//easy
	//parr_int(a, len);
	parr_double(a, len);
	printf("\n");

	len = arr_buf_max;
	full_random2arr(parr, len);								//hard
	insert_sortEx(parr, arr_buf_max);
	len = arr_buf_max;
	//parr_int(parr, len);
	//parr_double(parr, len);
	len = arr_buf_max;
	//check_arr_sort_int(parr, len);					//check
	check_arr_sort_double(parr, len);
	free(parr);
	return;
}



int main(void){
	insert_sort_test();
	insert_sortEx_test();
	return 0;
}
