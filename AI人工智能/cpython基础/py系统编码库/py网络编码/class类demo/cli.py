#coding=UTF-8

import socket



# 字符串操作声明(兼容python2/3)
'''
	如果你在操作字节串(bytes), 并且想要编码, 请使用*.decode();
	(python string 字符串操作, 需要经过*.decode() 的数据)
	如果你在操作字符串(str), 并且想要编码成字节串, 请使用*.encode();
	(python socket io 需要发送/接收经过*.encode() 的数据)
'''

class tcp_cli:
	# 私有数据(不可继承,'__'双下划线打头) [函数省缺值, 不能是私有数据, 或者类中的public 数据]
	__sfd = 0

	# 公开数据
	ip = ""
	port = 0

	# 构造函数
	def __init__(self, ip, port):
		self.ip = ip
		self.port = port
		self.__sfd = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
		self.__sfd.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
		self.__sfd.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, True)
		self.__sfd.connect((ip, port))

	# 析构函数
	def __del__(self):
		self.__sfd.close()

	# 接收data (如果不确定需要接收的数据量, 可以使用一个循环来接收所有的数据) 
	# [TCP 专属, py 只能这样接受TCP 数据, UDP 不支持这样做, 需要做数据截断和数据完整性校验]
	def recv_data(self):
		data_r = "".encode("utf-8")
		while(True):
			# 强制每次接收576字节数据, 省缺值: 4096 
			msg = self.__sfd.recv(576)
			if(msg):
				# msg 累加, 直接一次recv() 完成
				data_r += msg
			else:
				# 返回一个utf-8 二进制的块, 不是string, 不是tuple/list, 可以强转, 可用len() 获取长度
				return data_r

	# 发送data
	def send_data(self, msg):
		# 返回发送的数据长度[数据io 必须将string 强转为utf-8二进制数据, 否则发送io 报错]
		return self.__sfd.send(msg.encode("utf-8"))

	# say hello
	def say_hello(self):
		# 向客户端发送消息
		data_len_s = self.send_data("hello server!!")
		print("cli: send() data len = " + str(data_len_s))
		# 接收客户端返回的消息
		data_r = self.recv_data()
		print("cli: recv() data len = " + str(len(data_r)) + ", data: "+ data_r.decode("utf-8"))



if(__name__ == "__main__"):
	cli = tcp_cli("127.0.0.1", 9980)
	print("cli: server ip=", cli.ip, "server port=", cli.port)
	cli.say_hello()


