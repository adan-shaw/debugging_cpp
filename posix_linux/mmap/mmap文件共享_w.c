//编译:
//		gcc -g3 mmap.h ./mmap文件共享_w.c -o xw ; gcc -g3 mmap.h ./mmap文件共享_r.c -o xr 



#include "mmap.h"
#include <string.h>
#include <fcntl.h>



#define MMAP_SHARED_FILE "mmap_shared.file"	//mmap 映射绑定的文件名
#define MMAP_SHARED_FILE_SIZE (4096)				//mmap 映射总长



int main(void){
	int fd, tmp;
	char* pmap;
	mmap_frame_t m_frame;

	//主进程写, 其他进度读, 就一定是0664(其他进程是4权限, 子进程是6权限, 自己是6权限)
	fd = open(MMAP_SHARED_FILE, O_RDWR | O_CREAT, 0664);

	//mmap不能映射空文件,如果是空文件/新创建的文件,先往里面填充MMAP_SHARED_FILE_SIZE 的数据
	tmp = lseek(fd, 0, SEEK_END);
	if(tmp == 0)
		ftruncate(fd, MMAP_SHARED_FILE_SIZE);

	pmap = (char*)mmap(NULL, MMAP_SHARED_FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(pmap == MAP_FAILED){
		perror("mmap");
		close(fd);
		return -1;
	}

	//填充帧
	m_frame.a = -1;
	m_frame.b = 999;
	m_frame.c = 1.1;
	m_frame.d = 9.9;
	//帧-节长, 截断式读写, 安全(加换行'\n'和字符串终止符号'\0')
	strncpy(m_frame.buf, "hello parent, i am your 朋友\n\0", 32);

	//为了防止mmap 溢出, 必须从'tmp+第一帧'开始起跳
	for(tmp=MMAP_SHARED_FRAME_SIZE;tmp<MMAP_SHARED_FILE_SIZE;tmp+=MMAP_SHARED_FRAME_SIZE){
		memcpy(pmap,&m_frame,MMAP_SHARED_FRAME_SIZE);
		//printf("a=%d,b=%d,c=%lf,d=%lf,buf=%s\n",m_frame.a,m_frame.b,m_frame.c,m_frame.d,m_frame.buf);
		printf("tmp=%d, lseek()=%d\n", tmp, lseek(fd, 0, SEEK_END));
		pmap += MMAP_SHARED_FRAME_SIZE;
	}

	munmap(pmap, MMAP_SHARED_FILE_SIZE);
	close(fd);

	return 0;
}
