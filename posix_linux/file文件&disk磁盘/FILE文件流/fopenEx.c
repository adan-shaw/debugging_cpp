//fdopen(): 以'独占+读'的方式, 打开文件
/*
	略, 只读一般可以共享的, 不需要独占, 
	除非你为了读取的数据准确性, 读取期间禁止其他用户修改数据;
*/



//fdopen(): 以'独占+读写'的方式, 打开文件[成功返回0, 失败返回-1]
int fopenEx(FILE** fs, const char* path, int auth, const char* mode){
	//先解决文件的权限问题
	int fd = open(path,auth,0600);
	if(fd == -1){
		perror("open()");
		return -1;
	}

	//修改文件的访问权限(不需要重复操作)
	//fchmod(fd,0600);

	//再创建FILE 文件流
	*fs = fdopen(fd, mode);
	return 0;
}
