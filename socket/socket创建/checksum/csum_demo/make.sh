#!/bin/sh
gcc -g3 ./csum_icmp.c -o ./csum_icmp
gcc -g3 ./csum_igmp.c -o ./csum_igmp
gcc -g3 ./csum_ip.c -o ./csum_ip
gcc -g3 ./csum_tcp.c -o ./csum_tcp
gcc -g3 ./csum_udp.c -o ./csum_udp
gcc -g3 ./csum_sctp.c -o ./csum_sctp
