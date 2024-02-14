#include <stdio.h>
#include <pcap.h>
#include <netinet/ip.h>
//#include <netinet/icmp6.h>
#include <netinet/ip_icmp.h>
#include <netinet/if_ether.h>



void print_icmp_packet (const u_char * packet, int ts, unsigned int packet_len)
{
	struct ip *ip_hdr = (struct ip *) (packet + sizeof (struct ether_header));
	struct icmp *icmp_hdr = (struct icmp *) (packet + sizeof (struct ether_header) + sizeof (struct ip));

	printf ("[%ld ", ts);
	printf ("ICMP] Type: %d", icmp_hdr->icmp_type);
	printf (" Code: %d", icmp_hdr->icmp_code);
	printf (" Checksum: %04x", ntohs (icmp_hdr->icmp_cksum));
	printf (" ID: %04x", ntohs (ip_hdr->ip_id));
	printf (" Sequence: %04x\n", ntohs (icmp_hdr->icmp_seq));
}

int main (void)
{
	char errbuf[PCAP_ERRBUF_SIZE];
	pcap_t *handle;//pcap sfd 描述体(每个pcap 操作都离不开这个描述体, 相当于sfd;)
	struct bpf_program fp;
	bpf_u_int32 mask;
	int ts;
	unsigned int packet_len;
	u_char packet[65535];

	// Open the network interface for capturing
	handle = pcap_open_live ("eth0", 65535, 1, 1000, errbuf);
	if (handle == NULL)
	{
		fprintf (stderr, "pcap_open_live() failed: %s\n", errbuf);
		return 1;
	}

	// Compile and apply the filter to capture only ICMP packets
	pcap_compile (handle, &fp, "icmp", 0, mask);
	pcap_setfilter (handle, &fp);

	// Capture and print each ICMP packet
	while (1)
	{
		if (pcap_next_ex (handle, &packet, &packet_len) == 0)
		{
			break;										// End of capture or error occurred
		}
		ts = pcap_gettime ();				// Get the timestamp of the packet capture
		print_icmp_packet (packet, ts, packet_len);	// Print the ICMP packet details
	}

	// Cleanup resources
	pcap_freecode (&fp);					// Free the compiled filter program
	pcap_close (handle);					// Close the network interface handle
	return 0;
}
