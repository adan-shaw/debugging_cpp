//编译:
//		gcc -g3 mmap.h ./mmap匿名共享_w.c -o xw ; gcc -g3 mmap.h ./mmap匿名共享_r.c -o xr 



#include "mmap.h"
#include <string.h>
#include <fcntl.h>



#define MMAP_SHARED_FILE "mmap_shared.file"	//mmap 映射绑定的文件名
#define MMAP_SHARED_FILE_SIZE (4096)				//mmap 映射总长



int main(void){
	int fd, tmp;
	char* pmap;
	mmap_frame_t m_frame;

	pmap = (char*)mmap(NULL, MMAP_SHARED_FILE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if(pmap == MAP_FAILED){
		perror("mmap");
		close(fd);
		return -1;
	}

	//为了防止mmap 溢出, 必须从'tmp+第一帧'开始起跳
	for(tmp=MMAP_SHARED_FRAME_SIZE;tmp<MMAP_SHARED_FILE_SIZE;tmp+=MMAP_SHARED_FRAME_SIZE){
		memcpy(&m_frame,pmap,MMAP_SHARED_FRAME_SIZE);
		printf("a=%d,b=%d,c=%f,d=%lf,buf=%s\n",m_frame.a,m_frame.b,m_frame.c,m_frame.d,m_frame.buf);
		pmap += MMAP_SHARED_FRAME_SIZE;
	}

	munmap(pmap, MMAP_SHARED_FILE_SIZE);
	close(fd);

	return 0;
}
