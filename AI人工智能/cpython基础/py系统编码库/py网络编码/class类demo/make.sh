#!/bin/sh

#py=python
py=python3

# 启动srv
$py ./srv.py &
#$py ./srv_ssl.py &

# 启动cli
$py ./cli.py &
#$py ./cli_ssl.py &
