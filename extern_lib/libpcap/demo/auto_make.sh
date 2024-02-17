#!/bin/sh

gcc -g3 ./icmp_r_pcap.c -o icmp_r_pcap -lpcap

gcc -g3 crc32c_x86.c ./icmp_s_pcap.c -o icmp_s_pcap -lpcap
