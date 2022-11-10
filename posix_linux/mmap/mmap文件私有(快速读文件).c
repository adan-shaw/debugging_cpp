//编译:
//		gcc -g3 mmap.h ./mmap文件私有\(快速读文件\).c -o x 



#include "mmap.h"
#include <errno.h>
#include <string.h>
#include <fcntl.h>



//快速将整个文件的数据读入内存, 返回一个mmap 映射-字符串指针(自动根据文件大小,创建一个匹配大小的映射区)
//后续使用strlen() 来获取文件长度, 不安全, 遇到'\0'字符时, 会返回一个错误的字符串长度, 这个字符串长度不是文件总长;
char* mmap_file_r(const char* filepath){
	int fd, len;
	char* pbuf;

	fd = open(filepath, O_RDONLY);
	if(fd == -1){
		perror("open()");
		return NULL;
	}

	//读取文件长度
	len = lseek(fd, 0, SEEK_END);

	//建立映射(mmap 为只读时, 推荐使用: MAP_PRIVATE)
	pbuf = (char*)mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, 0);
	if(pbuf == MAP_FAILED){
		perror("mmap");
		close(fd);
		return NULL;
	}
	close(fd);

	return pbuf;
}

//快速将整个文件的数据读入内存, 返回一个mmap 映射-字符串指针, 回调返回文件实际总长(可以防止文件中出现'\0'字符)
char* mmap_fileEx_r(const char* filepath, unsigned int* len){
	int fd;
	char* pbuf;

	fd = open(filepath, O_RDONLY);
	if(fd == -1){
		perror("open()");
		return NULL;
	}

	//读取文件长度
	*len = lseek(fd, 0, SEEK_END);

	//建立映射(mmap 为只读时, 推荐使用: MAP_PRIVATE)
	pbuf = (char*)mmap(NULL, *len, PROT_READ, MAP_PRIVATE, fd, 0);
	if(pbuf == MAP_FAILED){
		perror("mmap");
		close(fd);
		return NULL;
	}
	close(fd);

	return pbuf;
}



//test read
void test_r(void){
	const char* pfilepath = "./mmap.h";
	char* pr;
	unsigned int pr_len = 0;

	//读取文件中, 不能含有字符串终止符号'\0'
	pr = mmap_file_r(pfilepath);
	if(!pr)
		return;
	pr_len += strlen(pr);
	printf("***len=%d\n, %s\n",pr_len,pr);
	mmap_close(pr,pr_len);


	pr_len = 0;
	//读取文件中任意数据
	pr = mmap_fileEx_r(pfilepath, &pr_len);
	if(!pr)
		return;

	printf("***len=%d\n, %s\n",pr_len,pr);
	mmap_close(pr,pr_len);

	return;
}



int main(void){
	test_r();
	return 0;
}
