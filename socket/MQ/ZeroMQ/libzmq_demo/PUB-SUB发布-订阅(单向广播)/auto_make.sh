#!/bin/sh
g++ server.c -o server -lzmq
g++ client1.c -o client1 -lzmq
g++ client2.c -o client2 -lzmq