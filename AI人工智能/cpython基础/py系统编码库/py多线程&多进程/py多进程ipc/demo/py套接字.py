#coding=UTF-8

import socket
import multiprocessing



def server():
	# AF_INET替换为AF_UNIX, 性能会不会好一点, 但需要换一种写法, 否则报错!!
	server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	server_socket.bind(('127.0.0.1', 8888))
	server_socket.listen(1)
	conn, addr = server_socket.accept()
	while(True):
		data = conn.recv(1024)
		if not data:
			break
		print(data.decode())
	conn.close()
	server_socket.close()



def client():
	# AF_INET替换为AF_UNIX, 性能会不会好一点, 但需要换一种写法, 否则报错!!
	client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	client_socket.connect(('127.0.0.1', 8888))
	client_socket.sendall(b'Hello, server')
	client_socket.close()



if(__name__ == '__main__'):
	server_process = multiprocessing.Process(target=server)
	client_process = multiprocessing.Process(target=client)
	server_process.start()
	client_process.start()
	server_process.join()
	client_process.join()
