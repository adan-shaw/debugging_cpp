//编译:
//		gcc -g3 marr_copy.h marr_copy.c -o x

#include "marr_copy.h"
#include <stdio.h>



//打印二位数组(顺便验证一下: 二维数组变量的实际存放方式, 是否是压扁为一维数组连续存放的)
void parr2(void* p,unsigned int row_max,unsigned int row_count){
	int tmp,tmp2;
	char* pc = (char*)p;
	for(tmp=0;tmp<row_count;tmp++)
		for(tmp2=0;tmp2<row_max;tmp2++)
			if(*pc != 'A')
				printf("err: %d-%d-%c\n",tmp,tmp2,*pc);
}

//打印三维数组(顺便验证一下: 三维数组变量的实际存放方式, 是否是压扁为一维数组连续存放的)
void parr3(void* p,unsigned int row_max,unsigned int row_count,unsigned int height){
	int tmp,tmp2,tmp3;
	char* pc = (char*)p;
	for(tmp=0;tmp<height;tmp++)
		for(tmp2=0;tmp2<row_count;tmp2++)
			for(tmp3=0;tmp3<row_max;tmp3++)
				if(*pc != 'A')
					printf("err: %d-%d-%d-%c\n",tmp,tmp2,tmp3,*pc);
}



int main(void){
	unsigned int row_max=64,row_count=64,height=64;
	static char c2[64][64];					//测试字符类型
	static char c3[64][64][64];
	static int i2[64][64];					//测试整形
	static int i3[64][64][64];
	static double d2[64][64];				//测试浮点
	static double d3[64][64][64];
	static void* v2[64][64];				//测试void* 类型
	static void* v3[64][64][64];

	static char copy_c2[64][64];
	static char copy_c3[64][64][64];
	static int copy_i2[64][64];
	static int copy_i3[64][64][64];
	static double copy_d2[64][64];
	static double copy_d3[64][64][64];
	static void* copy_v2[64][64];
	static void* copy_v3[64][64][64];

	static char copy_n_c2[64][64];
	static char copy_n_c3[64][64][64];
	static int copy_n_i2[64][64];
	static int copy_n_i3[64][64][64];
	static double copy_n_d2[64][64];
	static double copy_n_d3[64][64][64];
	static void* copy_n_v2[64][64];
	static void* copy_n_v3[64][64][64];



	//源二维,三维数组, 全部初始化'A'
	memsetEx2(&c2,65,row_max,row_count);
	memsetEx2(&i2,'A',row_max,row_count);
	memsetEx2(&d2,'A',row_max,row_count);
	memsetEx2(&v2,'A',row_max,row_count);
	memsetEx3(&c3,'A',row_max,row_count,height);
	memsetEx3(&i3,'A',row_max,row_count,height);
	memsetEx3(&d3,'A',row_max,row_count,height);
	memsetEx3(&v3,'A',row_max,row_count,height);

	//目标二维,三维数组, 全部初始化'B'
	memsetEx2(&copy_c2,'B',row_max,row_count);
	memsetEx2(&copy_i2,'B',row_max,row_count);
	memsetEx2(&copy_d2,'B',row_max,row_count);
	memsetEx2(&copy_v2,'B',row_max,row_count);
	memsetEx3(&copy_c3,'B',row_max,row_count,height);
	memsetEx3(&copy_i3,'B',row_max,row_count,height);
	memsetEx3(&copy_d3,'B',row_max,row_count,height);
	memsetEx3(&copy_v3,'B',row_max,row_count,height);

	//目标n 维数组, 全部初始化'B'
	memsetExn(&copy_n_c2,'B',2,row_max,row_count);
	memsetExn(&copy_n_i2,'B',2,row_max,row_count);
	memsetExn(&copy_n_d2,'B',2,row_max,row_count);
	memsetExn(&copy_n_v2,'B',2,row_max,row_count);
	memsetExn(&copy_n_c3,'B',3,row_max,row_count,height);
	memsetExn(&copy_n_i3,'B',3,row_max,row_count,height);
	memsetExn(&copy_n_d3,'B',3,row_max,row_count,height);
	memsetExn(&copy_n_v3,'B',3,row_max,row_count,height);


	//执行拷贝
	memcpyEx2(&copy_c2,&c2,row_max,row_count);
	memcpyEx2(&copy_i2,&i2,row_max,row_count);
	memcpyEx2(&copy_d2,&d2,row_max,row_count);
	memcpyEx2(&copy_v2,&v2,row_max,row_count);
	memcpyEx3(&copy_c3,&c3,row_max,row_count,height);
	memcpyEx3(&copy_i3,&i3,row_max,row_count,height);
	memcpyEx3(&copy_d3,&d3,row_max,row_count,height);
	memcpyEx3(&copy_v3,&v3,row_max,row_count,height);

	memcpyExn(&copy_n_c2,&c2,2,row_max,row_count);
	memcpyExn(&copy_n_i2,&i2,2,row_max,row_count);
	memcpyExn(&copy_n_d2,&d2,2,row_max,row_count);
	memcpyExn(&copy_n_v2,&v2,2,row_max,row_count);
	memcpyExn(&copy_n_c3,&c3,3,row_max,row_count,height);
	memcpyExn(&copy_n_i3,&i3,3,row_max,row_count,height);
	memcpyExn(&copy_n_d3,&d3,3,row_max,row_count,height);
	memcpyExn(&copy_n_v3,&v3,3,row_max,row_count,height);


	//校验结果
	parr2(&c2,row_max,row_count);
	parr2(&i2,row_max,row_count);
	parr2(&d2,row_max,row_count);
	parr2(&v2,row_max,row_count);
	parr3(&c3,row_max,row_count,height);
	parr3(&i3,row_max,row_count,height);
	parr3(&d3,row_max,row_count,height);
	parr3(&v3,row_max,row_count,height);

	parr2(&copy_c2,row_max,row_count);
	parr2(&copy_i2,row_max,row_count);
	parr2(&copy_d2,row_max,row_count);
	parr2(&copy_v2,row_max,row_count);
	parr3(&copy_c3,row_max,row_count,height);
	parr3(&copy_i3,row_max,row_count,height);
	parr3(&copy_d3,row_max,row_count,height);
	parr3(&copy_v3,row_max,row_count,height);

	parr2(&copy_n_c2,row_max,row_count);
	parr2(&copy_n_i2,row_max,row_count);
	parr2(&copy_n_d2,row_max,row_count);
	parr2(&copy_n_v2,row_max,row_count);
	parr3(&copy_n_c3,row_max,row_count,height);
	parr3(&copy_n_i3,row_max,row_count,height);
	parr3(&copy_n_d3,row_max,row_count,height);
	parr3(&copy_n_v3,row_max,row_count,height);

	return 0;
}
