//编译:
//		gcc ./getpwent.c -g3 -o x



#include <sys/types.h>
#include <pwd.h>
#include <stdio.h>



//1.遍历密码记录文件
//	(虽然密码记录文件被crypt()加密, 但是提取很容易)
/*
	密码文件存放位置: /etc/passwd


	struct passwd *getpwent(void);	//读取一次记录, 并后移
	void setpwent(void);						//重置到'密码记录文件头'
	void endpwent(void);						//关闭密码记录文件


	struct passwd {
		char   *pw_name;   // username
		char   *pw_passwd; // user password
		uid_t   pw_uid;    // user ID
		gid_t   pw_gid;    // group ID
		char   *pw_gecos;  // user information
		char   *pw_dir;    // home directory
		char   *pw_shell;  // shell program
	};
*/



//打印所有'用户名 + id值'
int main(void){
	struct passwd *pwd;

	//遍历密码记录文件(自动打开并占用, 单线程占用)
	while((pwd = getpwent()) != NULL)
		printf("%-8s %5ld\n", pwd->pw_name, (long) pwd->pw_uid);


	setpwent();//重置'密码记录文件'读取游标, 重置到头部

	endpwent();//释放'密码记录文件'的使用权.

	return 0;
}
