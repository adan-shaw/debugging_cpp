#!/bin/sh

# python2.7 的http 模块, 功能非常简陋, 代码量很少, 没有什么功能, 这里只做演示(不推荐使用python2.7);
#python -m BaseHTTPServer 8000 &
#python -m SimpleHTTPServer 8000 &



# python3.x 的http 模块, 功能比较完善
# 一切参数默认, 直接打开一个python 默认的http server, 监听端口8000(这种用法, 没有什么实用意义)
python3 -m http.server --bind 127.0.0.1 8000 --directory /tmp/ &



# 测试*.8000
curl 127.0.0.1:8000
