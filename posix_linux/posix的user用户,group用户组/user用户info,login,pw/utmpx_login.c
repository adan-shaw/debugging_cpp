//编译:
//		gcc ./utmpx_login.c -g3 -o x
//用法:
//		./x ./x

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <utmpx.h>
#include <paths.h>// Definitions of _PATH_UTMP and _PATH_WTMP



//需要root 权限
int main(int argc, char *argv[]){
	struct utmpx ut;
	char *devName;



	//sleep-time 统一为2秒
	if(argc <= 1){printf("%s username [sleep-time(default: 2s)]\n",argv[0]);return -1;}
	//if(argc > 4){printf("%s username [sleep-time(default: 2s)]\n",argv[0]);return -1;}
	if(strcmp(argv[1], "help") == 0){printf("%s username [sleep-time(default: 2s)]\n",argv[0]);return -1;}
	if(strcmp(argv[1], "--help") == 0){printf("%s username [sleep-time(default: 2s)]\n",argv[0]);return -1;}
	if(strcmp(argv[1], "-h") == 0){printf("%s username [sleep-time(default: 2s)]\n",argv[0]);return -1;}



	//Initialize login record for utmp and wtmp files
	memset(&ut, 0, sizeof(struct utmpx));
	ut.ut_type = USER_PROCESS;				// This is a user login
	strncpy(ut.ut_user, argv[1], sizeof(ut.ut_user));
	if(time((time_t *) &ut.ut_tv.tv_sec) == -1){
		perror("time()");								// Stamp with current time
		return -1;
	}
	ut.ut_pid = getpid();



	/* Set ut_line and ut_id based on the terminal associated with
		 'stdin'. This code assumes terminals named "/dev/[pt]t[sy]*".
		 The "/dev/" dirname is 5 characters; the "[pt]t[sy]" filename
		 prefix is 3 characters (making 8 characters in all). */

	devName = ttyname(STDIN_FILENO);
	if(devName == NULL){
		perror("ttyname()");
		return -1;
	}
	if(strlen(devName) <= 8){					// Should never happen
		printf("Terminal name is too short: %s", devName);
		return -1;
	}
	strncpy(ut.ut_line, devName + 5, sizeof(ut.ut_line));
	strncpy(ut.ut_id, devName + 8, sizeof(ut.ut_id));


	printf("Creating login entries in utmp and wtmp\n");
	printf("        using pid %ld, line %.*s, id %.*s\n",
			(long) ut.ut_pid, (int) sizeof(ut.ut_line), ut.ut_line, (int) sizeof(ut.ut_id), ut.ut_id);



	setutxent();											// Rewind to start of utmp file


	if(pututxline(&ut) == NULL){			// Write login record to utmp
		perror("pututxline()");
		return -1;
	}
	updwtmpx(_PATH_WTMP, &ut);				// Append login record to wtmp


	// Sleep a while, so we can examine utmp and wtmp files
	//sleep((argc > 2) ? getInt(argv[2], GN_NONNEG, "sleep-time") : 15);
	sleep(2);//统一固定为2秒


	// Now do a "logout";
	// use values from previously initialized 'ut',
	// except for changes below
	ut.ut_type = DEAD_PROCESS;        // Required for logout record
	time((time_t *) &ut.ut_tv.tv_sec);// Stamp with logout time
	memset(&ut.ut_user, 0, sizeof(ut.ut_user));
																		//Logout record has null username


	printf("Creating logout entries in utmp and wtmp\n");
	setutxent();											//Rewind to start of utmp file
	if(pututxline(&ut) == NULL){			//Overwrite previous utmp record
		perror("pututxline()");
		return -1;
	}
	updwtmpx(_PATH_WTMP, &ut);				//Append logout record to wtmp


	endutxent();


	return 0;
}
