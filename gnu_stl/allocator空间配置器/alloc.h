#pragma once

#include"twoSpace.h"



#ifdef __USE_MALLOC
	typedef __malloc_alloc_template<0> malloc_alloc;
	typedef malloc_alloc twoSpace;
#else
	//与SGI STL 接轨, 将__default_alloc_template<__NODE_ALLOCATOR_THREADS, 0> 重命名为twoSpace
	typedef __default_alloc_template<__NODE_ALLOCATOR_THREADS, 0> twoSpace;
#endif  !__USE_MALLOC



// T:        元素类型
// twoSpace: 空间配置器
// 注意:      该类只负责申请与归还对象的空间, 不否则空间中对象的构造与析构
template <class T, class twoSpace> class my_alloc{
	public:
		// 申请n个T类型对象大小的空间
		inline T *allocate(size_t n){
			return 0 == n ? 0 : (T*)twoSpace::allocate(n * sizeof(T));
		}
		// 申请一个T类型对象大小的空间
		inline T *allocate(void){
			return (T*)twoSpace::allocate(sizeof(T));
		}
		// 释放n个T类型对象大小的空间
		inline void deallocate(T *p, size_t n){
			if(0 != n)
				twoSpace::deallocate(p, n * sizeof(T));
		}
		// 释放一个T类型对象大小的空间
		inline void deallocate(T *p){
			twoSpace::deallocate(p, sizeof(T));
		}

		// 归还空间时, 先先调用该函数将对象中资源清理掉
		template <class T> inline void destroy(T* pointer){
			pointer->~T();
		}

		// 空间申请好后调用该函数: 利用placement-new完成对象的构造
		template <class T1, class T2> inline void construct(T1* p, const T2& value){
			new (p) T1(value);
		}
};



/*
void test_my_alloc(void){
	my_alloc<int, __default_alloc_template> space;
	int* arr = space.allocate(10);
	int* oneArr = space.allocate();

	arr[8] = 10;
	std::cout << sizeof(arr) / sizeof(arr[0]) << std::endl;
	*oneArr = 3;
	std::cout << sizeof(oneArr) / sizeof(oneArr[0]) << std::endl;

	space.deallocate(oneArr);
	space.deallocate(arr, 10);
}
*/
