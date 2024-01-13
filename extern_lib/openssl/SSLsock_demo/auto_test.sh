#!/bin/sh

# ./srv port listen_max path_ca path_key
./srv 127.0.0.1 9213 64 ca.pem priv.key &

# ./cli srv_ip srv_port (客户端总是报错: connect(): Connection refused)
./cli 127.0.0.1 9213 &
#./cli 127.0.0.1 9213 ca.pem priv.key &



# 客户端报错connect(): Connection refused 分析
# 1.使用方法错误, client 也应该指定证书(尝试失败, 按道理, cli 应该是从srv 自动获取证书的, passed)
# 	./cli 127.0.0.1 9213 ca.pem priv.key &
# 2.client 编码还有没有捋清楚的地方
# 3.server 编码, 有缺失, 没有捋清楚的地方(概率较低, server 已经正常监听了)
# 4.server 没有root 权限, 不能做SSL listen()[这种情况已经排除, 有root 也不行]
# 5.ca 自签证书生成方法出错(有可能, 但概率不大, 因为这个证书, 在其他地方使用也没问题)



# bug 待处理!!
