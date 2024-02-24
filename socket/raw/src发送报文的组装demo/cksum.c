#include <string.h>

// c/c++ 通用, 不需要特殊宏
#ifdef __cplusplus
	//extern "C" {
#endif

// 计算CRC16 校验和(更精简的checksum() 计算方法)
unsigned short cksum(unsigned char *data, unsigned int len){
	unsigned int sum = 0;
	unsigned short tmp, *value = (unsigned short*)data;

	while(len & 0xfffe){			//将data 数据按照2字节为单位累加起来(如果是len偶数, len=0x0000 才会退出; 如果是len奇数, len=0x0001 就会退出; )
		sum += *value++;				//取value 指针指向的值, 做累加; 累加后, 自动后移;
		len -= 2;
	}

	if(len & 0x01){						//判断len 是否为奇数(最后1字节), 若len 为奇数, 则data 需要利用unsigned short tmp 进行补位, 补8个0(8bit)
		tmp = ((*data) << 8)&0xff00;
		sum += tmp;							//补位后, 将unsigned short tmp 也累加进sum 校验和
	}

	//将32bit数据压缩成16bit数据
	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);

	//返回16 bit 的反码(即 CRC16校验和)
	return ~sum;
}

//cksumEx() 计算通用版Ex(tcp/udp 适用), 最后一个参数决定是tcp=1, udp=0;
//如果buffer 申请缓冲区不正确, 可以用: #include <alloca.h> 来代替, 直接在线程栈上面申请内存
unsigned short cksumEx (unsigned char *data, unsigned int len, unsigned int saddr, unsigned int daddr, unsigned short tcp_udp){
	unsigned short buffer[(len + 20) / sizeof(unsigned short)];

	// 填充ip 伪头部
	buffer[0] = saddr;
	buffer[1] = saddr >> 16;					// 4 byte
	buffer[2] = daddr;
	buffer[3] = daddr >> 16;					// 4 byte
	buffer[4] = 0;										// 2 byte(保留字段, 通常为 0)
	if(tcp_udp == 1)									// 2 byte(协议号, TCP的协议号是6, UDP的协议号是17)
		buffer[5] = 6;
	else
		buffer[5] = 17;
	buffer[6] = len;									// 2 byte(TCP 头部和数据总长度) -- 伪头部一共12 bit 
	memcpy (&buffer[7], data, len);// copy data to new buffer[] -- 从第13 bit 开始, 填充data 数据

	// 计算校验和(直接引用原理版计算, 更直观, 直接变成inline 内联函数更好, 省得维护多一个函数, 反正这里是2byte 粒度的)
	return cksum((unsigned char *)buffer, (len + 20) / sizeof(unsigned short));
}

#ifdef __cplusplus
	//};
#endif
