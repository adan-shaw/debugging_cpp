//编译:
//	gcc -g3 -nostartfiles ./bit_oper.c bit_oper.h -e test_all_bit_oper -o x
//	gcc -g3 -nostartfiles ./bit_oper.c bit_oper.h -e easy_bit -o x



#include<stdio.h>
#include<stdlib.h>
#include"bit_oper.h"



//测试所有'常用的嵌入式-位操作宏'
void test_all_bit_oper(void){
	unsigned int tmp = 0x12345678;
	unsigned short n;



	//test GET_BIT
	printf("0x%x的第0个位为: %d\n", tmp, GET_BIT(tmp,0));
	printf("0x%x的第1个位为: %d\n", tmp, GET_BIT(tmp,1));
	printf("0x%x的第2个位为: %d\n", tmp, GET_BIT(tmp,2));
	printf("0x%x的第3个位为: %d\n", tmp, GET_BIT(tmp,3));
	printf("0x%x的第4个位为: %d\n", tmp, GET_BIT(tmp,4));
	printf("0x%x的第5个位为: %d\n", tmp, GET_BIT(tmp,5));
	printf("0x%x的第6个位为: %d\n", tmp, GET_BIT(tmp,6));
	printf("0x%x的第7个位为: %d\n", tmp, GET_BIT(tmp,7));

	//test CLEAR_BIT(由于宏实在编译器期间就执行的, 你看到的结果是: 先执行CLEAR_BIT(), 后执行printf() )
	printf("0x%x的第3个位清0后, 值变为: %d\n", tmp, CLEAR_BIT(tmp,3));

	//test SET_BIT
	printf("0x%x的第3个位置1后, 值变为: %d\n", tmp, SET_BIT(tmp,3));



	//test GET_LOW_BYTE0
	printf("0x%x的第0个位字节为: 0x%x\n", tmp, GET_LOW_BYTE0(tmp));
	printf("0x%x的第1个位字节为: 0x%x\n", tmp, GET_LOW_BYTE1(tmp));
	printf("0x%x的第2个位字节为: 0x%x\n", tmp, GET_LOW_BYTE2(tmp));
	printf("0x%x的第3个位字节为: 0x%x\n", tmp, GET_LOW_BYTE3(tmp));

	//test CLEAR_LOW_BYTE0
	printf("清0 0x%x的第0个位字节后, 值变为: 0x%x\n", tmp, CLEAR_LOW_BYTE0(tmp));
	printf("清0 0x%x的第1个位字节后, 值变为: 0x%x\n", tmp, CLEAR_LOW_BYTE1(tmp));
	printf("清0 0x%x的第2个位字节后, 值变为: 0x%x\n", tmp, CLEAR_LOW_BYTE2(tmp));
	printf("清0 0x%x的第3个位字节后, 值变为: 0x%x\n", tmp, CLEAR_LOW_BYTE3(tmp));

	//test SET_LOW_BYTE0
	printf("置1 0x%x的第0个位字节后, 值变为: 0x%x\n", tmp, SET_LOW_BYTE0(tmp));
	printf("置1 0x%x的第1个位字节后, 值变为: 0x%x\n", tmp, SET_LOW_BYTE1(tmp));
	printf("置1 0x%x的第2个位字节后, 值变为: 0x%x\n", tmp, SET_LOW_BYTE2(tmp));
	printf("置1 0x%x的第3个位字节后, 值变为: 0x%x\n", tmp, SET_LOW_BYTE3(tmp));



	//判断tmp 的第3位是否为1
	tmp = 0x68;//二进制: 01101000b
	n = 3;
	if(tmp & (1 << n)){
		printf("0x%x的第%d个位字节为: 1; do something after\n", tmp, n);
	}
	else{
		printf("0x%x的第%d个位字节为: 0; do something after\n", tmp, n);
	}

	//从右边第3位数起, 连取2位, 打印出取出的值
	n = GET_BIT_M_TO_N(tmp,2,3);
	switch(n){
		case 0x0: printf("0x%x的第[3:2]位的值为(从右边第3位数起, 连取2位):00B\n",tmp); break;
		case 0x1: printf("0x%x的第[3:2]位的值为(从右边第3位数起, 连取2位):01B\n",tmp); break;
		case 0x2: printf("0x%x的第[3:2]位的值为(从右边第3位数起, 连取2位):10B\n",tmp); break;
		case 0x3: printf("0x%x的第[3:2]位的值为(从右边第3位数起, 连取2位):11B\n",tmp); break;
		default: printf("error: default\n"); break;
	}

	exit(0);
}



//测试简单位操作(左移, 右移, 位操作:逻辑运算, 都是对大数进行位操作, 并不是一bit一bit 进行位操作, 服务器偶尔会用到)
int easy_bit(void){
	int iTmp,iVal;



	//1.位运算: 左移, 右移
	iVal = 100;
	//全部数位向右移动一位,等价于:iTmp=iVal/2
	iTmp = iVal>>1;
	printf("iVal = 100; iTmp = iVal>>1; = %d\n",iTmp);

	//全部数位向左移动二位,等价于:iTmp=iVal*2^2=iVal*4
	iTmp = iVal<<1;
	printf("iVal = 100; iTmp = iVal<<1; = %d\n",iTmp);



	//2.位运算: 逻辑运算(听说这样做的效率比if 高)
	iVal = 100;
	//与
	printf("and-0: %d\n", iVal && 0);
	//或
	printf("or-0: %d\n", iVal || 0);
	//非
	printf("not-0: %d\n", !iVal);

	iVal = 0;
	printf("0 and 0: %d\n", iVal && iVal);
	printf("1 and 1:only%d\n", 1 && 1);

	exit(0);
}



