//编译:
//		gcc -g3 tool_func.h bubble_sort.c -o x 



//冒泡排序(Bubble Sort)[比较排序]
/*
	算法简述:
		重复进行相邻数组元素的两两比较, 并按规则进行交换, 直到没有元素再需要交换;
		最终使得大的元素逐渐沉到数列底部, 相较小的元素浮现到数列前端;


	算法操作步骤:
		1 比较相邻两个元素, 如果第一个比第二个大, 就交换位置;
		2 从第一对开始, 对数组中的每一对相邻的两个元素重复步骤1, 使得最大的元素沉到数组底部;
		3 重复步骤2, 除了底部已经排序好的元素(每一趟都会多一个以排序好的元素);
		4 重复以上步骤直到排序完成;


	算法分析:
		最好:T(n)=O(n);	数据全部正序, 
										bubble_sort()全部元素比较一遍^的次方, 没做交换
										bubble_sortEx全部元素比较一遍
		最差:T(n)=O(n^2);数据全部反序, 每次比较都触发交换(实际上if消耗更大, 交换消耗小)
		平均:T(n)={O(n^2)+O(n)}/2

	改进版:
		双向冒泡 + 0次交换就exit冒泡(从小到大)
		增加了一点内耗if, 但做了更多期望(在array数组元素偏多的情况下有效)



	选择使用int len, 而非unsigned len, 
	是为了进行逻辑判断时, 不容易造成-1=无穷大的局面, 更好地迎合大于0, 小于0等if判断(防止溢出)
*/



#include "tool_func.h"
#include <stdlib.h>



//#define arr_t int		//兼容int整形
#define arr_t double//兼容double浮点
#define arr_buf_max (8192)



//简单冒泡排序(从小到大排列)
void bubble_sort(arr_t* a, int len){
	int i,j;
	arr_t tmp;
	for(i = 0; i < len; ++i)
		for(j = 0; j < len-i-1; ++j)
			if(a[j] > a[j+1])
				SWAP_NUM(a[j], a[j+1], tmp);
	return;
}

//双向冒泡 + 0次交换就exit冒泡(从小到大排列)
void bubble_sortEx(arr_t* a, int len){
	int i,j,flag_exit;
	arr_t tmp;
	for(i=0; i < len; ++i){
		for(j=0,flag_exit=1; j < len-i-1; ++j)	//正向
			if(a[j] > a[j+1]){
				SWAP_NUM(a[j], a[j+1], tmp);
				flag_exit = 0;
			}
		if(flag_exit)
			break;
		for(j=len-i-2,flag_exit=1; j > 0; --j)	//反向
			if(a[j] < a[j-1]){
				SWAP_NUM(a[j], a[j-1], tmp);
				flag_exit = 0;
			}
		if(flag_exit)
			break;
	}
	return;
}

void bubble_sort_test(){
	arr_t a[] = { 520,0,1,9,56,100,1,85,5,3,-16,10,8,4,3,1,5,7,9,-2,6 };
	int len = sizeof(a) / sizeof(a[0]);
	arr_t* parr = malloc(arr_buf_max*sizeof(arr_t));

	bubble_sort(a, len);											//easy
	//parr_int(a, len);
	parr_double(a, len);
	printf("\n");

	len = arr_buf_max;
	full_random2arr(parr, len);								//hard
	bubble_sort(parr, arr_buf_max);
	len = arr_buf_max;
	//parr_int(parr, len);
	//parr_double(parr, len);
	len = arr_buf_max;
	//check_arr_sort_int(parr, len);					//check
	check_arr_sort_double(parr, len);
	free(parr);
	return;
}

void bubble_sortEx_test(){
	arr_t a[] = { 520,10,8,4,3,1,-15,7,9,2,60,1,9,56,100,-1,85,5,3,6 };
	int len = sizeof(a) / sizeof(a[0]);
	arr_t* parr = malloc(arr_buf_max*sizeof(arr_t));

	bubble_sortEx(a, len);										//easy
	//parr_int(a, len);
	parr_double(a, len);
	printf("\n");

	len = arr_buf_max;
	full_random2arr(parr, len);								//hard
	bubble_sortEx(parr, arr_buf_max);
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
	bubble_sort_test();
	bubble_sortEx_test();
	return 0;
}
