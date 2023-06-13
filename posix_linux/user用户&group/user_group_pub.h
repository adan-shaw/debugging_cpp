#include <stdio.h>
#include <stdlib.h>

#include <pwd.h>
#include <grp.h>



//1.根据'用户id' 获取'用户名'失败返回NULL
char *userNameFromId(uid_t uid);

//2.安全地根据用户名, 获取用户id, 失败返回-1
uid_t userIdFromName(const char *name);

//3.根据'组id' 获取'组名'失败返回NULL
char *groupNameFromId(gid_t gid);

//4.安全地获取'组id' by name, 失败返回-1
gid_t groupIdFromName(const char *name);
