#!/bin/sh

# 自动生成key & pem 密钥(自签证书)
#openssl genrsa -out priv.key 2048
#openssl req -new -x509 -key priv.key -out ca.pem -days 3650



key="./priv.key"

gcc -g3 cli.c -lssl -lcrypto -o cli

gcc -g3 srv.c -lssl -lcrypto -o srv

