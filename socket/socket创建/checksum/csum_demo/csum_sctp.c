#include <stdio.h>
#include <stdint.h>
#include <string.h>

// 计算 SCTP 校验和
uint16_t checksum_sctp (const void *buf, size_t len, uint32_t saddr, uint32_t daddr)
{
	uint16_t n_csum, *data = (uint16_t *)buf;
	uint32_t i, sum = 0;

	for (i = 0; i < len; i += 2)
	{
		sum += data[i / 2];
		if (sum > 0xFFFF)
		{
			sum = (sum & 0xFFFF) + (sum >> 16);
		}
	}

	if (len % 2 != 0)
	{
		sum += *((uint8_t *) buf + len - 1);
		if (sum > 0xFFFF)
		{
			sum = (sum & 0xFFFF) + (sum >> 16);
		}
	}

	sum += saddr;
	if (sum > 0xFFFF)
	{
		sum = (sum & 0xFFFF) + (sum >> 16);
	}
	sum += daddr;
	if (sum > 0xFFFF)
	{
		sum = (sum & 0xFFFF) + (sum >> 16);
	}
	sum += 0x0000FFFF;
	if (sum > 0xFFFF)
	{
		sum = (sum & 0xFFFF) + (sum >> 16);
	}

	n_csum = (uint16_t) (~sum);

	if (n_csum == 0)
	{
		return 0xFFFF;
	}

	return n_csum;
}



int main (void)
{
	uint8_t sctp_data[] = {
		// SCTP 头部
		0x00, 0x01, 																		//源端口号
		0x02, 0x03, 																		//目标端口号
		0x04, 0x05, 0x06, 0x07,													//验证flag 标记
		0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F	//checksum (64bit)
		// ... 数据部分 ...
	};

	uint32_t source_addr = 0x10000001;
	uint32_t dest_addr = 0x20000002;
	uint16_t n_csum = checksum_sctp (sctp_data, sizeof (sctp_data), source_addr, dest_addr);

	// 设置校验和到 SCTP 头部
	memcpy (sctp_data + 8, &n_csum, sizeof (n_csum));

	printf ("SCTP Checksum: 0x%04X\n", n_csum);

	return 0;
}
