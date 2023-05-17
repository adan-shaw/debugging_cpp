//编译:
//		g++ -g3 mt_allocator.cpp -o x



#include <ext/mt_allocator.h>
#include <cstring>
#include <iostream>



class father{
	public:
		father(int id = 0){
			memset(this, 0, sizeof(father));
			this->id = id;
		}
		void test(void){
			std::cout << "hello world you loveer, id = " << id << std::endl;
		}
	private:
		int id;
		char name[32];
};



typedef __gnu_cxx::__mt_alloc<father> mtAlloc;
typedef __gnu_cxx::__pool_base::_Tune mtAllocTune;



/*
struct _Tune{
	enum { _S_align = 8 };
	enum { _S_max_bytes = 128 };
	enum { _S_min_bin = 8 };
	enum { _S_chunk_size = 4096 - 4 * sizeof(void*) };
	enum { _S_max_threads = 4096 };
	enum { _S_freelist_headroom = 10 };

	//7 个关键参数, 初始化_Tune 只需初始化这7 个参数即可
	size_t    _M_align;							//8字节对齐
	size_t    _M_max_bytes;					//1024字节以上的内存直接用new分配
	size_t    _M_min_bin;						//可分配的最小的内存块大小32字节
	size_t    _M_chunk_size;				//每次从os申请的内存块的大小为1024字节
	size_t    _M_max_threads;				//可支持的最多的线程数是32
	size_t    _M_freelist_headroom;	//单线程能保存的空闲块的百分比为10%
	bool      _M_force_new;					//是否直接使用new和delete; 根据是否设置getenv("GLIBCXX_FORCE_NEW")
};
*/
int main(){
	mtAlloc pool;

	//创建&初始化_Tune 参数结构体
	mtAllocTune t_opt(8, 1024, 32, 1024, 32, 10, false);
	//mtAllocTune t_single(16, 5120, 32, 5120, 1, 10, false);

	//获取__mt_alloc 中的_Tune 参数结构体
	mtAllocTune t = pool._M_get_options();

	//设置_Tune 参数结构体在__mt_alloc 中生效
	pool._M_set_options(t_opt);



	// allocate 分配数据
	mtAlloc::pointer p1 = pool.allocate(sizeof( father ));
	mtAlloc::pointer p2 = pool.allocate(1024);

	//调用father() 构造函数
	pool.construct(p1, 999);

	//测试father() 构造函数, 是否已初始化
	p1->test();

	// free 释放数据
	pool.deallocate(p1, sizeof(father));
	pool.deallocate(p2, 1024);

	return 0;
}
