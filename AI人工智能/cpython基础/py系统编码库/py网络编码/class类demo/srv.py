#coding=UTF-8

import socket
import time



# 字符串操作声明(兼容python2/3)
'''
	如果你在操作字节串(bytes), 并且想要编码, 请使用*.decode();
	(python string 字符串操作, 需要经过*.decode() 的数据)
	如果你在操作字符串(str), 并且想要编码成字节串, 请使用*.encode();
	(python socket io 需要发送/接收经过*.encode() 的数据)
'''

class tcp_srv:
	# 私有数据(不可继承,'__'双下划线打头) [函数省缺值, 不能是私有数据, 或者类中的public 数据]
	__sfd = 0
	__running = False

	# 公开数据
	ip = ""
	port = 0
	listen_max = 0

	# 构造函数
	def __init__(self, ip, port, listen_max):
		self.ip = ip
		self.port = port
		self.listen_max = listen_max
		self.__sfd = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
		self.__sfd.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
		self.__sfd.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, True)
		self.__sfd.bind((ip, port))
		self.__sfd.listen(listen_max)
		self.__running = True

	# 析构函数
	def __del__(self):
		self.__running = False
		self.__sfd.close()

	# 判断服务器是否running(server 专属)
	def isRunning(self):
		return self.__running

	# 接收data (如果不确定需要接收的数据量, 可以使用一个循环来接收所有的数据) 
	# [TCP 专属, py 只能这样接受TCP 数据, UDP 不支持这样做, 需要做数据截断和数据完整性校验]
	# 服务器不能用这种while() 循环接收数据, 否则极度容易被client 卡死逻辑!!
	'''
	def recv_dataEx_dead(self, sfd):
		data_r = "".encode("utf-8")
		while(True):
			# 强制每次接收576字节数据, 省缺值: 4096 
			msg = sfd.recv(576)
			if(msg):
				# msg 累加, 直接一次recv() 完成
				data_r += msg
			else:
				# 返回一个utf-8 二进制的块, 不是string, 不是tuple/list, 可以强转, 可用len() 获取长度
				return data_r
	'''

	def recv_dataEx(self, sfd):
		# 返回一个utf-8 二进制的块, 不是string, 不是tuple/list, 可以强转, 可用len() 获取长度
		return sfd.recv(576)

	# 发送data
	def send_dataEx(self, sfd, msg):
		# 返回发送的数据长度[数据io 必须将string 强转为utf-8二进制数据, 否则发送io 报错]
		return sfd.send(msg.encode("utf-8"))

	# 执行一次阻塞accept() [你有时间, 可以建立一个线程专门跑一个while() 循环来循环accept(), 这里演示, 只做一次的accept()]
	def accept_cli(self):
		return self.__sfd.accept()



if(__name__ == "__main__"):
	srv = tcp_srv("127.0.0.1", 9980, listen_max=32)
	print("srv: server ip=", srv.ip, "server port=", srv.port)

	while(True):
		# 接收cli 的链接请求
		sfd_cli, addr = srv.accept_cli()
		# 接收客户端返回的消息
		data_r = srv.recv_dataEx(sfd_cli)
		print("srv: recv() data len = " + str(len(data_r)) + ", data: "+ data_r.decode("utf-8"))
		# 向客户端发送消息
		data_len_s = srv.send_dataEx(sfd_cli, "yes client, you have already connected to server!!")
		print("srv: send() data len = " + str(data_len_s))
		# 关闭client sfd
		sfd_cli.close()
		# 休眠一秒(循环休眠)
		#if(srv.isRunning):
			#time.sleep(1)


