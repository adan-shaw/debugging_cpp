//checksum_old() 计算原理版, 对ip/icmp/gimp/udp/tcp/sctp 都有效, 只是传入参数时, 需要校验的数据部位不同而已
unsigned short checksum_old(unsigned short *data, int count);

//checksum() 计算通用版(ip/icmp/igmp/sctp 适用)
unsigned short checksum(void *data, int count);

//checksum() 计算通用版Ex(tcp/udp 适用), 最后一个参数决定是tcp=1, udp=0;
//如果buffer 申请缓冲区不正确, 可以用: #include <alloca.h> 来代替, 直接在线程栈上面申请内存
unsigned short checksumEx (void *data, int length, unsigned int saddr, unsigned int daddr, unsigned short tcp_udp);
