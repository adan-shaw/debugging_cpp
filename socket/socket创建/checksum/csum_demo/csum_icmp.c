#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "checksum.h"

// 计算 ICMP 消息的校验和
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
	unsigned short n_csum = checksum (icmp_message, message_length);

	// 设置校验和到 ICMP 头部
	memcpy (icmp_message + 2, &n_csum, sizeof (n_csum));

	// 打印校验和(仅供调试)
	printf ("ICMP Checksum: 0x%04X\n", n_csum);

	return 0;
}
