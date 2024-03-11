#!/bin/sh

gcc -g3 unix_sock_frame.h usock_udp_cli.c -o x_cli
gcc -g3 unix_sock_frame.h usock_udp_srv.c -o x_srv 
