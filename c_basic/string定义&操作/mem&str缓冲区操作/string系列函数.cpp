//编译:
//		g++ -o x ./strncmp.cpp -g3 -lreadline


//strncmp() 函数的正确使用:
/*
	直接对比前n 个字符是否匹配, 匹配则截断!!

	比strstr() 子字符串对比法好, strstr()可以不是第一个字符开始对比的!!
	strstr() 可能有多串字符串是匹配的, 但是strncmp()不管, 只从第一个字符开始.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>



int main(void){
	char *p;
	int tmp;



	//简单将readline() 读到的字符串, 记录到'.bash_history'文件中.
	//ps: (可以不执行这个shell命令)
	while(true){
		p = readline("myshell:");
		if(strncmp(p,"quit",4) == 0)//前4个字符是"quit"则退出
			break;
		else{
			add_history(p);						//否则加入历史列表, 并执行!!
			system(p);								//你也可以选择执行这个shell命令
		}
	}

	exit(EXIT_SUCCESS);
}
