#coding=UTF-8

import socket
import ssl

class client_ssl:
	def send_hello(self,):
		# 生成SSL上下文
		context = ssl.SSLContext(ssl.PROTOCOL_TLS_CLIENT)
		# 加载信任根证书
		context.load_verify_locations('cert/ca.crt')

		# 与服务端建立socket连接
		with socket.create_connection(('127.0.0.1', 9443)) as sfd_cli:
			sfd_cli.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
			# 将socket打包成SSL socket, 其主要工作是完成密钥协商
			# 一定要注意的是这里的server_hostname不是指服务端IP, 而是指服务端证书中设置的CN, 我这里正好设置成127.0.1而已
			with context.wrap_socket(sfd_cli, server_hostname='127.0.0.1') as sfd_ssl_cli:
				# 向服务端发送消息
				data_s = "hello server"
				data_len_s = sfd_ssl_cli.send(data_s.encode("utf-8"))
				print("cli[ssl]: send() data len = " + str(data_len_s))
				# 接收服务端返回的消息
				data_r = sfd_ssl_cli.recv(1024)
				print("cli[ssl]: recv() data len = " + str(len(data_r)) + ", data: "+ data_r.decode("utf-8"))
				sfd_ssl_cli.close()

if(__name__ == "__main__"):
	cli = client_ssl()
	cli.send_hello()
