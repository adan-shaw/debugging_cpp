#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "checksum.h"

// 计算 TCP 伪头部和数据的校验和
int main (void)
{
	uint8_t tcp_data[] = {
		// TCP 头部
		0xD8, 0x5D,									// TCP 源端口
		0xDF, 0x1D,									// TCP 目的端口
		0x50, 0x18, 0x40, 0x00,			// 序列号
		0x40, 0x00, 0x40, 0x00,			// 确认号
		0x00, 0x10,									// 数据偏移量(4bit), 保留字段(6bit), SYN/FIN/RST/PSH/ACK/URG 控制阀(6bit)
		0x00, 0x00,									// 窗口
		0x67, 0x45,									// 校验和(这里先设置为 0, 稍后计算)
		0x00, 0x00,									// 紧急指针
		0x00, 0x00, 0x00, 0x00,			// [可删掉这部分]选项(默认24bit, 长度可变, 最长40bit = 8 + 32), 8bit 填充位(默认填充0, 如果可选字段拓展, 可以被占用)
		// ... 数据部分 ...
	};

	// TCP 头部和数据总长度
	size_t length = sizeof (tcp_data);

	// 源地址和目的地址(示例)
	uint32_t saddr = 0x12345678;
	uint32_t daddr = 0x87654321;

	// 计算校验和
	unsigned short n_csum = checksumEx (tcp_data, length, saddr, daddr, 1);

	// 设置校验和到 TCP 头部
	memcpy (tcp_data + 16, &n_csum, sizeof (n_csum));

	// 打印校验和(仅供调试)
	printf ("TCP Checksum: 0x%04X\n", n_csum);

	return 0;
}
