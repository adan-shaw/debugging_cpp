#include <stdio.h>
#include <time.h>

//'整形/浮点'通用number交换宏(需要tmp 变量)
#define SWAP_NUM(a,b,tmp) { (tmp)=(a); (a)=(b); (b)=(tmp); }



//从右边开始, 打印数组(%d=带负数int, %ld=无负数unsigned int)
//(从小到大排列, 打印出来是从大到小)
#define parr_int(arr, count){ while(count) printf("%d, ",arr[--count]); }
#define parr_double(arr, count){ while(count) printf("%lf, ",arr[--count]); }



//检查排序的正确性(从小到大, 正序)
#define check_arr_sort_int(arr, count){ \
	if(count > 1){ \
		while(count >= 2){ \
			if(arr[count-1] < arr[count-2]){ \
				printf("check_arr_sort_int() failed(从小到大, 正序)!!, \
					arr[%d]=%d, arr[%d]=%d\n",count-1,arr[count-1],count-2,arr[count-2]); \
				break; } else count-=1; } } }

#define check_arr_sort_double(arr, count){ \
	if(count > 1){ \
		while(count >= 2){ \
			if(arr[count-1] < arr[count-2]){ \
				printf("check_arr_sort_double() failed(从小到大, 正序)!!, \
					arr[%d]=%lf, arr[%d]=%lf\n",count-1,arr[count-1],count-2,arr[count-2]); \
				break; } else count-=1; } } }

//检查排序的正确性(从大到小, 倒序)
#define check_arr_sort_intEx(arr, count){ \
	if(count > 1){ \
		while(count >= 2){ \
			if(arr[count-1] > arr[count-2]){ \
				printf("check_arr_sort_int() failed(从大到小, 倒序)!!, \
					arr[%d]=%d, arr[%d]=%d\n",count-1,arr[count-1],count-2,arr[count-2]); \
				break; } else count-=1; } } }

#define check_arr_sort_doubleEx(arr, count){ \
	if(count > 1){ \
		while(count >= 2){ \
			if(arr[count-1] > arr[count-2]){ \
				printf("check_arr_sort_double() failed(从大到小, 倒序)!!, \
					arr[%d]=%lf, arr[%d]=%lf\n",count-1,arr[count-1],count-2,arr[count-2]); \
				break; } else count-=1; } } }



//填充随机数到数组
#define full_random2arr_max (100000000)
#define full_random2arr(arr, count) { while(count) arr[--count]=(clock()*time(NULL)%full_random2arr_max); }
