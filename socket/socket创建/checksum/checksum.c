#include <string.h>



//tcp/ip 通用的checksum() 计算函数(用户态), 内核态有自己的checksum() 计算函数



//chechksum 计算原理简释
/*
	# ip校验和的计算(ip/icmp/igmp/sctp 适用)
		ip校验是针对ip头部的, 即仅校验ip头部, 而对于ip数据部分的校验, 则交由相应的'四层协议'来保证, ip 头部中校验和字段为16bit; 

		计算原理如下: 
			把校验和字段设置为0
			计算ip头部中所有16bit的字之和
			将2中得到的和按位取反, 得到校验和; 


	# tcp/udp 适用, 带'ip伪首部':
		* TCP校验和的计算原理对于ip层协议来说, 其校验和只要计算ip头即可, 那相对的, 对于'四层协议'来说, 其校验和则需要计算四层头部与四层数据; 
			TCP校验需要将'ip伪首部'、TCP报头、TCP数据分为16位的字, 然后进行累加(如果总长度为奇数个字节, 则在最后增添一个位都为0的字节 ), 最后对累加的和进行按位取反即可; 
			'ip伪首部'包括源ip地址(4字节)、目的ip地址(4字节)、协议号(两字节)、tcp包长(2字节), 共14字节; 

		* UDP校验和的计算原理
			UDP校验与TCP校验基本上是一致的; 
			UDP校验需要将'ip伪首部'、UDP报头、UDP数据分为16位的字, 然后进行累加(如果总长度为奇数个字节, 则在最后增添一个位都为0的字节), 最后对累加的和进行按位取反即可; 
			'ip伪首部'包括源ip地址(4字节)、目的ip地址(4字节)、协议号(2字节)、TCP包长(2字节), 共12字节; 

		为什么要加入'ip伪首部', 详情可以查看: ./checksum加入伪头部的条件
*/



// checksum 调试时的技巧
/*
	* 以wireshark 为准:
		调试tcp/ip 报文时, 一切以wireshark 为准!!
		如果wireshark 说你某个部分算错了, 那你得考虑一下这部分是不是真的有问题!!
		这不是业务逻辑, 这是最简单的协议报文校验, wireshark 在校验协议报文方面, 应该是不怎么会出错的, 概率太小, 多想想自己的问题出在哪;

	* 如果wireshark 校验出错, 这种报文就是无效的, 在广域网中传播会有问题, 会被丢包的;
		因此, 如果wireshark 报错说checksum 校验错误, 即报文有问题, 需要一一校正!!
		(ps: 其实校验报文的工作, 比较麻烦复杂, 需要耐性)
*/



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





//checksum_old() 计算原理版, 对ip/icmp/gimp/udp/tcp/sctp 都有效, 只是传入参数时, 需要校验的数据部位不同而已
unsigned short checksum_old(unsigned short *data, int count){
	unsigned int sum = 0;
	unsigned char left_over[2] = {0};
	unsigned short n_csum;

	//计算所有数据的16bit对之和
	while( count > 1 ){
		sum += *(unsigned short*)data++;
		count -= 2;
	}

	//如果数据长度为奇数, 在该字节之后补一个字节(0); 然后将其转换为16bit整数, 加到上面计算的校验和中;
	if( count > 0 ){
		left_over[0] = *data;
		sum += *(unsigned short*)left_over;
	}

	//将32bit数据压缩成16bit数据, 即将进位加大校验和的低字节上, 直到没有进位为止;
	while (sum >> 16)
		sum = (sum & 0xffff) + (sum >> 16);

	//返回校验和的反码
	n_csum = (unsigned short)(~sum);
	if (n_csum == 0)
		return 0xFFFF;
	else
		return n_csum;
}



//checksum() 计算通用版(ip/icmp/igmp/sctp 适用)
unsigned short checksum(void *data, int count){
	unsigned int sum = 0;
	unsigned char left_over[2] = {0};
	unsigned short n_csum, *dataEx = data;

	//sum = 0;
	//计算所有数据的16bit对之和
	while( count > 1 ){
		sum += *(unsigned short*)dataEx++;
		count -= 2;
	}

	//如果数据长度为奇数, 在该字节之后补一个字节(0); 然后将其转换为16bit整数, 加到上面计算的校验和中;
	if( count > 0 ){
		left_over[0] = *dataEx;
		sum += *(unsigned short*)left_over;
	}

	//将32bit数据压缩成16bit数据, 即将进位加大校验和的低字节上, 直到没有进位为止;
	while (sum >> 16)
		sum = (sum & 0xffff) + (sum >> 16);

	//返回校验和的反码
	n_csum = (unsigned short)(~sum);
	if (n_csum == 0)
		return 0xFFFF;
	else
		return n_csum;
}



