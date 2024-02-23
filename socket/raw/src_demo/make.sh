#!/bin/sh

gcc -g3 cksum.h cksum.c ./raw_icmp_s.c -o raw_icmp_s
gcc -g3 cksum.h cksum.c ./raw_tcp_s.c -o raw_tcp_s
gcc -g3 cksum.h cksum.c ./raw_udp_s.c -o raw_udp_s
