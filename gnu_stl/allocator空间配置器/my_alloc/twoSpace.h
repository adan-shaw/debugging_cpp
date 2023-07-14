#pragma once

#include"oneSpace.h"



class twoSpace{
public:
	twoSpace(){};
	~twoSpace(){
		//析构时, 不需要清空内存池(why ??)
		/*
		int tmp;
		for(tmp=0;tmp<__NFREELISTS;tmp++){
			std::cout << __NFREELISTS << "; tmp=" << tmp << "; addr=" << free_list[tmp] << std::endl;
			if(free_list[tmp] != 0){
				std::cout << "free(" << tmp << ");" << std::endl;
				m_oneSpace.freeEx(free_list[tmp]);
				free_list[tmp] = 0;
			}
		}
		*/
	};

	//函数功能: 向空间配置器索要空间
	//参数n:    用户所需空间字节数
	//返回值:   返回空间的首地址
	void* mallocEx(size_t n){
		obj* volatile * my_free_list;
		obj* result;
		//restrict 它只可以用于限定和约束指针, 并表明指针是访问一个数据对象的唯一且初始的方式.即它告诉编译器, 
		//所有修改该指针所指向内存中内容的操作都必须通过该指针来修改,而不能通过其它途径(其它变量或指针)来修改

		//检测用户所需空间释放超过128(即是否为小块内存)
		if(n > (size_t)__MAX_BYTES)
			//不是小块内存交由一级空间配置器处理
			return m_oneSpace.mallocEx(n);

		//根据用户所需字节找到对应的桶号
		my_free_list = free_list + FREELIST_INDEX(n);
		result = *my_free_list;

		//如果该桶中没有内存块时, 向该桶中补充空间
		if(result == 0)
			//将n向上对齐到8的整数倍, 保证向桶中补充内存块时, 内存块一定是8的整数倍
			return refill(ROUND_UP(n));

		//维护桶中剩余内存块的链式关系
		*my_free_list = result->free_list_link;
		return (result);
	}

	//函数功能: 用户将空间归还给空间配置器
	//参数:    p空间首地址, n空间总大小
	void freeEx(void* p, size_t n){
		obj* q = (obj*)p;
		obj** my_free_list;
		//如果空间不是小块内存, 交给一级空间配置器回收
		if(n > (size_t)__MAX_BYTES)
			m_oneSpace.freeEx(p);
		else{
			//找到对应的哈希桶, 将内存挂在哈希桶中
			my_free_list = free_list + FREELIST_INDEX(n);
			q->free_list_link = *my_free_list;
			*my_free_list = q;
		}
	}

	//函数功能: 重新分配内存(直接使用oneSpace 里面的API, 不经过内存池, 无优化, 慎用)
	inline void* reallocEx(void* p, size_t new_size){
		return m_oneSpace.reallocEx(p, new_size);
	}

	//for test only
	/*
	void print_pool_info(){
		std::cout << "__ALIGN=" << __ALIGN << std::endl;
		std::cout << "__MAX_BYTES=" << __MAX_BYTES << std::endl;
		std::cout << "__NFREELISTS=" << __NFREELISTS << std::endl;
		std::cout << "start_free=" << static_cast <const void*>(start_free) << std::endl;
		std::cout << "end_free=" << static_cast <const void*>(end_free) << std::endl;
		std::cout << "heap_size=" << heap_size << std::endl;
	}
	*/



private:
	//oneSpace 实体
	oneSpace m_oneSpace;

	//最小的小区块的大小(不是8的整数倍, 向上对齐到8的整数倍)
	enum { __ALIGN = 8 };
	//最大的区块的大小(小于128 字节就是小内存)
	enum { __MAX_BYTES = 128 };
	//free_list的个数(hash桶的个数, sgi stl 只有16 个)
	enum { __NFREELISTS = __MAX_BYTES / __ALIGN };


	//如果用户所需内存块不是8的整数倍, 向上对齐到8的整数倍
	inline size_t ROUND_UP(size_t bytes){
		return (((bytes)+__ALIGN - 1) & ~(__ALIGN - 1));
	}


	//用联合体来定义链表节点
	union obj{
		union obj* free_list_link;
		char client_data[1];
	};


