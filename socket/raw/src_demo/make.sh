#/bin/sh

gcc -g3 ./raw_icmp_r.c -o raw_icmp_r
gcc -g3 ./easy_checksum.c ./raw_icmp_s.c -o raw_icmp_s
gcc -g3 ./raw_tcp_r.c -o raw_tcp_r
gcc -g3 ./easy_checksum.c ./raw_tcp_s.c -o raw_tcp_s
gcc -g3 ./raw_udp_r.c -o raw_udp_r
gcc -g3 ./easy_checksum.c ./raw_udp_s.c -o raw_udp_s
