#!/bin/sh

gcc -g3 -o x_srv unix_sock_frame.h usock_tcp_srv.c -o x_srv
gcc -g3 -o x_srv unix_sock_frame.h usock_tcp_cli.c -o x_cli 
