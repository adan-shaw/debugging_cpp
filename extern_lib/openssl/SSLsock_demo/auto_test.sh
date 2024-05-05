#!/bin/sh

# 启动SSL 需要root 权限, 否则(客户端总是报错: connect(): Connection refused), 这就是原因!!



# 杀死进程, 防止端口重复绑定
sudo pkill srv

# ./srv port listen_max path_ca path_key(不能使用相对路径, 所以使用`pwd`)
sudo ./srv 127.0.0.1 9213 64 `pwd`/ssl.key/ca.pem `pwd`/ssl.key/priv.key &

sleep 1

# ./cli srv_ip srv_port 
sudo ./cli 127.0.0.1 9213 &

# 杀死进程, 防止端口重复绑定
sleep 1
sudo pkill srv



# debug
# sudo gdb --args ./cli 127.0.0.1 9213
# sudo gdb --args ./srv 127.0.0.1 9213 64 `pwd`/ssl.key/ca.pem `pwd`/ssl.key/priv.key
