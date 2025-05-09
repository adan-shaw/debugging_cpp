//编译:
//		gcc -g3 ./FILE文件流seek+io读写.c -o x 


//游标
/*
	* 移动游标(所有的fgets,fputs,fprintf,fwrite等函数都会移动pos游标)
	* 如果读到文件末尾返回EOF, EOF其实就是一个宏: #define EOF (-1)
*/

//fread()/fwrite() 粒度控制(防止宽字符, 窄字符问题)
/*
	第二个参数控制粒度:
		窄字符:
			char buf=""; sizeof(char); strlen(buf);
		宽字符:
			wchar_t buf=L""; sizeof(wchar_t); wcslen(buf);
*/



#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>



//FILE* 'tmpfile文件流'的pos 游标移动
void fio_basic_seek(void){
	FILE* pfs;
	int pos,tmp;

	//创建一个tmpfile FILE*文件流
	pfs = tmpfile();
	if(pfs == NULL){
		perror("tmpfile()");
		return ;
	}

	//显示当前的文件游标位置(long int 类型)
	printf("ftell() = %ld\n",ftell(pfs));

	//快速重置到SEEK_SET(文件头)
	rewind(pfs);
	fputc('x',pfs);					//写入一个字符, pos游标自动向后移动1bit
	tmp=fgetc(pfs);					//读取一个字符, pos游标自动向后移动1bit
	if(tmp == EOF)
		printf("fgetc() failed(EOF)\n");
	else
		printf("fgetc(): %d\n",(char)tmp);

	//相位pos游标(以fromwhere为起点, 偏移offset个字节的位置)
	/*
		int fseek(FILE *stream, long offset, int fromwhere);
		fromwhere:
			SEEK_SET = 0
			SEEK_CUR = 1
			SEEK_END = 2
	*/
	fseek(pfs,1,SEEK_CUR);	//从当前位置开始,向后移动1 bit
	fseek(pfs,-1,SEEK_END);	//从文件尾部开始,移动1bit(即向文件头方向移动)

	fclose(pfs);
	return ;
}



void fio_basic(void){
	char buf[64];
	FILE* pfs;
	int tmp;

	memset(buf,'\0',64);
	strncpy(buf,"i am adan shaw!!!!",64);

	//创建一个tmpfile FILE*文件流
	pfs = tmpfile();
	if(pfs == NULL){
		perror("tmpfile()");
		return ;
	}

	fseek(pfs,0,SEEK_END);
	tmp = strlen(buf)+1;
	tmp = fwrite(buf, sizeof(char), tmp, pfs);
	printf("fwrite() 实际写入 = %d\n", tmp);

	memset(buf,'\0',64);
	//fread() 不会自动添加'\0' 字符串结束符号
	rewind(pfs);
	tmp = fread(buf, sizeof(char), sizeof(buf), pfs);
	printf("fread() 实际读出 = %d, buf = %s\n", tmp, buf);

	//冲刷io 流
	fflush(pfs);						//冲刷指定的pfs流
	fflush(NULL);						//冲刷进程内所有的流(包括stdin,stdout全部冲掉)

	fclose(pfs);
	return ;
}









int main(void){
	fio_basic_seek();
	fio_basic();
	return 0;
}



//文件在磁盘中的存储位置查询&修改(用得很少, 略)
void disk_pos(FILE* pfs){
	fpos_t fpos;

	//fgetpos() 获取file 在文件系统中的存储位置(磁盘的起始头地址 or file system的逻辑位置)
	if(fgetpos(pfs, &fpos) == -1){
		perror("fsetpos()");
		return;
	}
	else
		printf("fgetpos()= %d\n",fpos.__pos);

	//fsetpos() 移动file 到文件系统中的位置
	/*
		必须先fgetpos() 获取fpos_t 结构体, 
		然后才有fpos_t 结构体, 供你修改;
		(不用自己填充多余的参数, 只改__pos 即可完成完成移动)
	*/
	fpos.__pos+=8;//移动8 bit
	if(fsetpos(pfs, &fpos) == -1){
		perror("fsetpos()");
		return;
	}
}




