#coding=UTF-8


import zmq


context = zmq.Context()
socket = context.socket(zmq.SUB)
socket.connect("tcp://localhost:5554")
socket.setsockopt(zmq.SUBSCRIBE,'123'.encode('utf-8'))  # 消息过滤(只接受123开头的信息, 所以client2 正常来说是收不到消息的)


while(True):
	msg = socket.recv().decode('utf-8')
	print("client2: %s" % msg)
