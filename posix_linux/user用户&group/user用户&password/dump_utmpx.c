//编译:
//		gcc ./dump_utmpx.c -g3 -o x 
//用法:
//		./x ./x

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <utmpx.h>
#include <paths.h>



int main(int argc, char *argv[]){
	struct utmpx *ut;

	if(argc <= 1){printf("%s [utmp-pathname]\n",argv[0]);return -1;}
	//if(argc > 2){printf("%s [utmp-pathname]\n",argv[0]);return -1;}
	if(strcmp(argv[1], "help") == 0){printf("%s [utmp-pathname]\n",argv[0]);return -1;}
	if(strcmp(argv[1], "--help") == 0){printf("%s [utmp-pathname]\n",argv[0]);return -1;}
	if(strcmp(argv[1], "-h") == 0){printf("%s [utmp-pathname]\n",argv[0]);return -1;}


	// Use alternate file ifsupplied
	if(utmpxname(argv[1]) == -1){
		printf("utmpxname() failed");
		return -1;
	}



	setutxent();

	printf("user     type        PID line   id  host      date/time\n");

	while((ut = getutxent()) != NULL) {//Sequential scan to EOF
		printf("%-8s ", ut->ut_user);
		printf("%-9.9s ",
			(ut->ut_type == EMPTY) ?         "EMPTY" :
			//(ut->ut_type == RUN_LVL) ?       "RUN_LVL" :
			(ut->ut_type == BOOT_TIME) ?     "BOOT_TIME" :
			(ut->ut_type == NEW_TIME) ?      "NEW_TIME" :
			(ut->ut_type == OLD_TIME) ?      "OLD_TIME" :
			(ut->ut_type == INIT_PROCESS) ?  "INIT_PR" :
			(ut->ut_type == LOGIN_PROCESS) ? "LOGIN_PR" :
			(ut->ut_type == USER_PROCESS) ?  "USER_PR" :
			(ut->ut_type == DEAD_PROCESS) ?  "DEAD_PR" : "???");
		printf("%5ld %-6.6s %-3.5s %-9.9s",(long)ut->ut_pid,ut->ut_line,ut->ut_id,ut->ut_host);
		printf("%s", ctime((time_t *) &(ut->ut_tv.tv_sec)));
	}

	endutxent();

	return 0;
}
