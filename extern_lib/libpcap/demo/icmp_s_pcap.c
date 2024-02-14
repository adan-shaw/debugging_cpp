#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcap.h>
#include <netinet/ip_icmp.h>

int main (void)
{
	char *dev, errbuf[PCAP_ERRBUF_SIZE];
	pcap_t *handle;
	struct in_addr target_addr;
	struct icmp *icmp;
	struct timeval ts;
	unsigned char packet[1500];
	int packet_len;

	// 获取网络设备
	dev = pcap_lookupdev (errbuf);
	if (dev == NULL)
	{
		fprintf (stderr, "Device not found: %s\n", errbuf);
		return (2);
	}

	// 打开设备
	handle = pcap_open_live (dev, BUFSIZ, 1, 1000, errbuf);
	if (handle == NULL)
	{
		fprintf (stderr, "Error opening device %s: %s\n", dev, errbuf);
		return (2);
	}

	// 设置目标IP地址
	target_addr.s_addr = inet_addr ("192.168.1.1");	// 替换为你想要发送ping请求的目标IP地址

	// 创建ICMP回显请求数据包
	icmp = (struct icmp *) packet;
	icmp->icmp_type = ICMP_ECHO;	// 回显请求类型
	icmp->icmp_id = getpid ();		// 当前进程ID作为标识符
	icmp->icmp_seq = 1;						// 序列号(可选)
	gettimeofday (&ts, NULL);			// 获取当前时间戳
	memcpy (&icmp->icmp_cksum, &ts, sizeof (struct timeval));	// 将时间戳填充到校验和字段中(可选)
	memset ((char *) packet + sizeof (struct icmp), 0, BUFSIZ - sizeof (struct icmp));	// 填充零到数据部分(可选)
	packet_len = sizeof (struct icmp);	// 填充后的数据包长度(不包括校验和)

	// 计算ICMP数据包的校验和(可选)
	// icmp->icmp_cksum = pcap_checksum(packet, packet_len, NULL);

	// 发送ICMP回显请求数据包
	if (pcap_loop (handle, 1, pcap_sendpacket, NULL) < 0)
	{
		fprintf (stderr, "pcap_loop() failed: %s\n", pcap_geterr (handle));
		return (2);
	}

	// 关闭设备句柄和释放资源
	pcap_close (handle);
	return 0;
}
