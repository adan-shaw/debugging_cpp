//编译:
//		gcc ./acct_on.c -g3 -o x 
//运行:
//		sudo ./x



//用户的进程记账, 默认是不启动的(因为没有指定文件, 所以系统默认不会自动进行记账)

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



int main(void){
	//设置'用户的进程记账-输出文件'的路径
	//const char *acct_file = "./test_file";
	const char *acct_file = NULL;

	//关闭进程记账功能
	if(acct(acct_file) == -1){
		perror("acct() failed");
		return -1;
	}

	printf("Process accounting %s\n", (acct_file == NULL) ? "disabled" : "enabled");

	return 0;
}
