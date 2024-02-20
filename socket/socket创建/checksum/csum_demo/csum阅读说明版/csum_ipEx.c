#include <stdint.h>
#include <string.h>
#include <stdio.h>

// 计算16位字的校验和
uint16_t ones_complement_sum (const uint16_t * data, size_t length)
{
	uint32_t i, sum = 0;

	// 对每个16位字求和
	for (i = 0; i < length; ++i)
	{
		sum += data[i];
	}

	// 处理溢出
	while (sum >> 16)
	{
		sum = (sum & 0xFFFF) + (sum >> 16);
	}

	// 返回补码(按位取反)
	return (uint16_t) (~sum);
}

// 计算IP头部的校验和
uint16_t compute_ip_checksum (const void * ip_header, size_t header_length)
{
	// 计算IP头部的校验和(不包括伪头部)
	uint16_t n_csum = ones_complement_sum (ip_header, header_length / sizeof (uint16_t));

	// 如果校验和为0, 则返回全1(因为补码表示)
	if (n_csum == 0)
	{
		return 0xFFFF;
	}

	// 否则返回校验和的补码
	return n_csum;
}

int main (void)
{
	// 这个AI 示例报文是错误的(弃用)
	/*
	uint16_t ip_header[] = {
		0x4500,											// 版本和头部长度
		0x003C,											// 总长度
		0x4000,											// 标识
		0x4001,											// 标志和片段偏移
		0x4006,											// TTL
		0x115C,											// 协议(例如TCP为0x06)
		0x0800,											// 头部校验和(这里先设置为0, 稍后计算)
		0x4001,											// 源IP地址(大端序)
		0x0800,											// 目的IP地址(大端序)
		// ... 可能还有其他选项字段 ...
	};
	*/
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
	uint16_t n_csum = compute_ip_checksum (ip_header, header_length);

	// 设置校验和到 IP 头部
	memcpy (ip_header + 10, &n_csum, sizeof (n_csum));

	// 打印校验和(仅供调试)
	printf ("IP Header Checksum: 0x%04X\n", ip_header[9]);

	return 0;
}
