#include <stdint.h>
#include <string.h>
#include <stdio.h>

// 计算 ICMP 消息的校验和
uint16_t checksum_icmp (const void *icmp_message, size_t message_length)
{
	uint16_t n_csum, *data = (uint16_t *) icmp_message;
	uint32_t i, sum;

	if (message_length % 2 != 0)
	{
		data[message_length / 2] = (data[message_length / 2] & 0xFF) << 8;
		++message_length;
	}

	sum = 0;
	for (i = 0; i < message_length / sizeof (uint16_t); ++i)
	{
		sum += data[i];
		if (sum >> 16)
		{
			sum = (sum & 0xFFFF) + (sum >> 16);
		}
	}

	if (sum >> 16)
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

// 计算 IGMP 消息的校验和
uint16_t checksum_igmp (const void *igmp_message, size_t message_length)
{
	uint16_t n_csum, *data = (uint16_t *) igmp_message;
	uint32_t i, sum;

	if (message_length % 2 != 0)
	{
		data[message_length / 2] = (data[message_length / 2] & 0xFF) << 8;
		++message_length;
	}

	sum = 0;
	for (i = 0; i < message_length / sizeof (uint16_t); ++i)
	{
		sum += data[i];
	}

	while (sum >> 16)
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

// 计算 IP 校验和
uint16_t checksum_ip (const void *ip_header, size_t header_length)
{
	uint16_t n_csum, *data = (uint16_t *) ip_header;;
	uint32_t i, sum = 0;

	for (i = 0; i < header_length / sizeof (uint16_t); ++i)
	{
		sum += data[i];
	}

	while (sum >> 16)
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

// 计算 TCP 伪头部和数据的校验和
uint16_t checksum_tcp (const void *data, size_t length, uint32_t saddr, uint32_t daddr)
{
	uint32_t i, sum;
	uint16_t n_csum, buffer[length / sizeof (uint16_t) + 20 / sizeof (uint16_t)];

	// 填充伪头部
	buffer[0] = saddr;									//源ip 地址uint
	buffer[1] = saddr >> 16;
	buffer[2] = daddr;									//目的ip 地址uint
	buffer[3] = daddr >> 16;
	buffer[4] = 0;											// 保留字段, 通常为 0
	buffer[5] = 0;											// 协议号, TCP 为 6
	buffer[6] = length;									// TCP 头部和数据总长度
	memcpy (&buffer[7], data, length);	// 复制 TCP 头部和数据到缓冲区

	sum = 0;
	for (i = 0; i < (length + 20) / sizeof (uint16_t); i++)
	{
		sum += buffer[i];
		if (sum > 0xFFFF)
		{
			sum = (sum & 0xFFFF) + (sum >> 16);
		}
	}

	if (sum >> 16)
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

// 计算 UDP 校验和
uint16_t checksum_udp (const void *data, size_t length)
{
	uint16_t n_csum, *words = (uint16_t *) data;
	uint32_t i, sum = 0;

	for (i = 0; i < length / sizeof (uint16_t); i++)
	{
		sum += words[i];
	}

	if (length % sizeof (uint16_t) != 0)
	{
		sum += (uint16_t) ((const uint8_t *) data)[length - 1] << 8;
	}

	if (sum >> 16)
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
