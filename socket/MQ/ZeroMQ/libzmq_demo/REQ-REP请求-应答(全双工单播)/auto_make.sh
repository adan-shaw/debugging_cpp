#!/bin/sh
g++ server.c -o server -lzmq
g++ client.c -o client -lzmq
