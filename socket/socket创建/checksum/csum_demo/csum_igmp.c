#include <stdint.h>
#include <string.h>
#include <stdio.h>

// 计算 IGMP 消息的校验和
uint16_t checksum_igmp (const void *igmp_message, size_t message_length)
{
	uint32_t i, sum;
	uint16_t n_csum, *data = (uint16_t *) igmp_message;

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


//IGMP 有多个版本: V1 V2 V3 (比较麻烦复杂)
int main (void)
{
	// 以IGMPv1 为例:
	uint8_t igmp_message[] = {
		// IGMPV1 头部
		0x11,												// Version 版本
		0x22,												// Type 类型
		0x00, 0x00,									// 校验和(这里先设置为 0, 稍后计算)
		0x01, 0x02, 0x03, 0x04			// Group Addr
		// ... 可能还有其他数据 ...
	};

	// IGMP 消息长度(包括头部和数据)
	size_t message_length = sizeof (igmp_message);

	// 计算校验和并设置到 IGMP 消息的校验和字段
	uint16_t n_csum = checksum_igmp (igmp_message, message_length);

	// 设置校验和到 IGMP 头部
	memcpy (igmp_message + 2, &n_csum, sizeof (n_csum));

	// 打印校验和(仅供调试)
	printf ("IGMP Checksum: 0x%04X\n", n_csum);

	return 0;
}
