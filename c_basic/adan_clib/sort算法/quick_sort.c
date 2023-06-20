//编译:
//		gcc -g3 ./quick_sort.c -o x


#include <stdio.h>
#include <stdlib.h>



//快速排序(Quick Sort)
/*
	简述:
		分治和递归的应用;
		选取数组中一个元素为基准(pivot)P, 对数组进行排序,
		使得比P大的元素都在P的右边, 比P小的元素在P的左边;
		然后对以P为分界点的左右子串递归进行快排;


	算法操作步骤:
		1.从数列中选取一个元素作为基准;
		2.进行分区操作(partition);
			重新排列数列, 使得比基准小的元素都在其左边, 比基准小的元素都在它右边,
			即分为左子列, 基准, 右子列;
		3.对左右子列递归进行快排;


	算法分析:
		最好: o(nlogn)
		最坏: o(n^2)
		平均: o(nlogn)


	快速排序 vs 冒泡排序:
		快速排序是对冒泡排序的一种改进, 
		冒泡排序一次只能移动一次, 
		而快速排序一次就能将大于轴值的数移动到轴值的后面(升序), 减少了移动的次数;
*/



#include <stdio.h>
#include <stdlib.h>
#include "tool_func.h"



//#define arr_t int		//兼容int整形
#define arr_t double//兼容double浮点
#define arr_buf_max (8192)



//快速排序(从小到大排列)
void quick_sort(arr_t* arr, int low, int high){
	int x, y;
	arr_t tmp;
	if(low < high){
		x=low;																//这两个过度变量x,y 不能省掉!!
		y=high;
		tmp=arr[x];														//随便指定轴值(随机开始)
		while(x<y){
			while(arr[y]>=tmp && x<y)						//浮点可以比较大小, 等于(貌似不影响)
				y--;
			arr[x]=arr[y];
			while(arr[x]<=tmp && x<y)
				x++;
			arr[y]=arr[x];
		}
		arr[x]=tmp;
		quick_sort(arr,low,x-1);							//轴值左边-继续递归划分
		quick_sort(arr,x+1,high);							//轴值右边-继续递归划分
	}
	else//low >= high
		return;
}



int main(void){
	arr_t a[] = { 7,0,1,2,8,5,9,3,4,520,-11,56,100,771,85,57,-3,6,10,87,44,34,14,-54,74,94,24,64 };
	int len = sizeof(a) / sizeof(a[0]);
	arr_t* parr = malloc(arr_buf_max*sizeof(arr_t));

	quick_sort(a,0,len-1);									//easy
	//parr_int(a, len);
	parr_double(a, len);

	len = arr_buf_max;
	full_random2arr(parr, len);							//hard
	len = arr_buf_max;
	quick_sort(parr,0,len-1);
	len = arr_buf_max;
	//parr_int(parr, len);
	//parr_double(parr, len);
	len = arr_buf_max;
	//check_arr_sort_int(parr, len);				//check
	check_arr_sort_double(parr, len);
	free(parr);
	return 0;
}
