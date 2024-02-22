#!/bin/sh

gcc -g3 -lpthread ./dos_icmp.c -o dos_icmp

gcc -g3 -lpthread ./dos_syn.c -o dos_syn

gcc -g3 -lpthread ./dos_udp.c -o dos_udp

gcc -g3 -lpthread ./ping.c -o ping
