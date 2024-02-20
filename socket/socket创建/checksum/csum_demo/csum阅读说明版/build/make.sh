#!/bin/sh
gcc -g3 ../csum_icmpEx.c -o ./csum_icmp
gcc -g3 ../csum_igmpEx.c -o ./csum_igmp
gcc -g3 ../csum_ipEx.c -o ./csum_ip
gcc -g3 ../csum_tcpEx.c -o ./csum_tcp
gcc -g3 ../csum_udpEx.c -o ./csum_udp
gcc -g3 ../csum_sctpEx.c -o ./csum_sctp
