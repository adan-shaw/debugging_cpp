#!/bin/sh
g++ server.cpp -o server -lzmq
g++ sink.cpp -o sink -lzmq
g++ worker1.cpp -o worker1 -lzmq
g++ worker2.cpp -o worker2 -lzmq