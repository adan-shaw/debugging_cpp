#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "checksum.h"

// 计算 IP 校验和
int main (void)
{
	uint8_t ip_header[] = {
		// IP 头部
		0x45,												// 4bit + 4bit版本和头部长度
		0x00,												// 8 bit 区分服务
		0x00, 0x3C,									// 16 bit 数据总长度
		0x40, 0x00,									// 16 bit flag 标识
		0x40, 0x01,									// 3 bit 标志 + 13 bit 片偏移
		0x40,												// 8 bit 生存时间ttl
		0x06,												// 8 bit 协议类型(例如TCP为0x06)
		0x11, 0x5C,									// 校验和(这里先设置为 0, 稍后计算)
		0x08, 0x00, 0x40, 0x01,			// 源IP地址(大端序)
		0x08, 0x00, 0x40, 0x01,			// 目的IP地址(大端序)
		// ... 可能还有其他IP 选项字段 ...
		// ... 数据部分 ...
	};

	// IP头部长度(不包括选项字段)
	size_t header_length = sizeof (ip_header);

	// 计算校验和并设置到IP头部的校验和字段
	unsigned short n_csum = checksum (ip_header, header_length);

	// 设置校验和到 IP 头部
	memcpy (ip_header + 10, &n_csum, sizeof (n_csum));

	// 打印校验和(仅供调试)
	printf ("IP Header Checksum: 0x%04X\n", n_csum);

	return 0;
}
