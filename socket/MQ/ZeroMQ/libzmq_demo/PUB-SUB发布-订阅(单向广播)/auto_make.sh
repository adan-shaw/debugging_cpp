#!/bin/sh
gcc server.c -o server -lzmq
g++ client1.cpp -o client1 -lzmq -pthread
g++ client2.cpp -o client2 -lzmq -pthread