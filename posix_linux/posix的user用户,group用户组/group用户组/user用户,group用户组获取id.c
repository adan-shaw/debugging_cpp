//编译:
//		gcc user_group_pub.h user_group_pub.c ./user用户,group用户组获取id -g3 -o x 



#include "user_group_pub.h"



void test(void){
	const char* powner_id = "adan";
	const char* powner_gid = "adan";
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

	return;
}



int main(void){
	test();
	return 0;
}










