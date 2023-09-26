#coding=utf8



import socket

HOST = '127.0.0.1'
PORT = 8888
BUF_MAX = 512

# tcp
sfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0);

# 禁用异步, 防止demo 复杂化
#sfd.setblocking(True);

# 开启地址&端口重用
sfd.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, True)
sfd.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, True)

sfd.bind((HOST,PORT));

sfd.listen(True)

# 接收客户端
sfd_tmp, addr_info = sfd.accept()
with sfd_tmp:
	print('Connected by', addr_info)
	while True:
		# 同步接收
		data = sfd_tmp.recv(BUF_MAX)
		if not data:
			break
		# 同步发送
		sfd_tmp.sendall(data)

sfd_tmp.close()
sfd.close()
