#!/bin/sh
./csum_icmp
./csum_igmp
./csum_ip
./csum_tcp
./csum_udp
./csum_sctp

# 测试的正确结果鉴别:
:<<!
ICMP Checksum: 0xF9F3
IGMP Checksum: 0xD7EA
IP Header Checksum: 0x5D59
TCP Checksum: 0x3867
UDP Checksum: 0xCCB3
SCTP Checksum: 0xBFC7
!
