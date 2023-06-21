//编译:
//		gcc -g3 ./c_str字符串函数\(效率很低,不要用\)dead.c -o x 


#include <stdio.h>
#include <string.h>
#include <alloca.h>
#include <stdbool.h>



//1.删除前n个字符(效率太低, 还不如直接用strstr 拷贝'一个新的子字符串'出来, 用空间换时间)
char* c_erase(char* str_buf,unsigned int del_N){
	int tmp;

	if(del_N == 0)
		return str_buf;
	for(tmp = 0;str_buf[tmp] != '\0';tmp++)
		;
	if(del_N >= tmp){
		str_buf[0] = '\0';
		return str_buf;
	}

	for(tmp = 0;str_buf[del_N] != '\0';)
		str_buf[tmp++] = str_buf[del_N++];
	str_buf[tmp] = '\0';
	return str_buf;
}



//2.删除[n,m]之间的字符
//	(效率太低, 还不如直接用strstr, 分别拷贝出'前后两个子字符串'出来, 然后再用strcat 合并, 用空间换时间)
char* c_eraseEx(char* str_buf,unsigned int del_N,unsigned int del_M){
	int tmp;

	if(del_N >= del_M)//错误输入, 不执行删除
		return str_buf;
	for(tmp = 0;str_buf[tmp] != '\0';tmp++)
		;
	if(del_M > tmp)
		return str_buf;//不可能存在的溢出操作, 不执行删除

	for(;str_buf[del_M] != '\0';)
		str_buf[del_N++] = str_buf[++del_M];

	str_buf[del_N] = '\0';
	return str_buf;
}



//3.删除后n个字符
char* c_rerase(char* str_buf,unsigned int del_N){
	int tmp;

	if(del_N == 0)
		return str_buf;
	for(tmp = 0;str_buf[tmp] != '\0';tmp++)
		;
	if(del_N >= tmp){
		str_buf[0] = '\0';
		return str_buf;
	}

	for(;del_N > 0;del_N--)
		str_buf[tmp--] = '\0';
	str_buf[tmp] = '\0';
	return str_buf;
}



//4.扣出[n,m]之间的字符串,写入到引用buf中 [弃用!! 这么多for 循环, 实际效率更低]
void c_get_son(const char* str_buf,unsigned int n,unsigned int m,char* str_son,unsigned int str_son_len){
	int tmp;

	if(m < n)//出错, m 必须大于n
		return;
	if(m-n < str_son_len-1)//出错, 缓冲区必须能容纳所有字符串
		return;
	for(tmp=0;n<=m;n++){
		*(str_son) = str_buf[n];
		str_son++;
	}
	*(str_son) = '\0';
	return;
}






//+1.删除前n个字符
void c_erase_test(void){
	bool err = false;
	char buf[64] = "hellow world";
	char buf2[64] = "hellow world";
	char buf3[64] = "hellow world";
	char *p;



	p = c_erase(buf,5);
	if(strncmp(p,"w world",64) != 0){
		printf("1 c_erase_test() failed,p=%s\n",p);
		err = true;
	}

	p = c_erase(buf2,0);
	if(strncmp(p,"hellow world",64) != 0){
		printf("2 c_erase_test() failed,p=%s\n",p);
		err = true;
	}

	p = c_erase(buf3,32);
	if(strncmp(p,"",64) != 0){
		printf("3 c_erase_test() failed,p=%s\n",p);
		err = true;
	}

	if(err == false)
		printf("c_erase_test() success\n");

	return;
}



//+2.删除[n,m]之间的字符
void c_eraseEx_test(void){
	bool err = false;
	char buf[64] = "hellow world";
	char buf2[64] = "hellow world";
	char buf3[64] = "hellow world";
	char *p;



	p = c_eraseEx(buf,2,3);//注意, 是0 下标开始的
	if(strncmp(p,"heow world",64) != 0){
		printf("1 c_eraseEx_test() failed,p=%s\n",p);
		err = true;
	}

	p = c_eraseEx(buf2,1,0);
	if(strncmp(p,"hellow world",64) != 0){
		printf("2 c_eraseEx_test() failed,p=%s\n",p);
		err = true;
	}

	p = c_eraseEx(buf3,0,32);
	if(strncmp(p,"hellow world",64) != 0){
		printf("3 c_eraseEx_test() failed,p=%s\n",p);
		err = true;
	}

	if(err == false)
		printf("c_eraseEx_test() success\n");

	return;
}



//+3.删除后n个字符
void c_rerase_test(void){
	bool err = false;
	char buf[64] = "hellow world";
	char buf2[64] = "hellow world";
	char buf3[64] = "hellow world";
	char *p;



	p = c_rerase(buf,5);
	if(strncmp(p,"hellow ",64) != 0){
		printf("1 c_rerase_test() failed,p=%s\n",p);
		err = true;
	}

	p = c_rerase(buf2,0);
	if(strncmp(p,"hellow world",64) != 0){
		printf("2 c_rerase_test() failed,p=%s\n",p);
		err = true;
	}

	p = c_rerase(buf3,12);
	if(strncmp(p,"",64) != 0){
		printf("3 c_rerase_test() failed,p=%s\n",p);
		err = true;
	}

	if(err == false)
		printf("c_rerase_test() success\n");

	return;
}



//+4.扣出[n,m]之间的字符串,写入到引用buf中
void c_get_son_test(void){
	bool err = false;
	char buf[64] = "hellow world";
	char buf2[64] = "hellow world";
	char buf3[64] = "hellow world";
	char ret[64]="",ret2[64]="",ret3[16]="";



	c_get_son(buf,2,3,ret,64);//注意, 是0 下标开始的
	if(strncmp(ret,"ll",64) != 0){
		printf("1 c_get_son() failed,p=%s\n",ret);
		err = true;
	}

	c_get_son(buf2,1,0,ret2,64);
	if(strncmp(ret2,"",64) != 0){
		printf("2 c_get_son() failed,p=%s\n",ret2);
		err = true;
	}

	c_get_son(buf3,0,32,ret3,16);
	if(strncmp(ret3,"",16) != 0){
		printf("3 c_get_son() failed,p=%s\n",ret3);
		err = true;
	}

	if(err == false)
		printf("c_get_son() success\n");

	return;
}



int main(void){
	const char* src = "1234567890 abc";
	char des[64];
	size_t len,len2,len3;



	c_erase_test();		//+1.删除前n个字符
	c_eraseEx_test();	//+2.删除[n,m]之间的字符
	c_rerase_test();	//+3.删除后n个字符
	c_get_son_test();	//+4.扣出[n,m]之间的字符串,写入到引用buf中
	return 0;
}
