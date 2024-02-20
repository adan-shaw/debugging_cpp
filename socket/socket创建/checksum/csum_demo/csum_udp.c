#include <stdint.h>
#include <string.h>
#include <stdio.h>

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

	// 计算校验和
	uint16_t n_csum = checksum_udp (udp_data, length);

	// 设置校验和到 UDP 头部
	memcpy (udp_data + 6, &n_csum, sizeof (n_csum));

	// 打印校验和(仅供调试)
	printf ("UDP Checksum: 0x%04X\n", n_csum);

	return 0;
}
