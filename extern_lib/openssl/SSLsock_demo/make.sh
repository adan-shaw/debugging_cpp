#!/bin/sh

mkdir ssl.key

# 自动生成key & pem 密钥(自签证书)
openssl genrsa -out ./ssl.key/priv.key 2048
openssl req -new -x509 -key ./ssl.key/priv.key -out ./ssl.key/ca.pem -days 3650

# 自动生成私钥公钥
#openssl genrsa -out ./ssl.key/server.pem 2048 
#openssl rsa -in ./ssl.key/server.pem -pubout -out ./ssl.key/client.pem


key="./priv.key"

gcc -g3 cli.c -lssl -lcrypto -o cli

gcc -g3 srv.c -lssl -lcrypto -o srv

