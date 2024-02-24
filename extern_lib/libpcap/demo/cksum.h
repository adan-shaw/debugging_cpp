// 计算CRC16 校验和(更精简的checksum() 计算方法)
unsigned short cksum(unsigned char *data, unsigned int len);

//cksumEx() 计算通用版Ex(tcp/udp 适用), 最后一个参数决定是tcp=1, udp=0;
//如果buffer 申请缓冲区不正确, 可以用: #include <alloca.h> 来代替, 直接在线程栈上面申请内存
unsigned short cksumEx (unsigned char *data, unsigned int len, unsigned int saddr, unsigned int daddr, unsigned short tcp_udp);
