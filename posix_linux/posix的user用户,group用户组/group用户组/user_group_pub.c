#include "user_group_pub.h"



//1.根据'用户id' 获取'用户名'失败返回NULL
char *userNameFromId(uid_t uid){
	struct passwd *pwd;

	pwd = getpwuid(uid);
	return (pwd == NULL) ? NULL : pwd->pw_name;
}



//2.安全地根据用户名, 获取用户id, 失败返回-1
uid_t userIdFromName(const char *name){
	struct passwd *pwd;
	uid_t u;
	char *endptr;

	if(name == NULL || *name == '\0')
		return -1;									//错误字符串指针, 直接返回.

	u = strtol(name, &endptr, 10);//将字符串转换为long
	if(*endptr == '\0')
		return u;										//转换后, 如果返回空串, 直接返回?

	pwd = getpwnam(name);
	if(pwd == NULL){
		perror("getpwnam() failed");
		return -1;
	}

	return pwd->pw_uid;
}



//3.根据'组id' 获取'组名'失败返回NULL
char *groupNameFromId(gid_t gid){
	struct group *grp;

	grp = getgrgid(gid);
	return (grp == NULL) ? NULL : grp->gr_name;
}



//4.安全地获取'组id' by name, 失败返回-1
gid_t groupIdFromName(const char *name){
	struct group *grp;
	gid_t g;
	char *endptr;

	if(name == NULL || *name == '\0')
		return -1;									//错误字符串指针, 直接返回.

	g = strtol(name, &endptr, 10);//将字符串转换为long
	if(*endptr == '\0')
		return g;										//转换后, 如果返回空串, 直接返回?

	grp = getgrnam(name);
	if(grp == NULL){
		perror("getgrnam() failed");
		return -1;
	}

	return grp->gr_gid;
}



