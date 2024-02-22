#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "checksum.h"

// 计算 SCTP 校验和
int main (void)
{
	uint8_t sctp_data[] = {
		// SCTP 头部
		0x00, 0x01, 																		//源端口号
		0x02, 0x03, 																		//目标端口号
		0x04, 0x05, 0x06, 0x07,													//验证flag 标记
		0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F	//checksum (64bit)
		// ... 数据部分 ...
	};

	// IP头部长度(不包括选项字段)
	size_t sctp_length = sizeof (sctp_data);

	unsigned short n_csum = checksum (sctp_data, sctp_length);

	// 设置校验和到 SCTP 头部
	memcpy (sctp_data + 8, &n_csum, sizeof (n_csum));

	printf ("SCTP Checksum: 0x%04X\n", n_csum);

	return 0;
}
