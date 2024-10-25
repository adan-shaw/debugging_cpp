#coding=UTF-8



import socket

HOST = '127.0.0.1'
PORT = 8888
BUF_MAX = 512

# tcp
sfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0);

print("connect() to {0} {1}".format(HOST, PORT))
sfd.connect((HOST, PORT))

# 同步发送
sfd.sendall(b'Hello server, the world is so big')

# 同步接收
data = sfd.recv(BUF_MAX)
print('recv(): ', repr(data))

sfd.close()
