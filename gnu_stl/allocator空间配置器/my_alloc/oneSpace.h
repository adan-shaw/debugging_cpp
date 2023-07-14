#pragma once

#include<iostream>

//使用标准gnu malloc(),free() 函数来分配内存
#include<cstdlib>

//usleep(), 单位:微秒
#include <unistd.h>



//malloc(), free(), realloc()的sys api指定(可切换为jemalloc)
#define __MY_MALLOC malloc
#define __MY_FREE free
#define __MY_REALLOC realloc

// 进程分配内存失败: 内存不足[oom = out of memory]
#define __MY_ALLOC_FAILED { std::cerr<<"out of memory"<< std::endl; exit(-1); }

//是否开启oom处理例程oom_handler()函数
#define __MY_OOM_HANDLER 1

//防止一旦出现'oom内存不足'事件, 系统会因为频繁调用malloc 而卡死, 设置sleep 时间(弃用, 不做这么复杂的)
#define __MY_OOM_SLEEP 1
#define __MY_OOM_SLEEP_HOW_LONG 1000





class oneSpace{
private:
	//申请内存时, oom内存不足
	void* oomMalloc(size_t);
	//重新分配内存时, oom内存不足(一般是扩大内存时失败, 才会是这种情况)
	void* oomRealloc(void* , size_t);
	#ifdef __MY_OOM_HANDLER
		//oom 处理例程的函数指针[可修改函数指针, 实现更换自定义oom处理例程]
		void(*oom_handler)();
	#endif



public:
	oneSpace(){};
	~oneSpace(){};

	// 一级空间配置器的malloc封装
	inline void* mallocEx(size_t n){
		#ifdef __MY_OOM_HANDLER
			void* ptmp = __MY_MALLOC(n);
			if(NULL == ptmp)
				ptmp = oomMalloc(n);
			return ptmp;
		#else
			return __MY_MALLOC(n);
		#endif
	}

	// 一级空间配置器的free封装
	inline void freeEx(void* p){
		__MY_FREE(p);
	}

	// 一级空间配置器的realloc封装
	inline void* reallocEx(void* p, size_t new_size){
		#ifdef __MY_OOM_HANDLER
			void* ptmp = __MY_REALLOC(p, new_size);
			if(NULL == ptmp)
				ptmp = oomRealloc(p, new_size);
			return ptmp;
		#else
			return __MY_REALLOC(p, new_size);
		#endif
	}
};



//申请内存时, oom内存不足
void* oneSpace::oomMalloc(size_t n){
	#ifdef __MY_OOM_HANDLER
		//this->oom_handler()
		void(*poom_handler)() = oom_handler;
		void* ptmp;
		for(;;){
		//调用oom处理例程
		(*poom_handler)();
		//再次尝试设置内存 
		ptmp = __MY_MALLOC(n);
		if(ptmp)
			return(ptmp);
		#ifdef __MY_OOM_SLEEP
			usleep(__MY_OOM_SLEEP_HOW_LONG);
		#endif
		}
	#else
		//不调用oom处理例程, 直接终止程序
		__MY_ALLOC_FAILED;
	#endif
}



//重新分配内存时, oom内存不足(一般是扩大内存时失败, 才会是这种情况)
void* oneSpace::oomRealloc(void* p, size_t n){
	#ifdef __MY_OOM_HANDLER
		//this->oom_handler()
		void(*poom_handler)() = oom_handler;
		void* ptmp;
		for(;;){
		//调用oom处理例程
		(*poom_handler)();
		//再次尝试设置内存 
		ptmp = __MY_REALLOC(p, n);
		if(ptmp)
			return(ptmp);
		#ifdef __MY_OOM_SLEEP
			usleep(__MY_OOM_SLEEP_HOW_LONG);
		#endif
		}
	#else
		//不调用oom处理例程, 直接终止程序
		__MY_ALLOC_FAILED;
	#endif
}



//int main(void){return 0;}
