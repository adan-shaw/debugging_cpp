#include <stdio.h>

//c的引用后裔: 超级节省内存, 指针变量地址都省了(穿戴设备可能会这样用)
int main(void){
	int a[4] = {1,2,3,4};
	printf("%d\n",&a+1);//内存地址值
	printf("%p\n",&a+1);//内存地址值
	return 0;
}
