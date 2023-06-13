//编译:
//		gcc ./env查看\&修改.c -g3 -o x



//env '进程环境变量'查看&修改, 简介:
/*
	linux默认情况下, 命令行'所有'进程环境变量'字符串'的总数据量, 应小于128k
	putenv()在环境中设置var的限制似乎约为128k.

	单条'进程环境变量'字符串长度限制:
		Windows XP或更高版本:  8191个字符
		Windows 2000/NT4.0:  2047个字符
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>



//单条'进程环境变量'字符串长度限制=2048
#define ENV_VAL_STRLEN_MAX (2048)



//c 访问->当前'进程环境变量'的'字符串指针'-数组(固定命名, 约定命名, 不能修改变量名)
extern char **environ;





//1.打印当前进程所有的环境变量
void show_all_environ(void){
	char **ep;
	int count;

	for(ep=environ,count=0; *ep != NULL; ep++,count++)
		printf("%d: %s\n", count, *ep);
	return;
}





int main(void){
	char* pret, env_buf[ENV_VAL_STRLEN_MAX];
	int tmp;



	printf("1.打印当前进程所有的环境变量:\n");
	show_all_environ();
	printf("\n\n");



	//2.glibc 库对'进程环境变量'的操作
	printf("2.1 根据'进程环境变量'的名, 读取'进程环境变量'的值\n");
	strncpy(env_buf,"LOGNAME",sizeof(env_buf));
	pret = getenv(env_buf);
	if(pret != NULL)
		printf("getenv(%s) = %s\n", env_buf, pret);
	else
		printf("getenv() failed, this is no env named %s\n", env_buf);


	printf("2.2 新增'进程环境变量'条目(进程内有效, 进程退出会销毁)\n");
	strncpy(env_buf,"ADAN_TEST=\"come_love _me\"",sizeof(env_buf));
	tmp = putenv(env_buf);
	if(tmp != 0){
		printf("putenv()比较特殊, 失败不是返回-1, 而是返回随机值, 成功则返回0\n");
		perror("putenv()");
		return -1;
	}

	//检验刚putenv()添加的'进程环境变量'
	strncpy(env_buf,"ADAN_TEST",sizeof(env_buf));
	pret = getenv(env_buf);
	if(pret != NULL)
		printf("getenv(%s) = %s\n", env_buf, pret);
	else
		printf("getenv() failed, this is no env named %s\n", env_buf);



	//3.1 修改'进程环境变量'
	printf("3.1 修改'进程环境变量'\n");
	/*
		setenv()先找到对应的'进程环境变量,
		如果存在,则考虑覆盖修改.
		如果不存在, 则与putenv() 功能一致.
		setenv()第三参数:
			等于0,则跳过不操作.
			不等于0, 则覆盖.
	*/
	strncpy(env_buf,"ADAN_TEST",sizeof(env_buf));
	//覆盖式修改
	tmp = setenv(env_buf, "\"setenv()_love _me\"", 1);
	if(tmp == -1){
		perror("setenv()");
		return -1;
	}

	//检验刚putenv()添加的'进程环境变量'
	strncpy(env_buf,"ADAN_TEST",sizeof(env_buf));
	pret = getenv(env_buf);
	if(pret != NULL)
		printf("getenv(%s) = %s\n", env_buf, pret);
	else
		printf("getenv() failed, this is no env named %s\n", env_buf);


	//3.2 取消'进程环境变量'
	printf("3.2 取消'进程环境变量'\n");
	tmp = unsetenv("ADAN_TEST");
	if(tmp == -1){
		perror("unsetenv()");
		return -1;
	}

	//检验刚putenv()添加的'进程环境变量'
	pret = getenv("ADAN_TEST");
	if(pret != NULL)
		printf("getenv(%s) = %s\n", env_buf, pret);
	else
		printf("getenv() failed, this is no env named %s\n", env_buf);


	//3.3 清空所有'进程环境变量'
	printf("3.3 清空所有'进程环境变量'\n");
	if(clearenv() != 0){
		perror("clearenv()");
		return -1;
	}

	return 0;
}



