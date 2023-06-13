//编译:
//		gcc user_group_pub.h user_group_pub.c ./chown.c -g3 -o x 



#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <utime.h>
#include <stdlib.h>
#include <unistd.h>

#include <pwd.h>
#include <grp.h>
//#include "ugid_functions.h"//Declarations of userIdFromName()
//                           //and groupIdFromName()



//修改属主测试
void chown_test(void){
	const char* powner_id = "adan";
	const char* powner_gid = "adan";
	const char* filename = "chown.c";
	uid_t uid;
	gid_t gid;



	uid = userIdFromName(powner_id);
	if(uid == -1){
		printf("userIdFromName(%s) failed\n",powner_id);
		return;
	}

	gid = groupIdFromName(powner_gid);
	if(gid == -1){
		printf("groupIdFromName(%s) failed\n",powner_gid);
		return;
	}

	if(chown(filename, uid, gid) == -1) {
		perror("chown() failed");
		return;
	}

	return;
}



int main(void){
	chown_test();
	return 0;
}








