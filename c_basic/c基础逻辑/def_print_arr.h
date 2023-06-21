#include<stdio.h>



//打印int/uint, long/ulong 数组
//(len_tmp 不能是常量, 不能是数组len 记录器; 必须是len_tmp变量实体, 因为需要进行自减操作)
#ifndef print_arr_num
	#define print_arr_num(arr, len_tmp) { do{fprintf(stderr, "%ld, ", arr[--len_tmp]);}while(len_tmp > 0);}
#endif

//打印float/double/long double 数组(printf 函数, 最高是%lf, 并不能像正则表达式一样, 使用%llf)
#ifndef print_arr_float
	#define print_arr_float(arr, len_tmp) { do{fprintf(stderr, "%lf, ", arr[--len_tmp]);}while(len_tmp > 0);}
#endif
