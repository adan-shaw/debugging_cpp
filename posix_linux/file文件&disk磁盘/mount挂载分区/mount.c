//编译:
//		gcc ./mount.c -g3 -o x 


#include <stdio.h>
#include <sys/mount.h>



//flags list:
/*
	MS_BIND         create a bind mount
	MS_DIRSYNC      synchronous directory updates
	MS_MANDLOCK     permit mandatory locking
	MS_MOVE         atomically move subtree
	MS_NOATIME      don't update atime(last access time)
	MS_NODEV        don't permit device access
	MS_NODIRATIME   don't update atime on directories
	MS_NOEXEC       don't allow executables
	MS_NOSUID       disable set-user/group-ID programs
	MS_RDONLY       read-only mount                        只读挂载
	MS_REC          recursive mount
	MS_REMOUNT      remount                                重新挂载
	MS_SYNCHRONOUS  make writes synchronous
*/

//fstype list:
/*
	执行命令查询本系统支持的fstype: cat /proc/filesystems
		没有的设备, 支持的filesystem:
		nodev			sysfs
		nodev			rootfs
		nodev			ramfs
		nodev			bdev
		nodev			proc
		nodev			cpuset
		nodev			cgroup
		nodev			cgroup2
		nodev			tmpfs						//tmpfs 经常用
		nodev			devtmpfs
		nodev			debugfs
		nodev			tracefs
		nodev			securityfs
		nodev			sockfs
		nodev			dax
		nodev			bpf
		nodev			pipefs
		nodev			hugetlbfs
		nodev			devpts
		nodev			mqueue
		nodev			pstore
		nodev			autofs
		nodev			binfmt_misc
		支持的filesystem:
			ext3
			ext2
			ext4
*/



int main(void){
	unsigned long flags = MS_RDONLY | MS_REMOUNT;
	char* data = NULL;
	char* fstype = NULL;
	const char* source = "";
	const char* target = "";

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



