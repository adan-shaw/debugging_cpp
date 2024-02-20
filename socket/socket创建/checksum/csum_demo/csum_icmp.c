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



int main (void)
{
	uint8_t icmp_message[] = {
		// ICMP 头部
		0x08, 					// ICMP类型(例如回显请求为8)
		0x00, 					// ICMP代码(例如回显请求为0)
		0x00, 0x00,			// ICMP校验和(这里先设置为0, 稍后计算)
		0x01, 0x02,			// 标识符
		0x03, 0x04,			// 序列号
		// ... 可能还有其他数据 ...
	};

	// ICMP消息长度(包括头部和数据)
	size_t message_length = sizeof (icmp_message);

	// 计算校验和并设置到ICMP消息的校验和字段
	uint16_t n_csum = checksum_icmp (icmp_message, message_length);

	// 设置校验和到 ICMP 头部
	memcpy (icmp_message + 2, &n_csum, sizeof (n_csum));

	// 打印校验和(仅供调试)
	printf ("ICMP Checksum: 0x%04X\n", n_csum);

	return 0;
}
