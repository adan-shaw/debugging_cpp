#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "checksum.h"

// 计算 IGMP 消息的校验和
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
	unsigned short n_csum = checksum (igmp_message, message_length);

	// 设置校验和到 IGMP 头部
	memcpy (igmp_message + 2, &n_csum, sizeof (n_csum));

	// 打印校验和(仅供调试)
	printf ("IGMP Checksum: 0x%04X\n", n_csum);

	return 0;
}
