#!/bin/sh
gcc server.c -o server -lzmq
gcc sink.c -o sink -lzmq
gcc worker1.c -o worker1 -lzmq
gcc worker2.c -o worker2 -lzmq