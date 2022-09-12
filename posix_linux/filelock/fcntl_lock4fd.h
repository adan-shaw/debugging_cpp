//同一个pid 可以对同一个fd多次加锁, 毫无影响, 不会产生任何冲突, 一定会成功的;
//多进程, 不同pid 对同一个fd, 多次加锁的时候, 才会有冲突问题的产生;

//如果对fd的所有数据进行加锁, 建议用flock(), 原子操作, 性能更好, 还不用填充数据, 麻烦;


//设置一把读锁
void readLock(int fd, short start, short whence, short len);

//设置一把读锁(不阻塞等待加锁)
int readLocknb(int fd, short start, short whence, short len);

//设置一把写锁
void writeLock(int fd, short start, short whence, short len);

//设置一把写锁(不阻塞等待加锁)
int writeLocknb(int fd, short start, short whence, short len);

//解锁(统一解锁的方式)
void unlock(int fd, short start, short whence, short len);


