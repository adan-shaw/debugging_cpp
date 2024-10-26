#!/bin/sh

py=python
#py=python3

$py ./server.py &
$py ./client.py &
$py ./worker.py &
