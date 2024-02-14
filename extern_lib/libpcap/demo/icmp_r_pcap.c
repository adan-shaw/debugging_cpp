#include <stdio.h>
#include <pcap.h>
#include <netinet/ip.h>
//#include <netinet/icmp6.h>
#include <netinet/ip_icmp.h>
#include <netinet/if_ether.h>



void print_icmp_packet (const u_char * packet, const struct pcap_pkthdr *header)
{
	struct ip *ip_hdr = (struct ip *) (packet + sizeof (struct ether_header));
	struct icmp *icmp_hdr = (struct icmp *) (packet + sizeof (struct ether_header) + sizeof (struct ip));

	printf ("[%ld-%ld", header->ts.tv_sec, header->ts.tv_usec);
	printf ("ICMP] Type: %d", icmp_hdr->icmp_type);
	printf (" Code: %d", icmp_hdr->icmp_code);
	printf (" Checksum: %04x", ntohs (icmp_hdr->icmp_cksum));
	printf (" ID: %04x", ntohs (ip_hdr->ip_id));
	printf (" Sequence: %04x\n", ntohs (icmp_hdr->icmp_seq));
}

int check_alive_eth_dev(void){
	char errbuf[PCAP_ERRBUF_SIZE];
	char *dev = pcap_lookupdev(errbuf);
	if (dev == NULL) {
		fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
		return -1;
	}
	return 0;
}

#define LIBPCAP_PACKET_MAX (576)

int main (void)
{
	char errbuf[PCAP_ERRBUF_SIZE];
	struct bpf_program fp;//过滤规则info 容器
	const char filter_exp[] = "icmp";
	const char dev_name[] = "lo";
	bpf_u_int32 mask;
	struct pcap_pkthdr header;
	const u_char *packet;

	// 以混杂模式, 打开网络设备"lo"(回环网络) [第三参数=1, 表示混杂模式], 返回pcap_t 通用sfd info容器(libpcap 专用)
	pcap_t *handle = pcap_open_live (dev_name, LIBPCAP_PACKET_MAX, 1, 1000, errbuf);
	if (handle == NULL)
	{
		if(check_alive_eth_dev() == -1)
			printf("check_alive_eth_dev() failed!!");
		fprintf (stderr, "pcap_open_live() failed: [%s]-%s\n", dev_name, errbuf);
		return -1;
	}

	// 生成过滤规则: 专门过滤icmp [Compile the filter to capture only ICMP packets]
	if(pcap_compile (handle, &fp, filter_exp, 0, mask) == -1) {
		fprintf(stderr, "Couldn't compile filter: [%s]-%s\n", dev_name, errbuf);
		return -1;
	}

	// 设置过滤规则生效 [Apply the filter to capture]
	if(pcap_setfilter (handle, &fp) == -1) {
		fprintf(stderr, "Couldn't apply filter: [%s]-%s\n", dev_name, errbuf);
		return -1;
	}

	// 抓包, 并打印每一帧ICMP 包 [ Capture and print each ICMP packet ]
	while (1)
	{
		// 单次抓包libpcap 抓包, 没有回调函数, 直接取出数据buf 进行处理
		if ((packet = pcap_next(handle, &header)) == NULL)
		{
			// 一旦出错, 表示读取已经结束[End of capture or error occurred]
			break;
		}
		print_icmp_packet (packet, &header);
	}

	pcap_freecode (&fp);//释放过滤规则info 容器
	pcap_close (handle);//释放pcap_t 通用sfd info容器(libpcap 专用)
	return 0;
}
