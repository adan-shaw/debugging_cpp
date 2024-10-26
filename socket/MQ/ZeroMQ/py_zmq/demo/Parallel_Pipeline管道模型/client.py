#coding=UTF-8


import zmq


context = zmq.Context()
socket = context.socket(zmq.PULL)
socket.bind("tcp://*:5558")


while(True):
	msg = socket.recv().decode('utf-8')
	print("client: %s" % msg)
