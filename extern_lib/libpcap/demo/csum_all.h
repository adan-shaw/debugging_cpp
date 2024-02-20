#include <stddef.h>
#include <stdint.h>

// 计算 ICMP 消息的校验和 -- [+2]
uint16_t checksum_icmp (const void *icmp_message, size_t message_length);

// 计算 IGMP 消息的校验和 -- [v1=+2, v2=? v3=?]
uint16_t checksum_igmp (const void *igmp_message, size_t message_length);

// 计算 IP 校验和 -- [+10]
uint16_t checksum_ip (const void *ip_header, size_t header_length);

// 计算 SCTP 校验和 -- [+8]
uint16_t checksum_sctp (const void *buf, size_t len, uint32_t saddr, uint32_t daddr);

// 计算 TCP 伪头部和数据的校验和 -- [+16]
uint16_t checksum_tcp (const void *data, size_t length, uint32_t saddr, uint32_t daddr);

// 计算 UDP 校验和 -- [+6]
uint16_t checksum_udp (const void *data, size_t length);
