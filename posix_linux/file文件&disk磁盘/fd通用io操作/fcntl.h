//int fcntl(int fd, int cmd, ... );
/*
	fcntl() 功能简单目录:
		*1.复制一个现有的描述符			(cmd = F_DUPFD)
		*2.获得／设置文件状态标记		(cmd = F_GETFL / F_SETFL)
		*3.获得／设置异步I/O所有权	(cmd = F_GETOWN / F_SETOWN)
		*4.获得／设置记录锁				(cmd = F_GETLK / F_SETLK / F_SETLKW) [参考: fcntl_lock4fd.c]

	F_DUPFD 返回一个如下描述的(文件)描述符:
		(1)最小的大于或等于arg的一个可用的描述符
		(2)与原始操作符一样的某对象的引用
		(3)如果对象是文件(file)的话,返回一个新的描述符,
			 这个描述符与arg共享相同的偏移量(offset)
		(4)相同的访问模式(读,写或读/写)
		(5)相同的文件状态标志(如:两个文件描述符共享相同的状态标志)
		(6)与新的文件描述符结合在一起的close-on-exec标志,
			 被设置成交叉式访问execve(2)的系统调用

	F_GETFL 取得fd的文件状态标志,如同下面的描述一样(arg被忽略)

	F_SETFL 设置给arg描述符状态标志,可以更改的几个标志是: O_APPEND, O_NONBLOCK, O_SYNC和O_ASYNC.

	**异步所有权** (不知道如何操作)
	F_GETOWN 取得当前正在接收SIGIO或者SIGURG信号的进程id或进程组id,
					 进程组id返回成负值(arg被忽略)

	F_SETOWN 设置将接收SIGIO和SIGURG信号的进程id或进程组id,
					 进程组id通过提供负值的arg来说明,否则,arg将被认为是进程id
*/

//设置fd nonblocking								[对fcntl()的F_GETFL返回值进行'| 或运算'=新增]
#define __set_nonblocking(sfd) {fcntl(sfd, F_SETFL, fcntl(sfd,F_GETFL,0)|O_NONBLOCK);}
//#define __set_nonblocking(sfd) {if(fcntl(sfd, F_SETFL, fcntl(sfd,F_GETFL,0)|O_NONBLOCK) == -1) perror("fcntl()");}

//设置fd blocking(取消nonblocking)	[对fcntl()的F_GETFL返回值进行'& 与运算'=削减]
#define __set_blocking(sfd) {fcntl(sfd, F_SETFL, fcntl(sfd,F_GETFL,0)&O_NONBLOCK);}
//#define __set_nonblocking(sfd) {if(fcntl(sfd, F_SETFL, fcntl(sfd,F_GETFL,0)&O_NONBLOCK) == -1) perror("fcntl()");}

//检查fd 是否alive[返回-1, 表示dead; 其他值都是alive]
#define __check_fd_alive(sfd) (fcntl(sfd, F_GETFL))

//等价于dup()
#define __dupfd(sfd) (fcntl(sfd, F_DUPFD, 0))

//关闭close-on-exec
#define __close-on-exec-off(sfd) (fcntl(sfd, F_SETFD, 0))
//开启close-on-exec
#define __close-on-exec-on(sfd) (fcntl(sfd, F_SETFD, 1))



//关于close-on-exec标志声明:
/*
	F_GETFD 取得与文件描述符fd联合close-on-exec标志, 类似FD_CLOEXEC.
					如果返回值和FD_CLOEXEC进行与运算结果是0的话,
					文件保持交叉式访问exec(), 否则如果通过exec()运行的话,
					文件将被关闭(arg被忽略)

	F_SETFD 设置close-on-exec旗标.
					该旗标以参数arg的FD_CLOEXEC位决定.


	fork()不是exec()!! fork()是写时cp,写时dup.
	言外之意就是,如果你不写,fd 永远都一直只有一份!!无需要close-on-exec.
	fork()之后无需操作任何fd,你操作了反而会dup多一份,操作就要自己手动关闭.

	close-on-exec 标志是给exec() 新建全新进程系列函数使用的,
	如果创建一个全新的函数肯定会涉及<资源争夺>, 关闭了比较好.
	但fork(), 你不去动fd 资源, 根本什么事情都不会发生, 一点资源争夺都没有.

	这个操作少用, 可以忽略;
	fcntl(fd, F_SETFD, true);		//启动close-on-exec
	fcntl(fd, F_SETFD, false);	//关闭close-on-exec
*/