	//链表节点池, 最大16个节点(初始化为0, 通过交换指针进行切换, 也就是释放时, 非0 则可释放)
	obj* free_list[__NFREELISTS] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	//start_free与end_free用来标记内存池中大块内存的起始与末尾位置
	char *start_free = 0;
	char *end_free = 0;
	//用来记录该空间配置器已向系统索要了多少的内存块
	size_t heap_size = 0;

	//哈希函数, 根据用户提供字节数找到对应的桶号
	inline size_t FREELIST_INDEX(size_t bytes){
		return (((bytes)+__ALIGN - 1) / __ALIGN - 1);
	}


	//函数功能: 向哈希桶中补充空间
	//参数n:   小块内存字节数
	//返回值:   首个小块内存的首地址
	void* refill(size_t n){
		obj** my_free_list;
		obj* result;
		obj* current_obj, *next_obj;
		int i;
		//一次性向内存池索要20个n字节的小块内存
		int nobjs = 20;
		char * chunk = chunk_alloc(n, nobjs);
		//如果只要了一块, 直接返回给用户使用
		if(1 == nobjs)
			return(chunk);

		//找到对应的桶号
		my_free_list = free_list + FREELIST_INDEX(n);

		//将第一块返回值用户, 其他块连接在对应的桶中
		//注: 此处代码逻辑比较简单, 但标准库实现稍微有点复杂, 同学们可以自己实现
		result = (obj*)chunk;
		*my_free_list = next_obj = (obj*)(chunk + n);
		for (i = 1; ; i++){
			current_obj = next_obj;
			next_obj = (obj*)((char *)next_obj + n);
			if(nobjs - 1 == i){
				current_obj->free_list_link = 0;
				break;
			}
			else
				current_obj->free_list_link = next_obj;
		}
		return(result);
	}

	char* chunk_alloc(size_t size, int& nobjs){
		char * result;
		int i;
		obj** my_free_list, *p;
		size_t bytes_to_get;
		//计算nobjs个size字节内存块的总大小以及内存池中剩余空间总大小
		size_t total_bytes = size * nobjs;
		size_t bytes_left = end_free - start_free;

		//如果内存池可以提供total_bytes字节, 返回
		if(bytes_left >= total_bytes){
			result = start_free;
			start_free += total_bytes;
			return(result);
		}
		else if(bytes_left >= size){
			//nobjs块无法提供, 但是至少可以提供1块size字节内存块, 提供后返回
			nobjs = bytes_left / size;
			total_bytes = size * nobjs;
			result = start_free;
			start_free += total_bytes;
			return(result);
		}
		else{
			//内存池空间不足, 连一块小块村内都不能提供
			//向系统堆求助, 往内存池中补充空间
			//计算向内存中补充空间大小: 本次空间总大小两倍 + 向系统申请总大小/16
			bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
			//如果内存池有剩余空间(该空间一定是8的整数倍), 将该空间挂到对应哈希桶中
			if(bytes_left > 0){
				//找对用哈希桶, 将剩余空间挂在其上
				obj** my_free_list = free_list + FREELIST_INDEX(bytes_left);
				((obj*)start_free)->free_list_link = *my_free_list;
				*my_free_list = (obj*)start_free;
			}
			//通过系统堆向内存池补充空间, 如果补充成功, 递归继续分配
			start_free = (char *)malloc(bytes_to_get);
			if(0 == start_free){
				//通过系统堆补充空间失败, 在哈希桶中找是否有没有使用的较大的内存块
				for (i = size; i <= __MAX_BYTES; i += __ALIGN){
					my_free_list = free_list + FREELIST_INDEX(i);
					p = *my_free_list;
					//如果有, 将该内存块补充进内存池, 递归继续分配
					if(0 != p){
						*my_free_list = p->free_list_link;
						start_free = (char *)p;
						end_free = start_free + i;
						return(chunk_alloc(size, nobjs));
					}
				}
				//山穷水尽, 只能向一级空间配置器求助
				//注意: 此处一定要将end_free置空, 因为一级空间配置器一旦抛异常就会出问题
				end_free = 0;
				start_free = (char*)m_oneSpace.mallocEx(bytes_to_get);
			}
			//通过系统堆向内存池补充空间成功, 更新信息并继续分配
			heap_size += bytes_to_get;
			end_free = start_free + bytes_to_get;
			return(chunk_alloc(size, nobjs));
		}
	}
};

