#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "checksum.h"

// 计算 UDP 校验和
int main (void)
{
	uint8_t udp_data[] = {
		// UDP 头部
		0x12, 0x34,									// 源端口
		0x56, 0x78,									// 目的端口
		0x00, 0x1C,									// UDP 数据总长
		0x00, 0x00,									// 校验和(这里先设置为 0, 稍后计算)
		// ... 数据部分 ...
	};

	// UDP 头部和数据总长度
	size_t length = sizeof (udp_data);

	// 源地址和目的地址(示例)
	uint32_t saddr = 0x12345678;
	uint32_t daddr = 0x87654321;

	// 计算校验和
	unsigned short n_csum = checksumEx (udp_data, length, saddr, daddr, 0);

	// 设置校验和到 UDP 头部
	memcpy (udp_data + 6, &n_csum, sizeof (n_csum));

	// 打印校验和(仅供调试)
	printf ("UDP Checksum: 0x%04X\n", n_csum);

	return 0;
}
