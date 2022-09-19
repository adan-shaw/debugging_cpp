//编译:
//		gcc -g3 ./ftok获取唯一key.c -o x

#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <errno.h>

/*
	ftok函数原型:
		key_t ftok(const char *pathname, int proj_id);

	ftok的作用:
		'共享内存,消息队列,信号量', 它们三个都是找一个中间介质, 来进行通信的;
		这时候, ftok()就可以排上用场了, ftok()可以轻松生产一个唯一的key 供'共享内存,消息队列,信号量'使用;

	ftok的用法:
		* ftok根据路径名, 提取文件信息; 再根据文件信息及proj_id合成key;
			(路径可以随意,但必须存在; 文件读写权限随意, 文件类型随意, 不需要可读可写, 只需要文件的inode 唯一值即可)

		* 指定的路径必须存在, ftok根据文件的inode值, 在系统内取一个唯一的数值, 跟proj_id合成key, 与文件的权限无关;
			如果该文件已经被占用, 则需要保证proj_id 值, 与上一个占用者不一致;
			(一般这个需要手动判断, 程序员自己判断)

		* proj_id是可以根据自己的约定, 随意设置;
			这个数字, 有的称之为project ID; 
			在UNIX系统上, 它的取值是1到255;
*/



int main(void){
	const char *file_name = "./x";
	key_t kfd;

	kfd = ftok(file_name, 0x57);
	if(kfd == -1){
		perror("ftok()");
		return -1;
	}
	return 0;
}
