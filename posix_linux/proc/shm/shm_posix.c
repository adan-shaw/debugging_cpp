//编译:
//		gcc -g3 -lrt ./shm_posix.c -o x 

#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>



#define SHM_POSIX_FILE_PATH "/x"     //chroot()默认在'/'根目录,不能修改,修改需要root权限,还会报错;
#define SHM_POSIX_FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)
#define MMAP_SHARED_FILE_SIZE (4096) //mmap 映射总长

//mmap 映射数据帧
struct mmap_frame{
	int a;
	unsigned int b;
	float c;
	double d;
	char buf[32];
};
typedef struct mmap_frame mmap_frame_t;



void shm_posix_test(void){
	int shmfd,tmp;
	void* pmap;
	mmap_frame_t m_frame;
	mmap_frame_t* pframe;

	//创建shm_posix 内存文件, 得到shmfd
	shmfd = shm_open(SHM_POSIX_FILE_PATH, O_CREAT|O_EXCL|O_RDWR, SHM_POSIX_FILE_MODE);
	if(shmfd == -1){
		perror("shm_open");
		return;
	}

	//mmap不能映射空文件,如果是空文件/新创建的文件,先往里面填充MMAP_SHARED_FILE_SIZE 的数据
	tmp = lseek(shmfd, 0, SEEK_END);
	if(tmp == 0)
		ftruncate(shmfd, MMAP_SHARED_FILE_SIZE);

	pmap = mmap(NULL, MMAP_SHARED_FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
	if(pmap == MAP_FAILED){
		perror("mmap");
		shm_unlink(SHM_POSIX_FILE_PATH);
		return;
	}

	m_frame.a = -1;
	m_frame.b = 999;
	m_frame.c = 1.1;
	m_frame.d = 9.9;										//填充mmap_frame_t结构体
	strncpy(m_frame.buf, "hello fucker, i am your 朋友\0", 32);

	memcpy(pmap,&m_frame,sizeof(mmap_frame_t));

	pframe = (mmap_frame_t*)pmap;				//打印shm_posix 的内容
	printf("pmap: {%d,%d,%f,%lf,%s}\n",pframe->a,pframe->b,pframe->c,pframe->d,pframe->buf);

	munmap(pmap, MMAP_SHARED_FILE_SIZE);//释放mmap 映射
	shm_unlink(SHM_POSIX_FILE_PATH);		//释放shm_posix 内存文件(根据路径来释放)
	return;
}

int main(void){
	shm_posix_test();
	return 0;
}



//shm_posix 函数原型:
/*
	#include <sys/mman.h>
	#include <sys/stat.h>
	#include <fcntl.h>

	int shm_open(const char *name, int oflag, mode_t mode);
			name:  文件名
			oflag: O_CREAT|O_EXCL|O_RDWR, (O_CREAT|O_EXCL|O_RDONLY 只读访问用的比较少)
			mode:  S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH
			成功返回fd, 失败返回-1;

	int shm_unlink(const char *name);
			成功返回0, 失败返回-1;

	编译选项: -lrt
*/

//shm_posix 简介
/*
	相当于创建了一个内存文件(内存块), shm_posix 只提供创建和释放的API, 创建之后只有一个文件fd;
	具体提取操作, 还是需要用mmap把这个内存文件(内存块)fd, 映射到虚拟内存地址上, 再进行指针操作;
	ps: mmap 进行内存映射, 可以把fd 映射成void* 指针;

	shm_posix, 受进程chroot() 约束!!
	如果你使用root+chroot(), 尝试修改chroot(), 修改成功之后, debian 马上会拒绝报错:
		shm_unlink(): Function not implemented (功能未实现)
		shm_open(): Function not implemented
	因此, 暂时不要逃脱chroot(), 这可能是出于对系统安全的考虑, 故意不让你修改chroot()的;
*/
