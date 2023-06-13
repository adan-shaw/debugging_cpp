//编译:
//		gcc ./acct_on.c -g3 -o x 
//运行:
//		sudo ./x



//进程记账, 一般不需要开启, 默认是不启动的(因为没有指定文件, 所以系统一般不自动进行记账)

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



int main(void){
	//设置'进程记账-输出文件'的路径
	//const char *acct_name = "./test_file";
	const char *acct_name = NULL;

	//关闭进程记账功能
	if(acct(acct_name) == -1){
		perror("acct() failed");
		return -1;
	}

	printf("Process accounting %s\n", (acct_name == NULL) ? "disabled" : "enabled");

	return 0;
}
