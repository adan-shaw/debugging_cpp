#pragma once

#include"twoSpace.h"



// T:        元素类型
// twoSpace: 空间配置器
// 注意:      该类只负责申请与归还对象的空间, 不否则空间中对象的构造与析构
template <class T> class my_alloc{
public:
	my_alloc(){};
	~my_alloc(){};

	// 申请n个T类型对象大小的空间
	inline T *allocate(size_t n){
		return 0 == n ? 0 : (T*)m_twoSpace.mallocEx(n * sizeof(T));
	}
	// 申请一个T类型对象大小的空间
	inline T *allocate(void){
		return (T*)m_twoSpace.mallocEx(sizeof(T));
	}
	// 释放n个T类型对象大小的空间
	inline void deallocate(T *p, size_t n){
		if(0 != n)
			m_twoSpace.freeEx(p, n * sizeof(T));
	}
	// 释放一个T类型对象大小的空间
	inline void deallocate(T *p){
		m_twoSpace.freeEx(p, sizeof(T));
	}

	// 归还空间时, 先先调用该函数将对象中资源清理掉
	inline void destroy(T* p){
		p->~T();
	}

	// 空间申请好后调用该函数: 利用placement-new完成对象的构造
	inline void construct(T* p){
		new (p) T;
	}

private:
	twoSpace m_twoSpace;
};



//测试my_alloc[alloc.h]
/*
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
*/
