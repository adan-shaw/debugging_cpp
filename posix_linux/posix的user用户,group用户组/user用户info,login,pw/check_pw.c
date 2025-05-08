//编译:
//		gcc ./check_pw.c -g3 -lcrypt -o x 

//检查密码程序(也可以改造成密码扫描, 密码诈骗木马)

#define _BSD_SOURCE		//Get getpass() declaration from <unistd.h>
#define _XOPEN_SOURCE	//Get crypt() declaration from <unistd.h>
#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <shadow.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>



//全部是单线程查询版本, 没有使用_r()多线程兼容版本



//输入用户名, 检查用户密码(一次只能操作一个用户)
void test_user_password(void);



int main(void){
	test_user_password();
	return 0;
}



//输入用户名, 检查用户密码(一次只能操作一个用户)
void test_user_password(void){
	char *username, *password, *encrypted, *p;
	struct passwd *pwd;
	struct spwd *spwd;
	bool authOk;
	size_t len;
	long lnmax;



	//获取用户名MAX 长度失败, 指定一个可行的MAX值
	//(256是posix标准, 肯定可用, 操作系统用户名最长一般只能是256个字符)
	lnmax = sysconf(_SC_LOGIN_NAME_MAX);
	if(lnmax == -1)										// iflimit is indeterminate
		lnmax = 256;										//make a guess


	//分配用户名存储空间
	username = (char*)malloc(lnmax);
	if(username == NULL){
		perror("malloc() failed");
		return;
	}


	//从stdin中获取用户名
	printf("Username: ");
	fflush(stdin);
	if(fgets(username, lnmax, stdin) == NULL)
		goto test_user_password_quit;		//Exit on EOF(没有获取到用户名,就退出该函数)

	//获取到用户名, 检查其长度
	len = strlen(username);
	if(username[len - 1] == '\n')
		username[len - 1] = '\0';				//Remove trailing '\n'


	pwd = getpwnam(username);
	if(pwd == NULL){
		perror("getpwnam() failed,couldn't get password record");
		goto test_user_password_quit;
	}

	spwd = getspnam(username);{
	if(spwd == NULL && errno == EACCES)
		perror("getspnam() failed,no permission to read shadow password file");
		goto test_user_password_quit;
	}
	if(spwd != NULL)									//ifthere is a shadow password record
		pwd->pw_passwd = spwd->sp_pwdp;	//Use the shadow password


	password = getpass("Password: ");


	//Encrypt password and erase cleartext version immediately
	encrypted = crypt(password, pwd->pw_passwd);
	for(p = password; *p != '\0';)
		*p++ = '\0';

	if(encrypted == NULL){
		perror("crypt() failed");
		goto test_user_password_quit;
	}


	authOk = strcmp(encrypted, pwd->pw_passwd) == 0;
	if(!authOk) {
		printf("Incorrect password\n");
		goto test_user_password_quit;
	}
	else
		printf("Successfully authenticated: UID=%ld\n",(long)pwd->pw_uid);

	// Now do authenticated work...


test_user_password_quit:
	//释放资源
	free(username);
	return;
}
