#!/bin/sh
g++ server.cpp -o server -lzmq
g++ client1.cpp -o client1 -lzmq
g++ client2.cpp -o client2 -lzmq