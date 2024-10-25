#coding=UTF-8

import socket

# 字符串操作声明(兼容python2/3)
'''
	如果你在操作字节串(bytes), 并且想要编码, 请使用*.decode();
	(python string 字符串操作, 需要经过*.decode() 的数据)
	如果你在操作字符串(str), 并且想要编码成字节串, 请使用*.encode();
	(python socket io 需要发送/接收经过*.encode() 的数据)
'''

class server:
	def start_listen(self):
		sfd = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
		sfd.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
		sfd.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, True)
		sfd.bind(('127.0.0.1',9922))
		sfd.listen(5)

		while(True):
			# 接收客户端连接
			sfd_cli, addr = sfd.accept()
			# 接收客户端返回的消息
			data_r = sfd_cli.recv(1024)
			print("srv: recv() data len = " + str(len(data_r)) + ", data: "+ data_r.decode("utf-8"))
			# 向客户端发送信息
			data_s = "FU client, go to hell"
			data_len_s = sfd_cli.send(data_s.encode("utf-8"))
			print("srv: send() data len = " + str(data_len_s))
			sfd_cli.close()

if(__name__ == "__main__"):
	srv = server()
	srv.start_listen()
