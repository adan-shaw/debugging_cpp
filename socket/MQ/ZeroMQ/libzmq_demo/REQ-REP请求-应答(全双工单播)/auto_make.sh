#!/bin/sh
g++ server.cpp -o server -lzmq
g++ client.cpp -o client -lzmq
