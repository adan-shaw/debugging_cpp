#coding=UTF-8

import socket
import ssl

class server_ssl:
	def start_listen(self):
		# 生成SSL上下文
		context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
		# 加载服务器所用证书和私钥
		#context.load_cert_chain('ssl.key/srv.crt', 'ssl.key/server_rsa_private.pem.unsecure')
		context.load_cert_chain('./ssl.key/ca.pem', './ssl.key/priv.key')

		# 监听端口(证书定义了443 端口, 必须使用443 端口通信)
		with socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0) as sfd_srv:
			sfd_srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
			sfd_srv.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, True)
			sfd_srv.bind(('127.0.0.1', 443))
			sfd_srv.listen(5)
			# 将socket打包成SSL socket, 其主要工作是完成密钥协商
			with context.wrap_socket(sfd_srv, server_side=True) as sfd_ssl_srv:
				while(True):
					# 接收客户端连接
					sfd_ssl_tmp, addr = sfd_ssl_srv.accept()
					# 接收客户端返回的消息
					data_r = sfd_ssl_tmp.recv(1024)
					print("srv[ssl]: recv() data len = " + str(len(data_r)) + ", data: "+ data_r.decode("utf-8"))
					# 向客户端发送信息
					data_s = "FU client, go to hell"
					data_len_s = sfd_ssl_tmp.send(data_s.encode("utf-8"))
					print("srv[ssl]: send() data len = " + str(data_len_s))
					sfd_ssl_tmp.close()

if(__name__ == "__main__"):
	srv = server_ssl()
	srv.start_listen()


