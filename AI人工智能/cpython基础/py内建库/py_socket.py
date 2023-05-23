#coding=utf-8



import socket



# 现在的python 编程, 基本上都会用到框架, 所以根本不需要自己搭建socket 软件, 所以, 尽量不学, 省略, 知道就好;



# unix socket
sfd = socket.socket(AF_UNIX, SOCK_STREAM, 0); # tcp
sfd = socket.socket(AF_UNIX, SOCK_DGRAM, 0);  # udp

# af_inet socket
sfd = socket.socket(AF_INET, SOCK_STREAM, 0);
sfd = socket.socket(AF_INET, SOCK_DGRAM, 0);



# socket API list
sfd.bind();
sfd.recv();
sfd.send();
sfd.sendall();
sfd.recvfrom();
sfd.sendto();
sfd.close();
sfd.getpeername();
sfd.getsockname();
sfd.settimeout();
sfd.gettimeout();
sfd.fileno();
sfd.setblocking();
sfd.makefile();
sfd.setsockopt();
sfd.getsockopt();

# tcp 专用
sfd.listen();
sfd.accept();
sfd.connect();
sfd.connect_ex();



