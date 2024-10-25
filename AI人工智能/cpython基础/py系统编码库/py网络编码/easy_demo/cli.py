#coding=UTF-8

import socket

# 字符串操作声明(兼容python2/3)
'''
	如果你在操作字节串(bytes), 并且想要编码, 请使用*.decode()
	如果你在操作字符串(str), 并且想要编码成字节串, 请使用*.encode()
'''

class client:
	def send_hello(self):
		# 与服务端建立连接
		sfd = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
		sfd.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
		sfd.connect(('127.0.0.1',9922))

		# 向服务端发送消息
		data_s = "hello server"
		data_len_s = sfd.send(data_s.encode("utf-8"))
		print("cli: send() data len = " + str(data_len_s))

		# 接收服务端返回的消息
		data_r = sfd.recv(1024)
		print("cli: recv() data len = " + str(len(data_r)) + ", data: "+ data_r.decode("utf-8"))
		sfd.close()



if(__name__ == "__main__"):
	cli = client()
	cli.send_hello()
