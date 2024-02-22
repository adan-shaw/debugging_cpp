#!/bin/sh
gcc -g3 ./checksum.h ./checksum.c ./csum_icmp.c -o ./csum_icmp
gcc -g3 ./checksum.h ./checksum.c ./csum_igmp.c -o ./csum_igmp
gcc -g3 ./checksum.h ./checksum.c ./csum_ip.c -o ./csum_ip
gcc -g3 ./checksum.h ./checksum.c ./csum_tcp.c -o ./csum_tcp
gcc -g3 ./checksum.h ./checksum.c ./csum_udp.c -o ./csum_udp
gcc -g3 ./checksum.h ./checksum.c ./csum_sctp.c -o ./csum_sctp
