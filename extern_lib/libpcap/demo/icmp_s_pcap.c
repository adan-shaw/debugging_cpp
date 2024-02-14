#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcap.h>

#include <netinet/ip_icmp.h>
#include <linux/in.h>


#define LIBPCAP_PACKET_MAX (576)

int main (void)
{
	char *dev, errbuf[PCAP_ERRBUF_SIZE];
	pcap_t *handle;
	struct in_addr target_addr;
	struct icmp *icmp;
	struct timeval ts;
	unsigned char packet[LIBPCAP_PACKET_MAX];
	int packet_len;

	// 获取可用的网络设备
	if ((dev = pcap_lookupdev (errbuf)) == NULL)
	{
		fprintf (stderr, "Device not found: %s\n", errbuf);
		return -1;
	}

	// 以混杂模式, 打开网络设备"lo"(回环网络) [第三参数=1, 表示混杂模式], 返回pcap_t 通用sfd info容器(libpcap 专用)
	handle = pcap_open_live (dev, LIBPCAP_PACKET_MAX, 1, 1000, errbuf);
	if (handle == NULL)
	{
		fprintf (stderr, "Error opening device %s: %s\n", dev, errbuf);
		return -1;
	}

	// 设置目标IP地址(ping 需要ip 地址, 但不需要指定端口)
	target_addr.s_addr = inet_addr ("127.0.0.1");

	// 填充icmp 数据包
	icmp = (struct icmp *) packet;
	icmp->icmp_type = ICMP_ECHO;			// 回显请求类型
	icmp->icmp_id = getpid ();				// 当前进程ID作为标识符
	icmp->icmp_seq = 1;								// 序列号(可选)

	gettimeofday (&ts, NULL);					// 获取当前时间戳
	packet_len = sizeof (struct icmp);// 填充后的数据包长度(不包括校验和)
	memcpy (&icmp->icmp_cksum, &ts, sizeof (struct timeval));	// 将时间戳填充到校验和字段中(可选)
	icmp->icmp_cksum = csum(packet, packet_len);			// 计算ICMP数据包的校验和(可选)
	memset ((char *) packet + sizeof (struct icmp), 0, LIBPCAP_PACKET_MAX - sizeof (struct icmp));// 剩余部分数据, 填充为0(可选, 不填充也行)

	// 循环发送ICMP回显请求数据包, 会自动调用pcap_sendpacket 回调函数, 进行icmp 数据发送
	if (pcap_loop (handle, 1, pcap_sendpacket, NULL) < 0)
	{
		fprintf (stderr, "pcap_loop() failed: [%s]-%s\n", dev, pcap_geterr (handle));
		return -1;
	}

	pcap_close (handle);
	return 0;
}
