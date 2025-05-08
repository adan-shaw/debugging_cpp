//fdopen(): 以'独占+读写'的方式, 打开文件[成功返回0, 失败返回-1]
int fopenEx(FILE** fs, const char* path, int auth, const char* mode);
