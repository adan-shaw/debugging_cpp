#coding=utf8



import socket
import time

HOST = '127.0.0.1'
PORT = 8888
BUF_MAX = 512

# udp
sfd = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, 0);

#sfd.setblocking(True);

# 开启地址&端口重用
sfd.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, True)
sfd.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, True)

sfd.bind((HOST,PORT));

#sfd.listen(True)
#sfd_tmp, addr_info = sfd.accept()

# 同步接收udp 报式报文
data, addr_info = sfd.recvfrom(BUF_MAX)
print(addr_info)
print('recvfrom():', repr(data))

# 同步发送udp 报式报文
sfd.sendto(b'Hello client, server reply your message ',addr_info)

time.sleep(1)

sfd.close()
