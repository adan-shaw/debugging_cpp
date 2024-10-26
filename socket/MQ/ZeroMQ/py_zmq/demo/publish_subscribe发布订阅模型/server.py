#coding=UTF-8


import zmq
import time
import sys


context = zmq.Context()
socket = context.socket(zmq.PUB)
socket.bind("tcp://*:5554")


while(True):
	msg = "every client listen up, you are all ***"
	socket.send(msg.encode('utf-8'))
	time.sleep(1)
