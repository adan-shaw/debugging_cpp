#include <stdint.h>
#include <string.h>
#include <stdio.h>

// 计算16位字的校验和
uint16_t checksum (const uint16_t * data, size_t length)
{
	uint32_t i, sum = 0;

	// 对每个16位字求和
	for (i = 0; i < length; ++i)
	{
		sum += data[i];
		if (sum >> 16)
		{
			// 如果高16位有进位, 则将其加到低16位
			sum = (sum & 0xFFFF) + (sum >> 16);
		}
	}

	// 如果求和结果的高16位不为0, 则再次求和
	if (sum >> 16)
	{
		sum = (sum & 0xFFFF) + (sum >> 16);
	}

	// 返回补码(按位取反后加1)
	return (uint16_t) (~sum);
}

// 计算ICMP消息的校验和
uint16_t compute_icmp_checksum (const void *icmp_message, size_t message_length)
{
	// 将消息转换为16位字的数组
	uint16_t n_csum, *data = (uint16_t *) icmp_message;

	// ICMP校验和需要包括整个ICMP消息(头部和数据)
	// 如果是奇数字节, 最后一个字节前面补0
	if (message_length % 2 != 0)
	{
		// 最后一个字节前面补0, 形成16位字
		data[message_length / 2] = (data[message_length / 2] & 0xFF) << 8;
		++message_length;						// 考虑补上的0字节
	}

	// 计算校验和
	n_csum = checksum (data, message_length / sizeof (uint16_t));

	// 如果校验和为0, 返回全1(因为补码表示)
	if (n_csum == 0)
	{
		return 0xFFFF;
	}

	// 否则返回校验和的补码
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
	uint16_t n_csum = compute_icmp_checksum (icmp_message, message_length);
	memcpy ((uint8_t *) & icmp_message[2], &n_csum, sizeof (n_csum));

	// 打印校验和(仅供调试)
	printf ("ICMP Checksum: 0x%04X\n", n_csum);

	return 0;
}
