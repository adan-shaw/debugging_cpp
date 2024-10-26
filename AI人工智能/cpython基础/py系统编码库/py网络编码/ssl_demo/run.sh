#!/bin/sh

# 仅支持python3, python2 不支持: ssl.PROTOCOL_TLS_CLIENT
#py=python
py=python3

# 启动srv
#$py ./srv.py &
$py ./srv_ssl.py &

sleep 1

# 启动cli
#$py ./cli.py &
$py ./cli_ssl.py &
