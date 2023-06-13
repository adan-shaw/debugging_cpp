//编译:
//		gcc user_group_pub.h user_group_pub.c ./acct_v3_view.c -g3 -o x 
//用法:
//		./x ./x


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/acct.h>
#include <limits.h>
#include "user_group_pub.h"



#define TIME_BUF_SIZE (128)

//This is true since glibc 2.6
#define GLIBC_DEFINES_ACCT_V3
#ifndef GLIBC_DEFINES_ACCT_V3
	struct acct_v3{
		char        ac_flag;
		char        ac_version;
		u_int16_t   ac_tty;
		u_int32_t   ac_exitcode;
		u_int32_t   ac_uid;
		u_int32_t   ac_gid;
		u_int32_t   ac_pid;
		u_int32_t   ac_ppid;
		u_int32_t   ac_btime;
		float       ac_etime;
		comp_t      ac_utime;
		comp_t      ac_stime;
		comp_t      ac_mem;
		comp_t      ac_io;
		comp_t      ac_rw;
		comp_t      ac_minflt;
		comp_t      ac_majflt;
		comp_t      ac_swaps;
		char        ac_comm[ACCT_COMM];
	};
#endif



//Convert comp_t value into long long
static long long comptToLL(comp_t ct){
	const int EXP_SIZE = 3;      //3-bit, base-8 exponent
	const int MANTISSA_SIZE = 13;//Followed by 13-bit mantissa
	const int MANTISSA_MASK = (1 << MANTISSA_SIZE) - 1;
	long long mantissa, exp;

	mantissa = ct & MANTISSA_MASK;
	exp = (ct >> MANTISSA_SIZE) & ((1 << EXP_SIZE) - 1);
	return mantissa << (exp * 3);//Power of 8 = left shift 3 bits
}



int main(int argc, char *argv[]){
	int acctFile;
	struct acct_v3 ac;
	ssize_t numRead;
	char *s;
	char timeBuf[TIME_BUF_SIZE];
	struct tm *loc;
	time_t t;



	if(argc != 2 || strcmp(argv[1], "--help") == 0){
		printf("%s file\n", argv[0]);
		return -1;
	}


	acctFile = open(argv[1], O_RDONLY);
	if(acctFile == -1){
		perror("open() failed");
		return -1;
	}

	printf("ver. command    flags   term.   PID   PPID  user     group"
				 "      start date+time     CPU   elapsed\n");
	printf("                       status                             "
				 "                          time    time\n");


	while((numRead = read(acctFile,&ac,sizeof(struct acct_v3))) > 0){
		if(numRead != sizeof(struct acct_v3)){
			perror("read() failed");
			close(acctFile);
			return -1;
		}

		printf("%1d    ", (int) ac.ac_version);
		printf("%-8.8s   ", ac.ac_comm);

		printf("%c", (ac.ac_flag & AFORK) ? 'F' : '-') ;
		printf("%c", (ac.ac_flag & ASU)   ? 'S' : '-') ;
		printf("%c", (ac.ac_flag & AXSIG) ? 'X' : '-') ;
		printf("%c", (ac.ac_flag & ACORE) ? 'C' : '-') ;

		printf(" %#6lx   ", (unsigned long) ac.ac_exitcode);

		printf(" %5ld %5ld  ", (long) ac.ac_pid, (long) ac.ac_ppid);

		s = userNameFromId(ac.ac_uid);
		printf("%-8.8s ", (s == NULL) ? "???" : s);

		s = groupNameFromId(ac.ac_gid);
		printf("%-8.8s ", (s == NULL) ? "???" : s);

		t = ac.ac_btime;
		loc = localtime(&t);
		if(loc == NULL){
			printf("???Unknown time???  ");
		}
		else{
			strftime(timeBuf, TIME_BUF_SIZE, "%Y-%m-%d %T ", loc);
			printf("%s ", timeBuf);
		}

		printf("%5.2f %7.2f ", (double) (comptToLL(ac.ac_utime) +
				comptToLL(ac.ac_stime)) / sysconf(_SC_CLK_TCK), ac.ac_etime / sysconf(_SC_CLK_TCK));
		printf("\n");
	}

	if(numRead == -1){
		perror("read() failed at last");
		close(acctFile);
		return -1;
	}

	close(acctFile);
	return 0;
}
