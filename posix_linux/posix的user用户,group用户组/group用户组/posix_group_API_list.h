//头文件
#include <grp.h>


//结构体
struct group g_tmp;


//结构体参数:
/*
	char   *gr_name The name of the group.
	gid_t   gr_gid  Numerical group ID.
	char  **gr_mem  Pointer to a null-terminated array of character pointers to member names.
*/


//api list
void           endgrent(void);
struct group  *getgrent(void);
struct group  *getgrgid(gid_t);
int            getgrgid_r(gid_t, struct group *, char *, size_t, struct group **);
struct group  *getgrnam(const char *);
int            getgrnam_r(const char *, struct group *, char *, size_t , struct group **);
void           setgrent(void);
