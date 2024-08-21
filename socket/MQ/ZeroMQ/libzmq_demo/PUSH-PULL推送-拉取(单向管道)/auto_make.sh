#!/bin/sh
g++ server.c -o server -lzmq
g++ sink.c -o sink -lzmq
g++ worker1.c -o worker1 -lzmq
g++ worker2.c -o worker2 -lzmq