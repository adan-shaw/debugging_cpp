#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcap.h>

#include <netinet/ip_icmp.h>
#include <linux/in.h>

#include "csum_all.h"



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



//测试接收端: icmp_r_pcap.c, 直接使用ping 127.0.0.1 测试, 更方便;
//结论:      icmp_r_pcap 接收段没问题, icmp_s_pcap 发送端有问题
//ps:       你可以拿ping 命令的报文, 和自己发送的icmp_s_pcap 进行对比, 看看有多大差异, 就知道对错了;

#define LIBPCAP_PACKET_MAX (576)

int main (void)
{
	char *dev_default, errbuf[PCAP_ERRBUF_SIZE];
	const char dev_name[] = "lo";
	pcap_t *handle;
	struct icmp *icmp;
	struct timeval ts;
	unsigned char packet[LIBPCAP_PACKET_MAX];
	int packet_len;
	struct sockaddr_in dest;
	struct ip ip_hdr;
	struct icmp icmp_hdr;

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
	packet[0] = 0x00;	//目的MAC [ip a 可以看到lo 网口的MAC 地址， 0.0.0.0.0.0]
	packet[1] = 0x00;
	packet[2] = 0x00;
	packet[3] = 0x00;
	packet[4] = 0x00;
	packet[5] = 0x00;
	packet[6] = 0x00;	//源MAC
	packet[7] = 0x00;
	packet[8] = 0x00;
	packet[9] = 0x00;
	packet[10] = 0x00;
	packet[11] = 0x00;
	packet[12] = 0x08;//协议类型: 0x0800 = 普通ip 协议内容
	packet[13] = 0x00;

	// 填充目的地址的sockaddr_in (后面sendto 需要用)
	dest.sin_family = AF_INET;
	dest.sin_port = 0;
	dest.sin_addr.s_addr = inet_addr ("127.0.0.1");

	// 填充IP报头
	//memset (&ip_hdr, 0, sizeof (ip_hdr));
	ip_hdr.ip_v = 4;
	ip_hdr.ip_hl = 5;																										// 5 x 4 bytes = 最短ip 报文头
	ip_hdr.ip_tos = 0;
	ip_hdr.ip_len = htons (sizeof (struct ip) + sizeof (struct icmp));
	ip_hdr.ip_id = htons (getpid());																		// 示例ID
	ip_hdr.ip_off = 0;
	ip_hdr.ip_ttl = 64;																									// 示例TTL
	ip_hdr.ip_p = IPPROTO_ICMP;
	ip_hdr.ip_src.s_addr = inet_addr ("127.0.0.1");											// 示例: 源IP
	ip_hdr.ip_dst.s_addr = dest.sin_addr.s_addr;
	ip_hdr.ip_sum = 0;																									// ip checksum 值初始化
	ip_hdr.ip_sum = checksum_ip (&ip_hdr, sizeof (ip_hdr));							// ip checksum 值计算
																																			// (wireshark 显示仍然没有计算正确? 答: 以libpcap 为准, 绝对不会错的软件, 出错了就再检查)

	// 拷贝IP 报头到数据包中
	memcpy (packet + 14, &ip_hdr, sizeof (ip_hdr));

	// 填充ICMP报头
	//memset (&icmp_hdr, 0, sizeof (icmp_hdr));
	icmp_hdr.icmp_type = ICMP_ECHO;
	icmp_hdr.icmp_code = 0;
	icmp_hdr.icmp_id = htons (getpid());																// 与IP报头匹配
	icmp_hdr.icmp_seq = htons (1);																			// 序列号
	icmp_hdr.icmp_cksum = 0;																						// icmp checksum 值初始化
	icmp_hdr.icmp_cksum = checksum_icmp (&icmp_hdr, sizeof (icmp_hdr));	// 设置ICMP 报头校验和

	// 拷贝ICMP 报头到数据包中
	memcpy (packet + 14 + sizeof (ip_hdr), &icmp_hdr, sizeof (icmp_hdr));

	// 剩余部分数据, 填充为0(可选, 不填充也行)
	memset (packet + 14 + sizeof (ip_hdr) + sizeof (struct icmp), 0, LIBPCAP_PACKET_MAX - sizeof (struct icmp));

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
