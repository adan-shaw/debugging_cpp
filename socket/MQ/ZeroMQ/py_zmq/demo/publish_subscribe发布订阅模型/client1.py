#coding=UTF-8


import zmq


context = zmq.Context()
socket = context.socket(zmq.SUB)
socket.connect("tcp://localhost:5554")
socket.setsockopt(zmq.SUBSCRIBE,''.encode('utf-8'))  # 接收所有消息


while(True):
	msg = socket.recv().decode('utf-8')
	print("client1: %s" % msg)
