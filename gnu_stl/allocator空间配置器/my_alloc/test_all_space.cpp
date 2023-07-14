//编译:
//		g++ -g3 ./oneSpace.h ./twoSpace.h ./alloc.h ./test_all_space.cpp -o x



#include<iostream>
#include<vector>
#include"twoSpace.h"
#include"alloc.h"



class AA{
public:
	//使用默认构造函数, 默认析构函数[等于弃用]
	AA(){}
	~AA(){}
	//取代构造函数
	void init(void){
		a = 999;
		b = -1;
	}
	//取代析构函数
	void destory(void){
		std::cout << a << b << std::endl;
	}
private:
	int a;
	int b;
};



//测试my_alloc[alloc.h]
void test_my_alloc(void){
	my_alloc<int> space;
	my_alloc<AA> sp;
	my_alloc<std::vector<int>> sp_vec;

	int* arr = space.allocate(10);
	int* oneArr = space.allocate();

	//class AA 测试
	AA* pAA = sp.allocate();
	sp.construct(pAA);
	sp.destroy(pAA);

	//std::vector 测试
	std::vector<int>* pvec = sp_vec.allocate();
	sp_vec.construct(pvec);
	sp_vec.destroy(pvec);



	//int 数组测试
	arr[8] = 10;
	std::cout << sizeof(arr) / sizeof(arr[0]) << std::endl;
	*oneArr = 3;
	std::cout << sizeof(oneArr) / sizeof(oneArr[0]) << std::endl;

	space.deallocate(oneArr);
	space.deallocate(arr, 10);

	return;
}


int main(void){
	twoSpace m_twoSpace;
	int n;
	void* p;
	AA *pAA;
	std::vector<int> *pvec;

	#ifdef __USE_MALLOC
		std::cout << "__USE_MALLOC yes\n" << std::endl;
	#else
		//默认情况下, SGI_STL使用二级空间配置器, __USE_MALLOC = no[没有这个宏]
		std::cout << "__USE_MALLOC no\n" << std::endl;
	#endif



	//使用优化后的mallocEx && freeEx, 创建普通buf缓冲区
	for(n=0;n<99999;n++){
		p = m_twoSpace.mallocEx(1);
		m_twoSpace.freeEx(p,1);
	}
	//std::cout << "print info:" << std::endl;
	//m_twoSpace.print_pool_info();



	//使用优化后的mallocEx && freeEx, 创建自定义类
	for(n=0;n<99999;n++){
		pAA = (AA*)m_twoSpace.mallocEx(sizeof(AA));
		pAA->init();
		pAA->destory();
		m_twoSpace.freeEx(pAA,sizeof(AA));
	}
	//std::cout << "print info:" << std::endl;
	//m_twoSpace.print_pool_info();



	//使用优化后的mallocEx && freeEx, 创建stl 容器
	for(n=0;n<99999;n++){
		pvec = (std::vector<int>*)m_twoSpace.mallocEx(sizeof(std::vector<int>));
		//调用构造函数失败, 导致容器不能正常使用, 
		//一调用函数就溢出(gnu stl 的默认空间配置器, 不知道是怎样用的, 也不知道怎样替换)
		//pvec->vector();
		//pvec->push_back(1);
		//pvec->~vector();

		//正确的调用方法:
		pvec = new(pvec) std::vector<int>;
		pvec->push_back(1);
		pvec->~vector();
		m_twoSpace.freeEx(pvec,sizeof(std::vector<int>));
	}
	//std::cout << "print info:" << std::endl;
	//m_twoSpace.print_pool_info();





	//测试my_alloc[alloc.h]
	test_my_alloc();

	return 0;
}
