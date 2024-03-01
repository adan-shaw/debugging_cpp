#!/bin/sh

gcc -g3 ./tcp_basic.c tcp_basic_io.c ./tcp_basic_srv.c -o x_srv; 
gcc -g3 ./tcp_basic.c tcp_basic_io.c ./tcp_basic_cli.c -o x_cli; 
