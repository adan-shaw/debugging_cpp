//编译:
//		gcc ./getpwnam_r.c -g3 -o x


#include <sys/types.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>



//test getpwnam_r()
void test_getpwnam_r(void);



int main(void){
	test_getpwnam_r();
	return 0;
}



//test getpwnam_r()
void test_getpwnam_r(void){
	struct passwd pwd,*result;
	const char *test_user = "adan";
	char *buf = NULL;
	size_t bufSize;
	int tmp;


	//先获取用户名的'名字长度max 限制'
	bufSize = sysconf(_SC_GETPW_R_SIZE_MAX);
	buf = (char*)malloc(bufSize);
	if(buf == NULL){
		perror("malloc() failed");
		return;
	}
	memset(buf,'\0',bufSize);


	//再根据名字, 提取出用户的struct passwd 记录
	tmp = getpwnam_r(test_user, &pwd, buf, bufSize, &result);
	if(tmp != 0){
		printf("getpwnam_r() failed");
		free(buf);
		return;
	}
	if(result != NULL)
		printf("Name: %s\n", pwd.pw_gecos);//解析记录
	else
		printf("Not found\n");


	//释放资源
	free(buf);
	return;
}
