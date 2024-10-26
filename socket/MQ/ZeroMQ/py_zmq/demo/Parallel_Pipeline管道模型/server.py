#coding=UTF-8


import zmq
import time


context = zmq.Context()
socket = context.socket(zmq.PUSH)
socket.bind("tcp://*:5557")


while(True):
	msg = "every worker listen up, you guys is all ***"
	socket.send(msg.encode('utf-8'))
	print("server: 已发送")
	time.sleep(1)
