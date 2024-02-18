#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcap.h>

#include <netinet/ip_icmp.h>
#include <linux/in.h>


#define LIBPCAP_PACKET_MAX (576)

#if defined(__x86_64__)
#include "crc32c_x86.c"
#define crc32c crc32c_x86
#warning "Using crc32c with x86 accelerations"
#endif



//libpcap 的AF_PACKET 发送/接收方式:
/*
	* 单次发送pcap_sendpacket(), 单次抓包(接收)pcap_next();
		[实际上, 可以使用逻辑循环, 其实简单比较好, 深度使用libpcap 库, 可以试试自带的循环'发送/接收']

	* 循环发送:
		- pcap_sendqueue_queue()    - 数据读入发送队列
		- pcap_sendqueue_transmit() - 一次性执行批量发送(性能高)
		- pcap_sendqueue_destroy()  - 发送队列资源销毁
		- pcap_next_ex()            - 发送数据'读出/读入'时, 会用到的辅助功能

		循环抓包(接收):
		- pcap_loop()               - 循环抓包(接收)函数, 第三参数是回调函数指针, 抓包之后, 调用回调函数, 自动进行拆包处理;
		- pcap_next_ex()            - 发送数据'读出/读入'时, 会用到的辅助功能

	总结:
		循环'发送/接收'的优点是: 效率高, 但逻辑需要熟练libpcap 库的使用;
*/

#define TTL_MAX (32)



int main (void)
{
	char *dev_default, errbuf[PCAP_ERRBUF_SIZE];
	const char dev_name[] = "lo";
	pcap_t *handle;
	struct icmp *icmp;
	struct ip *ip;
	struct timeval ts;
	unsigned char packet[LIBPCAP_PACKET_MAX];
	int packet_len;

	// 获取可用的网络设备(不能用默认可用设备, 大概率会自动选择wlp3s0, 而不是lo, 所以你才看不到数据, 必须强制指定网卡名为: "lo")
	if ((dev_default = pcap_lookupdev (errbuf)) == NULL)
	{
		fprintf (stderr, "Device not found: %s\n", errbuf);
		return -1;
	}
	fprintf (stdout, "pcap_lookupdev() return: [%s]\n", dev_default);

	// 以混杂模式, 打开网络设备"lo"(回环网络) [第三参数=1, 表示混杂模式], 返回pcap_t 通用sfd info容器(libpcap 专用)
	handle = pcap_open_live (dev_name, LIBPCAP_PACKET_MAX, 1, 1000, errbuf);
	if (handle == NULL)
	{
		fprintf (stderr, "Error opening device %s: %s\n", dev_name, errbuf);
		return -1;
	}

	//设置MAC 地址("lo" 网口的MAC 地址, 都是0.0.0.0.0.0, 目的地址, 发送地址, 都是这个, 但你不能不填, 否则出错!!)
	packet[0] = 0x00;//目的MAC [ip a 可以看到lo 网口的MAC 地址， 0.0.0.0.0.0]
	packet[1] = 0x00;
	packet[2] = 0x00;
	packet[3] = 0x00;
	packet[4] = 0x00;
	packet[5] = 0x00;
	packet[6] = 0x00;//源MAC
	packet[7] = 0x00;
	packet[8] = 0x00;
	packet[9] = 0x00;
	packet[10] = 0x00;
	packet[11] = 0x00;

	//ip 报文(没ip 报文, 何来icmp 报文? ip 报文必须跟上)
	packet_len = sizeof(struct ip) + sizeof(struct icmp);//在ICMP 的报文中没有Data字段, 所以整个IP报文的长度
	ip = (struct ip *) (packet + 12);
	//开始填充IP首部
	ip->ip_v = IPVERSION;
	ip->ip_hl = sizeof(struct ip)>>2;
	ip->ip_tos = 0;
	ip->ip_len = htons(packet_len);
	ip->ip_id = 0;
	ip->ip_off = 0;
	ip->ip_ttl = TTL_MAX;
	ip->ip_p = IPPROTO_ICMP;
	ip->ip_sum = 0;
	ip->ip_dst.s_addr = inet_addr ("127.0.0.1");// 设置目标IP地址(ping 需要ip 地址, 但不需要指定端口)

	// 填充icmp 数据包( 发送数据组装不正确, 发出去的数据包, 会被当成垃圾丢弃, 常见的查看方法: tcpdump 过滤法则方案: !tcp and !udp )
	icmp = (struct icmp *) (packet + 12 + 20);
	icmp->icmp_type = ICMP_ECHO;			// 回显请求类型
	icmp->icmp_id = getpid ();				// 当前进程ID作为标识符
	icmp->icmp_seq = 1;								// 序列号(可选)

	gettimeofday (&ts, NULL);					// 获取当前时间戳
	packet_len = sizeof (struct icmp);// 填充后的数据包长度(不包括校验和)
	memcpy (&icmp->icmp_cksum, &ts, sizeof (struct timeval));	// 将时间戳填充到校验和字段中(可选)
	crc32c(icmp->icmp_cksum, packet, packet_len);							// 计算ICMP数据包的校验和(可选)
	memset ((char *) packet + 12 + sizeof (struct icmp), 0, LIBPCAP_PACKET_MAX - sizeof (struct icmp));// 剩余部分数据, 填充为0(可选, 不填充也行)

	// 循环发送ICMP回显请求数据包(pcap_sendpacket() 正确返回值, 只有0)
	if (pcap_sendpacket(handle, packet, LIBPCAP_PACKET_MAX) != 0)
	{
		fprintf (stderr, "pcap_sendpacket() failed: [%s]-%s\n", dev_name, pcap_geterr (handle));
		return -1;
	}
	else
		// 获取发送了多长的数据, 还需要再调用: pcap_inject();
		fprintf (stdout, "pcap_sendpacket() return: [%s]-%d\n", dev_name, pcap_inject(handle, packet, LIBPCAP_PACKET_MAX));

	pcap_close (handle);
	return 0;
}
