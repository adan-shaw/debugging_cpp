#!/bin/sh

mkdir tmp

touch ./tmp/uwsgi.log

touch ./tmp/uwsgi.pid

# 启动uwsgi
uwsgi --ini ./config.ini



# 重启uWSGI服务器
#sudo service uwsgi restart

# 查看所有uWSGI进程
ps aux | grep uwsgi

# 停止所有uWSGI进程
#sudo pkill -f uwsgi -9



# 测试uwsgi
curl 127.0.0.1:8001
