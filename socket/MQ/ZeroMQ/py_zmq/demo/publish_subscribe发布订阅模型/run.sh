#!/bin/sh

py=python
#py=python3

$py ./server.py &
$py ./client1.py &
$py ./client2.py &
