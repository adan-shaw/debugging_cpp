//编译:
//		g++ -g3 ./malloc强行调用构造函数.cpp -o x 

#include <stdio.h>
#include <stdlib.h>
#include <map>

int main(void){
	std::map<int,int>::iterator it;
	std::map<int,int>* pmap = (std::map<int,int>*)malloc(sizeof(std::map<int,int>));//强行申请类内存

	new (pmap) std::map<int,int>;		//调用构造函数placement-new 方法

	//遍历测试: 证明构造函数已生效, 类可以正常使用
	pmap->insert({1,1});
	pmap->insert({2,1});
	pmap->insert({3,1});
	for(it = pmap->begin(); it != pmap->end(); it++)
		printf("%d,%d\n", it->first, it->second);

	pmap->~map();										//调用析构函数
	free(pmap);											//释放类内存
	return 0;
}
