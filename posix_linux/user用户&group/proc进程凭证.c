//编译:
//		gcc -g3 user_group_pub.h user_group_pub.c ./proc进程凭证.c -o x 



//单线程查询版本(没有使用_r()多线程兼容版本)



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <ctype.h>

#include "user_group_pub.h"
#include <sys/fsuid.h>



#define SG_SIZE (NGROUPS_MAX + 1)



void test_all(void);



int main(void){
	test_all();
	return 0;
}



void test_all(void){
	uid_t ruid, euid, suid, fsuid;
	gid_t rgid, egid, sgid, fsgid;
	gid_t suppGroups[SG_SIZE];
	int numGroups, j;
	char *p;



	//1.查询进程的pid, ppid, pgid, sid
	printf("parent: PID=%ld, PPID=%ld, PGID=%ld, SID=%ld\n\n",\
			(long)getpid(),(long)getppid(),(long)getpgrp(),(long)getsid(0));



	//获取uid 信息
	if(getresuid(&ruid, &euid, &suid) == -1){
		perror("getresuid() failed");
		return;
	}
	//获取gid 信息
	if(getresgid(&rgid, &egid, &sgid) == -1){
		perror("getresgid() failed");
		return;
	}



	/* Attempts to change the file-system IDs are always ignored
		 for unprivileged processes, but even so, the following
		 calls return the current file-system IDs */

	fsuid = setfsuid(0);
	fsgid = setfsgid(0);

	//打印刚才获取到的uid 信息
	printf("UID: ");
	p = userNameFromId(ruid);
	printf("real=%s (%ld); ", (p == NULL) ? "???" : p, (long)ruid);
	p = userNameFromId(euid);
	printf("eff=%s (%ld); ", (p == NULL) ? "???" : p, (long)euid);
	p = userNameFromId(suid);
	printf("saved=%s (%ld); ", (p == NULL) ? "???" : p, (long)suid);
	p = userNameFromId(fsuid);
	printf("fs=%s (%ld); ", (p == NULL) ? "???" : p, (long)fsuid);
	printf("\n\n");

	//打印刚才获取到的gid 信息
	printf("GID: ");
	p = groupNameFromId(rgid);
	printf("real=%s (%ld); ", (p == NULL) ? "???" : p, (long)rgid);
	p = groupNameFromId(egid);
	printf("eff=%s (%ld); ", (p == NULL) ? "???" : p, (long)egid);
	p = groupNameFromId(sgid);
	printf("saved=%s (%ld); ", (p == NULL) ? "???" : p, (long)sgid);
	p = groupNameFromId(fsgid);
	printf("fs=%s (%ld); ", (p == NULL) ? "???" : p, (long)fsgid);
	printf("\n\n");



	//获取多个gid 信息(ps: gid = 用户组)
	numGroups = getgroups(SG_SIZE, suppGroups);
	if(numGroups == -1){
		perror("getgroups() failed");
		return;
	}

	//打印多个gid 信息
	printf("Supplementary groups (%d): ", numGroups);//打印gid 统计数量
	for (j = 0; j < numGroups; j++) {
		p = groupNameFromId(suppGroups[j]);
		printf("%s (%ld) ", (p == NULL) ? "???" : p, (long)suppGroups[j]);
	}
	printf("\n");


	return;
}
