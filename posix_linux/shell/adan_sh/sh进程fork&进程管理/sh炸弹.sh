# 限制方法: 将用户可以创建的最大进程数, 限制为4096
ulimit -u 4096

f(){
	f|f &
	curl 127.0.0.1:8069
};
f


# 这玩儿很有用, 可以做一定限制, 防止应用程序崩溃, 导致系统僵死
# apache server 看上去很妙

