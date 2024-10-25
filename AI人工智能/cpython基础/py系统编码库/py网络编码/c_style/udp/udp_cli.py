#coding=UTF-8



import socket
import time

HOST = '127.0.0.1'
PORT = 8888
BUF_MAX = 512

# udp
sfd = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, 0);

print("sendto() to {0} {1}".format(HOST, PORT))

# 同步发送udp 报式报文
sfd.sendto(b'Hello server, i am client',(HOST, PORT))

# 同步接收udp 报式报文
data, addr_info = sfd.recvfrom(BUF_MAX)
print(addr_info)
print('recvfrom():', repr(data))

time.sleep(1)

sfd.close()