//checksum() 计算通用版Ex(tcp/udp 适用), 最后一个参数决定是tcp=1, udp=0;
//如果buffer 申请缓冲区不正确, 可以用: #include <alloca.h> 来代替, 直接在线程栈上面申请内存
unsigned short checksumEx (void *data, int length, unsigned int saddr, unsigned int daddr, unsigned short tcp_udp){
	unsigned short buffer[(length + 20) / sizeof(unsigned short)];

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
	buffer[6] = length;								// 2 byte(TCP 头部和数据总长度) -- 伪头部一共12 bit 
	memcpy (&buffer[7], data, length);// copy data to new buffer[] -- 从第13 bit 开始, 填充data 数据

	// 计算校验和(直接引用原理版计算, 更直观, 直接变成inline 内联函数更好, 省得维护多一个函数, 反正这里是2byte 粒度的)
	return checksum_old(buffer, (length + 20) / sizeof(unsigned short));
}





//ps: tcp/ip 报文填充时, 还是靠原始的内存填充, 一字节一字节地填充的, 所以要比较熟读tcp/ip 的报文细节才行;



//tcp/ip 协议栈的结构体描述
/*
	* struct iphdr {} 等价于struct ip {} 结构体, 只是struct ip {} 结构体使用时更方便一些;
		(但使用struct iphdr {} 比较标准一点, 因为其它的协议tcp/udp/icmp/igmp/sctp, 报文结构体的命名都是 struct ***hdr {} 这种格式的)

	* struct ip6_hdr {}
	* struct tcphdr {}
	* struct udphdr {}
	* struct icmphdr {}
	* struct icmp6_hdr {}
	* struct igmp {}
*/



//不同协议的计算方法list:
/*
	计算checksum 时, 需要先将tcp/ip 报文中的checksum uint 先置0(这是统一做法), 再进行checksum() 计算,
	(ps: 因此, tcp/ip 协议栈填充时, 一般都将checksum 放在最后再进行填充)

	ip:			ip 头 + ip 头延长部分(如果有就加, 没有就不加, 或者+0)
	icmp:		icmp 头 + icmp 头延长部分
	igmpv1:	igmpv1 头 + igmpv1 头延长部分
	igmpv2:	igmpv2 头 + igmpv2 头延长部分
	igmpv3:	igmpv3 头 + igmpv3 头延长部分
	udp:		udp 头(含ip 伪头部) + udp 数据
	tcp:		tcp 头(含ip 伪头部) + tcp 头延长部分 + tcp 数据
	sctp:		sctp 头 + sctp 头延长部分 + sctp 数据
*/



//内存拷贝整个报头时, 不同协议的拷贝地址起点'刻度'list:
/*
RAW(原始套接字):
	ip:			+0 (起点);
	icmp:		+sizeof(struct ip) + sizeof(unsigned char ipEx); # ipEx 延长部分, 没有可以不加, 或者+0
	igmpv1:	+sizeof(struct ip) + sizeof(unsigned char ipEx);
	igmpv2:	+sizeof(struct ip) + sizeof(unsigned char ipEx);
	igmpv3:	+sizeof(struct ip) + sizeof(unsigned char ipEx);
	udp:		+sizeof(struct ip) + sizeof(unsigned char ipEx);
	tcp:		+sizeof(struct ip) + sizeof(unsigned char ipEx);
	sctp:		+sizeof(struct ip) + sizeof(unsigned char ipEx);

ETH(链路层套接字):
	eth:		+0 (起点)
	ip:			+14(固定)
	icmp:		+14 + sizeof(struct ip) + sizeof(unsigned char ipEx); # ipEx 延长部分, 没有可以不加, 或者+0
	igmpv1:	+14 + sizeof(struct ip) + sizeof(unsigned char ipEx);
	igmpv2:	+14 + sizeof(struct ip) + sizeof(unsigned char ipEx);
	igmpv3:	+14 + sizeof(struct ip) + sizeof(unsigned char ipEx);
	udp:		+14 + sizeof(struct ip) + sizeof(unsigned char ipEx);
	tcp:		+14 + sizeof(struct ip) + sizeof(unsigned char ipEx);
	sctp:		+14 + sizeof(struct ip) + sizeof(unsigned char ipEx);
*/



//内存拷贝checksum to 报头时, 不同协议的拷贝地址起点'刻度'list(固定的刻度):
/*
	计算checksum 时, 需要先将tcp/ip 报文中的checksum uint 先置0(这是统一做法), 再进行checksum() 计算,
	(ps: 因此, tcp/ip 协议栈填充时, 一般都将checksum 放在最后再进行填充)

	ip:			+10
	icmp:		+2
	igmpv1:	+2
	igmpv2:	+2
	igmpv3:	+2
	udp:		+6
	tcp:		+16
	sctp:		+8

ps:
	具体看checksum 定义在协议报文的哪个位置, 固定位置, 固定刻写;
*/





