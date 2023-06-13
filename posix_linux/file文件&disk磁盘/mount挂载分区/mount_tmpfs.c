//编译:
//		gcc ./mount_tmpfs.c -g3 -o x 


#include <stdio.h>
#include <sys/mount.h>



//需要root 权限, 需要保证source 文件夹存在, target 文件夹存在;
int main(void){
	unsigned long flags = MS_REMOUNT;
	char* data = NULL;
	char* fstype = "tmpfs";
	const char* source = "/home/adan/tmpfs2";
	const char* target = "/home/adan/tmpfs2_ok";

	//执行挂载
	if(mount(source,target,fstype,flags,data) == -1){
		perror("mount() failed");
		return -1;
	}

	//解除挂载
	if(umount(target) == -1){
		perror("umount() failed");
		return -1;
	}

	return 0;
}



