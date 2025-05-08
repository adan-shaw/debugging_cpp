//(这个统一项目出发点是好的, 但在实施的时候, 遭到ibm intel 等公司的抛弃, debug 很多;
//详情你可以阅读一下关于posix aio 的吐嘈文章, 现在这个异步方案已经被抛弃了.)

//posix 异步io api 接口:
/*
用途:
	极大提高程序io 性能, 但是并不是有序的io 接口函数,
	这样写出来的程序, 只要磁盘不忙就会不停写磁盘, 直到出现读写极限(延时)
	也就是现在大部分USB3.0 的垃圾U 盘的工作原理,
	但是一般不能长时间这样写盘的, 服务器程序需要一个有序的写盘过程.
	所以其实同步写盘 + flush() 操作就足够了
	同步 + flush() 比posix_async_io() 还要慢一点:
	同步 + flush() 写盘操作, 一旦执行就会马上写内存,
	posix_async_io() 则马上返回, 等待设备(磁盘)空闲的时候自动执行;
*/



//头文件
#include <aio.h>
#include <fcntl.h>;
#include <signal.h>;
#include <time.h>;
//#include <sys/types.h>;


//结构提
struct aiocb aio_info;
//结构提成员
/*
	//int             aio_fildes     File descriptor.
	//off_t           aio_offset     File offset.
	//volatile void  *aio_buf        Location of buffer.
	//size_t          aio_nbytes     Length of transfer.
	//int             aio_reqprio    Request priority offset.
	//struct sigevent aio_sigevent   Signal number and value.
	//int             aio_lio_opcode Operation to be performed.
*/


//api
int      aio_cancel(int, struct aiocb *);
int      aio_error(const struct aiocb *);
int      aio_fsync(int, struct aiocb *);
int      aio_read(struct aiocb *);
ssize_t  aio_return(struct aiocb *);
int      aio_suspend(const struct aiocb *const [], int, const struct timespec *);
int      aio_write(struct aiocb *);
int      lio_listio(int, struct aiocb *const [], int, struct sigevent *);

