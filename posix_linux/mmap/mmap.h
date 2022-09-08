#include <sys/mman.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>



//关闭mmap[直接定义成宏就行,出错可能会导致内存泄漏,指针p/长度len错误,都会导致释放失败]
//void mmap_close(void* p, size_t len){};
#define mmap_close(p,len) (munmap(p,len))


//命令mmap 立刻写盘: 
//	MS_ASYNC(执行后马上返回, 只命令mmap写盘, 不等待写盘操作完成就返回)
//	MS_SYNC(等待写盘操作完成才返回)
//void mmap_sync(void* p, size_t len, int flags){};
#define mmap_sync(p,len) (msync(p,len,MS_SYNC))
#define mmap_async(p,len) (msync(p,len,MS_ASYNC))


//根据数据量, 自动决定mmap 应该分配多少个内存页
#define mmap_get_page_num(n,ret) { ret=n/4096; if((n%4096) != 0) ret+=1; }


//如果mmap 设置了MAP_SHARED 共享属性, 就可以通过查看/proc 里面的信息, 找到这个共享mmap
//如果mmap 设置了MAP_PRIVATE 私有属性, 在/proc 就看不到任何信息(私有mmap 不可查询)
#define CHECK_MMAP_SHARED_CMD "cat /proc/%ld/maps | grep zero"

//使用system() shell命令的方式, 查询打印mmap 信息(buf_cmd 理应大于等于 32bit, 绝对不能少于32bit)
#define CHECK_MMAP_SHARED(buf_cmd) { snprintf(buf_cmd, sizeof(buf_cmd), "cat /proc/%ld/maps | grep zero", (long) getpid()); system(buf_cmd); }



//mmap 帧设计
struct mmap_frame{
	int a;
	unsigned int b;
	float c;
	double d;
	char buf[32];
};

typedef struct mmap_frame mmap_frame_t;

//mmap 映射帧-节长
#define MMAP_SHARED_FRAME_SIZE (sizeof(mmap_frame_t))
