#include <stdint.h>
#include <string.h>
#include <stdio.h>

// 计算 16 位字的校验和
uint16_t ones_complement_sum (const uint16_t * data, size_t length)
{
	uint32_t i, sum = 0;

	// 对每个 16 位字求和
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

// 计算 IGMP 消息的校验和
uint16_t compute_igmp_checksum (const void *igmp_message, size_t message_length)
{
	// 将消息转换为 16 位字的数组
	uint16_t n_csum, *data = (uint16_t *) igmp_message;

	// 如果是奇数字节, 最后一个字节前面补 0
	if (message_length % 2 != 0)
	{
		// 最后一个字节前面补 0, 形成 16 位字
		data[message_length / 2] = (data[message_length / 2] & 0xFF) << 8;
		++message_length;						// 考虑补上的 0 字节
	}

	// 计算校验和
	n_csum = ones_complement_sum (data, message_length / sizeof (uint16_t));

	// 如果校验和为 0, 返回全 1(因为补码表示)
	if (n_csum == 0)
	{
		return 0xFFFF;
	}

	// 否则返回校验和的补码
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
	uint16_t n_csum = compute_igmp_checksum (igmp_message, message_length);
	memcpy ((uint8_t *) & igmp_message[2], &n_csum, sizeof (n_csum));

	// 打印校验和(仅供调试)
	printf ("IGMP Checksum: 0x%04X\n", n_csum);

	return 0;
}
