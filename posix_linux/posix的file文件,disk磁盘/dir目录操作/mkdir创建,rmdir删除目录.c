//编译:
//		gcc -g3 ./mkdir创建,rmdir删除目录.c -o x



#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <limits.h>
#include <dirent.h>
#include <stddef.h>

#include <libgen.h>



void dir_basic_test(void){
	int tmp;



	//0.获取当前路径的'指定信息', 如_PC_NAME_MAX=255, 允许的最大文件名长度(不需要询问,出错一般是路径错误,函数基本不会出错)
	tmp = pathconf(".", _PC_NAME_MAX);
	if(tmp == -1){
		perror("pathconf()");
		return;
	}
	printf("_PC_NAME_MAX=%d\n",tmp);

	printf("dir基础测试:\n");

	//1.重命名-文件
	tmp = rename("./x","./x_new");
	if(tmp == -1){
		perror("rename()");
		return ;
	}

	//2.创建link(目标, '生成的link 存放位置'和名称)
	tmp = link("./x_new","./x");
	if(tmp == -1){
		perror("link()");
		return;
	}

	//3.移除link
	tmp = unlink("./x");
	if(tmp == -1){
		perror("unlink()");
		return;
	}

	//4.创建文件夹
	tmp = mkdir("./.myMkdir",0600);
	if(tmp == -1){
		perror("mkdir()");
		return;
	}

	//5.移除文件夹
	tmp = rmdir("./.myMkdir");
	if(tmp == -1){
		perror("rmdir()");
		return;
	}

	//6.删除-文件
	tmp = remove("./x_new");
	if(tmp == -1){
		perror("remove()");
		return;
	}

	return ;
}



int main(void){
	dir_basic_test();
	return 0;
}