//
//<< fcntl() 的宏定义(英文版from man手册) >>
//
//int fcntl(int fd, int cmd, ... /* arg */ );
/*
	F_DUPFD - Duplicate file descriptor.
						[复制并返回新的文件描述符,与dup 类似,passed!!]

	*FD_CLOEXEC - Close the file descriptor upon execution of
							 an exec family function.
							 [只要fork(),父进程就关闭自己的文件描述符,即close-on-exec]

	*F_DUPFD_CLOEXEC - Duplicate file descriptor with
										the close-on-exec flag FD_CLOEXEC set.
										[只要复制之后, 就关闭原文件描述符]

	F_GETFD  - Get file descriptor flags.
	F_SETFD  - Set file descriptor flags.
	F_GETFL  - Get file status flags and file access modes.
	F_SETFL  - Set file status flags.
	F_GETLK  - Get record locking information.
	F_SETLK  - Set record locking information.
	F_SETLKW - Set record locking information; wait if blocked.
	F_GETOWN - Get process or process group ID to
						 receive SIGURG signals.
	F_SETOWN - Set process or process group ID to
						 receive SIGURG signals.

	//宏定义for fcntl() 锁
	F_RDLCK  - Shared or read lock.[读文件锁-可分享, 多个]
	F_UNLCK  - Unlock.[解文件锁]
	F_WRLCK  - Exclusive or write lock.[写文件锁-唯一, 可保证进程唯一]

	//宏定义for fcntl() seek()
	SEEK_SET [文件头]
	SEEK_CUR [当前文件位置]
	SEEK_END [文件尾]

	//宏定义for open() and openat().
	O_CLOEXEC		- The FD_CLOEXEC flag associated with
								the new descriptor shall be set to
								close the file descriptor upon execution of
								an exec family function.
	O_CREAT			- Create file if it does not exist.[当该文件不存在时创建]
	O_DIRECTORY - Fail if file is a non-directory file.
	O_EXCL			- Exclusive use flag.
								[O_CREAT和O_EXCL同时设置, 当文件已存在时, open()失败,
								保证不破坏已存在的文件]
	O_NOCTTY		- Do not assign controlling terminal.
	O_NOFOLLOW	- Do not follow symbolic links.
	O_TRUNC			- Truncate flag.
	O_TTY_INIT	- Set the termios structure terminal parameters to
								a state that provides conforming behavior;

	//宏定义for open(), openat(), and fcntl() -- 异步同步控制
	O_APPEND		- Set append mode.[追加模式]
	O_NONBLOCK	- Non-blocking mode.[异步模式]
	O_DSYNC			- Write according to synchronized I/O data integrity completion.
								[同步到disk-写]
	O_RSYNC			- Synchronized read I/O operations. [同步到disk-读]
	O_SYNC			- Write according to synchronized I/O file integrity completion.
								[同步读写模式]

	//动态访问设备
	O_ACCMODE		- Mask for file access modes.

	//权限控制
	O_EXEC			- Open for execute only (non-directory files).
								The result is unspecified if this flag is applied to a directory.[可执行]
	O_RDONLY		- Open for reading only.[只读]
	O_RDWR			- Open for reading and writing.[读写]
	O_SEARCH		- Open directory for search only.
								The result is unspecified if this flag is applied to a non-directory file.
								[允许可以搜索, 仅文件夹有效]
	O_WRONLY		- Open for writing only.[只写]


	AT_FDCWD		- Use the current working directory to determine the target of relative file paths.
								[以当前目录作为相对路径的参照物, 即'.']
	AT_EACCESS	- Check access using effective user and group ID.
								[检查用户对文件的访问权]

	AT_SYMLINK_NOFOLLOW - Do not follow symbolic links.
	AT_SYMLINK_FOLLOW		- Follow symbolic link.


	//宏定义for posix_fadvise():
	POSIX_FADV_DONTNEED		- The application expects that it will not access the specified data in the near future.
	POSIX_FADV_NOREUSE		- The application expects to access
													the specified data once and then not reuse it thereafter.
	POSIX_FADV_NORMAL			- The application has no advice to give on its behavior with respect to the specified data.
													It is the default characteristic if no advice is given for an open file.
	POSIX_FADV_RANDOM			- The application expects to access the specified data in a random order.
	POSIX_FADV_SEQUENTIAL - The application expects to access the specified data sequentially from lower offsets to higher offsets.
	POSIX_FADV_WILLNEED		- The application expects to access the specified data in the near future.
*/


