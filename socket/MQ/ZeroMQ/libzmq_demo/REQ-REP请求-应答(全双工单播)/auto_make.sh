#!/bin/sh
gcc server.c -o server -lzmq
gcc client.c -o client -lzmq
